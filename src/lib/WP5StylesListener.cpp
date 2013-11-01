/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2003 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2004 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 *
 * For further information visit http://librevenge.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#include "WP5StylesListener.h"
#include "RVNGTable.h"
#include "WP5FileStructure.h"
#include "RVNGFileStructure.h"
#include "librevenge_internal.h"
#include "WP5SubDocument.h"

WP5StylesListener::WP5StylesListener(std::list<RVNGPageSpan> &pageList, RVNGTableList tableList, std::vector<WP5SubDocument *> &subDocuments) :
	WP5Listener(),
	RVNGStylesListener(pageList),
	m_currentPage(),
	m_nextPage(),
	m_tableList(tableList),
	m_currentTable(0),
	m_tempMarginLeft(1.0),
	m_tempMarginRight(1.0),
	m_currentPageHasContent(false),
	m_isSubDocument(false),
	m_subDocuments(subDocuments),
	m_pageListHardPageMark(m_pageList.end())
{
}

void WP5StylesListener::endDocument()
{
	insertBreak(RVNG_SOFT_PAGE_BREAK); // pretend we just had a soft page break (for the last page)
}

void WP5StylesListener::endSubDocument()
{
	insertBreak(RVNG_SOFT_PAGE_BREAK); // pretend we just had a soft page break (for the last page)
}

void WP5StylesListener::insertBreak(uint8_t breakType)
{
	if (m_isSubDocument)
		return;

	//if (!isUndoOn())
	//{
	RVNGTableList tableList;
	switch (breakType)
	{
	case RVNG_PAGE_BREAK:
	case RVNG_SOFT_PAGE_BREAK:
	{
		if ((m_pageList.size() > 0) && (m_currentPage==m_pageList.back())
		        && (m_pageListHardPageMark != m_pageList.end()))
		{
			m_pageList.back().setPageSpan(m_pageList.back().getPageSpan() + 1);
		}
		else
		{
			m_pageList.push_back(RVNGPageSpan(m_currentPage));
			if (m_pageListHardPageMark == m_pageList.end())
				--m_pageListHardPageMark;
		}
		m_currentPage = RVNGPageSpan(m_pageList.back(), 0.0, 0.0);
		m_currentPage.setPageSpan(1);

		for(std::vector<RVNGHeaderFooter>::const_iterator HFiter = (m_nextPage.getHeaderFooterList()).begin();
		        HFiter != (m_nextPage.getHeaderFooterList()).end(); ++HFiter)
		{
			if ((*HFiter).getOccurence() != NEVER)
			{
				m_currentPage.setHeaderFooter((*HFiter).getType(), (*HFiter).getInternalType(),
				                              (*HFiter).getOccurence(), (*HFiter).getSubDocument(), (*HFiter).getTableList());
				_handleSubDocument((*HFiter).getSubDocument(), RVNG_SUBDOCUMENT_HEADER_FOOTER, (*HFiter).getTableList());
			}
			else
				m_currentPage.setHeaderFooter((*HFiter).getType(), (*HFiter).getInternalType(),
				                              (*HFiter).getOccurence(), 0, (*HFiter).getTableList());
		}
		m_nextPage = RVNGPageSpan();
		m_currentPageHasContent = false;
	}
	break;
	default:
		break;
	}
	if (breakType == RVNG_PAGE_BREAK)
	{
		m_pageListHardPageMark = m_pageList.end();
		m_currentPage.setMarginLeft(m_tempMarginLeft);
		m_currentPage.setMarginRight(m_tempMarginRight);
	}
	//}
}

void WP5StylesListener::pageMarginChange(uint8_t side, uint16_t margin)
{
	//if (!isUndoOn())
	//{
	double marginInch = (double)((double)margin / (double)RVNG_NUM_WPUS_PER_INCH);
	switch(side)
	{
	case RVNG_TOP:
		m_currentPage.setMarginTop(marginInch);
		break;
	case RVNG_BOTTOM:
		m_currentPage.setMarginBottom(marginInch);
		break;
	default:
		break;
	}
	//}
}

void WP5StylesListener::pageFormChange(uint16_t length, uint16_t width, RVNGFormOrientation orientation)
{
	//if (!isUndoOn())
	//{
	double lengthInch = (double)((double)length / (double)RVNG_NUM_WPUS_PER_INCH);
	double widthInch = (double)((double)width / (double)RVNG_NUM_WPUS_PER_INCH);
	if (!m_currentPageHasContent)
	{
		m_currentPage.setFormLength(lengthInch);
		m_currentPage.setFormWidth(widthInch);
		m_currentPage.setFormOrientation(orientation);
	}
	//}
}


void WP5StylesListener::marginChange(uint8_t side, uint16_t margin)
{
	if (!isUndoOn())
	{
		if (m_isSubDocument)
			return;

		std::list<RVNGPageSpan>::iterator Iter;
		double marginInch = (double)((double)margin / (double)RVNG_NUM_WPUS_PER_INCH);
		switch(side)
		{
		case RVNG_LEFT:
			if (!m_currentPageHasContent && (m_pageListHardPageMark == m_pageList.end()))
				m_currentPage.setMarginLeft(marginInch);
			else if (m_currentPage.getMarginLeft() > marginInch)
			{
				// Change the margin for the current page and for all pages in the list since the last Hard Break
				m_currentPage.setMarginLeft(marginInch);
				for (Iter = m_pageListHardPageMark; Iter != m_pageList.end(); ++Iter)
				{
					(*Iter).setMarginLeft(marginInch);
				}
			}
			m_tempMarginLeft = marginInch;
			break;
		case RVNG_RIGHT:
			if (!m_currentPageHasContent && (m_pageListHardPageMark == m_pageList.end()))
				m_currentPage.setMarginRight(marginInch);
			else if (m_currentPage.getMarginRight() > marginInch)
			{
				// Change the margin for the current page and for all pages in the list since the last Hard Break
				m_currentPage.setMarginRight(marginInch);
				for (Iter = m_pageListHardPageMark; Iter != m_pageList.end(); ++Iter)
				{
					(*Iter).setMarginRight(marginInch);
				}
			}
			m_tempMarginRight = marginInch;
			break;
		default:
			break;
		}

	}

}

void WP5StylesListener::headerFooterGroup(uint8_t headerFooterType, uint8_t occurenceBits, WP5SubDocument *subDocument)
{
	if (subDocument)
		m_subDocuments.push_back(subDocument);

	if (!isUndoOn())
	{
		RVNG_DEBUG_MSG(("WordPerfect: headerFooterGroup (headerFooterType: %i, occurenceBits: %i)\n",
		               headerFooterType, occurenceBits));
		bool tempCurrentPageHasContent = m_currentPageHasContent;
		if (headerFooterType <= WP5_HEADER_FOOTER_GROUP_FOOTER_B)
		{
			RVNGHeaderFooterType rvngType = ((headerFooterType <= WP5_HEADER_FOOTER_GROUP_HEADER_B) ? HEADER : FOOTER);

			RVNGHeaderFooterOccurence rvngOccurence;
			if (occurenceBits & WP5_HEADER_FOOTER_GROUP_ALL_BIT)
				rvngOccurence = ALL;
			else if (occurenceBits & WP5_HEADER_FOOTER_GROUP_EVEN_BIT)
				rvngOccurence = EVEN;
			else if (occurenceBits & WP5_HEADER_FOOTER_GROUP_ODD_BIT)
				rvngOccurence = ODD;
			else
				rvngOccurence = NEVER;
			RVNGTableList tableList;

			if ((rvngType == HEADER) && tempCurrentPageHasContent)
			{
				if (rvngOccurence != NEVER)
					m_nextPage.setHeaderFooter(rvngType, headerFooterType, rvngOccurence, subDocument, tableList);
				else
					m_nextPage.setHeaderFooter(rvngType, headerFooterType, rvngOccurence, 0, tableList);
			}
			else /* FOOTER || !tempCurrentPageHasContent */
			{
				if (rvngOccurence != NEVER)
				{
					m_currentPage.setHeaderFooter(rvngType, headerFooterType, rvngOccurence, subDocument, tableList);
					_handleSubDocument(subDocument, RVNG_SUBDOCUMENT_HEADER_FOOTER, tableList);
				}
				else
					m_currentPage.setHeaderFooter(rvngType, headerFooterType, rvngOccurence, 0, tableList);
			}
		}
		m_currentPageHasContent = tempCurrentPageHasContent;
	}
}


void WP5StylesListener::suppressPageCharacteristics(uint8_t suppressCode)
{
	if (!isUndoOn())
	{
		RVNG_DEBUG_MSG(("WordPerfect: suppressPageCharacteristics (suppressCode: %u)\n", suppressCode));
		if (suppressCode & WP5_PAGE_GROUP_SUPPRESS_HEADER_A)
			m_currentPage.setHeadFooterSuppression(RVNG_HEADER_A, true);
		if (suppressCode & WP5_PAGE_GROUP_SUPPRESS_HEADER_B)
			m_currentPage.setHeadFooterSuppression(RVNG_HEADER_B, true);
		if (suppressCode & WP5_PAGE_GROUP_SUPPRESS_FOOTER_A)
			m_currentPage.setHeadFooterSuppression(RVNG_FOOTER_A, true);
		if (suppressCode & WP5_PAGE_GROUP_SUPPRESS_FOOTER_B)
			m_currentPage.setHeadFooterSuppression(RVNG_FOOTER_B, true);
	}
}

void WP5StylesListener::startTable()
{
	if (!isUndoOn())
	{
		m_currentPageHasContent = true;
		m_currentTable = new RVNGTable();
		m_tableList.add(m_currentTable);
	}
}

void WP5StylesListener::insertRow(uint16_t /* rowHeight */, bool /* isMinimumHeight */, bool /* isHeaderRow */)
{
	if (!isUndoOn())
	{
		m_currentPageHasContent = true;
		if (!m_currentTable)
			throw ParseException();
		m_currentTable->insertRow();
	}
}

void WP5StylesListener::insertCell(uint8_t colSpan, uint8_t rowSpan, uint8_t borderBits,
                                   const RGBSColor * /* cellFgColor */, const RGBSColor * /* cellBgColor */,
                                   const RGBSColor * /* cellBorderColor */, RVNGVerticalAlignment /* cellVerticalAlignment */,
                                   bool /* useCellAttributes */, uint32_t /* cellAttributes */)
{
	if (!isUndoOn())
	{
		m_currentPageHasContent = true;
		if (!m_currentTable)
			throw ParseException();
		m_currentTable->insertCell(colSpan, rowSpan, borderBits);
	}
}

void WP5StylesListener::_handleSubDocument(const RVNGSubDocument *subDocument, RVNGSubDocumentType subDocumentType,
        RVNGTableList tableList, int /* nextTableIndice */)
{
	// We don't want to actual insert anything in the case of a sub-document, but we
	// do want to capture whatever table-related information is within it..
	if (!isUndoOn())
	{
		bool oldIsSubDocument = m_isSubDocument;
		m_isSubDocument = true;
		if (subDocumentType == RVNG_SUBDOCUMENT_HEADER_FOOTER)
		{
			bool oldCurrentPageHasContent = m_currentPageHasContent;
			RVNGTable *oldCurrentTable = m_currentTable;
			RVNGTableList oldTableList = m_tableList;
			m_tableList = tableList;

			if (subDocument)
				static_cast<const WP5SubDocument *>(subDocument)->parse(this);

			m_tableList = oldTableList;
			m_currentTable = oldCurrentTable;
			m_currentPageHasContent = oldCurrentPageHasContent;
		}
		else
		{
			if (subDocument)
				static_cast<const WP5SubDocument *>(subDocument)->parse(this);
		}
		m_isSubDocument = oldIsSubDocument;
	}
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
