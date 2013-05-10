/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
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

#include "WPXContentListener.h"
#include "WPXPageSpan.h"
#include "libwpd_internal.h"
#include <limits>

_WPXContentParsingState::_WPXContentParsingState() :
	m_textAttributeBits(0),
	m_fontSize(12.0/*WP6_DEFAULT_FONT_SIZE*/), // FIXME ME!!!!!!!!!!!!!!!!!!! HELP WP6_DEFAULT_FONT_SIZE
	m_fontName(new WPXString(/*WP6_DEFAULT_FONT_NAME*/"Times New Roman")), // EN PAS DEFAULT FONT AAN VOOR WP5/6/etc
	m_fontColor(new RGBSColor(0x00,0x00,0x00,0x64)), //Set default to black. Maybe once it will change, but for the while...
	m_highlightColor(0),

	m_isParagraphColumnBreak(false),
	m_isParagraphPageBreak(false),
	m_paragraphJustification(WPX_PARAGRAPH_JUSTIFICATION_LEFT),
	m_tempParagraphJustification(0),
	m_paragraphLineSpacing(1.0),

	m_isDocumentStarted(false),
	m_isPageSpanOpened(false),
	m_isSectionOpened(false),
	m_isPageSpanBreakDeferred(false),
	m_isHeaderFooterWithoutParagraph(false),

	m_isSpanOpened(false),
	m_isParagraphOpened(false),
	m_isListElementOpened(false),

	m_firstParagraphInPageSpan(true),

	m_numRowsToSkip(),
	m_tableDefinition(),
	m_currentTableCol(0),
	m_currentTableRow(0),
	m_currentTableCellNumberInRow(0),
	m_isTableOpened(false),
	m_isTableRowOpened(false),
	m_isTableColumnOpened(false),
	m_isTableCellOpened(false),
	m_wasHeaderRow(false),
	m_isCellWithoutParagraph(false),
	m_isRowWithoutCell(false),
	m_cellAttributeBits(0x00000000),
	m_paragraphJustificationBeforeTable(WPX_PARAGRAPH_JUSTIFICATION_LEFT),

	m_currentPage(0),
	m_numPagesRemainingInSpan(0),
	m_currentPageNumber(1),

	m_sectionAttributesChanged(false),
	m_numColumns(1),
	m_textColumns(),
	m_isTextColumnWithoutParagraph(false),

	m_pageFormLength(11.0),
	m_pageFormWidth(8.5f),
	m_pageFormOrientation(PORTRAIT),

	m_pageMarginLeft(1.0),
	m_pageMarginRight(1.0),
	m_pageMarginTop(1.0),
	m_pageMarginBottom(1.0),

	m_paragraphMarginLeft(0.0),
	m_paragraphMarginRight(0.0),
	m_paragraphMarginTop(0.0),
	m_paragraphMarginBottom(0.0),
	m_leftMarginByPageMarginChange(0.0),
	m_rightMarginByPageMarginChange(0.0),
	m_sectionMarginLeft(0.0),
	m_sectionMarginRight(0.0),
	m_leftMarginByParagraphMarginChange(0.0),
	m_rightMarginByParagraphMarginChange(0.0),
	m_leftMarginByTabs(0.0),
	m_rightMarginByTabs(0.0),

	m_listReferencePosition(0.0),
	m_listBeginPosition(0.0),

	m_paragraphTextIndent(0.0),
	m_textIndentByParagraphIndentChange(0.0),
	m_textIndentByTabs(0.0),
	m_currentListLevel(0),

	m_alignmentCharacter('.'),
	m_tabStops(),
	m_isTabPositionRelative(false),

	m_subDocuments(),

	m_inSubDocument(false),
	m_isNote(false),
	m_subDocumentType(WPX_SUBDOCUMENT_NONE)
{
}

_WPXContentParsingState::~_WPXContentParsingState()
{
	DELETEP(m_fontName);
	DELETEP(m_fontColor);
	DELETEP(m_highlightColor);
}

WPXContentListener::WPXContentListener(std::list<WPXPageSpan> &pageList, WPXDocumentInterface *documentInterface) :
	WPXListener(pageList),
	m_ps(new WPXContentParsingState),
	m_documentInterface(documentInterface),
	m_metaData()
{
}

WPXContentListener::~WPXContentListener()
{
	DELETEP(m_ps);
}

void WPXContentListener::startDocument()
{
	if (!m_ps->m_isDocumentStarted)
	{
		// FIXME: this is stupid, we should store a property list filled with the relevant metadata
		// and then pass that directly..

		m_documentInterface->setDocumentMetaData(m_metaData);

		m_documentInterface->startDocument();
	}

	m_ps->m_isDocumentStarted = true;
}

void WPXContentListener::startSubDocument()
{
	m_ps->m_isDocumentStarted = true;
	m_ps->m_inSubDocument = true;
}

void WPXContentListener::endDocument()
{
	if (!m_ps->m_isPageSpanOpened)
		_openSpan();

	if (m_ps->m_isTableOpened)
		_closeTable();
	if (m_ps->m_isParagraphOpened)
		_closeParagraph();
	if (m_ps->m_isListElementOpened)
		_closeListElement();

	m_ps->m_currentListLevel = 0;
	_changeList(); // flush the list exterior

	// close the document nice and tight
	_closeSection();
	_closePageSpan();
	m_documentInterface->endDocument();
}

void WPXContentListener::endSubDocument()
{
	if (m_ps->m_isTableOpened)
		_closeTable();
	if (m_ps->m_isParagraphOpened)
		_closeParagraph();
	if (m_ps->m_isListElementOpened)
		_closeListElement();

	m_ps->m_currentListLevel = 0;
	_changeList(); // flush the list exterior
}

void WPXContentListener::_openSection()
{
	if (!m_ps->m_isSectionOpened)
	{
		if (!m_ps->m_isPageSpanOpened)
			_openPageSpan();

		WPXPropertyList propList;

		propList.insert("fo:margin-left", m_ps->m_sectionMarginLeft);
		propList.insert("fo:margin-right", m_ps->m_sectionMarginRight);
		if (m_ps->m_numColumns > 1)
		{
			propList.insert("libwpd:margin-bottom", 1.0);
			propList.insert("text:dont-balance-text-columns", false);
		}
		else
			propList.insert("libwpd:margin-bottom", 0.0);

		WPXPropertyListVector columns;
		typedef std::vector<WPXColumnDefinition>::const_iterator CDVIter;
		for (CDVIter iter = m_ps->m_textColumns.begin(); iter != m_ps->m_textColumns.end(); ++iter)
		{
			WPXPropertyList column;
			// The "style:rel-width" is expressed in twips (1440 twips per inch) and includes the left and right Gutter
			column.insert("style:rel-width", (*iter).m_width * 1440.0, WPX_TWIP);
			column.insert("fo:start-indent", (*iter).m_leftGutter);
			column.insert("fo:end-indent", (*iter).m_rightGutter);
			columns.append(column);
		}
		if (!m_ps->m_isSectionOpened)
			m_documentInterface->openSection(propList, columns);

		m_ps->m_sectionAttributesChanged = false;
		m_ps->m_isSectionOpened = true;
	}
}

void WPXContentListener::_closeSection()
{
	if (m_ps->m_isSectionOpened && !m_ps->m_isTableOpened)
	{
		if (m_ps->m_isParagraphOpened)
			_closeParagraph();
		if (m_ps->m_isListElementOpened)
			_closeListElement();
		_changeList();

		m_documentInterface->closeSection();

		m_ps->m_sectionAttributesChanged = false;
		m_ps->m_isSectionOpened = false;
	}
}

void WPXContentListener::_insertPageNumberParagraph(WPXPageNumberPosition position, WPXNumberingType numberingType, WPXString fontName, double fontSize)
{
	WPXPropertyList propList;
	switch (position)
	{
	case PAGENUMBER_POSITION_TOP_LEFT:
	case PAGENUMBER_POSITION_BOTTOM_LEFT:
		// doesn't require a paragraph prop - it is the default
		propList.insert("fo:text-align", "left");
		break;
	case PAGENUMBER_POSITION_TOP_RIGHT:
	case PAGENUMBER_POSITION_BOTTOM_RIGHT:
		propList.insert("fo:text-align", "end");
		break;
	case PAGENUMBER_POSITION_TOP_CENTER:
	case PAGENUMBER_POSITION_BOTTOM_CENTER:
	case PAGENUMBER_POSITION_NONE:
	case PAGENUMBER_POSITION_TOP_LEFT_AND_RIGHT:
	case PAGENUMBER_POSITION_BOTTOM_LEFT_AND_RIGHT:
	case PAGENUMBER_POSITION_TOP_INSIDE_LEFT_AND_RIGHT:
	case PAGENUMBER_POSITION_BOTTOM_INSIDE_LEFT_AND_RIGHT:
	default:
		propList.insert("fo:text-align", "center");
		break;
	}

	m_documentInterface->openParagraph(propList, WPXPropertyListVector());

	propList.clear();
	propList.insert("style:font-name", fontName.cstr());
	propList.insert("fo:font-size", fontSize, WPX_POINT);
	m_documentInterface->openSpan(propList);


	propList.clear();
	propList.insert("style:num-format", _numberingTypeToString(numberingType));
	m_documentInterface->insertField(WPXString("text:page-number"), propList);

	propList.clear();
	m_documentInterface->closeSpan();

	m_documentInterface->closeParagraph();
}

void WPXContentListener::_openPageSpan()
{
	if (m_ps->m_isPageSpanOpened)
		return;

	if (!m_ps->m_isDocumentStarted)
		startDocument();

	// Hack to be sure that the paragraph margins are consistent even if the page margin changes
	if (m_ps->m_leftMarginByPageMarginChange != 0)
		m_ps->m_leftMarginByPageMarginChange += m_ps->m_pageMarginLeft;
	if (m_ps->m_rightMarginByPageMarginChange != 0)
		m_ps->m_rightMarginByPageMarginChange += m_ps->m_pageMarginRight;
	if (m_ps->m_sectionMarginLeft != 0)
		m_ps->m_sectionMarginLeft += m_ps->m_pageMarginLeft;
	if (m_ps->m_sectionMarginRight != 0)
		m_ps->m_sectionMarginRight += m_ps->m_pageMarginRight;
	m_ps->m_listReferencePosition += m_ps->m_pageMarginLeft;
	m_ps->m_listBeginPosition += m_ps->m_pageMarginLeft;

	if ( m_pageList.empty() || (m_ps->m_currentPage >= m_pageList.size()) )
	{
		WPD_DEBUG_MSG(("m_pageList.empty() || (m_ps->m_currentPage >= m_pageList.size())\n"));
		throw ParseException();
	}

	std::list<WPXPageSpan>::iterator currentPageSpanIter = m_pageList.begin();
	for ( unsigned i = 0; i < m_ps->m_currentPage; i++ )
		++currentPageSpanIter;

	WPXPageSpan currentPage = (*currentPageSpanIter);

	WPXPropertyList propList;
	propList.insert("libwpd:num-pages", currentPage.getPageSpan());

	propList.insert("libwpd:is-last-page-span", ((m_ps->m_currentPage + 1 == m_pageList.size()) ? true : false));
	propList.insert("fo:page-height", currentPage.getFormLength());
	propList.insert("fo:page-width", currentPage.getFormWidth());
	if (currentPage.getFormOrientation() == LANDSCAPE)
		propList.insert("style:print-orientation", "landscape");
	else
		propList.insert("style:print-orientation", "portrait");
	propList.insert("fo:margin-left", currentPage.getMarginLeft());
	propList.insert("fo:margin-right", currentPage.getMarginRight());
	propList.insert("fo:margin-top", currentPage.getMarginTop());
	propList.insert("fo:margin-bottom", currentPage.getMarginBottom());

	if (!m_ps->m_isPageSpanOpened)
		m_documentInterface->openPageSpan(propList);

	m_ps->m_isPageSpanOpened = true;

	m_ps->m_pageFormWidth = currentPage.getFormWidth();
	m_ps->m_pageMarginLeft = currentPage.getMarginLeft();
	m_ps->m_pageMarginRight = currentPage.getMarginRight();
	m_ps->m_pageMarginTop = currentPage.getMarginTop();
	m_ps->m_pageMarginBottom = currentPage.getMarginBottom();

	// Hack to be sure that the paragraph margins are consistent even if the page margin changes
	// Compute new values
	if (m_ps->m_leftMarginByPageMarginChange != 0)
		m_ps->m_leftMarginByPageMarginChange -= m_ps->m_pageMarginLeft;
	if (m_ps->m_rightMarginByPageMarginChange != 0)
		m_ps->m_rightMarginByPageMarginChange -= m_ps->m_pageMarginRight;
	if (m_ps->m_sectionMarginLeft != 0)
		m_ps->m_sectionMarginLeft -= m_ps->m_pageMarginLeft;
	if (m_ps->m_sectionMarginRight != 0)
		m_ps->m_sectionMarginRight -= m_ps->m_pageMarginRight;
	m_ps->m_listReferencePosition -= m_ps->m_pageMarginLeft;
	m_ps->m_listBeginPosition -= m_ps->m_pageMarginLeft;

	m_ps->m_paragraphMarginLeft = m_ps->m_leftMarginByPageMarginChange + m_ps->m_leftMarginByParagraphMarginChange
	                              + m_ps->m_leftMarginByTabs;
	m_ps->m_paragraphMarginRight = m_ps->m_rightMarginByPageMarginChange + m_ps->m_rightMarginByParagraphMarginChange
	                               + m_ps->m_rightMarginByTabs;


	// we insert page numbers by inserting them into the header/footer of the wordperfect document. if we don't wind up
	// inserting a header/footer to encapsulate them inside, it will be necessary to invent one just for this purpose
	bool pageNumberInserted = false;

	std::vector<WPXHeaderFooter> headerFooterList = currentPage.getHeaderFooterList();
	for (std::vector<WPXHeaderFooter>::iterator iter = headerFooterList.begin(); iter != headerFooterList.end(); ++iter)
	{
		if (((*iter).getOccurence() != NEVER) && !currentPage.getHeaderFooterSuppression((*iter).getInternalType()))
		{
			propList.clear();
			switch ((*iter).getOccurence())
			{
			case ODD:
				propList.insert("libwpd:occurence", "odd");
				break;
			case EVEN:
				propList.insert("libwpd:occurence", "even");
				break;
			case ALL:
				propList.insert("libwpd:occurence", "all");
				break;
			case NEVER:
			default:
				break;
			}

			if ((*iter).getType() == HEADER)
			{
				m_documentInterface->openHeader(propList);
				if (!currentPage.getPageNumberSuppression() &&
				        ((currentPage.getPageNumberPosition() >= PAGENUMBER_POSITION_TOP_LEFT &&
				          currentPage.getPageNumberPosition() <= PAGENUMBER_POSITION_TOP_LEFT_AND_RIGHT) ||
				         currentPage.getPageNumberPosition() == PAGENUMBER_POSITION_TOP_INSIDE_LEFT_AND_RIGHT))
				{
					_insertPageNumberParagraph(currentPage.getPageNumberPosition(), currentPage.getPageNumberingType(),
					                           currentPage.getPageNumberingFontName(), currentPage.getPageNumberingFontSize());
					pageNumberInserted = true;
				}
			}
			else
				m_documentInterface->openFooter(propList);

			WPD_DEBUG_MSG(("Header Footer Element: Starting to parse the subDocument\n"));
			handleSubDocument((*iter).getSubDocument(), WPX_SUBDOCUMENT_HEADER_FOOTER, (*iter).getTableList(), 0);
			WPD_DEBUG_MSG(("Header Footer Element: End of the subDocument parsing\n"));
			if ((*iter).getType() == HEADER)
				m_documentInterface->closeHeader();
			else
			{
				if (currentPage.getPageNumberPosition() >= PAGENUMBER_POSITION_BOTTOM_LEFT &&
				        currentPage.getPageNumberPosition() != PAGENUMBER_POSITION_TOP_INSIDE_LEFT_AND_RIGHT &&
				        !currentPage.getPageNumberSuppression())
				{
					_insertPageNumberParagraph(currentPage.getPageNumberPosition(), currentPage.getPageNumberingType(),
					                           currentPage.getPageNumberingFontName(), currentPage.getPageNumberingFontSize());
					pageNumberInserted = true;
				}
				m_documentInterface->closeFooter();
			}

			WPD_DEBUG_MSG(("Header Footer Element: type: %i occurence: %i\n",
			               (*iter).getType(), (*iter).getOccurence()));
		}
	}

	if (!pageNumberInserted && currentPage.getPageNumberPosition() != PAGENUMBER_POSITION_NONE && !currentPage.getPageNumberSuppression())
	{
		if (currentPage.getPageNumberPosition() >= PAGENUMBER_POSITION_BOTTOM_LEFT &&
		        currentPage.getPageNumberPosition() != PAGENUMBER_POSITION_TOP_INSIDE_LEFT_AND_RIGHT)
		{
			propList.clear();
			propList.insert("libwpd:occurence", "all");
			m_documentInterface->openFooter(propList);
			_insertPageNumberParagraph(currentPage.getPageNumberPosition(), currentPage.getPageNumberingType(),
			                           currentPage.getPageNumberingFontName(), currentPage.getPageNumberingFontSize());
			m_documentInterface->closeFooter();
		}
		else
		{
			propList.clear();
			propList.insert("libwpd:occurence", "all");
			m_documentInterface->openHeader(propList);
			_insertPageNumberParagraph(currentPage.getPageNumberPosition(), currentPage.getPageNumberingType(),
			                           currentPage.getPageNumberingFontName(), currentPage.getPageNumberingFontSize());
			m_documentInterface->closeHeader();
		}
	}

	// first paragraph in span (necessary for resetting page number)
	m_ps->m_firstParagraphInPageSpan = true;

	/* Some of this would maybe not be necessary, but it does not do any harm
	 * and apparently solves some troubles */
	m_ps->m_pageFormLength = currentPage.getFormLength();
	m_ps->m_pageFormWidth = currentPage.getFormWidth();
	m_ps->m_pageFormOrientation = currentPage.getFormOrientation();
	m_ps->m_pageMarginLeft = currentPage.getMarginLeft();
	m_ps->m_pageMarginRight = currentPage.getMarginRight();

	m_ps->m_paragraphMarginLeft = m_ps->m_leftMarginByPageMarginChange + m_ps->m_leftMarginByParagraphMarginChange
	                              + m_ps->m_leftMarginByTabs;
	m_ps->m_paragraphMarginRight = m_ps->m_rightMarginByPageMarginChange + m_ps->m_rightMarginByParagraphMarginChange
	                               + m_ps->m_rightMarginByTabs;

	m_ps->m_paragraphTextIndent = m_ps->m_textIndentByParagraphIndentChange + m_ps->m_textIndentByTabs;

	m_ps->m_numPagesRemainingInSpan = (unsigned)(currentPage.getPageSpan() - 1);
	m_ps->m_currentPage++;
}

void WPXContentListener::_closePageSpan()
{
	if (m_ps->m_isPageSpanOpened)
	{
		if (m_ps->m_isSectionOpened)
			_closeSection();

		m_documentInterface->closePageSpan();
	}

	m_ps->m_isPageSpanOpened = false;
	m_ps->m_isPageSpanBreakDeferred = false;
}

void WPXContentListener::_openParagraph()
{
	if (m_ps->m_isTableOpened && !m_ps->m_isTableCellOpened)
		return;

	if (!m_ps->m_isParagraphOpened && !m_ps->m_isListElementOpened)
	{
		if (!m_ps->m_isTableOpened && (!m_ps->m_inSubDocument || m_ps->m_subDocumentType == WPX_SUBDOCUMENT_TEXT_BOX))
		{
			if (m_ps->m_sectionAttributesChanged)
				_closeSection();

			if (!m_ps->m_isSectionOpened)
				_openSection();
		}

		WPXPropertyListVector tabStops;
		_getTabStops(tabStops);

		WPXPropertyList propList;
		_appendParagraphProperties(propList);

		if (!m_ps->m_isParagraphOpened)
			m_documentInterface->openParagraph(propList, tabStops);

		_resetParagraphState();
		m_ps->m_firstParagraphInPageSpan = false;
	}
}

void WPXContentListener::_resetParagraphState(const bool isListElement)
{
	m_ps->m_isParagraphColumnBreak = false;
	m_ps->m_isParagraphPageBreak = false;
	if (isListElement)
	{
		m_ps->m_isListElementOpened = true;
		m_ps->m_isParagraphOpened = false;
	}
	else
	{
		m_ps->m_isListElementOpened = false;
		m_ps->m_isParagraphOpened = true;
	}
	m_ps->m_paragraphMarginLeft = m_ps->m_leftMarginByPageMarginChange + m_ps->m_leftMarginByParagraphMarginChange;
	m_ps->m_paragraphMarginRight = m_ps->m_rightMarginByPageMarginChange + m_ps->m_rightMarginByParagraphMarginChange;
	m_ps->m_leftMarginByTabs = 0.0;
	m_ps->m_rightMarginByTabs = 0.0;
	m_ps->m_paragraphTextIndent = m_ps->m_textIndentByParagraphIndentChange;
	m_ps->m_textIndentByTabs = 0.0;
	m_ps->m_isCellWithoutParagraph = false;
	m_ps->m_isTextColumnWithoutParagraph = false;
	m_ps->m_isHeaderFooterWithoutParagraph = false;
	m_ps->m_tempParagraphJustification = 0;
	m_ps->m_listReferencePosition = m_ps->m_paragraphMarginLeft + m_ps->m_paragraphTextIndent;
	m_ps->m_listBeginPosition = m_ps->m_paragraphMarginLeft + m_ps->m_paragraphTextIndent;
}

void WPXContentListener::_appendJustification(WPXPropertyList &propList, int justification)
{
	switch (justification)
	{
	case WPX_PARAGRAPH_JUSTIFICATION_LEFT:
		// doesn't require a paragraph prop - it is the default
		propList.insert("fo:text-align", "left");
		break;
	case WPX_PARAGRAPH_JUSTIFICATION_CENTER:
		propList.insert("fo:text-align", "center");
		break;
	case WPX_PARAGRAPH_JUSTIFICATION_RIGHT:
		propList.insert("fo:text-align", "end");
		break;
	case WPX_PARAGRAPH_JUSTIFICATION_FULL:
		propList.insert("fo:text-align", "justify");
		break;
	case WPX_PARAGRAPH_JUSTIFICATION_FULL_ALL_LINES:
		propList.insert("fo:text-align", "justify");
		propList.insert("fo:text-align-last", "justify");
		break;
	default:
		break;
	}
}

void WPXContentListener::_appendParagraphProperties(WPXPropertyList &propList, const bool isListElement)
{
	int justification;
	if (m_ps->m_tempParagraphJustification)
		justification = m_ps->m_tempParagraphJustification;
	else
		justification = m_ps->m_paragraphJustification;
	_appendJustification(propList, justification);

	if (!m_ps->m_isTableOpened)
	{
		// these properties are not appropriate when a table is opened..
		if (isListElement)
		{
			propList.insert("fo:margin-left", (m_ps->m_listBeginPosition - m_ps->m_paragraphTextIndent));
			propList.insert("fo:text-indent", m_ps->m_paragraphTextIndent);
		}
		else
		{
			propList.insert("fo:margin-left", m_ps->m_paragraphMarginLeft);
			propList.insert("fo:text-indent", m_ps->m_listReferencePosition - m_ps->m_paragraphMarginLeft);
		}
		propList.insert("fo:margin-right", m_ps->m_paragraphMarginRight);
	}
	propList.insert("fo:margin-top", m_ps->m_paragraphMarginTop);
	propList.insert("fo:margin-bottom", m_ps->m_paragraphMarginBottom);
	propList.insert("fo:line-height", m_ps->m_paragraphLineSpacing, WPX_PERCENT);

	if (!m_ps->m_inSubDocument && m_ps->m_firstParagraphInPageSpan)
	{
		std::list<WPXPageSpan>::iterator currentPageSpanIter = m_pageList.begin();
		for ( unsigned i = 0; i < (unsigned)(m_ps->m_currentPage - 1); i+=(unsigned)(*currentPageSpanIter).getPageSpan())
			++currentPageSpanIter;

		WPXPageSpan currentPage = (*currentPageSpanIter);
		if (currentPage.getPageNumberOverriden())
			propList.insert("style:page-number", currentPage.getPageNumberOverride());
	}

	_insertBreakIfNecessary(propList);
}

void WPXContentListener::_insertText(const WPXString &textBuffer)
{
	if (textBuffer.len() <= 0)
		return;

	WPXString tmpText;
	const char ASCII_SPACE = 0x0020;

	int numConsecutiveSpaces = 0;
	WPXString::Iter i(textBuffer);
	for (i.rewind(); i.next();)
	{
		if (*(i()) == ASCII_SPACE)
			numConsecutiveSpaces++;
		else
			numConsecutiveSpaces = 0;

		if (numConsecutiveSpaces > 1)
		{
			if (tmpText.len() > 0)
			{
				m_documentInterface->insertText(tmpText);
				tmpText.clear();
			}

			m_documentInterface->insertSpace();
		}
		else
		{
			tmpText.append(i());
		}
	}

	m_documentInterface->insertText(tmpText);
}

void WPXContentListener::_insertBreakIfNecessary(WPXPropertyList &propList)
{
	if (m_ps->m_isParagraphPageBreak && !m_ps->m_inSubDocument) // no hard page-breaks in subdocuments
		propList.insert("fo:break-before", "page");
	else if (m_ps->m_isParagraphColumnBreak)
	{
		if (m_ps->m_numColumns > 1)
			propList.insert("fo:break-before", "column");
		else
			propList.insert("fo:break-before", "page");
	}
}

void WPXContentListener::_getTabStops(WPXPropertyListVector &tabStops)
{
	for (unsigned i=0; i<m_ps->m_tabStops.size(); i++)
	{
		WPXPropertyList tmpTabStop;

		// type
		switch (m_ps->m_tabStops[i].m_alignment)
		{
		case RIGHT:
			tmpTabStop.insert("style:type", "right");
			break;
		case CENTER:
			tmpTabStop.insert("style:type", "center");
			break;
		case DECIMAL:
			tmpTabStop.insert("style:type", "char");
			tmpTabStop.insert("style:char", "."); // Assume a decimal point for now
			break;
		case LEFT:
		case BAR:
		default:  // Left alignment is the default and BAR is not handled in OOo
			break;
		}

		// leader character
		if (m_ps->m_tabStops[i].m_leaderCharacter != 0x0000)
		{
			WPXString sLeader;
			sLeader.sprintf("%c", m_ps->m_tabStops[i].m_leaderCharacter);
			tmpTabStop.insert("style:leader-text", sLeader);
			tmpTabStop.insert("style:leader-style", "solid");
		}

		// position
		double position = m_ps->m_tabStops[i].m_position;
		if (m_ps->m_isTabPositionRelative)
			position -= m_ps->m_leftMarginByTabs;
		else
			position -= m_ps->m_paragraphMarginLeft + m_ps->m_sectionMarginLeft + m_ps->m_pageMarginLeft;
		if (position < 0.00005f && position > -0.00005f)
			position = 0.0;
		tmpTabStop.insert("style:position", position);


		/* TODO: fix situations where we have several columns or are inside a table and the tab stop
		 *       positions are absolute (relative to the paper edge). In this case, they have to be
		 *       computed for each column or each cell in table. (Fridrich) */
		tabStops.append(tmpTabStop);
	}
}

void WPXContentListener::_closeParagraph()
{
	if (m_ps->m_isParagraphOpened)
	{
		if (m_ps->m_isSpanOpened)
			_closeSpan();

		m_documentInterface->closeParagraph();
	}

	m_ps->m_isParagraphOpened = false;
	m_ps->m_currentListLevel = 0;

	if (!m_ps->m_isTableOpened && m_ps->m_isPageSpanBreakDeferred && !m_ps->m_inSubDocument)
		_closePageSpan();
}

void WPXContentListener::_openListElement()
{
	if (m_ps->m_isTableOpened && !m_ps->m_isTableCellOpened)
		return;

	if (!m_ps->m_isParagraphOpened && !m_ps->m_isListElementOpened)
	{
		if (!m_ps->m_isTableOpened && (!m_ps->m_inSubDocument || m_ps->m_subDocumentType == WPX_SUBDOCUMENT_TEXT_BOX))
		{
			if (m_ps->m_sectionAttributesChanged)
				_closeSection();

			if (!m_ps->m_isSectionOpened)
				_openSection();
		}

		WPXPropertyList propList;
		_appendParagraphProperties(propList, true);

		WPXPropertyListVector tabStops;
		_getTabStops(tabStops);

		if (!m_ps->m_isListElementOpened)
			m_documentInterface->openListElement(propList, tabStops);
		_resetParagraphState(true);
	}
}

void WPXContentListener::_closeListElement()
{
	if (m_ps->m_isListElementOpened)
	{
		if (m_ps->m_isSpanOpened)
			_closeSpan();

		m_documentInterface->closeListElement();
	}

	m_ps->m_isListElementOpened = false;
	m_ps->m_currentListLevel = 0;

	if (!m_ps->m_isTableOpened && m_ps->m_isPageSpanBreakDeferred && !m_ps->m_inSubDocument)
		_closePageSpan();
}

const double WPX_DEFAULT_SUPER_SUB_SCRIPT = 58.0;

void WPXContentListener::_openSpan()
{
	if (m_ps->m_isTableOpened && !m_ps->m_isTableCellOpened)
		return;

	if (!m_ps->m_isParagraphOpened && !m_ps->m_isListElementOpened)
		_changeList();
	if (m_ps->m_currentListLevel == 0)
		_openParagraph();
	else
		_openListElement();

	// The behaviour of WP6+ is following: if an attribute bit is set in the cell attributes, we cannot
	// unset it; if it is set, we can set or unset it
	uint32_t attributeBits = (m_ps->m_textAttributeBits | m_ps->m_cellAttributeBits);
	uint8_t fontSizeAttributes;
	double fontSizeChange;
	// the font size attribute bits are mutually exclusive and the cell attributes prevail
	if ((m_ps->m_cellAttributeBits & 0x0000001f) != 0x00000000)
		fontSizeAttributes = (uint8_t)(m_ps->m_cellAttributeBits & 0x0000001f);
	else
		fontSizeAttributes = (uint8_t)(m_ps->m_textAttributeBits & 0x0000001f);
	switch (fontSizeAttributes)
	{
	case 0x01:  // Extra large
		fontSizeChange = 2.0;
		break;
	case 0x02: // Very large
		fontSizeChange = 1.5f;
		break;
	case 0x04: // Large
		fontSizeChange = 1.2f;
		break;
	case 0x08: // Small print
		fontSizeChange = 0.8f;
		break;
	case 0x10: // Fine print
		fontSizeChange = 0.6f;
		break;
	default: // Normal
		fontSizeChange = 1.0;
		break;
	}

	WPXPropertyList propList;
	if (attributeBits & WPX_SUPERSCRIPT_BIT)
	{
		WPXString sSuperScript("super ");
		sSuperScript.append(doubleToString(WPX_DEFAULT_SUPER_SUB_SCRIPT));
		sSuperScript.append("%");
		propList.insert("style:text-position", sSuperScript);
	}
	else if (attributeBits & WPX_SUBSCRIPT_BIT)
	{
		WPXString sSubScript("sub ");
		sSubScript.append(doubleToString(WPX_DEFAULT_SUPER_SUB_SCRIPT));
		sSubScript.append("%");
		propList.insert("style:text-position", sSubScript);
	}
	if (attributeBits & WPX_ITALICS_BIT)
		propList.insert("fo:font-style", "italic");
	if (attributeBits & WPX_BOLD_BIT)
		propList.insert("fo:font-weight", "bold");
	if (attributeBits & WPX_STRIKEOUT_BIT)
		propList.insert("style:text-line-through-type", "single");
	if (attributeBits & WPX_DOUBLE_UNDERLINE_BIT)
		propList.insert("style:text-underline-type", "double");
	else if (attributeBits & WPX_UNDERLINE_BIT)
		propList.insert("style:text-underline-type", "single");
	if (attributeBits & WPX_OUTLINE_BIT)
		propList.insert("style:text-outline", "true");
	if (attributeBits & WPX_SMALL_CAPS_BIT)
		propList.insert("fo:font-variant", "small-caps");
	if (attributeBits & WPX_BLINK_BIT)
		propList.insert("style:text-blinking", "true");
	if (attributeBits & WPX_SHADOW_BIT)
		propList.insert("fo:text-shadow", "1pt 1pt");

	if (m_ps->m_fontName)
		propList.insert("style:font-name", m_ps->m_fontName->cstr());

	propList.insert("fo:font-size", fontSizeChange*m_ps->m_fontSize, WPX_POINT);

	// Here we give the priority to the redline bit over the font color. This is how WordPerfect behaves:
	// redline overrides font color even if the color is changed when redline was already defined.
	// When redline finishes, the color is back.
	if (attributeBits & WPX_REDLINE_BIT)
		propList.insert("fo:color", "#ff3333");  // #ff3333 = a nice bright red
	else if (m_ps->m_fontColor)
		propList.insert("fo:color", _colorToString(m_ps->m_fontColor));
	if (m_ps->m_highlightColor)
		propList.insert("fo:background-color", _colorToString(m_ps->m_highlightColor));

	if (!m_ps->m_isSpanOpened)
		m_documentInterface->openSpan(propList);

	m_ps->m_isSpanOpened = true;
}

void WPXContentListener::_closeSpan()
{
	if (m_ps->m_isSpanOpened)
	{
		_flushText();

		m_documentInterface->closeSpan();
	}

	m_ps->m_isSpanOpened = false;
}

void WPXContentListener::_openTable()
{
	_closeTable();

	WPXPropertyList propList;
	switch (m_ps->m_tableDefinition.m_positionBits)
	{
	case WPX_TABLE_POSITION_ALIGN_WITH_LEFT_MARGIN:
		propList.insert("table:align", "left");
		propList.insert("fo:margin-left", 0.0);
		break;
	case WPX_TABLE_POSITION_ALIGN_WITH_RIGHT_MARGIN:
		propList.insert("table:align", "right");
		break;
	case WPX_TABLE_POSITION_CENTER_BETWEEN_MARGINS:
		propList.insert("table:align", "center");
		break;
	case WPX_TABLE_POSITION_ABSOLUTE_FROM_LEFT_MARGIN:
		propList.insert("table:align", "left");
		propList.insert("fo:margin-left", _movePositionToFirstColumn(m_ps->m_tableDefinition.m_leftOffset) -
		                m_ps->m_pageMarginLeft - m_ps->m_sectionMarginLeft + m_ps->m_paragraphMarginLeft);
		break;
	case WPX_TABLE_POSITION_FULL:
		propList.insert("table:align", "margins");
		propList.insert("fo:margin-left", m_ps->m_paragraphMarginLeft);
		propList.insert("fo:margin-right", m_ps->m_paragraphMarginRight);
		break;
	default:
		break;
	}

	// cater for the possibility to have the column/page break just before the table
	_insertBreakIfNecessary(propList);
	m_ps->m_isParagraphColumnBreak = false;
	m_ps->m_isParagraphPageBreak = false;

	double tableWidth = 0.0;
	WPXPropertyListVector columns;
	typedef std::vector<WPXColumnDefinition>::const_iterator CDVIter;
	for (CDVIter iter = m_ps->m_tableDefinition.m_columns.begin(); iter != m_ps->m_tableDefinition.m_columns.end(); ++iter)
	{
		WPXPropertyList column;
		// The "style:rel-width" is expressed in twips (1440 twips per inch) and includes the left and right Gutter
		column.insert("style:column-width", (*iter).m_width);
		columns.append(column);

		tableWidth += (*iter).m_width;
	}
	propList.insert("style:width", tableWidth);

	m_documentInterface->openTable(propList, columns);
	m_ps->m_isTableOpened = true;

	m_ps->m_currentTableRow = (-1);
	m_ps->m_currentTableCol = (-1);
	m_ps->m_currentTableCellNumberInRow = (-1);
}

void WPXContentListener::_closeTable()
{
	if (m_ps->m_isTableOpened)
	{
		if (m_ps->m_isTableRowOpened)
			_closeTableRow();

		m_documentInterface->closeTable();
	}

	m_ps->m_currentTableRow = (-1);
	m_ps->m_currentTableCol = (-1);
	m_ps->m_currentTableCellNumberInRow = (-1);
	m_ps->m_isTableOpened = false;
	m_ps->m_wasHeaderRow = false;

	_closeParagraph();
	_closeListElement();
	_changeList();

	// handle case where a section attributes changed in the middle of the table
	if (m_ps->m_sectionAttributesChanged && !m_ps->m_inSubDocument)
		_closeSection();

	// handle case where page span is closed in the middle of a table
	if (m_ps->m_isPageSpanBreakDeferred && !m_ps->m_inSubDocument)
		_closePageSpan();
}

void WPXContentListener::_openTableRow(const double height, const bool isMinimumHeight, const bool isHeaderRow)
{
	if (!m_ps->m_isTableOpened)
		throw ParseException();

	if (m_ps->m_isTableRowOpened)
		_closeTableRow();

	m_ps->m_currentTableCol = 0;
	m_ps->m_currentTableCellNumberInRow = 0;


	WPXPropertyList propList;
	if (isMinimumHeight && height != 0.0) // minimum height kind of stupid if it's not set, right?
		propList.insert("style:min-row-height", height);
	else if (height != 0.0) // this indicates that wordperfect didn't set a height
		propList.insert("style:row-height", height);

	// Only the first "Header Row" in a table is the actual "Header Row"
	// The following "Header Row" flags are ignored
	if (isHeaderRow & !m_ps->m_wasHeaderRow)
	{
		propList.insert("libwpd:is-header-row", true);
		m_ps->m_wasHeaderRow = true;
	}
	else
		propList.insert("libwpd:is-header-row", false);

	m_documentInterface->openTableRow(propList);

	m_ps->m_isTableRowOpened = true;
	m_ps->m_isRowWithoutCell = true;
	m_ps->m_currentTableRow++;
}

void WPXContentListener::_closeTableRow()
{
	if (m_ps->m_isTableRowOpened)
	{
		if (m_ps->m_currentTableCol < 0)
			throw ParseException();
		while ((unsigned long)m_ps->m_currentTableCol < (unsigned long)m_ps->m_numRowsToSkip.size())
		{
			if (!m_ps->m_numRowsToSkip[(size_t)(m_ps->m_currentTableCol)]) // This case should not happen, but does :-(
			{
				// m_ps->m_currentTableCol++;
				// Fill the table row untill the end with empty cells
				RGBSColor tmpCellBorderColor(0x00, 0x00, 0x00, 0x64);
				_openTableCell(1, 1, 0xFF, 0, 0, &tmpCellBorderColor, TOP);
				_closeTableCell();
			}
			else
				m_ps->m_numRowsToSkip[(size_t)(m_ps->m_currentTableCol++)]--;
		}

		if (m_ps->m_isTableCellOpened)
			_closeTableCell();
		// FIXME: this will need some love so that we actually insert covered cells with proper attributes
		if (m_ps->m_isRowWithoutCell)
		{
			m_ps->m_isRowWithoutCell = false;
			WPXPropertyList tmpBlankList;
			m_documentInterface->insertCoveredTableCell(tmpBlankList);
		}
		m_documentInterface->closeTableRow();
	}
	m_ps->m_isTableRowOpened = false;
}

const double WPX_DEFAULT_TABLE_BORDER_WIDTH = 0.0007f;

static void addBorderProps(const char *border, bool borderOn, const WPXString &borderColor, WPXPropertyList &propList)
{
#if 0
// WLACH: a (not working, obviously) sketch of an alternate way of doing this
// in case it turns out to be desirable. Right now it appears not, as we would have to
// retranslate them on import to OOo (because they don't completely support xsl-fo)
// .. but it would make things way easier in Abi.
	if (borderOn)
	{
		propList.insert("fo:border-left-width", WPX_DEFAULT_TABLE_BORDER_WIDTH);
		propList.insert("fo:border-left-style", "solid");
		propList.insert("fo:border-left-color", borderColor);
	}
	else
		propList.insert("fo:border-left-width", 0.0);
#endif

	WPXString borderStyle;
	borderStyle.sprintf("fo:border-%s", border);
	WPXString props;
	if (borderOn)
	{
		props.append(doubleToString(WPX_DEFAULT_TABLE_BORDER_WIDTH));
		props.append("in solid ");
		props.append(borderColor);
	}
	else
		props.sprintf("0.0in");
	propList.insert(borderStyle.cstr(), props);
}

void WPXContentListener::_openTableCell(const uint8_t colSpan, const uint8_t rowSpan, const uint8_t borderBits,
                                        const RGBSColor *cellFgColor, const RGBSColor *cellBgColor,
                                        const RGBSColor *cellBorderColor, const WPXVerticalAlignment cellVerticalAlignment)
{
	if (!m_ps->m_isTableOpened || !m_ps->m_isTableRowOpened)
		throw ParseException();

	uint8_t tmpColSpan = colSpan;
	if (m_ps->m_isTableCellOpened)
		_closeTableCell();

	if (m_ps->m_currentTableCol < 0)
		throw ParseException();

	while ((unsigned long)m_ps->m_currentTableCol < (unsigned long)m_ps->m_numRowsToSkip.size() &&
	        m_ps->m_numRowsToSkip[(size_t)(m_ps->m_currentTableCol)])
	{
		m_ps->m_numRowsToSkip[(size_t)(m_ps->m_currentTableCol)]--;
		m_ps->m_currentTableCol++;
	}

	WPXPropertyList propList;
	propList.insert("libwpd:column", m_ps->m_currentTableCol);
	propList.insert("libwpd:row", m_ps->m_currentTableRow);

	propList.insert("table:number-columns-spanned", colSpan);
	propList.insert("table:number-rows-spanned", rowSpan);

	WPXString borderColor = _colorToString(cellBorderColor);
	addBorderProps("left", !(borderBits & WPX_TABLE_CELL_LEFT_BORDER_OFF), borderColor, propList);
	addBorderProps("right", !(borderBits & WPX_TABLE_CELL_RIGHT_BORDER_OFF), borderColor, propList);
	addBorderProps("top", !(borderBits & WPX_TABLE_CELL_TOP_BORDER_OFF), borderColor, propList);
	addBorderProps("bottom", !(borderBits & WPX_TABLE_CELL_BOTTOM_BORDER_OFF), borderColor, propList);

	switch (cellVerticalAlignment)
	{
	case TOP:
		propList.insert("style:vertical-align", "top");
		break;
	case MIDDLE:
		propList.insert("style:vertical-align", "middle");
		break;
	case BOTTOM:
		propList.insert("style:vertical-align", "bottom");
		break;
	case FULL: // full not in XSL-fo?
	default:
		break;
	}
	propList.insert("fo:background-color", _mergeColorsToString(cellFgColor, cellBgColor));
	m_documentInterface->openTableCell(propList);
	m_ps->m_currentTableCellNumberInRow++;
	m_ps->m_isTableCellOpened = true;
	m_ps->m_isCellWithoutParagraph = true;
	m_ps->m_isRowWithoutCell = false;

	if (m_ps->m_currentTableCol < 0)
		throw ParseException();

	while ((unsigned long)m_ps->m_currentTableCol < (unsigned long)m_ps->m_numRowsToSkip.size() &&(tmpColSpan > 0))
	{
		if (m_ps->m_numRowsToSkip[(size_t)(m_ps->m_currentTableCol)]) // This case should not happen, but it happens in real-life documents :-(
		{
			m_ps->m_numRowsToSkip[(size_t)(m_ps->m_currentTableCol)]=0;
		}
		m_ps->m_numRowsToSkip[(size_t)(m_ps->m_currentTableCol)] += (rowSpan - 1);
		m_ps->m_currentTableCol++;
		tmpColSpan--;
	}
}

void WPXContentListener::_closeTableCell()
{
	if (m_ps->m_isTableCellOpened)
	{
		if (m_ps->m_isCellWithoutParagraph)
			_openSpan();
		if (m_ps->m_isParagraphOpened)
			_closeParagraph();
		if (m_ps->m_isListElementOpened)
			_closeListElement();
		m_ps->m_currentListLevel = 0;
		_changeList();
		m_ps->m_cellAttributeBits = 0x00000000;

		m_documentInterface->closeTableCell();
	}
	m_ps->m_isTableCellOpened = false;
}


/**
Creates an new document state. Saves the old state on a "stack".
*/
void WPXContentListener::handleSubDocument(const WPXSubDocument *subDocument, WPXSubDocumentType subDocumentType,
        WPXTableList tableList, unsigned nextTableIndice)
{
	// save our old parsing state on our "stack"
	WPXContentParsingState *oldPS = m_ps;
	m_ps = new WPXContentParsingState();
	// BEGIN: copy page properties into the new parsing state
	m_ps->m_pageFormWidth = oldPS->m_pageFormWidth;
	m_ps->m_pageMarginLeft = oldPS->m_pageMarginLeft;
	m_ps->m_pageMarginRight = oldPS->m_pageMarginRight;
	m_ps->m_subDocuments = oldPS->m_subDocuments;
	m_ps->m_isNote = oldPS->m_isNote;
	m_ps->m_subDocumentType = subDocumentType;
	m_ps->m_isDocumentStarted = true;
	m_ps->m_isPageSpanOpened = true;
	if (m_ps->m_subDocumentType == WPX_SUBDOCUMENT_TEXT_BOX)
	{
		m_ps->m_pageMarginLeft = 0.0;
		m_ps->m_pageMarginRight = 0.0;
		m_ps->m_sectionAttributesChanged = true;
	}
	// END: copy page properties into the new parsing state
	m_ps->m_inSubDocument = true;
	bool oldIsUndoOn = isUndoOn();
	setUndoOn(false);
	// Check whether the document is calling itself
	if ((subDocument) && (m_ps->m_subDocuments.find(subDocument) == m_ps->m_subDocuments.end()))
	{
		m_ps->m_subDocuments.insert(subDocument);
		if (subDocumentType == WPX_SUBDOCUMENT_HEADER_FOOTER)
			m_ps->m_isHeaderFooterWithoutParagraph = true;
		_handleSubDocument(subDocument, subDocumentType, tableList, nextTableIndice);
		if (m_ps->m_isHeaderFooterWithoutParagraph)
		{
			_openSpan();
			_closeParagraph();
		}
	}

	// restore our old parsing state

	setUndoOn(oldIsUndoOn);
	if (m_ps->m_subDocumentType == WPX_SUBDOCUMENT_TEXT_BOX)
		_closeSection();
	delete m_ps;
	m_ps = oldPS;
}

void WPXContentListener::insertBreak(const uint8_t breakType)
{
	if (!isUndoOn())
	{
		switch (breakType)
		{
		case WPX_COLUMN_BREAK:
			if (!m_ps->m_isPageSpanOpened && !m_ps->m_inSubDocument)
				_openSpan();
			if (m_ps->m_isParagraphOpened)
				_closeParagraph();
			if (m_ps->m_isListElementOpened)
				_closeListElement();
			m_ps->m_isParagraphColumnBreak = true;
			m_ps->m_isTextColumnWithoutParagraph = true;
			break;
		case WPX_PAGE_BREAK:
			if (!m_ps->m_isPageSpanOpened && !m_ps->m_inSubDocument)
				_openSpan();
			if (m_ps->m_isParagraphOpened)
				_closeParagraph();
			if (m_ps->m_isListElementOpened)
				_closeListElement();
			m_ps->m_isParagraphPageBreak = true;
			break;
			// TODO: (.. line break?)
		default:
			break;
		}

		if (m_ps->m_inSubDocument)
			return;

		switch (breakType)
		{
		case WPX_PAGE_BREAK:
		case WPX_SOFT_PAGE_BREAK:
			if (m_ps->m_numPagesRemainingInSpan > 0)
				m_ps->m_numPagesRemainingInSpan--;
			else
			{
				if (!m_ps->m_isTableOpened && !m_ps->m_isParagraphOpened && !m_ps->m_isListElementOpened)
					_closePageSpan();
				else
					m_ps->m_isPageSpanBreakDeferred = true;
			}
			m_ps->m_currentPageNumber++;
			break;
		default:
			break;
		}
	}
}

void WPXContentListener::lineSpacingChange(const double lineSpacing)
{
	if (!isUndoOn())
	{
		m_ps->m_paragraphLineSpacing = lineSpacing;
	}
}

void WPXContentListener::justificationChange(const uint8_t justification)
{
	if (!isUndoOn())
	{
		// We discovered that if there is not a paragraph break before justificationChange,
		// newer versions of WordPerfect add a temporary hard return just before the code.
		// So, we will mimick them!
		if (m_ps->m_isParagraphOpened)
			_closeParagraph();
		if (m_ps->m_isListElementOpened)
			_closeListElement();

		m_ps->m_currentListLevel = 0;

		switch (justification)
		{
		case 0x00:
			m_ps->m_paragraphJustification = WPX_PARAGRAPH_JUSTIFICATION_LEFT;
			break;
		case 0x01:
			m_ps->m_paragraphJustification = WPX_PARAGRAPH_JUSTIFICATION_FULL;
			break;
		case 0x02:
			m_ps->m_paragraphJustification = WPX_PARAGRAPH_JUSTIFICATION_CENTER;
			break;
		case 0x03:
			m_ps->m_paragraphJustification = WPX_PARAGRAPH_JUSTIFICATION_RIGHT;
			break;
		case 0x04:
			m_ps->m_paragraphJustification = WPX_PARAGRAPH_JUSTIFICATION_FULL_ALL_LINES;
			break;
		case 0x05:
			m_ps->m_paragraphJustification = WPX_PARAGRAPH_JUSTIFICATION_DECIMAL_ALIGNED;
			break;
		default:
			break;
		}
	}
}

double WPXContentListener::_getNextTabStop() const
{
	for (std::vector<WPXTabStop>::const_iterator iter = m_ps->m_tabStops.begin(); iter != (m_ps->m_tabStops.end() - 1); ++iter)
	{
		if (iter->m_position
		        - (m_ps->m_isTabPositionRelative ? 0.0 : (m_ps->m_pageMarginLeft + m_ps->m_sectionMarginLeft + m_ps->m_leftMarginByParagraphMarginChange))
		        == (m_ps->m_leftMarginByTabs + m_ps->m_textIndentByTabs + m_ps->m_textIndentByParagraphIndentChange))
			return (iter+1)->m_position
			       - (m_ps->m_isTabPositionRelative ? 0.0 : (m_ps->m_pageMarginLeft + m_ps->m_sectionMarginLeft + m_ps->m_leftMarginByParagraphMarginChange));
		if (iter->m_position
		        - (m_ps->m_isTabPositionRelative ? 0.0 : (m_ps->m_pageMarginLeft + m_ps->m_sectionMarginLeft + m_ps->m_leftMarginByParagraphMarginChange))
		        > (m_ps->m_leftMarginByTabs + m_ps->m_textIndentByTabs + m_ps->m_textIndentByParagraphIndentChange))
			return iter->m_position
			       - (m_ps->m_isTabPositionRelative ? 0.0 : (m_ps->m_pageMarginLeft + m_ps->m_sectionMarginLeft + m_ps->m_leftMarginByParagraphMarginChange));
	}
	return (std::numeric_limits<double>::min)();
}

double WPXContentListener::_getPreviousTabStop() const
{
	for (std::vector<WPXTabStop>::reverse_iterator riter = m_ps->m_tabStops.rbegin(); riter != (m_ps->m_tabStops.rend() - 1); ++riter)
	{
		if (riter->m_position
		        - (m_ps->m_isTabPositionRelative ? 0.0 : (m_ps->m_pageMarginLeft + m_ps->m_sectionMarginLeft + m_ps->m_leftMarginByParagraphMarginChange))
		        == (m_ps->m_leftMarginByTabs + m_ps->m_textIndentByTabs + m_ps->m_textIndentByParagraphIndentChange))
			return (riter+1)->m_position
			       - (m_ps->m_isTabPositionRelative ? 0.0 : (m_ps->m_pageMarginLeft + m_ps->m_sectionMarginLeft + m_ps->m_leftMarginByParagraphMarginChange));
		if (riter->m_position
		        - (m_ps->m_isTabPositionRelative ? 0.0 : (m_ps->m_pageMarginLeft + m_ps->m_sectionMarginLeft + m_ps->m_leftMarginByParagraphMarginChange))
		        < (m_ps->m_leftMarginByTabs + m_ps->m_textIndentByTabs + m_ps->m_textIndentByParagraphIndentChange))
			return riter->m_position
			       - (m_ps->m_isTabPositionRelative ? 0.0 : (m_ps->m_pageMarginLeft + m_ps->m_sectionMarginLeft + m_ps->m_leftMarginByParagraphMarginChange));
	}
	return (std::numeric_limits<double>::max)();
}

WPXString WPXContentListener::_colorToString(const RGBSColor *color)
{
	WPXString tmpString;

	if (color)
	{
		double fontShading = (double)((double)color->m_s/100.0); //convert the percents to double between 0 and 1
		// Mix fontShading amount of given color with (1-fontShading) of White (#ffffff)
		int fontRed = (int)0xFF + (int)((double)color->m_r*fontShading) - (int)((double)0xFF*fontShading);
		int fontGreen = (int)0xFF + (int)((double)color->m_g*fontShading) - (int)((double)0xFF*fontShading);
		int fontBlue = (int)0xFF + (int)((double)color->m_b*fontShading) - (int)((double)0xFF*fontShading);
		tmpString.sprintf("#%.2x%.2x%.2x", fontRed, fontGreen, fontBlue);
	}
	else
		tmpString.sprintf("#%.2x%.2x%.2x", 0xFF, 0xFF, 0xFF); // default to white: we really shouldn't be calling this function in that case though

	return tmpString;
}

WPXString WPXContentListener::_mergeColorsToString(const RGBSColor *fgColor,
        const RGBSColor * /* bgColor */)
{
	WPXString tmpColor;
	RGBSColor tmpFgColor, tmpBgColor;

	if (fgColor)
	{
		tmpFgColor.m_r = fgColor->m_r;
		tmpFgColor.m_g = fgColor->m_g;
		tmpFgColor.m_b = fgColor->m_b;
		tmpFgColor.m_s = fgColor->m_s;
	}
	else
	{
		tmpFgColor.m_r = tmpFgColor.m_g = tmpFgColor.m_b = 0xFF;
		tmpFgColor.m_s = 0x64; // 100%
	}

	double fgAmount = (double)tmpFgColor.m_s/100.0;
	double wtAmount = 255.0*(1.0-fgAmount);

	int bgRed   = (int)(((double)tmpFgColor.m_r*fgAmount)+wtAmount);
	int bgGreen = (int)(((double)tmpFgColor.m_g*fgAmount)+wtAmount);
	int bgBlue  = (int)(((double)tmpFgColor.m_b*fgAmount)+wtAmount);

	tmpColor.sprintf("#%.2x%.2x%.2x", bgRed, bgGreen, bgBlue);

	return tmpColor;
}

double WPXContentListener::_movePositionToFirstColumn(double position)
{
	if (m_ps->m_numColumns <= 1)
		return position;
	double tempSpaceRemaining = position - m_ps->m_pageMarginLeft - m_ps->m_sectionMarginLeft;
	position -= m_ps->m_textColumns[0].m_leftGutter;
	for (unsigned i = 0; i < (unsigned)(m_ps->m_textColumns.size() - 1); i++)
	{
		if ((tempSpaceRemaining -= m_ps->m_textColumns[i].m_width - m_ps->m_textColumns[i].m_rightGutter) > 0)
		{
			position -= m_ps->m_textColumns[i].m_width - m_ps->m_textColumns[i].m_leftGutter
			            + m_ps->m_textColumns[i+1].m_leftGutter;
			tempSpaceRemaining -= m_ps->m_textColumns[i].m_rightGutter;
		}
		else
			return position;
	}
	return position;
}

uint32_t WPXContentListener::_mapNonUnicodeCharacter(uint32_t character)
{
	if (*(m_ps->m_fontName) == "Symbol")
		return _mapSymbolFontCharacter(character);

	if (*(m_ps->m_fontName) == "Dingbats")
		return _mapDingbatsFontCharacter(character);

	return character;
}

uint32_t WPXContentListener::_mapSymbolFontCharacter(uint32_t character)
{
	if (character >= 0x0020 && character <= 0x7E)
	{
		static const uint32_t _symbolFontMap1 [] =
		{
			0x0020, 0x0021, 0x2200, 0x0023, 0x2203, 0x0025, 0x0026, 0x220D, // 0x20 ..
			0x0028, 0x0029, 0x2217, 0x002B, 0x002C, 0x2212, 0x002E, 0x002F,
			0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037,
			0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F,
			0x2245, 0x0391, 0x0392, 0x03A7, 0x0394, 0x0395, 0x03A6, 0x0393,
			0x0397, 0x0399, 0x03D1, 0x039A, 0x039B, 0x039C, 0x039D, 0x039F,
			0x03A0, 0x0398, 0x03A1, 0x03A3, 0x03A4, 0x03A5, 0x03C2, 0x03A9,
			0x039E, 0x03A8, 0x0396, 0x005B, 0x2234, 0x005D, 0x22A5, 0x005F,
			0xF8E5, 0x03B1, 0x03B2, 0x03C7, 0x03B4, 0x03B5, 0x03C6, 0x03B3,
			0x03B7, 0x03B9, 0x03D5, 0x03BA, 0x03BB, 0x03BC, 0x03BD, 0x03BF,
			0x03C0, 0x03B8, 0x03C1, 0x03C3, 0x03C4, 0x03C5, 0x03D6, 0x03C9,
			0x03BE, 0x03C8, 0x03B6, 0x007B, 0x007C, 0x007D, 0x223C          // .. 0x7E
		};

		return _symbolFontMap1[character - 0x20];
	}
	if (character >= 0x00A0 && character <= 0xFE)
	{
		static const uint32_t _symbolFontMap2 [] =
		{
			0x20AC, 0x03D2, 0x2032, 0x2264, 0x2044, 0x221E, 0x0192, 0x2663, // 0xA0 ..
			0x2666, 0x2665, 0x2660, 0x2194, 0x2190, 0x2191, 0x2192, 0x2193,
			0x00B0, 0x00B1, 0x2033, 0x2265, 0x00D7, 0x221D, 0x2202, 0x2022,
			0x00F7, 0x2260, 0x2261, 0x2248, 0x2026, 0x23D0, 0x23AF, 0x21B5,
			0x2135, 0x2111, 0x211C, 0x2118, 0x2297, 0x2295, 0x2205, 0x2229,
			0x222A, 0x2283, 0x2287, 0x2284, 0x2282, 0x2286, 0x2208, 0x2209,
			0x2220, 0x2207, 0x00AE, 0x00A9, 0x2122, 0x220F, 0x221A, 0x22C5,
			0x00AC, 0x2227, 0x2228, 0x21D4, 0x21D0, 0x21D1, 0x21D2, 0x21D3,
			0x25CA, 0x3008, 0x00AE, 0x00A9, 0x2122, 0x2211, 0x239B, 0x239C,
			0x239D, 0x23A1, 0x23A2, 0x23A3, 0x23A7, 0x23A8, 0x23A9, 0x23AA,
			0xF8FF, 0x3009, 0x222B, 0x2320, 0x23AE, 0x2321, 0x239E, 0x239F,
			0x23A0, 0x23A4, 0x23A5, 0x23A6, 0x23AB, 0x23AC, 0x23AD          // .. 0xFE
		};

		return _symbolFontMap2[character - 0xA0];
	}
	return character;
}

uint32_t WPXContentListener::_mapDingbatsFontCharacter(uint32_t character)
{
	if (character >= 0x20 && character <= 0x7E)
	{
		static const uint32_t _dingbatsFontMap1 [] =
		{
			0x0020, 0x2701, 0x2702, 0x2703, 0x2704, 0x260E, 0x2706, 0x2707, // 0x20 ..
			0x2708, 0x2709, 0x261B, 0x261E, 0x270C, 0x270D, 0x270E, 0x270F,
			0x2710, 0x2711, 0x2712, 0x2713, 0x2714, 0x2715, 0x2716, 0x2717,
			0x2718, 0x2719, 0x271A, 0x271B, 0x271C, 0x271D, 0x271E, 0x271F,
			0x2720, 0x2721, 0x2722, 0x2723, 0x2724, 0x2725, 0x2726, 0x2727,
			0x2605, 0x2729, 0x272A, 0x272B, 0x272C, 0x272D, 0x272E, 0x272F,
			0x2730, 0x2731, 0x2732, 0x2733, 0x2734, 0x2735, 0x2736, 0x2737,
			0x2738, 0x2739, 0x273A, 0x273B, 0x273C, 0x273D, 0x273E, 0x273F,
			0x2740,	0x2741, 0x2742, 0x2743, 0x2744, 0x2745, 0x2746, 0x2747,
			0x2748, 0x2749, 0x274A, 0x274B, 0x25CF, 0x274D, 0x25A0, 0x274F,
			0x2750, 0x2751, 0x2752, 0x25B2, 0x25BC, 0x25C6, 0x2756, 0x25D7,
			0x2758, 0x2759, 0x275A, 0x275B, 0x275C, 0x275D, 0x275E          // .. 0x7E
		};

		return _dingbatsFontMap1[character - 0x20];
	}
	if (character >= 0x80 && character <= 0x8D)
	{
		static const uint32_t _dingbatsFontMap2 [] =
		{
			0x2768, 0x2769, 0x276A, 0x276B, 0x276C, 0x276D, 0x276E, 0x276F, // 0x80 ..
			0x2770, 0x2771, 0x2772, 0x2773, 0x2774, 0x2775                  // .. 0x8D
		};

		return _dingbatsFontMap2[character - 0x80];
	}
	if (character >= 0xA1 && character <= 0xEF)
	{
		static const uint32_t _dingbatsFontMap3 [] =
		{
			0x2761, 0x2762, 0x2763, 0x2764, 0x2765, 0x2766, 0x2767, 0x2663, // 0xA1 ..
			0x2666, 0x2665, 0x2660, 0x2460, 0x2461, 0x2462, 0x2463, 0x2464,
			0x2465, 0x2466, 0x2467, 0x2468, 0x2469, 0x2776, 0x2777, 0x2778,
			0x2779, 0x277A, 0x277B, 0x277C, 0x277D, 0x277E, 0x277F, 0x2780,
			0x2781, 0x2782, 0x2783, 0x2784, 0x2785, 0x2786, 0x2787, 0x2788,
			0x2789, 0x278A, 0x278B, 0x278C, 0x278D, 0x278E, 0x278F, 0x2790,
			0x2791, 0x2792, 0x2793, 0x2794, 0x2192, 0x2194, 0x2195, 0x2798,
			0x2799, 0x279A, 0x279B, 0x279C, 0x279D, 0x279E, 0x279F, 0x27A0,
			0x27A1, 0x27A2, 0x27A3, 0x27A4, 0x27A5, 0x27A6, 0x27A7, 0x27A8,
			0x27A9, 0x27AA, 0x27AB, 0x27AC, 0x27AD, 0x27AE, 0x27AF          // .. 0xEF
		};

		return _dingbatsFontMap3[character - 0xA1];
	}
	if (character >= 0xF1 && character <=0xFE)
	{
		static const uint32_t _dingbatsFontMap4 [] =
		{
			0x27B1, 0x27B2, 0x27B3, 0x27B4, 0x27B5, 0x27B6, 0x27B7, 0x27B8, // 0xF1 ..
			0x27B9, 0x27BA, 0x27BB, 0x27BC, 0x27BD, 0x27BE                  // .. OxFE
		};

		return _dingbatsFontMap4[character - 0xF1];
	}
	return character;
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
