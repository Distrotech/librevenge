/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2003 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2003 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#include "WP6StylesListener.h"
#include "WPXTable.h"
#include "WP6FileStructure.h"
#include "WPXFileStructure.h"
#include "libwpd_internal.h"
#include "WP6SubDocument.h"
#include "WP6PrefixDataPacket.h"

// WP6StylesListener: creates intermediate table and page span representations, given a
// sequence of messages passed to it by the parser.

WP6StylesListener::WP6StylesListener(std::list<WPXPageSpan> &pageList, WPXTableList tableList) :
	WP6Listener(),
	WPXStylesListener(pageList),
	m_currentPage(),
	m_tableList(tableList),
	m_currentTable(0),
	m_tempMarginLeft(1.0),
	m_tempMarginRight(1.0),
	m_currentPageHasContent(false),
	m_isTableDefined(false),
	m_isSubDocument(false),
	m_subDocuments(),
	m_pageListHardPageMark(m_pageList.end())
{
}

void WP6StylesListener::endDocument()
{
	insertBreak(WPX_SOFT_PAGE_BREAK); // pretend we just had a soft page break (for the last page)
}

void WP6StylesListener::endSubDocument()
{
	insertBreak(WPX_SOFT_PAGE_BREAK); // pretend we just had a soft page break (for the last page)
}

void WP6StylesListener::insertBreak(const uint8_t breakType)
{
	if (m_isSubDocument)
		return;

	if (!isUndoOn())
	{
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
				m_pageList.push_back(m_currentPage);
				if (m_pageListHardPageMark == m_pageList.end())
					--m_pageListHardPageMark;
			}
			m_currentPage = WPXPageSpan(m_pageList.back(), 0.0, 0.0);
			m_currentPage.setPageSpan(1);
			m_currentPageHasContent = false;
			break;
		default:
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

void WP6StylesListener::pageNumberingChange(const WPXPageNumberPosition pageNumberingPosition, const uint16_t pageNumberFontPointSize, const uint16_t pageNumberFontPID)
{
	if (!isUndoOn())
	{
		m_currentPage.setPageNumberPosition(pageNumberingPosition);

		if (pageNumberFontPID)
		{
			WPXString pidFontName = WP6Listener::getFontNameForPID(pageNumberFontPID);
			if (!!pidFontName)
				m_currentPage.setPageNumberingFontName(pidFontName);
		}

		m_currentPage.setPageNumberingFontSize(wpuToFontPointSize(pageNumberFontPointSize));
	}
}

void WP6StylesListener::pageMarginChange(const uint8_t side, const uint16_t margin)
{
	if (!isUndoOn())
	{
		double marginInch = (double)((double)margin / (double)WPX_NUM_WPUS_PER_INCH);
		switch(side)
		{
		case WPX_TOP:
			m_currentPage.setMarginTop(marginInch);
			break;
		case WPX_BOTTOM:
			m_currentPage.setMarginBottom(marginInch);
			break;
		default:
			break;
		}
	}
}

void WP6StylesListener::pageFormChange(const uint16_t length, const uint16_t width, const WPXFormOrientation orientation)
{
	if (!isUndoOn())
	{
		double lengthInch = (double)((double)length / (double)WPX_NUM_WPUS_PER_INCH);
		double widthInch = (double)((double)width / (double)WPX_NUM_WPUS_PER_INCH);
		if (!m_currentPageHasContent)
		{
			m_currentPage.setFormLength(lengthInch);
			m_currentPage.setFormWidth(widthInch);
			m_currentPage.setFormOrientation(orientation);
		}
	}
}

void WP6StylesListener::marginChange(const uint8_t side, const uint16_t margin)
{
	if (!isUndoOn())
	{
		if (m_isSubDocument)
			return; // do not deal with L/R margins in headers, footer and notes

		std::list<WPXPageSpan>::iterator Iter;
		double marginInch = (double)((double)margin / (double)WPX_NUM_WPUS_PER_INCH);
		switch(side)
		{
		case WPX_LEFT:
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
		case WPX_RIGHT:
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

void WP6StylesListener::headerFooterGroup(const uint8_t headerFooterType, const uint8_t occurenceBits, const uint16_t textPID)
{
	if (!isUndoOn())
	{
		WPD_DEBUG_MSG(("WordPerfect: headerFooterGroup (headerFooterType: %i, occurenceBits: %i, textPID: %i)\n",
		               headerFooterType, occurenceBits, textPID));
		bool tempCurrentPageHasContent = m_currentPageHasContent;
		if (headerFooterType <= WP6_HEADER_FOOTER_GROUP_FOOTER_B) // ignore watermarks for now
		{
			WPXHeaderFooterType wpxType = ((headerFooterType <= WP6_HEADER_FOOTER_GROUP_HEADER_B) ? HEADER : FOOTER);

			WPXHeaderFooterOccurence wpxOccurence;
			if (occurenceBits & WP6_HEADER_FOOTER_GROUP_EVEN_BIT && occurenceBits & WP6_HEADER_FOOTER_GROUP_ODD_BIT)
				wpxOccurence = ALL;
			else if (occurenceBits & WP6_HEADER_FOOTER_GROUP_EVEN_BIT)
				wpxOccurence = EVEN;
			else
				wpxOccurence = ODD;

			WPXTableList tableList;
			m_currentPage.setHeaderFooter(wpxType, headerFooterType, wpxOccurence,
			                              ((textPID && WP6Listener::getPrefixDataPacket(textPID)) ? WP6Listener::getPrefixDataPacket(textPID)->getSubDocument() : 0), tableList);
			_handleSubDocument(((textPID && WP6Listener::getPrefixDataPacket(textPID)) ? WP6Listener::getPrefixDataPacket(textPID)->getSubDocument() : 0), WPX_SUBDOCUMENT_HEADER_FOOTER, tableList);
		}
		m_currentPageHasContent = tempCurrentPageHasContent;
	}
}

void WP6StylesListener::suppressPageCharacteristics(const uint8_t suppressCode)
{
	if (!isUndoOn())
	{
		WPD_DEBUG_MSG(("WordPerfect: suppressPageCharacteristics (suppressCode: %u)\n", suppressCode));

		if (suppressCode & WP6_PAGE_GROUP_SUPPRESS_PAGE_NUMBER)
			m_currentPage.setPageNumberSuppression(true);

		if (suppressCode & WP6_PAGE_GROUP_SUPPRESS_HEADER_A)
			m_currentPage.setHeadFooterSuppression(WPX_HEADER_A, true);
		if (suppressCode & WP6_PAGE_GROUP_SUPPRESS_HEADER_B)
			m_currentPage.setHeadFooterSuppression(WPX_HEADER_B, true);
		if (suppressCode & WP6_PAGE_GROUP_SUPPRESS_FOOTER_A)
			m_currentPage.setHeadFooterSuppression(WPX_FOOTER_A, true);
		if (suppressCode & WP6_PAGE_GROUP_SUPPRESS_FOOTER_B)
			m_currentPage.setHeadFooterSuppression(WPX_FOOTER_B, true);
	}
}

void WP6StylesListener::setPageNumber(const uint16_t pageNumber)
{
	if (!isUndoOn())
	{
		m_currentPage.setPageNumber(pageNumber);
	}
}

void WP6StylesListener::setPageNumberingType(const WPXNumberingType pageNumberingType)
{
	if (!isUndoOn())
	{
		m_currentPage.setPageNumberingType(pageNumberingType);
	}
}

void WP6StylesListener::defineTable(const uint8_t /* position */, const uint16_t /* leftOffset */)
{
	if (!isUndoOn())
	{
		m_currentPageHasContent = true;
		m_currentTable = new WPXTable();
		m_tableList.add(m_currentTable);
		m_isTableDefined = true;
	}
}

void WP6StylesListener::startTable()
{
	if (!isUndoOn() && !m_isTableDefined)
	{
		m_currentPageHasContent = true;
		m_currentTable = new WPXTable();
		m_tableList.add(m_currentTable);
		m_isTableDefined = false;
	}

}

void WP6StylesListener::endTable()
{
	if (!isUndoOn())
	{
		m_isTableDefined = false;
		m_currentTable = 0;
	}
}

void WP6StylesListener::insertRow(const uint16_t /* rowHeight */, const bool /* isMinimumHeight */, const bool /* isHeaderRow */)
{
	if (!isUndoOn() && m_currentTable)
	{
		m_currentPageHasContent = true;
		m_currentTable->insertRow();
	}
}

void WP6StylesListener::insertCell(const uint8_t colSpan, const uint8_t rowSpan, const uint8_t borderBits,
                                   const RGBSColor * /* cellFgColor */, const RGBSColor * /* cellBgColor */,
                                   const RGBSColor * /* cellBorderColor */, const WPXVerticalAlignment /* cellVerticalAlignment */,
                                   const bool /* useCellAttributes */, const uint32_t /* cellAttributes */)
{
	if (!isUndoOn() && m_currentTable)
	{
		m_currentPageHasContent = true;
		m_currentTable->insertCell(colSpan, rowSpan, borderBits);
	}
}

void WP6StylesListener::noteOn(const uint16_t textPID)
{
	if (!isUndoOn())
	{
		m_currentPageHasContent = true;
		_handleSubDocument(((textPID && WP6Listener::getPrefixDataPacket(textPID)) ? WP6Listener::getPrefixDataPacket(textPID)->getSubDocument() : 0), WPX_SUBDOCUMENT_NOTE, m_tableList);
	}
}

void WP6StylesListener::insertTextBox(const WP6SubDocument *subDocument)
{
	if (!isUndoOn() && subDocument)
	{
		m_currentPageHasContent = true;
		_handleSubDocument(subDocument, WPX_SUBDOCUMENT_TEXT_BOX, m_tableList);
	}
}

void WP6StylesListener::commentAnnotation(const uint16_t textPID)
{
	if (!isUndoOn())
	{
		m_currentPageHasContent = true;
		_handleSubDocument(((textPID && WP6Listener::getPrefixDataPacket(textPID)) ? WP6Listener::getPrefixDataPacket(textPID)->getSubDocument() : 0), WPX_SUBDOCUMENT_COMMENT_ANNOTATION, m_tableList);
	}
}

void WP6StylesListener::_handleSubDocument(const WPXSubDocument *subDocument, WPXSubDocumentType subDocumentType, WPXTableList tableList,
        int /* nextTableIndice */)
{
	// We don't want to actual insert anything in the case of a sub-document, but we
	// do want to capture whatever table-related information is within it..
	std::set <const WPXSubDocument *> oldSubDocuments;
	oldSubDocuments = m_subDocuments;
	// prevent entering in an endless loop
	if ((subDocument) && (oldSubDocuments.find(subDocument) == oldSubDocuments.end()))
	{
		m_subDocuments.insert(subDocument);
		bool oldIsSubDocument = m_isSubDocument;
		m_isSubDocument = true;
		WPXTable *oldCurrentTable = m_currentTable;
		if (subDocumentType == WPX_SUBDOCUMENT_HEADER_FOOTER)
		{
			bool oldCurrentPageHasContent = m_currentPageHasContent;
			WPXTableList oldTableList = m_tableList;
			m_tableList = tableList;

			if (subDocument)
				static_cast<const WP6SubDocument *>(subDocument)->parse(this);
			endTable();

			m_tableList = oldTableList;
			m_currentPageHasContent = oldCurrentPageHasContent;
		}
		else
		{
			if (subDocument)
				static_cast<const WP6SubDocument *>(subDocument)->parse(this);
			endTable();
		}
		m_currentTable = oldCurrentTable;
		m_isSubDocument = oldIsSubDocument;
		m_subDocuments = oldSubDocuments;
	}
}

void WP6StylesListener::undoChange(const uint8_t undoType, const uint16_t /* undoLevel */)
{
	if (undoType == WP6_UNDO_GROUP_INVALID_TEXT_START)
		setUndoOn(true);
	else if (undoType == WP6_UNDO_GROUP_INVALID_TEXT_END)
		setUndoOn(false);
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
