/* libwpd
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002-2003 Marc Maurer (j.m.maurer@student.utwente.nl)
 * Copyright (C) 2004 Fridrich Strba (fridrich.strba@bluewin.ch)
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
#endif

_WPXParsingState::_WPXParsingState(bool sectionAttributesChanged) :
	m_textAttributeBits(0),
	m_textAttributesChanged(false),
	m_fontSize(12.0f/*WP6_DEFAULT_FONT_SIZE*/), // FIXME ME!!!!!!!!!!!!!!!!!!! HELP WP6_DEFAULT_FONT_SIZE
	m_fontName(new UTF8String(/*WP6_DEFAULT_FONT_NAME*/"Times New Roman")), // EN PAS DEFAULT FONT AAN VOOR WP5/6/etc
	m_fontColor(new RGBSColor(0x00,0x00,0x00,0x64)), //Set default to black. Maybe once it will change, but for the while...
	m_highlightColor(NULL),

	m_isParagraphColumnBreak(false),
	m_isParagraphPageBreak(false),
	m_paragraphLineSpacing(1.0f),
	m_paragraphJustification(WPX_PARAGRAPH_JUSTIFICATION_LEFT),
//	m_tempParagraphJustification(0),

	m_isSectionOpened(false),
	m_isPageSpanBreakDeferred(false),

	m_isParagraphOpened(false),
	m_isParagraphClosed(false),
	m_isSpanOpened(false),
	m_numDeferredParagraphBreaks(0),

	m_currentTableCol(0),
	m_currentTableRow(0),
	m_isTableOpened(false),
	m_isTableRowOpened(false),
	m_isTableCellOpened(false),
	m_wasHeaderRow(false),
	m_isCellWithoutParagraph(false),
	m_cellAttributeBits(0),
	m_paragraphJustificationBeforeTable(WPX_PARAGRAPH_JUSTIFICATION_LEFT),

	m_isPageSpanOpened(false),
	m_nextPageSpanIndice(0),
	m_numPagesRemainingInSpan(0),

	m_sectionAttributesChanged(sectionAttributesChanged),
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

	m_paragraphTextIndent(0.0f),
	m_textIndentByParagraphIndentChange(0.0f),
	m_textIndentByTabs(0.0f),
/*	m_currentRow(-1),
	m_currentColumn(-1),

	m_currentListLevel(0),
	m_putativeListElementHasParagraphNumber(false),
	m_putativeListElementHasDisplayReferenceNumber(false),

	m_noteTextPID(0),
	m_inSubDocument(false)
*/

	m_alignmentCharacter('.'),
	m_isTabPositionRelative(false)
{
}

_WPXParsingState::~_WPXParsingState()
{
	DELETEP(m_fontName);
	DELETEP(m_fontColor);
	DELETEP(m_highlightColor);
}

WPXHLListener::WPXHLListener(vector<WPXPageSpan *> *pageList, WPXHLListenerImpl *listenerImpl) :
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
	// FIXME: this is stupid, we should store a property list filled with the relevant metadata
	// and then pass that directly..

	m_listenerImpl->setDocumentMetaData(m_metaData);

	m_listenerImpl->startDocument();
	_openPageSpan();
}

void WPXHLListener::_openSection()
{
	_closeSection();

	WPXPropertyList propList;
	propList.insert("num-columns", WPXPropertyFactory::newIntProp(m_ps->m_numColumns));

	if (m_ps->m_numColumns > 1)
		propList.insert("margin-bottom", WPXPropertyFactory::newFloatProp(1.0f));
	else
		propList.insert("margin-bottom", WPXPropertyFactory::newFloatProp(0.0f));

	m_listenerImpl->openSection(propList, m_ps->m_textColumns);

	m_ps->m_sectionAttributesChanged = false;
	m_ps->m_isSectionOpened = true;
}

void WPXHLListener::_closeSection()
{
	_closeParagraph();
	if (m_ps->m_isSectionOpened)
		m_listenerImpl->closeSection();

	m_ps->m_isSectionOpened = false;
}

void WPXHLListener::_openPageSpan()
{
	_closePageSpan();

	// Hack to be sure that the paragraph margins are consistent even if the page margin changes
	m_ps->m_leftMarginByPageMarginChange += m_ps->m_pageMarginLeft;
	m_ps->m_rightMarginByPageMarginChange += m_ps->m_pageMarginRight;
	
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
	propList.insert("num-pages", WPXPropertyFactory::newIntProp(currentPage->getPageSpan()));
	propList.insert("is-last-page-span", WPXPropertyFactory::newIntProp(isLastPageSpan));
	propList.insert("page-height", WPXPropertyFactory::newFloatProp(currentPage->getFormLength()));
	propList.insert("page-width", WPXPropertyFactory::newFloatProp(currentPage->getFormWidth()));
	propList.insert("print-orientation", WPXPropertyFactory::newIntProp(currentPage->getFormOrientation())); 
	propList.insert("margin-left", WPXPropertyFactory::newFloatProp(currentPage->getMarginLeft()));
	propList.insert("margin-right", WPXPropertyFactory::newFloatProp(currentPage->getMarginRight()));
	propList.insert("margin-top", WPXPropertyFactory::newFloatProp(currentPage->getMarginTop()));
	propList.insert("margin-bottom", WPXPropertyFactory::newFloatProp(currentPage->getMarginBottom()));
	
	m_listenerImpl->openPageSpan(propList);

	m_ps->m_pageFormWidth = currentPage->getFormWidth();
	m_ps->m_pageMarginLeft = currentPage->getMarginLeft();
	m_ps->m_pageMarginRight = currentPage->getMarginRight();

	// Hack to be sure that the paragraph margins are consistent even if the page margin changes
	// Compute new values
	m_ps->m_leftMarginByPageMarginChange -= m_ps->m_pageMarginLeft;
	m_ps->m_rightMarginByPageMarginChange -= m_ps->m_pageMarginRight;

	m_ps->m_paragraphMarginLeft = m_ps->m_leftMarginByPageMarginChange + m_ps->m_leftMarginByParagraphMarginChange
			+ m_ps->m_leftMarginByTabs;
	m_ps->m_paragraphMarginRight = m_ps->m_rightMarginByPageMarginChange + m_ps->m_rightMarginByParagraphMarginChange
			+ m_ps->m_rightMarginByTabs;

	vector<WPXHeaderFooter> headerFooterList = currentPage->getHeaderFooterList();
	for (vector<WPXHeaderFooter>::iterator iter = headerFooterList.begin(); iter != headerFooterList.end(); iter++)
	{
		if (!currentPage->getHeaderFooterSuppression((*iter).getInternalType()))
		{
			WPXPropertyList propList;
			propList.insert("occurence", WPXPropertyFactory::newIntProp((*iter).getOccurence()));
			if ((*iter).getType() == HEADER)
				m_listenerImpl->openHeader(propList); 
			else
				m_listenerImpl->openFooter(propList); 

			handleSubDocument((*iter).getTextPID(), true, (*iter).getTableList());
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

	m_ps->m_numPagesRemainingInSpan = (currentPage->getPageSpan() - 1);
	m_ps->m_nextPageSpanIndice++;
	m_ps->m_isPageSpanOpened = true;
	_openSection();
}

void WPXHLListener::_closePageSpan()
{
	if (m_ps->m_isPageSpanOpened)
	{
		_closeSection();
		m_listenerImpl->closePageSpan();
		m_ps->m_isPageSpanOpened = false;
	}
}

void WPXHLListener::_openParagraph()
{
	_closeParagraph();

	WPXTabStop tmp_tabStop;
	vector<WPXTabStop> tabStops;
	for (int i=0; i<m_ps->m_tabStops.size(); i++)
	{
		tmp_tabStop = m_ps->m_tabStops[i];
		if (m_ps->m_isTabPositionRelative)
			tmp_tabStop.m_position -= m_ps->m_leftMarginByTabs;
		else
			tmp_tabStop.m_position -= m_ps->m_paragraphMarginLeft + m_ps->m_pageMarginLeft;
		/* TODO: fix situations where we have several columns or are inside a table and the tab stop
		 *       positions are absolute (relative to the paper edge). In this case, they have to be
		 *       computed for each column or each cell in table. (Fridrich) */
		tabStops.push_back(tmp_tabStop);
	}
	WPXPropertyList propList;
	_appendParagraphProperties(propList, 0);

	m_listenerImpl->openParagraph(propList, tabStops);

	if (m_ps->m_numDeferredParagraphBreaks > 0)
		m_ps->m_numDeferredParagraphBreaks--;

	m_ps->m_isParagraphColumnBreak = false;
	m_ps->m_isParagraphPageBreak = false;
	m_ps->m_isParagraphOpened = true;
	m_ps->m_paragraphMarginLeft = m_ps->m_leftMarginByPageMarginChange + m_ps->m_leftMarginByParagraphMarginChange;
	m_ps->m_paragraphMarginRight = m_ps->m_rightMarginByPageMarginChange + m_ps->m_rightMarginByParagraphMarginChange;
	m_ps->m_leftMarginByTabs = 0.0f;
	m_ps->m_rightMarginByTabs = 0.0f;
	m_ps->m_paragraphTextIndent = m_ps->m_textIndentByParagraphIndentChange;
	m_ps->m_textIndentByTabs = 0.0f;	

	_openSpan();
}


void WPXHLListener::_appendParagraphProperties(WPXPropertyList &propList, int justification)
{
	propList.insert("justification", WPXPropertyFactory::newIntProp(justification));
	propList.insert("margin-left", WPXPropertyFactory::newFloatProp(m_ps->m_paragraphMarginLeft));
	propList.insert("margin-right", WPXPropertyFactory::newFloatProp(m_ps->m_paragraphMarginRight));
	propList.insert("margin-top", WPXPropertyFactory::newFloatProp(m_ps->m_paragraphMarginTop));
	propList.insert("margin-bottom", WPXPropertyFactory::newFloatProp(m_ps->m_paragraphMarginBottom));
	propList.insert("text-indent", WPXPropertyFactory::newFloatProp(m_ps->m_paragraphTextIndent));
	propList.insert("line-spacing", WPXPropertyFactory::newFloatProp(m_ps->m_paragraphLineSpacing));
	propList.insert("column-break", WPXPropertyFactory::newIntProp(m_ps->m_isParagraphColumnBreak));
	propList.insert("page-break", WPXPropertyFactory::newIntProp(m_ps->m_isParagraphPageBreak));
}

void WPXHLListener::_closeParagraph()
{
	_closeSpan();
	if (m_ps->m_isParagraphOpened)
		m_listenerImpl->closeParagraph();

	m_ps->m_isParagraphOpened = false;
}

void WPXHLListener::_openSpan()
{
	_closeSpan();
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
	propList.insert("text-attribute-bits", WPXPropertyFactory::newIntProp(attributeBits & 0xffffffe0));
	propList.insert("font-name", WPXPropertyFactory::newStringProp(*(m_ps->m_fontName)));
	propList.insert("font-size", WPXPropertyFactory::newFloatProp(fontSizeChange*m_ps->m_fontSize));
	propList.insert("color", WPXPropertyFactory::newStringProp(_colorToString(m_ps->m_fontColor)));
	propList.insert("text-background-color", WPXPropertyFactory::newStringProp(_colorToString(m_ps->m_highlightColor)));

	m_listenerImpl->openSpan(propList);

	m_ps->m_isSpanOpened = true;
}

void WPXHLListener::_closeSpan()
{
	if (m_ps->m_isSpanOpened)
		m_listenerImpl->closeSpan();

	m_ps->m_isSpanOpened = false;
}

void WPXHLListener::_openTable()
{
	_closeTable();

	WPXPropertyList propList;
	propList.insert("alignment", WPXPropertyFactory::newIntProp(m_ps->m_tableDefinition.m_positionBits));
	propList.insert("margin-left", WPXPropertyFactory::newFloatProp(m_ps->m_paragraphMarginLeft));
	propList.insert("margin-right", WPXPropertyFactory::newFloatProp(m_ps->m_paragraphMarginRight));
	propList.insert("left-offset", WPXPropertyFactory::newFloatProp(m_ps->m_tableDefinition.m_leftOffset));

	m_listenerImpl->openTable(propList, m_ps->m_tableDefinition.columns);
	m_ps->m_isTableOpened = true;

	m_ps->m_currentTableRow = (-1);
	m_ps->m_currentTableCol = (-1);
}

void WPXHLListener::_closeTable()
{
	_closeTableRow();

	if (m_ps->m_isTableOpened)
	{
		m_listenerImpl->closeTable();
		m_ps->m_currentTableRow = (-1);
		m_ps->m_currentTableCol = (-1);
	}
	m_ps->m_isTableOpened = false;
	m_ps->m_wasHeaderRow = false;

	// handle case where page span is closed in the middle of a table
	if (m_ps->m_isPageSpanBreakDeferred)
	{
	    _closePageSpan();
	    m_ps->m_isPageSpanBreakDeferred = false;
	}
}

void WPXHLListener::_openTableRow(const float height, const bool isMinimumHeight, const bool isHeaderRow)
{
	_closeTableRow();
	m_ps->m_currentTableCol = 0;

	WPXPropertyList propList;
	propList.insert("height", WPXPropertyFactory::newFloatProp(height));
	propList.insert("is-minimum-height", WPXPropertyFactory::newIntProp(isMinimumHeight));

	// Only the first "Header Row" in a table is the actual "Header Row"
	// The following "Header Row" flags are ignored
	if (isHeaderRow & !m_ps->m_wasHeaderRow)
	{
		propList.insert("is-header-row", WPXPropertyFactory::newIntProp(true));		
		m_ps->m_wasHeaderRow = true;
	}
	else
		propList.insert("is-header-row", WPXPropertyFactory::newIntProp(false));		

	m_listenerImpl->openTableRow(propList);

	m_ps->m_isTableRowOpened = true;
	m_ps->m_currentTableRow++;
}

void WPXHLListener::_closeTableRow()
{
	_closeTableCell();

	if (m_ps->m_isTableRowOpened)
		m_listenerImpl->closeTableRow();
	m_ps->m_isTableRowOpened = false;
}

void WPXHLListener::_openTableCell(const uint8_t colSpan, const uint8_t rowSpan, const bool boundFromLeft, const bool boundFromAbove,
				   const uint8_t borderBits, const RGBSColor * cellFgColor, const RGBSColor * cellBgColor,
				   const RGBSColor * cellBorderColor, const WPXVerticalAlignment cellVerticalAlignment)
{
	_closeTableCell();

	WPXPropertyList propList;
	propList.insert("col", WPXPropertyFactory::newIntProp(m_ps->m_currentTableCol));		
	propList.insert("row", WPXPropertyFactory::newIntProp(m_ps->m_currentTableRow));		

	if (!boundFromLeft && !boundFromAbove)
	{
		propList.insert("col-span", WPXPropertyFactory::newIntProp(colSpan));
		propList.insert("row-span", WPXPropertyFactory::newIntProp(rowSpan));
		propList.insert("border-bits", WPXPropertyFactory::newIntProp(borderBits));
		propList.insert("vertical-alignment", WPXPropertyFactory::newIntProp(cellVerticalAlignment));
		propList.insert("color", WPXPropertyFactory::newStringProp(_mergeColorsToString(cellFgColor, cellBgColor)));
		propList.insert("border-color", WPXPropertyFactory::newStringProp(_colorToString(cellBorderColor)));
		m_listenerImpl->openTableCell(propList);
		m_ps->m_isTableCellOpened = true;
	}
	else
		m_listenerImpl->insertCoveredTableCell(propList);

	m_ps->m_currentTableCol++;
}

void WPXHLListener::_closeTableCell()
{
	if (m_ps->m_isCellWithoutParagraph)
		_openParagraph();
	_closeParagraph();
	m_ps->m_cellAttributeBits = 0x00000000;
	if (m_ps->m_isTableCellOpened)
		m_listenerImpl->closeTableCell();

	m_ps->m_isTableCellOpened = false;
}


/**
Creates an new document state. Saves the old state on a "stack".
*/
void WPXHLListener::handleSubDocument(uint16_t textPID, const bool isHeaderFooter, WPXTableList *tableList)
{
	// save our old parsing state on our "stack"
	WPXParsingState *oldPS = m_ps;
	m_ps = new WPXParsingState(false); // false: don't open a new section unless we must inside this type of sub-document
	// BEGIN: copy page properties into the new parsing state
	m_ps->m_pageFormWidth = oldPS->m_pageFormWidth;
	m_ps->m_pageMarginLeft = oldPS->m_pageMarginLeft;
	m_ps->m_pageMarginRight = oldPS->m_pageMarginRight;
	// END: copy page properties into the new parsing state
	_handleSubDocument(textPID, isHeaderFooter, tableList);

	// restore our old parsing state
	delete m_ps;
	m_ps = oldPS;
}

void WPXHLListener::insertBreak(const uint8_t breakType)
{
	if (!isUndoOn())
	{
		_flushText();
		switch (breakType)
		{
		case WPX_COLUMN_BREAK:
			if (m_ps->m_isTextColumnWithoutParagraph)
			{
				_openParagraph(); // handle a case where two column breaks are following each other
				_flushText();
			}
			m_ps->m_numDeferredParagraphBreaks++;
			m_ps->m_isParagraphColumnBreak = true;
			m_ps->m_isTextColumnWithoutParagraph = true;
			break;
		case WPX_PAGE_BREAK:
			m_ps->m_numDeferredParagraphBreaks++;
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
				_openPageSpan();
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

UTF8String WPXHLListener::_colorToString(const RGBSColor * color)
{
	UTF8String tmpString;

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

UTF8String WPXHLListener::_mergeColorsToString(const RGBSColor *fgColor, const RGBSColor *bgColor)
{
	UTF8String tmpColor;
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
	float bgAmount = max(((float)tmpBgColor.m_s-(float)tmpFgColor.m_s)/100.0f, 0.0f);

	int bgRed = min((int)(((float)tmpFgColor.m_r*fgAmount)+((float)tmpBgColor.m_r*bgAmount)), 255);
	int bgGreen = min((int)(((float)tmpFgColor.m_g*fgAmount)+((float)tmpBgColor.m_g*bgAmount)), 255);
	int bgBlue = min((int)(((float)tmpFgColor.m_b*fgAmount)+((float)tmpBgColor.m_b*bgAmount)), 255);

	tmpColor.sprintf("#%.2x%.2x%.2x", bgRed, bgGreen, bgBlue);

	return tmpColor;
}
