/* libwpd
 * Copyright (C) 2003 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2004 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
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
 
#include "WP42StylesListener.h"
#include "WPXTable.h"
#include "WP42FileStructure.h"
#include "WPXFileStructure.h"
#include "libwpd_internal.h"

WP42StylesListener::WP42StylesListener(std::list<WPXPageSpan> &pageList, std::vector<WP42SubDocument *> &subDocuments) : 
	WP42Listener(),
	WPXStylesListener(pageList),
	m_currentPage(),
	m_nextPage(),
	m_subDocuments(subDocuments),
	m_tempMarginLeft(1.0),
	m_tempMarginRight(1.0),
	m_currentPageHasContent(false),
	m_isSubDocument(false),
	m_pageListHardPageMark(m_pageList.end())
{
}

void WP42StylesListener::endDocument()
{	
	insertBreak(WPX_SOFT_PAGE_BREAK); // pretend we just had a soft page break (for the last page)
}

void WP42StylesListener::endSubDocument()
{	
	insertBreak(WPX_SOFT_PAGE_BREAK); // pretend we just had a soft page break (for the last page)
}

void WP42StylesListener::insertBreak(uint8_t breakType)
{
	if (m_isSubDocument)
		return;

	if (!isUndoOn())
	{	
		WPXTableList tableList;
		switch (breakType) 
		{
		case WPX_PAGE_BREAK:
		case WPX_SOFT_PAGE_BREAK:
			if ((m_pageList.size() > 0) && (m_currentPage==m_pageList.back())
				&& (m_pageListHardPageMark != m_pageList.end()))
			{
				m_pageList.back().setPageSpan(m_pageList.back().getPageSpan() + 1);
			}
			else
			{
				m_pageList.push_back(WPXPageSpan(m_currentPage));
				if (m_pageListHardPageMark == m_pageList.end())
				{
					m_pageListHardPageMark--;
				}
			}
			m_currentPage = WPXPageSpan(m_pageList.back(), 0.0, 0.0);
			m_currentPage.setPageSpan(1);

			for(std::vector<WPXHeaderFooter>::const_iterator HFiter = (m_nextPage.getHeaderFooterList()).begin();
				HFiter != (m_nextPage.getHeaderFooterList()).end(); HFiter++)
			{
				if ((*HFiter).getOccurence() != NEVER)
				{
					m_currentPage.setHeaderFooter((*HFiter).getType(), (*HFiter).getInternalType(),
						(*HFiter).getOccurence(), (*HFiter).getSubDocument(), (*HFiter).getTableList());
					_handleSubDocument((*HFiter).getSubDocument(), WPX_SUBDOCUMENT_HEADER_FOOTER, (*HFiter).getTableList());
				}
				else
				{
					m_currentPage.setHeaderFooter((*HFiter).getType(), (*HFiter).getInternalType(),
						(*HFiter).getOccurence(), 0, (*HFiter).getTableList());
				}	
			}
			m_nextPage = WPXPageSpan();
			m_currentPageHasContent = false;
			break;
		}
		if (breakType == WPX_PAGE_BREAK)
		{
			m_pageListHardPageMark = m_pageList.end();
			m_currentPage.setMarginLeft(m_tempMarginLeft);
			m_currentPage.setMarginRight(m_tempMarginRight);
		}
	}
}


void WP42StylesListener::headerFooterGroup(uint8_t headerFooterDefinition, WP42SubDocument *subDocument)
{
	if (subDocument)
		m_subDocuments.push_back(subDocument);

	if (!isUndoOn())
	{
		bool tempCurrentPageHasContent = m_currentPageHasContent;

		uint8_t headerFooterType = (headerFooterDefinition & 0x03);
		WPXHeaderFooterType wpxType = ((headerFooterType <= WPX_HEADER_B) ? HEADER : FOOTER);

		uint8_t occurenceBits = ((headerFooterDefinition & 0xFC) >> 2);

		WPD_DEBUG_MSG(("WordPerfect: headerFooterGroup (headerFooterType: %i, occurenceBits: %i)\n", 
			       headerFooterType, occurenceBits));

		WPXHeaderFooterOccurence wpxOccurence;

		if (occurenceBits & WP42_HEADER_FOOTER_GROUP_ALL_BIT)
			wpxOccurence = ALL;
		else if (occurenceBits & WP42_HEADER_FOOTER_GROUP_EVEN_BIT)
			wpxOccurence = EVEN;
		else if (occurenceBits & WP42_HEADER_FOOTER_GROUP_ODD_BIT)
			wpxOccurence = ODD;
		else
			wpxOccurence = NEVER;

		WPXTableList tableList;

		if ((wpxType == HEADER) && tempCurrentPageHasContent)
			m_nextPage.setHeaderFooter(wpxType, headerFooterType, wpxOccurence, subDocument, tableList);
		else /* FOOTER || !tempCurrentPageHasContent */
		{
			if (wpxOccurence != NEVER)
			{
				m_currentPage.setHeaderFooter(wpxType, headerFooterType, wpxOccurence, subDocument, tableList);
				_handleSubDocument(subDocument, WPX_SUBDOCUMENT_HEADER_FOOTER, tableList);
			}
			else
				m_currentPage.setHeaderFooter(wpxType, headerFooterType, wpxOccurence, 0, tableList);
		}
		m_currentPageHasContent = tempCurrentPageHasContent;
	}
}	

void WP42StylesListener::suppressPageCharacteristics(uint8_t suppressCode)
{
	if (!isUndoOn())
	{
		if (suppressCode & 0x01)
		{
			m_currentPage.setHeadFooterSuppression(WPX_HEADER_A, true);
			m_currentPage.setHeadFooterSuppression(WPX_HEADER_B, true);
			m_currentPage.setHeadFooterSuppression(WPX_FOOTER_A, true);
			m_currentPage.setHeadFooterSuppression(WPX_FOOTER_B, true);
		}
		if (suppressCode & 0x08)
		{
			m_currentPage.setHeadFooterSuppression(WPX_HEADER_A, true);
			m_currentPage.setHeadFooterSuppression(WPX_HEADER_B, true);
		}
		if (suppressCode & 0x10)
			m_currentPage.setHeadFooterSuppression(WPX_HEADER_A, true);
		if (suppressCode & 0x20)
			m_currentPage.setHeadFooterSuppression(WPX_HEADER_B, true);
		if (suppressCode & 0x40)
			m_currentPage.setHeadFooterSuppression(WPX_FOOTER_A, true);
		if (suppressCode & 0x80)
			m_currentPage.setHeadFooterSuppression(WPX_FOOTER_B, true);
	}
}

void WP42StylesListener::_handleSubDocument(const WPXSubDocument *subDocument, WPXSubDocumentType subDocumentType, 
						WPXTableList /* tableList */, int /* nextTableIndice */)
{
	if (!isUndoOn()) 
	{
		bool oldIsSubDocument = m_isSubDocument;
		m_isSubDocument = true;
		if (subDocumentType == WPX_SUBDOCUMENT_HEADER_FOOTER) 
		{
			bool oldCurrentPageHasContent = m_currentPageHasContent;

			if (subDocument)
				static_cast<const WP42SubDocument *>(subDocument)->parse(this);

			m_currentPageHasContent = oldCurrentPageHasContent;
		}
		else
		{
			if (subDocument)
				static_cast<const WP42SubDocument *>(subDocument)->parse(this);
		}
		m_isSubDocument = oldIsSubDocument;
	}
}
