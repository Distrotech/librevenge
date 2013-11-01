/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
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

#include "WP3StylesListener.h"
#include "RVNGTable.h"
#include "WP3FileStructure.h"
#include "RVNGFileStructure.h"
#include "librevenge_internal.h"
#include "WP3SubDocument.h"

WP3StylesListener::WP3StylesListener(std::list<RVNGPageSpan> &pageList, RVNGTableList tableList, std::vector<WP3SubDocument *> &subDocuments) :
	WP3Listener(),
	RVNGStylesListener(pageList),
	m_currentPage(RVNGPageSpan()),
	m_tableList(tableList),
	m_currentTable(0),
	m_tempMarginLeft(1.0),
	m_tempMarginRight(1.0),
	m_currentPageHasContent(false),
	m_isSubDocument(false),
	m_subDocuments(subDocuments),
	m_pageListHardPageMark(m_pageList.end())
{
	m_pageListHardPageMark = m_pageList.end();
}

void WP3StylesListener::endDocument()
{
	insertBreak(RVNG_SOFT_PAGE_BREAK); // pretend we just had a soft page break (for the last page)
	setUndoOn(false);
}

void WP3StylesListener::endSubDocument()
{
	insertBreak(RVNG_SOFT_PAGE_BREAK); // pretend we just had a soft page break (for the last page)
	setUndoOn(false);
}

void WP3StylesListener::insertBreak(const uint8_t breakType)
{
	if (m_isSubDocument)
		return;

	if (!isUndoOn())
	{
		m_currentPageHasContent = true;
		RVNGTableList tableList;
		switch (breakType)
		{
		case RVNG_PAGE_BREAK:
		case RVNG_SOFT_PAGE_BREAK:
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
			m_currentPageHasContent = false;
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
	}
}

void WP3StylesListener::undoChange(const uint8_t /* undoType */, const uint16_t /* undoLevel */)
{
// enable when have understood the undo change functions in WP3 file-format
#if 0
	if (undoType == 0x00) // begin invalid text
		setUndoOn(true);
	else if (undoType == 0x01) // end invalid text
		setUndoOn(false);
#endif
}

void WP3StylesListener::pageMarginChange(const uint8_t side, const uint16_t margin)
{
	if (!isUndoOn())
	{
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
	}
}

void WP3StylesListener::marginChange(const uint8_t side, const uint16_t margin)
{
	if (!isUndoOn())
	{
		if (m_isSubDocument)
			return; // do not collect L/R margin information in sub documents

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

void WP3StylesListener::pageFormChange(const uint16_t length, const uint16_t width, const RVNGFormOrientation orientation)
{
	if (!isUndoOn())
	{
		double lengthInch = (double)((double)length / (double)RVNG_NUM_WPUS_PER_INCH);
		double widthInch = (double)((double)width / (double)RVNG_NUM_WPUS_PER_INCH);
		if (!m_currentPageHasContent)
		{
			m_currentPage.setFormLength(lengthInch);
			m_currentPage.setFormWidth(widthInch);
			m_currentPage.setFormOrientation(orientation);
		}
	}
}

void WP3StylesListener::headerFooterGroup(const uint8_t headerFooterType, const uint8_t occurenceBits, WP3SubDocument *subDocument)
{
	if (!isUndoOn())
	{
		if (subDocument)
			m_subDocuments.push_back(subDocument);

		RVNG_DEBUG_MSG(("WordPerfect: headerFooterGroup (headerFooterType: %i, occurenceBits: %i)\n",
		               headerFooterType, occurenceBits));
		bool tempCurrentPageHasContent = m_currentPageHasContent;
		if (headerFooterType <= WP3_HEADER_FOOTER_GROUP_FOOTER_B) // ignore watermarks for now
		{
			RVNGHeaderFooterType rvngType = ((headerFooterType <= WP3_HEADER_FOOTER_GROUP_HEADER_B) ? HEADER : FOOTER);

			RVNGHeaderFooterOccurence rvngOccurence;
			if ((occurenceBits & WP3_HEADER_FOOTER_GROUP_EVEN_BIT) && (occurenceBits & WP3_HEADER_FOOTER_GROUP_ODD_BIT))
				rvngOccurence = ALL;
			else if (occurenceBits & WP3_HEADER_FOOTER_GROUP_EVEN_BIT)
				rvngOccurence = EVEN;
			else if (occurenceBits & WP3_HEADER_FOOTER_GROUP_ODD_BIT)
				rvngOccurence = ODD;
			else
				rvngOccurence = NEVER;

			RVNGTableList tableList;

			if (rvngOccurence != NEVER)
			{
				m_currentPage.setHeaderFooter(rvngType, headerFooterType, rvngOccurence, subDocument, tableList);
				_handleSubDocument(subDocument, RVNG_SUBDOCUMENT_HEADER_FOOTER, tableList);
			}
			else
				m_currentPage.setHeaderFooter(rvngType, headerFooterType, rvngOccurence, 0, tableList);
		}
		m_currentPageHasContent = tempCurrentPageHasContent;
	}
}


void WP3StylesListener::suppressPage(const uint16_t suppressCode)
{
	if (!isUndoOn())
	{
		RVNG_DEBUG_MSG(("WordPerfect: suppressPageCharacteristics (suppressCode: %u)\n", suppressCode));
		if (suppressCode & WP3_PAGE_GROUP_SUPPRESS_HEADER_A)
			m_currentPage.setHeadFooterSuppression(RVNG_HEADER_A, true);
		if (suppressCode & WP3_PAGE_GROUP_SUPPRESS_HEADER_B)
			m_currentPage.setHeadFooterSuppression(RVNG_HEADER_B, true);
		if (suppressCode & WP3_PAGE_GROUP_SUPPRESS_FOOTER_A)
			m_currentPage.setHeadFooterSuppression(RVNG_FOOTER_A, true);
		if (suppressCode & WP3_PAGE_GROUP_SUPPRESS_FOOTER_B)
			m_currentPage.setHeadFooterSuppression(RVNG_FOOTER_B, true);
	}
}

void WP3StylesListener::startTable()
{
	if (!isUndoOn())
	{
		m_currentPageHasContent = true;
		m_currentTable = new RVNGTable();
		m_tableList.add(m_currentTable);
	}
}

void WP3StylesListener::insertRow()
{
	if (!isUndoOn())
	{
		m_currentPageHasContent = true;
		m_currentTable->insertRow();
	}
}

void WP3StylesListener::insertCell()
{
	if (!isUndoOn())
	{
		m_currentPageHasContent = true;
#if 0
		m_currentTable->insertCell(colSpan, rowSpan, borderBits);
#endif
	}
}

void WP3StylesListener::_handleSubDocument(const RVNGSubDocument *subDocument, RVNGSubDocumentType subDocumentType,
        RVNGTableList tableList, int /* nextTableIndice */)
{
	bool oldIsSubDocument = m_isSubDocument;
	m_isSubDocument = true;
	bool oldIsUndoOn = isUndoOn();
	if (subDocumentType == RVNG_SUBDOCUMENT_HEADER_FOOTER)
	{
		bool oldCurrentPageHasContent = m_currentPageHasContent;
		RVNGTable *oldCurrentTable = m_currentTable;
		RVNGTableList oldTableList = m_tableList;
		m_tableList = tableList;

		if (subDocument)
			static_cast<const WP3SubDocument *>(subDocument)->parse(this);

		m_tableList = oldTableList;
		m_currentTable = oldCurrentTable;
		m_currentPageHasContent = oldCurrentPageHasContent;
	}
	else
	{
		if (subDocument)
			static_cast<const WP3SubDocument *>(subDocument)->parse(this);
	}
	m_isSubDocument = oldIsSubDocument;
	setUndoOn(oldIsUndoOn);
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
