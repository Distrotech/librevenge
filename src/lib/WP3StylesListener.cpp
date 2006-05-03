/* libwpd
 * Copyright (C) 2004 Marc Maurer (j.m.maurer@student.utwente.nl)
 *  
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by 
 * Corel Corporation or Corel Corporation Limited."
 */
 
#include "WP3StylesListener.h"
#include "WPXTable.h"
#include "WP3FileStructure.h"
#include "WPXFileStructure.h"
#include "libwpd_internal.h"
#include "WP3SubDocument.h"

WP3StylesListener::WP3StylesListener(std::list<WPXPageSpan *> *pageList, WPXTableList tableList, std::vector<WP3SubDocument *>&subDocuments) : 
	WP3Listener(pageList, NULL),
	m_currentPage(new WPXPageSpan()),
	m_nextPage(NULL),
	m_tableList(tableList), 
	m_tempMarginLeft(1.0f),
	m_tempMarginRight(1.0f),
	m_currentPageHasContent(false),
	m_subDocuments(subDocuments)
{
}

void WP3StylesListener::endDocument()
{	
	insertBreak(WPX_PAGE_BREAK); // pretend we just had a soft page break (for the last page)
	delete(m_currentPage); // and delete the non-existent page that was allocated as a result (scandalous waste!)
}

void WP3StylesListener::insertBreak(const uint8_t breakType)
{
	if (!isUndoOn())
	{	
		switch (breakType) 
		{
		case WPX_PAGE_BREAK:
		case WPX_SOFT_PAGE_BREAK:
			if (WPXListener::m_pageList->size() > 0 && (*m_currentPage)==(*(m_pageList->back())))
			{
				int oldPageSpan = m_pageList->back()->getPageSpan();
				m_pageList->back()->setPageSpan(oldPageSpan + 1);
				delete(m_currentPage);
			}
			else
			{
				m_pageList->push_back(m_currentPage);
			}
			m_currentPage = new WPXPageSpan(*(m_pageList->back()));
			if (m_nextPage)
			{
				for(std::vector<WPXHeaderFooter>::const_iterator HFiter = (m_nextPage->getHeaderFooterList()).begin();
					HFiter != (m_nextPage->getHeaderFooterList()).end(); HFiter++)
				{
					if ((*HFiter).getOccurence() != NEVER)
					{
						m_currentPage->setHeaderFooter((*HFiter).getType(), (*HFiter).getInternalType(),
							(*HFiter).getOccurence(), (*HFiter).getSubDocument(), (*HFiter).getTableList());
						_handleSubDocument((*HFiter).getSubDocument(), true, (*HFiter).getTableList());
					}
					else
						m_currentPage->setHeaderFooter((*HFiter).getType(), (*HFiter).getInternalType(),
							(*HFiter).getOccurence(), NULL, (*HFiter).getTableList());	
					
				}
				delete (m_nextPage);
				m_nextPage = NULL;
			}
			m_currentPage->setMarginLeft(m_tempMarginLeft);
			m_currentPage->setMarginRight(m_tempMarginRight);
			m_currentPageHasContent = false;
			break;
		}
	}
}

void WP3StylesListener::pageMarginChange(const uint8_t side, const uint16_t margin)
{
	if (!isUndoOn()) 
	{
		float marginInch = (float)((double)margin / (double)WPX_NUM_WPUS_PER_INCH);
		switch(side)
		{
			case WPX_TOP:
				m_currentPage->setMarginTop(marginInch);
				break;
			case WPX_BOTTOM:
				m_currentPage->setMarginBottom(marginInch);
				break;
		}
	}
}

void WP3StylesListener::marginChange(const uint8_t side, const uint16_t margin)
{
	if (!isUndoOn()) 
	{		
		float marginInch = (float)((double)margin / (double)WPX_NUM_WPUS_PER_INCH);
		switch(side)
		{
			case WPX_LEFT:
				if (!m_currentPageHasContent)
					m_currentPage->setMarginLeft(marginInch);
				m_tempMarginLeft = marginInch;
				break;
			case WPX_RIGHT:
				if (!m_currentPageHasContent)
					m_currentPage->setMarginRight(marginInch);
				m_tempMarginRight = marginInch;
				break;
		}
		
	}

}

void WP3StylesListener::pageFormChange(const uint16_t length, const uint16_t width, const WPXFormOrientation orientation, const bool isPersistent)
{
	if (!isUndoOn())
	{
		// TODO: handle the isPersistent information
		float lengthInch = (float)((double)length / (double)WPX_NUM_WPUS_PER_INCH);
		float widthInch = (float)((double)width / (double)WPX_NUM_WPUS_PER_INCH);
		if (!m_currentPageHasContent)
		{
			m_currentPage->setFormLength(lengthInch);
			m_currentPage->setFormWidth(widthInch);
			m_currentPage->setFormOrientation(orientation);
		}
	}
}

void WP3StylesListener::headerFooterGroup(const uint8_t headerFooterType, const uint8_t occurenceBits, WP3SubDocument *subDocument)
{
	if (!isUndoOn()) 
	{
		if (subDocument)
			m_subDocuments.push_back(subDocument);			

		WPD_DEBUG_MSG(("WordPerfect: headerFooterGroup (headerFooterType: %i, occurenceBits: %i)\n", 
			       headerFooterType, occurenceBits));
		bool tempCurrentPageHasContent = m_currentPageHasContent;
		if (headerFooterType <= WP3_HEADER_FOOTER_GROUP_FOOTER_B) // ignore watermarks for now
		{
			WPXHeaderFooterType wpxType = ((headerFooterType <= WP3_HEADER_FOOTER_GROUP_HEADER_B) ? HEADER : FOOTER);
			
			WPXHeaderFooterOccurence wpxOccurence;
			if ((occurenceBits & WP3_HEADER_FOOTER_GROUP_EVEN_BIT) && (occurenceBits & WP3_HEADER_FOOTER_GROUP_ODD_BIT))
				wpxOccurence = ALL;
			else if (occurenceBits & WP3_HEADER_FOOTER_GROUP_EVEN_BIT)
				wpxOccurence = EVEN;
			else if (occurenceBits & WP3_HEADER_FOOTER_GROUP_ODD_BIT)
				wpxOccurence = ODD;
			else
				wpxOccurence = NEVER;

			WPXTableList tableList;

			
			if ((wpxType == HEADER) && tempCurrentPageHasContent)
			{
				if (!m_nextPage)
					m_nextPage = new WPXPageSpan();
				m_nextPage->setHeaderFooter(wpxType, headerFooterType, wpxOccurence, subDocument, tableList);
			}

			else /* FOOTER || !tempCurrentPageHasContent */
			{
				if (wpxOccurence != NEVER)
				{
					m_currentPage->setHeaderFooter(wpxType, headerFooterType, wpxOccurence, subDocument, tableList);
					_handleSubDocument(subDocument, true, tableList);
				}
				else
					m_currentPage->setHeaderFooter(wpxType, headerFooterType, wpxOccurence, NULL, tableList);
			}
		}
		m_currentPageHasContent = tempCurrentPageHasContent;
	}
}


void WP3StylesListener::suppressPage(const uint16_t suppressCode)
{
	if (!isUndoOn()) 
	{			
		WPD_DEBUG_MSG(("WordPerfect: suppressPageCharacteristics (suppressCode: %u)\n", suppressCode));
		if (suppressCode & WP3_PAGE_GROUP_SUPPRESS_HEADER_A)
			m_currentPage->setHeadFooterSuppression(WPX_HEADER_A, true);
		if (suppressCode & WP3_PAGE_GROUP_SUPPRESS_HEADER_B)
			m_currentPage->setHeadFooterSuppression(WPX_HEADER_B, true);
		if (suppressCode & WP3_PAGE_GROUP_SUPPRESS_FOOTER_A)
			m_currentPage->setHeadFooterSuppression(WPX_FOOTER_A, true);
		if (suppressCode & WP3_PAGE_GROUP_SUPPRESS_FOOTER_B)
			m_currentPage->setHeadFooterSuppression(WPX_FOOTER_B, true);			
	}
}

void WP3StylesListener::startTable()
{
	if (!isUndoOn()) 
	{			
		m_currentPageHasContent = true;
		m_currentTable = new WPXTable();
		m_tableList.add(m_currentTable);
	}
}

void WP3StylesListener::insertRow()
{
	if (!isUndoOn() && m_currentTable != NULL) 
	{
		m_currentPageHasContent = true;
		m_currentTable->insertRow();
	}
}

void WP3StylesListener::insertCell()
{
	if (!isUndoOn() && m_currentTable != NULL)
	{
		m_currentPageHasContent = true;
#if 0
		m_currentTable->insertCell(colSpan, rowSpan, borderBits);
#endif
	}
}

void WP3StylesListener::_handleSubDocument(const WPXSubDocument *subDocument, const bool isHeaderFooter, WPXTableList tableList, int nextTableIndice)
{
	// We don't want to actual insert anything in the case of a sub-document, but we
	// do want to capture whatever table-related information is within it..
	if (!isUndoOn()) 
	{
		// prevent entering in an endless loop		
		if (isHeaderFooter) 
		{
			WPXTable * oldCurrentTable = m_currentTable;
			WPXTableList oldTableList = m_tableList;
			m_tableList = tableList;
			marginChange(WPX_NUM_WPUS_PER_INCH, WPX_LEFT);
			marginChange(WPX_NUM_WPUS_PER_INCH, WPX_RIGHT);

			subDocument->parse(this);

			m_tableList = oldTableList;
			m_currentTable = oldCurrentTable;
		}
		else
		{
			subDocument->parse(this);
		}
	}
}
