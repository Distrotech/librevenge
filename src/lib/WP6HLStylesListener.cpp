/* libwpd
 * Copyright (C) 2003 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2003 Marc Maurer (j.m.maurer@student.utwente.nl)
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by 
 * Corel Corporation or Corel Corporation Limited."
 */
 
#include "WP6HLStylesListener.h"
#include "WPXTable.h"
#include "WP6FileStructure.h"
#include "WPXFileStructure.h"
#include "libwpd_internal.h"

// WP6HLStylesListener: creates intermediate table and page span representations, given a
// sequence of messages passed to it by the parser.

WP6HLStylesListener::WP6HLStylesListener(vector<WPXPageSpan *> *pageList, WPXTableList  *tableList) : 
	WP6HLListener(pageList, NULL),
	m_currentPage(new WPXPageSpan()),
	m_tableList(tableList), 
	m_tempMarginLeft(1.0f),
	m_tempMarginRight(1.0f),
	m_currentPageHasContent(false),
	m_isTableDefined(false)
{
}

void WP6HLStylesListener::endDocument()
{	
	insertBreak(WPX_SOFT_PAGE_BREAK); // pretend we just had a soft page break (for the last page)
	delete(m_currentPage); // and delete the non-existent page that was allocated as a result (scandalous waste!)
}

void WP6HLStylesListener::insertBreak(const guint8 breakType)
{
	if (!isUndoOn())
	{	
		switch (breakType) 
		{
		case WPX_PAGE_BREAK:
		case WPX_SOFT_PAGE_BREAK:
			if (WPXHLListener::m_pageList->size() > 0 && (*m_currentPage)==(*(m_pageList->back())))
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
			m_currentPage->setMarginLeft(m_tempMarginLeft);
			m_currentPage->setMarginRight(m_tempMarginRight);
			m_currentPageHasContent = false;
			break;
		}
	}
}

void WP6HLStylesListener::pageMarginChange(const guint8 side, const guint16 margin)
{
	if (!isUndoOn()) 
	{
		float marginInch = (float)((double)margin / (double)WPX_NUM_WPUS_PER_INCH);
		switch(side)
		{
			case WP6_PAGE_GROUP_TOP_MARGIN_SET:
				m_currentPage->setMarginTop(marginInch);
				break;
			case WP6_PAGE_GROUP_BOTTOM_MARGIN_SET:
				m_currentPage->setMarginBottom(marginInch);
				break;
		}
	}
}

void WP6HLStylesListener::marginChange(const guint8 side, const guint16 margin)
{
	if (!isUndoOn()) 
	{		
		float marginInch = (float)((double)margin / (double)WPX_NUM_WPUS_PER_INCH);
		switch(side)
		{
			case WP6_COLUMN_GROUP_LEFT_MARGIN_SET:
				if (!m_currentPageHasContent)
					m_currentPage->setMarginLeft(marginInch);
				m_tempMarginLeft = marginInch;
				break;
			case WP6_COLUMN_GROUP_RIGHT_MARGIN_SET:
				if (!m_currentPageHasContent)
					m_currentPage->setMarginRight(marginInch);
				m_tempMarginRight = marginInch;
				break;
		}
		
	}

}

void WP6HLStylesListener::headerFooterGroup(const guint8 headerFooterType, const guint8 occurenceBits, const guint16 textPID)
{
	if (!isUndoOn()) 
	{			
		WPD_DEBUG_MSG(("WordPerfect: headerFooterGroup (headerFooterType: %i, occurenceBits: %i, textPID: %i)\n", 
			       headerFooterType, occurenceBits, textPID));
		if (headerFooterType <= WP6_HEADER_FOOTER_GROUP_FOOTER_B) // ignore watermarks for now
		{
			WPXTableList *tableList = new WPXTableList; 
			m_currentPage->setHeaderFooter(headerFooterType, occurenceBits, textPID, tableList);
			_handleSubDocument(textPID, true, tableList);
		}
	}
}

void WP6HLStylesListener::suppressPageCharacteristics(const guint8 suppressCode)
{
	if (!isUndoOn()) 
	{			
		WPD_DEBUG_MSG(("WordPerfect: suppressPageCharacteristics (suppressCode: %u)\n", suppressCode));
		if (suppressCode & WP6_PAGE_GROUP_SUPPRESS_HEADER_A)
			m_currentPage->setHeadFooterSuppression(WP6_HEADER_FOOTER_GROUP_HEADER_A, true);
		if (suppressCode & WP6_PAGE_GROUP_SUPPRESS_HEADER_B)
			m_currentPage->setHeadFooterSuppression(WP6_HEADER_FOOTER_GROUP_HEADER_B, true);
		if (suppressCode & WP6_PAGE_GROUP_SUPPRESS_FOOTER_A)
			m_currentPage->setHeadFooterSuppression(WP6_HEADER_FOOTER_GROUP_FOOTER_A, true);
		if (suppressCode & WP6_PAGE_GROUP_SUPPRESS_FOOTER_B)
			m_currentPage->setHeadFooterSuppression(WP6_HEADER_FOOTER_GROUP_FOOTER_B, true);			
	}
}

void WP6HLStylesListener::defineTable(guint8 position, guint16 leftOffset)
{
	if (!isUndoOn()) 
	{			
		m_currentPageHasContent = true;
		m_currentTable = new WPXTable();
		m_tableList->add(m_currentTable);
		m_isTableDefined = true;
	}
}


void WP6HLStylesListener::startTable()
{
	if (!isUndoOn() && !m_isTableDefined) 
	{			
		m_currentPageHasContent = true;
		m_currentTable = new WPXTable();
		m_tableList->add(m_currentTable);
		m_isTableDefined = false;
	}
}

void WP6HLStylesListener::endTable()
{
	if (!isUndoOn())
	{
		m_isTableDefined = false;
	}
}

void WP6HLStylesListener::insertRow()
{
	if (!isUndoOn() && m_currentTable != NULL) 
	{
		m_currentPageHasContent = true;
		m_currentTable->insertRow();
	}
}

void WP6HLStylesListener::insertCell(const guint8 colSpan, const guint8 rowSpan, const bool boundFromLeft, const bool boundFromAbove, 
				     const guint8 borderBits, 
				     const RGBSColor * cellFgColor, const RGBSColor * cellBgColor)
{
	if (!isUndoOn() && m_currentTable != NULL)
	{
		m_currentPageHasContent = true;
		m_currentTable->insertCell(colSpan, rowSpan, boundFromLeft, boundFromAbove, borderBits);
	}
}

void WP6HLStylesListener::noteOn(const guint16 textPID)
{
	if (!isUndoOn()) 
	{
		m_currentPageHasContent = true; 		
		_handleSubDocument(textPID, false, NULL);
	}
}

void WP6HLStylesListener::_handleSubDocument(guint16 textPID, const bool isHeaderFooter, WPXTableList *tableList)
{
	// We don't want to actual insert anything in the case of a sub-document, but we
	// do want to capture whatever table-related information is within it..
	if (!isUndoOn()) 
	{
		if (textPID)
		{
			WPXTableList * oldTableList = m_tableList;
			WPXTable * oldCurrentTable = m_currentTable;
			if (tableList)
			{
				m_tableList = tableList;
				m_currentTable = NULL;
			}
			WP6LLListener::getPrefixDataPacket(textPID)->parse(this);

			m_tableList = oldTableList;
			m_currentTable = oldCurrentTable;
		}
	}
}
