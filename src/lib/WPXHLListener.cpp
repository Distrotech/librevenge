/* libwpd
 * Copyright (C) 2002-2005 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2004 Net Integration Technologies (http://www.net-itech.com)
 * Copyright (C) 2002-2003 Marc Maurer (j.m.maurer@student.utwente.nl)
 * Copyright (C) 2004-2005 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include "WPXHLListener.h"
#include "WPXPageSpan.h"
#include "libwpd_internal.h"
#include "WPXProperty.h"
#ifdef _MSC_VER
#include <minmax.h>
#define LIBWPD_MIN min
#define LIBWPD_MAX max
#else
#define LIBWPD_MIN std::min
#define LIBWPD_MAX std::max
#endif

_WPXParsingState::_WPXParsingState() :
	m_textAttributeBits(0),
	m_fontSize(12.0f/*WP6_DEFAULT_FONT_SIZE*/), // FIXME ME!!!!!!!!!!!!!!!!!!! HELP WP6_DEFAULT_FONT_SIZE
	m_fontName(new WPXString(/*WP6_DEFAULT_FONT_NAME*/"Times New Roman")), // EN PAS DEFAULT FONT AAN VOOR WP5/6/etc
	m_fontColor(new RGBSColor(0x00,0x00,0x00,0x64)), //Set default to black. Maybe once it will change, but for the while...
	m_highlightColor(NULL),

	m_isParagraphColumnBreak(false),
	m_isParagraphPageBreak(false),
	m_paragraphLineSpacing(1.0f),
	m_paragraphJustification(WPX_PARAGRAPH_JUSTIFICATION_LEFT),
	m_tempParagraphJustification(0),

	m_isDocumentStarted(false),
	m_isPageSpanOpened(false),
	m_isSectionOpened(false),
	m_isPageSpanBreakDeferred(false),

	m_isSpanOpened(false),
	m_isParagraphOpened(false),
	m_isListElementOpened(false),

	m_currentTableCol(0),
	m_currentTableRow(0),
	m_isTableOpened(false),
	m_isTableRowOpened(false),
	m_isTableCellOpened(false),
	m_wasHeaderRow(false),
	m_isCellWithoutParagraph(false),
	m_cellAttributeBits(0),
	m_paragraphJustificationBeforeTable(WPX_PARAGRAPH_JUSTIFICATION_LEFT),

	m_nextPageSpanIndice(0),
	m_numPagesRemainingInSpan(0),

	m_sectionAttributesChanged(false),
	m_numColumns(1),
	m_isTextColumnWithoutParagraph(false),

	m_pageFormLength(11.0f),
	m_pageFormWidth(8.5f),
	m_pageFormOrientation(PORTRAIT),

	m_pageMarginLeft(1.0f),
	m_pageMarginRight(1.0f),

	m_paragraphMarginLeft(0.0f),
	m_paragraphMarginRight(0.0f),
	m_paragraphMarginTop(0.0f),
	m_paragraphMarginBottom(0.0f),
	m_leftMarginByPageMarginChange(0.0f),
	m_rightMarginByPageMarginChange(0.0f),
	m_leftMarginByParagraphMarginChange(0.0f),
	m_rightMarginByParagraphMarginChange(0.0f),
	m_leftMarginByTabs(0.0f),
	m_rightMarginByTabs(0.0f),
	
	m_listReferencePosition(0.0f),
	m_listBeginPosition(0.0f),

	m_paragraphTextIndent(0.0f),
	m_textIndentByParagraphIndentChange(0.0f),
	m_textIndentByTabs(0.0f),
	m_currentListLevel(0),
#if 0
	m_putativeListElementHasParagraphNumber(false),
	m_putativeListElementHasDisplayReferenceNumber(false),

	m_noteTextPID(0),
#endif
	m_alignmentCharacter('.'),
	m_isTabPositionRelative(false),
	m_inSubDocument(false),
	m_isNote(false)
{
}

_WPXParsingState::~_WPXParsingState()
{
	DELETEP(m_fontName);
	DELETEP(m_fontColor);
	DELETEP(m_highlightColor);
}

WPXHLListener::WPXHLListener(std::vector<WPXPageSpan *> *pageList, WPXHLListenerImpl *listenerImpl) :
	WPXLLListener(),
	m_pageList(pageList),
	m_listenerImpl(listenerImpl),
	m_ps(new WPXParsingState),
	m_isUndoOn(false)
{
}

WPXHLListener::~WPXHLListener()
{
	DELETEP(m_ps);
}

void WPXHLListener::startDocument()
{
	if (!m_ps->m_isDocumentStarted)
	{
		// FIXME: this is stupid, we should store a property list filled with the relevant metadata
		// and then pass that directly..

		m_listenerImpl->setDocumentMetaData(m_metaData);

		m_listenerImpl->startDocument();
	}
	
	m_ps->m_isDocumentStarted = true;
}

void WPXHLListener::_openSection()
{
	if (!m_ps->m_isSectionOpened)
	{
		if (!m_ps->m_isPageSpanOpened)
			_openPageSpan();

		WPXPropertyList propList;

		if (m_ps->m_numColumns > 1)
		{
			propList.insert("fo:margin-bottom", 1.0f);
			propList.insert("text:dont-balance-text-columns", false);
		}
		else
			propList.insert("fo:margin-bottom", 0.0f);

		WPXPropertyListVector columns;
 		typedef std::vector<WPXColumnDefinition>::const_iterator CDVIter;
	 	for (CDVIter iter = m_ps->m_textColumns.begin(); iter != m_ps->m_textColumns.end(); iter++)
		{
			WPXPropertyList column;
			// The "style:rel-width" is expressed in twips (1440 twips per inch) and includes the left and right Gutter
			column.insert("style:rel-width", (*iter).m_width * 1440.0f, TWIP);
			column.insert("fo:margin-left", (*iter).m_leftGutter);
			column.insert("fo:margin-right", (*iter).m_rightGutter);
			columns.append(column);
		}
		if (!m_ps->m_isSectionOpened)
			m_listenerImpl->openSection(propList, columns);

		m_ps->m_sectionAttributesChanged = false;
		m_ps->m_isSectionOpened = true;
	}
}

void WPXHLListener::_closeSection()
{
	if (m_ps->m_isSectionOpened)
	{
		if (m_ps->m_isParagraphOpened)
			_closeParagraph();
		if (m_ps->m_isListElementOpened)
			_closeListElement();
		_changeList();

		m_listenerImpl->closeSection();

		m_ps->m_sectionAttributesChanged = false;
	}
	m_ps->m_isSectionOpened = false;
		
}

void WPXHLListener::_openPageSpan()
{
	if (m_ps->m_isPageSpanOpened)
		return;

	if (!m_ps->m_isDocumentStarted)
		startDocument();

	// Hack to be sure that the paragraph margins are consistent even if the page margin changes
	m_ps->m_leftMarginByPageMarginChange += m_ps->m_pageMarginLeft;
	m_ps->m_rightMarginByPageMarginChange += m_ps->m_pageMarginRight;
	m_ps->m_listReferencePosition += m_ps->m_pageMarginLeft;
	m_ps->m_listBeginPosition += m_ps->m_pageMarginLeft;
	
	if ( !m_pageList ||
	     (m_pageList && m_ps->m_nextPageSpanIndice > (int)m_pageList->size() - 1)
	   )
	{
		throw ParseException();
	}

	WPXPageSpan *currentPage = (*m_pageList)[m_ps->m_nextPageSpanIndice];
	currentPage->makeConsistent(1);
	bool isLastPageSpan;
	(m_pageList->size() <= (m_ps->m_nextPageSpanIndice+1)) ? isLastPageSpan = true : isLastPageSpan = false;
	
	WPXPropertyList propList;
	propList.insert("libwpd:num-pages", currentPage->getPageSpan());
	propList.insert("libwpd:is-last-page-span", isLastPageSpan);
	propList.insert("fo:page-height", currentPage->getFormLength());
	propList.insert("fo:page-width", currentPage->getFormWidth());
	if (currentPage->getFormOrientation() == LANDSCAPE)
		propList.insert("style:print-orientation", "landscape"); 
	else
		propList.insert("style:print-orientation", "portrait"); 
	propList.insert("fo:margin-left", currentPage->getMarginLeft());
	propList.insert("fo:margin-right", currentPage->getMarginRight());
	propList.insert("fo:margin-top", currentPage->getMarginTop());
	propList.insert("fo:margin-bottom", currentPage->getMarginBottom());
	
	if (!m_ps->m_isPageSpanOpened)
		m_listenerImpl->openPageSpan(propList);

	m_ps->m_isPageSpanOpened = true;

	m_ps->m_pageFormWidth = currentPage->getFormWidth();
	m_ps->m_pageMarginLeft = currentPage->getMarginLeft();
	m_ps->m_pageMarginRight = currentPage->getMarginRight();

	// Hack to be sure that the paragraph margins are consistent even if the page margin changes
	// Compute new values
	m_ps->m_leftMarginByPageMarginChange -= m_ps->m_pageMarginLeft;
	m_ps->m_rightMarginByPageMarginChange -= m_ps->m_pageMarginRight;
	m_ps->m_listReferencePosition -= m_ps->m_pageMarginLeft;
	m_ps->m_listBeginPosition -= m_ps->m_pageMarginLeft;

	m_ps->m_paragraphMarginLeft = m_ps->m_leftMarginByPageMarginChange + m_ps->m_leftMarginByParagraphMarginChange
			+ m_ps->m_leftMarginByTabs;
	m_ps->m_paragraphMarginRight = m_ps->m_rightMarginByPageMarginChange + m_ps->m_rightMarginByParagraphMarginChange
			+ m_ps->m_rightMarginByTabs;

	std::vector<WPXHeaderFooter> headerFooterList = currentPage->getHeaderFooterList();
	for (std::vector<WPXHeaderFooter>::iterator iter = headerFooterList.begin(); iter != headerFooterList.end(); iter++)
	{
		if (!currentPage->getHeaderFooterSuppression((*iter).getInternalType()))
		{
			WPXPropertyList propList;
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
			}

			if ((*iter).getType() == HEADER)
				m_listenerImpl->openHeader(propList); 
			else
				m_listenerImpl->openFooter(propList); 

			handleSubDocument((*iter).getTextPID(), true, (*iter).getTableList(), 0);
			if ((*iter).getType() == HEADER)
				m_listenerImpl->closeHeader();
			else
				m_listenerImpl->closeFooter(); 

			WPD_DEBUG_MSG(("Header Footer Element: type: %i occurence: %i pid: %i\n",
				       (*iter).getType(), (*iter).getOccurence(), (*iter).getTextPID()));
		}
	}

	/* Some of this would maybe not be necessary, but it does not do any harm 
	 * and apparently solves some troubles */
	m_ps->m_pageFormLength = currentPage->getFormLength();
	m_ps->m_pageFormWidth = currentPage->getFormWidth();
	m_ps->m_pageFormOrientation = currentPage->getFormOrientation();
	m_ps->m_pageMarginLeft = currentPage->getMarginLeft();
	m_ps->m_pageMarginRight = currentPage->getMarginRight();

	m_ps->m_paragraphMarginLeft = m_ps->m_leftMarginByPageMarginChange + m_ps->m_leftMarginByParagraphMarginChange
			+ m_ps->m_leftMarginByTabs;
	m_ps->m_paragraphMarginRight = m_ps->m_rightMarginByPageMarginChange + m_ps->m_rightMarginByParagraphMarginChange
			+ m_ps->m_rightMarginByTabs;

	m_ps->m_paragraphTextIndent = m_ps->m_textIndentByParagraphIndentChange + m_ps->m_textIndentByTabs;

	m_ps->m_numPagesRemainingInSpan = (currentPage->getPageSpan() - 1);
	m_ps->m_nextPageSpanIndice++;
}

void WPXHLListener::_closePageSpan()
{
	if (m_ps->m_isPageSpanOpened)
	{
		if (m_ps->m_isSectionOpened)
			_closeSection();

		m_listenerImpl->closePageSpan();
	}
	
	m_ps->m_isPageSpanOpened = false;
	m_ps->m_isPageSpanBreakDeferred = false;
}

void WPXHLListener::_openParagraph()
{
	if (m_ps->m_isTableOpened && !m_ps->m_isTableCellOpened)
		return;
		 
	if (!m_ps->m_isParagraphOpened && !m_ps->m_isListElementOpened)
	{
		if (!m_ps->m_isTableOpened && !m_ps->m_inSubDocument)
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
			m_listenerImpl->openParagraph(propList, tabStops);

		_resetParagraphState();
	}
}

void WPXHLListener::_resetParagraphState(const bool isListElement)
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
	m_ps->m_leftMarginByTabs = 0.0f;
	m_ps->m_rightMarginByTabs = 0.0f;
	m_ps->m_paragraphTextIndent = m_ps->m_textIndentByParagraphIndentChange;
	m_ps->m_textIndentByTabs = 0.0f;
	m_ps->m_isCellWithoutParagraph = false;
	m_ps->m_isTextColumnWithoutParagraph = false;	
	m_ps->m_tempParagraphJustification = 0;
	m_ps->m_listReferencePosition = m_ps->m_paragraphMarginLeft + m_ps->m_paragraphTextIndent;
	m_ps->m_listBeginPosition = m_ps->m_paragraphMarginLeft + m_ps->m_paragraphTextIndent;
}

void WPXHLListener::_appendJustification(WPXPropertyList &propList, int justification)
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
	}
}

void WPXHLListener::_appendParagraphProperties(WPXPropertyList &propList, const bool isListElement)
{
	int justification;
	if (m_ps->m_tempParagraphJustification) 
		justification = m_ps->m_tempParagraphJustification;
	else
		justification = m_ps->m_paragraphJustification;
	_appendJustification(propList, justification);

	if (m_ps->m_numColumns <= 1 && !m_ps->m_isTableOpened)
	{
		// these properties are not appropriate inside multiple columns or when
		// a table is opened..
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
	propList.insert("fo:line-height", m_ps->m_paragraphLineSpacing, PERCENT);
	if (m_ps->m_isParagraphColumnBreak)
		propList.insert("fo:break-before", "column");
	else if (m_ps->m_isParagraphPageBreak)
		propList.insert("fo:break-before", "page");
}

void WPXHLListener::_getTabStops(WPXPropertyListVector &tabStops)
{
	for (int i=0; i<m_ps->m_tabStops.size(); i++)
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
		default:  // Left alignment is the default and BAR is not handled in OOo
			break;
		}
		
		// leader character
		if (m_ps->m_tabStops[i].m_leaderCharacter != 0x0000)
		{
			WPXString sLeader;
			sLeader.sprintf("%c", m_ps->m_tabStops[i].m_leaderCharacter);
			tmpTabStop.insert("style:leader-char", sLeader);
		}

		// position
		float position = m_ps->m_tabStops[i].m_position;
		if (m_ps->m_isTabPositionRelative)
			position -= m_ps->m_leftMarginByTabs;
		else
			position -= m_ps->m_paragraphMarginLeft + m_ps->m_pageMarginLeft;
		tmpTabStop.insert("style:position", position);
		

		/* TODO: fix situations where we have several columns or are inside a table and the tab stop
		 *       positions are absolute (relative to the paper edge). In this case, they have to be
		 *       computed for each column or each cell in table. (Fridrich) */
		tabStops.append(tmpTabStop);
	}
}

void WPXHLListener::_closeParagraph()
{
	if (m_ps->m_isParagraphOpened)
	{
		if (m_ps->m_isSpanOpened)
			_closeSpan();

		m_listenerImpl->closeParagraph();
	}

	m_ps->m_isParagraphOpened = false;
	m_ps->m_currentListLevel = 0;
}

void WPXHLListener::_openListElement()
{
	if (!m_ps->m_isParagraphOpened && !m_ps->m_isListElementOpened)
	{
		if (!m_ps->m_isTableOpened && !m_ps->m_isSectionOpened && !m_ps->m_inSubDocument)
			_openSection();

		WPXPropertyList propList;
		_appendParagraphProperties(propList, true);

		WPXPropertyListVector tabStops;
		_getTabStops(tabStops);

		if (!m_ps->m_isListElementOpened)
			m_listenerImpl->openListElement(propList, tabStops);
		_resetParagraphState(true);
	}
}

void WPXHLListener::_closeListElement()
{
	if (m_ps->m_isListElementOpened)
	{
		if (m_ps->m_isSpanOpened)
			_closeSpan();

		m_listenerImpl->closeListElement();
	}
	
	m_ps->m_isListElementOpened = false;
	m_ps->m_currentListLevel = 0;
}

const float WPX_DEFAULT_SUPER_SUB_SCRIPT = 58.0f; 

void WPXHLListener::_openSpan()
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
	float fontSizeChange;
	// the font size attribute bits are mutually exclusive and the cell attributes prevail
	if ((m_ps->m_cellAttributeBits & 0x0000001f) != 0x00000000)
		fontSizeAttributes = (uint8_t)(m_ps->m_cellAttributeBits & 0x0000001f);
	else
		fontSizeAttributes = (uint8_t)(m_ps->m_textAttributeBits & 0x0000001f);
	switch (fontSizeAttributes)
	{
	case 0x01:  // Extra large
		fontSizeChange = 2.0f;
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
		fontSizeChange = 1.0f;
		break;
	}

	WPXPropertyList propList;
 	if (attributeBits & WPX_SUPERSCRIPT_BIT) {
		WPXString sSuperScript;
		sSuperScript.sprintf("super %f%%", WPX_DEFAULT_SUPER_SUB_SCRIPT);
		propList.insert("style:text-position", sSuperScript);
	}
 	else if (attributeBits & WPX_SUBSCRIPT_BIT) {
		WPXString sSubScript;
		sSubScript.sprintf("sub %f%%", WPX_DEFAULT_SUPER_SUB_SCRIPT);
		propList.insert("style:text-position", sSubScript);
	}
	if (attributeBits & WPX_ITALICS_BIT)
		propList.insert("fo:font-style", "italic");
	if (attributeBits & WPX_BOLD_BIT)
		propList.insert("fo:font-weight", "bold");
	if (attributeBits & WPX_STRIKEOUT_BIT)
		propList.insert("style:text-crossing-out", "single-line");
	if (attributeBits & WPX_DOUBLE_UNDERLINE_BIT) 
		propList.insert("style:text-underline", "double");
 	else if (attributeBits & WPX_UNDERLINE_BIT) 
		propList.insert("style:text-underline", "single");
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
	propList.insert("fo:font-size", fontSizeChange*m_ps->m_fontSize, POINT);

	// Here we give the priority to the redline bit over the font color. This is how WordPerfect behaves:
	// redline overrides font color even if the color is changed when redline was already defined.
	// When redline finishes, the color is back.
	if (attributeBits & WPX_REDLINE_BIT)
		propList.insert("fo:color", "#ff3333");  // #ff3333 = a nice bright red
	else if (m_ps->m_fontColor)
		propList.insert("fo:color", _colorToString(m_ps->m_fontColor));
	if (m_ps->m_highlightColor)
		propList.insert("style:text-background-color", _colorToString(m_ps->m_highlightColor));

	if (!m_ps->m_isSpanOpened)
		m_listenerImpl->openSpan(propList);

	m_ps->m_isSpanOpened = true;
}

void WPXHLListener::_closeSpan()
{
	if (m_ps->m_isSpanOpened)
	{
		_flushText();

		m_listenerImpl->closeSpan();
	}
	
	m_ps->m_isSpanOpened = false;
}

void WPXHLListener::_openTable()
{
	_closeTable();
	
	WPXPropertyList propList;
	switch (m_ps->m_tableDefinition.m_positionBits)
	{
	case WPX_TABLE_POSITION_ALIGN_WITH_LEFT_MARGIN:
		propList.insert("table:align", "left");
		propList.insert("fo:margin-left", 0.0f);
		break;
	case WPX_TABLE_POSITION_ALIGN_WITH_RIGHT_MARGIN:
		propList.insert("table:align", "right");
		break;
	case WPX_TABLE_POSITION_CENTER_BETWEEN_MARGINS:
		propList.insert("table:align", "center");
		break;
	case WPX_TABLE_POSITION_ABSOLUTE_FROM_LEFT_MARGIN:
		propList.insert("table:align", "left");
		propList.insert("fo:margin-left", m_ps->m_tableDefinition.m_leftOffset - 
				m_ps->m_pageMarginLeft + m_ps->m_paragraphMarginLeft);
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
	if (m_ps->m_isParagraphPageBreak)
		propList.insert("fo:break-before", "page");
	else if (m_ps->m_isParagraphColumnBreak)
		propList.insert("fo:break-before", "column");
	m_ps->m_isParagraphColumnBreak = false;
	m_ps->m_isParagraphPageBreak = false;

 	float tableWidth = 0.0f;
	WPXPropertyListVector columns;
 	typedef std::vector<WPXColumnDefinition>::const_iterator CDVIter;
 	for (CDVIter iter = m_ps->m_tableDefinition.columns.begin(); iter != m_ps->m_tableDefinition.columns.end(); iter++)
 	{
		WPXPropertyList column;
		// The "style:rel-width" is expressed in twips (1440 twips per inch) and includes the left and right Gutter
		column.insert("style:column-width", (*iter).m_width);
		columns.append(column);

 		tableWidth += (*iter).m_width;
 	}
	propList.insert("style:width", tableWidth);

	m_listenerImpl->openTable(propList, columns);
	m_ps->m_isTableOpened = true;

	m_ps->m_currentTableRow = (-1);
	m_ps->m_currentTableCol = (-1);
}

void WPXHLListener::_closeTable()
{
	if (m_ps->m_isTableOpened)
	{
		if (m_ps->m_isTableRowOpened)
			_closeTableRow();

		m_listenerImpl->closeTable();
	}

	m_ps->m_currentTableRow = (-1);
	m_ps->m_currentTableCol = (-1);
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

void WPXHLListener::_openTableRow(const float height, const bool isMinimumHeight, const bool isHeaderRow)
{
	_closeTableRow();
	
	m_ps->m_currentTableCol = 0;

	WPXPropertyList propList;
	if (isMinimumHeight && height != 0.0f) // minimum height kind of stupid if it's not set, right?
		propList.insert("style:min-row-height", height);
	else if (height != 0.0f) // this indicates that wordperfect didn't set a height
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

	m_listenerImpl->openTableRow(propList);

	m_ps->m_isTableRowOpened = true;
	m_ps->m_currentTableRow++;
}

void WPXHLListener::_closeTableRow()
{
	if (m_ps->m_isTableRowOpened)
	{
		if (m_ps->m_isTableCellOpened)
			_closeTableCell();
		m_listenerImpl->closeTableRow();
	}
	m_ps->m_isTableRowOpened = false;
}

const float WPX_DEFAULT_TABLE_BORDER_WIDTH = 0.0007f;

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
		propList.insert("fo:border-left-width", 0.0f);
#endif
	
	WPXString borderStyle;
	borderStyle.sprintf("fo:border-%s", border);
	WPXString props;
	if (borderOn)
		props.sprintf("%finch solid %s", WPX_DEFAULT_TABLE_BORDER_WIDTH, borderColor.cstr());
	else
		props.sprintf("0.0inch");
	propList.insert(borderStyle.cstr(), props);
	WPXString borderOff;
}

void WPXHLListener::_openTableCell(const uint8_t colSpan, const uint8_t rowSpan, const bool boundFromLeft, const bool boundFromAbove,
				   const uint8_t borderBits, const RGBSColor * cellFgColor, const RGBSColor * cellBgColor,
				   const RGBSColor * cellBorderColor, const WPXVerticalAlignment cellVerticalAlignment)
{
	_closeTableCell();

	WPXPropertyList propList;
	propList.insert("libwpd:column", m_ps->m_currentTableCol);		
	propList.insert("libwpd:row", m_ps->m_currentTableRow);		

	if (!boundFromLeft && !boundFromAbove)
	{
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
			propList.insert("fo:vertical-align", "top");
			break;
		case MIDDLE:
			propList.insert("fo:vertical-align", "middle");
			break;
		case BOTTOM:
			propList.insert("fo:vertical-align", "bottom");
			break;
		case FULL: // full not in XSL-fo?
		default:
			break;
		}
		propList.insert("fo:background-color", _mergeColorsToString(cellFgColor, cellBgColor));
		m_listenerImpl->openTableCell(propList);
		m_ps->m_isTableCellOpened = true;
	}
	else
		m_listenerImpl->insertCoveredTableCell(propList);

	m_ps->m_currentTableCol++;
}

void WPXHLListener::_closeTableCell()
{
	if (m_ps->m_isTableCellOpened)
	{
		if (m_ps->m_isCellWithoutParagraph)
			_openSpan();
		_closeParagraph();
		_closeListElement();
		_changeList();
		m_ps->m_cellAttributeBits = 0x00000000;

		m_listenerImpl->closeTableCell();
	}
	m_ps->m_isTableCellOpened = false;
}


/**
Creates an new document state. Saves the old state on a "stack".
*/
void WPXHLListener::handleSubDocument(uint16_t textPID, const bool isHeaderFooter, WPXTableList tableList, int nextTableIndice)
{
	// save our old parsing state on our "stack"
	WPXParsingState *oldPS = m_ps;
	m_ps = new WPXParsingState();
	// BEGIN: copy page properties into the new parsing state
	m_ps->m_pageFormWidth = oldPS->m_pageFormWidth;
	m_ps->m_pageMarginLeft = oldPS->m_pageMarginLeft;
	m_ps->m_pageMarginRight = oldPS->m_pageMarginRight;
	m_ps->m_subDocumentTextPIDs = oldPS->m_subDocumentTextPIDs;
	// END: copy page properties into the new parsing state
	m_ps->m_inSubDocument = true;
	// Check whether the document is calling its own TextPID
	if ((m_ps->m_subDocumentTextPIDs.find(textPID) == m_ps->m_subDocumentTextPIDs.end()) || (!textPID))
	{
		m_ps->m_subDocumentTextPIDs.insert(textPID);
		_handleSubDocument(textPID, isHeaderFooter, tableList, nextTableIndice);
	}

	// restore our old parsing state
	
	delete m_ps;
	m_ps = oldPS;
}

void WPXHLListener::insertBreak(const uint8_t breakType)
{
	if (!isUndoOn())
	{
		switch (breakType)
		{
		case WPX_COLUMN_BREAK:
			if (m_ps->m_isParagraphOpened)
				_closeParagraph();
			if (m_ps->m_isListElementOpened)
				_closeListElement();
			m_ps->m_isParagraphColumnBreak = true;
			m_ps->m_isTextColumnWithoutParagraph = true;
			break;
		case WPX_PAGE_BREAK:
			if (m_ps->m_isParagraphOpened)
				_closeParagraph();
			if (m_ps->m_isListElementOpened)
				_closeListElement();
			m_ps->m_isParagraphPageBreak = true;
			break;
			// TODO: (.. line break?)
		}
		switch (breakType)
		{
		case WPX_PAGE_BREAK:
		case WPX_SOFT_PAGE_BREAK:
			if (m_ps->m_numPagesRemainingInSpan > 0)
				m_ps->m_numPagesRemainingInSpan--;
			else
			{
			    if (!m_ps->m_isTableOpened)
				_closePageSpan();
			    else
				m_ps->m_isPageSpanBreakDeferred = true;
			}
		default:
			break;
		}
	}
}

void WPXHLListener::lineSpacingChange(const float lineSpacing)
{
	if (!isUndoOn())
	{
		m_ps->m_paragraphLineSpacing = lineSpacing;
	}
}

void WPXHLListener::justificationChange(const uint8_t justification)
{
	if (!isUndoOn())
	{
		// could be done simply by:
		// m_ps->m_paragraphJustification = justification;
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
		}
	}
}

WPXString WPXHLListener::_colorToString(const RGBSColor * color)
{
	WPXString tmpString;

	if (color) 
	{
		float fontShading = (float)((float)color->m_s/100.0f); //convert the percents to float between 0 and 1
		// Mix fontShading amount of given color with (1-fontShading) of White (#ffffff)
		int fontRed = (int)0xFF + (int)((float)color->m_r*fontShading) - (int)((float)0xFF*fontShading);
		int fontGreen = (int)0xFF + (int)((float)color->m_g*fontShading) - (int)((float)0xFF*fontShading);
		int fontBlue = (int)0xFF + (int)((float)color->m_b*fontShading) - (int)((float)0xFF*fontShading);
		tmpString.sprintf("#%.2x%.2x%.2x", fontRed, fontGreen, fontBlue);
	}
	else
		tmpString.sprintf("#%.2x%.2x%.2x", 0xFF, 0xFF, 0xFF); // default to white: we really shouldn't be calling this function in that case though

	return tmpString;
}

WPXString WPXHLListener::_mergeColorsToString(const RGBSColor *fgColor, const RGBSColor *bgColor)
{
	WPXString tmpColor;
	RGBSColor tmpFgColor, tmpBgColor;

	if (fgColor != NULL) {
		tmpFgColor.m_r = fgColor->m_r;
		tmpFgColor.m_g = fgColor->m_g;
		tmpFgColor.m_b = fgColor->m_b;
		tmpFgColor.m_s = fgColor->m_s;
	}
	else {
		tmpFgColor.m_r = tmpFgColor.m_g = tmpFgColor.m_b = 0xFF;
		tmpFgColor.m_s = 0x64; // 100%
	}
	if (bgColor != NULL) {
		tmpBgColor.m_r = bgColor->m_r;
		tmpBgColor.m_g = bgColor->m_g;
		tmpBgColor.m_b = bgColor->m_b;
		tmpBgColor.m_s = bgColor->m_s;
	}
	else {
		tmpBgColor.m_r = tmpBgColor.m_g = tmpBgColor.m_b = 0xFF;
		tmpBgColor.m_s = 0x64; // 100%
	}

	float fgAmount = (float)tmpFgColor.m_s/100.0f;
	float bgAmount = LIBWPD_MAX(((float)tmpBgColor.m_s-(float)tmpFgColor.m_s)/100.0f, 0.0f);

	int bgRed = LIBWPD_MIN((int)(((float)tmpFgColor.m_r*fgAmount)+((float)tmpBgColor.m_r*bgAmount)), 255);
	int bgGreen = LIBWPD_MIN((int)(((float)tmpFgColor.m_g*fgAmount)+((float)tmpBgColor.m_g*bgAmount)), 255);
	int bgBlue = LIBWPD_MIN((int)(((float)tmpFgColor.m_b*fgAmount)+((float)tmpBgColor.m_b*bgAmount)), 255);

	tmpColor.sprintf("#%.2x%.2x%.2x", bgRed, bgGreen, bgBlue);

	return tmpColor;
}
