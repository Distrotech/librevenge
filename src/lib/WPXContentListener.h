/* libwpd
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

#ifndef WPXCONTENTLISTENER_H
#define WPXCONTENTLISTENER_H

#include "WPXTable.h"
#include "WPXPropertyListVector.h"
#include "WPXPropertyList.h"
#include "libwpd_internal.h"
#include "WPXSubDocument.h"
#include "WPXPageSpan.h"
#include "WPXDocumentInterface.h"
#include "WPXListener.h"
#include <vector>
#include <list>
#include <set>

typedef struct _WPXTableDefinition WPXTableDefinition;
struct _WPXTableDefinition
{
	_WPXTableDefinition() : m_positionBits(0), m_leftOffset(0.0), m_columns(), m_columnsProperties() {};
	uint8_t m_positionBits;
	double m_leftOffset;
	std::vector < WPXColumnDefinition > m_columns;
	std::vector < WPXColumnProperties > m_columnsProperties;
};

typedef struct _WPXContentParsingState WPXContentParsingState;
struct _WPXContentParsingState
{
	_WPXContentParsingState();
	~_WPXContentParsingState();

	uint32_t m_textAttributeBits;
	double m_fontSize;
	WPXString *m_fontName;
	RGBSColor *m_fontColor;
	RGBSColor *m_highlightColor;

	bool m_isParagraphColumnBreak;
	bool m_isParagraphPageBreak;
	uint8_t m_paragraphJustification;
	uint8_t m_tempParagraphJustification; // TODO: remove this one after the tabs are properly implemented
	double m_paragraphLineSpacing;

	bool m_isDocumentStarted;
	bool m_isPageSpanOpened;
	bool m_isSectionOpened;
	bool m_isPageSpanBreakDeferred;
	bool m_isHeaderFooterWithoutParagraph;

	bool m_isSpanOpened;
	bool m_isParagraphOpened;
	bool m_isListElementOpened;

	bool m_firstParagraphInPageSpan;

	std::vector<unsigned int> m_numRowsToSkip;
	WPXTableDefinition m_tableDefinition;
	int m_currentTableCol;
	int m_currentTableRow;
	int m_currentTableCellNumberInRow;
	bool m_isTableOpened;
	bool m_isTableRowOpened;
	bool m_isTableColumnOpened;
	bool m_isTableCellOpened;
	bool m_wasHeaderRow;
	bool m_isCellWithoutParagraph;
	bool m_isRowWithoutCell;
	uint32_t m_cellAttributeBits;
	uint8_t m_paragraphJustificationBeforeTable;
	
	unsigned m_currentPage;
	int m_numPagesRemainingInSpan;
	int m_currentPageNumber;

	bool m_sectionAttributesChanged;
	int m_numColumns;
	std::vector < WPXColumnDefinition > m_textColumns;
	bool m_isTextColumnWithoutParagraph;

	double m_pageFormLength;
	double m_pageFormWidth;
	WPXFormOrientation m_pageFormOrientation;

	double m_pageMarginLeft;
	double m_pageMarginRight;
	double m_pageMarginTop;
	double m_pageMarginBottom;
	double m_paragraphMarginLeft;  // resulting paragraph margin that is one of the paragraph
	double m_paragraphMarginRight; // properties
	double m_paragraphMarginTop;
	double m_paragraphMarginBottom;
	double m_leftMarginByPageMarginChange;  // part of the margin due to the PAGE margin change
	double m_rightMarginByPageMarginChange; // inside a page that already has content.
	double m_sectionMarginLeft;  // In multicolumn sections, the above two will be rather interpreted
	double m_sectionMarginRight; // as section margin change 
	double m_leftMarginByParagraphMarginChange;  // part of the margin due to the PARAGRAPH
	double m_rightMarginByParagraphMarginChange; // margin change (in WP6)
	double m_leftMarginByTabs;  // part of the margin due to the LEFT or LEFT/RIGHT Indent; the
	double m_rightMarginByTabs; // only part of the margin that is reset at the end of a paragraph	

	double m_listReferencePosition; // position from the left page margin of the list number/bullet
	double m_listBeginPosition; // position from the left page margin of the beginning of the list

	double m_paragraphTextIndent; // resulting first line indent that is one of the paragraph properties
	double m_textIndentByParagraphIndentChange; // part of the indent due to the PARAGRAPH indent (WP6???)
	double m_textIndentByTabs; // part of the indent due to the "Back Tab" or "Left Tab"

	uint8_t m_currentListLevel;
	
	uint16_t m_alignmentCharacter;
	std::vector<WPXTabStop> m_tabStops;
	bool m_isTabPositionRelative;

	std::set <const WPXSubDocument *> m_subDocuments;

	bool m_inSubDocument;
	bool m_isNote;
	WPXSubDocumentType m_subDocumentType;

private:
	_WPXContentParsingState(const _WPXContentParsingState&);
	_WPXContentParsingState& operator=(const _WPXContentParsingState&);
};

class WPXContentListener : public WPXListener
{
protected:
	WPXContentListener(std::list<WPXPageSpan> &pageList, WPXDocumentInterface *documentInterface);
	virtual ~WPXContentListener();

	void startDocument();
	void startSubDocument();
	void endDocument();
	void endSubDocument();
	void handleSubDocument(const WPXSubDocument *subDocument, WPXSubDocumentType subDocumentType, WPXTableList tableList, int nextTableIndice);
	void insertBreak(const uint8_t breakType);
	void lineSpacingChange(const double lineSpacing);
	void justificationChange(const uint8_t justification);

	WPXContentParsingState *m_ps; // parse state
	WPXDocumentInterface * m_documentInterface;
	WPXPropertyList m_metaData;

	virtual void _handleSubDocument(const WPXSubDocument *subDocument, WPXSubDocumentType subDocumentType, WPXTableList tableList, int nextTableIndice) = 0;
	virtual void _flushText() = 0;
	virtual void _changeList() = 0;

	void _openSection();
	void _closeSection();

	void _openPageSpan();
	void _closePageSpan();

	void _appendParagraphProperties(WPXPropertyList &propList, const bool isListElement=false);
	void _getTabStops(WPXPropertyListVector &tabStops);
	void _appendJustification(WPXPropertyList &propList, int justification);
	void _resetParagraphState(const bool isListElement=false);
	virtual void _openParagraph();
	void _closeParagraph();

	void _openListElement();
	void _closeListElement();

	void _openSpan();
	void _closeSpan();

	void _openTable();
	void _closeTable();
	void _openTableRow(const double height, const bool isMinimumHeight, const bool isHeaderRow);
	void _closeTableRow();
	void _openTableCell(const uint8_t colSpan, const uint8_t rowSpan, const uint8_t borderBits, 
				const RGBSColor * cellFgColor, const RGBSColor * cellBgColor,
				const RGBSColor * cellBorderColor,
				const WPXVerticalAlignment cellVerticalAlignment);
	void _closeTableCell();

	double _movePositionToFirstColumn(double position);

	double _getNextTabStop() const;
	double _getPreviousTabStop() const;
	
        void _insertText(const WPXString &textBuffer);

	void _insertBreakIfNecessary(WPXPropertyList &propList);
	
	void _insertPageNumberParagraph(WPXPageNumberPosition position, WPXNumberingType type, WPXString fontName, double fontSize);

	uint16_t _mapNonUnicodeCharacter(uint16_t character);
	
private:
	WPXContentListener(const WPXContentListener&);
	WPXContentListener& operator=(const WPXContentListener&);
	WPXString _colorToString(const RGBSColor * color);
	WPXString _mergeColorsToString(const RGBSColor *fgColor, const RGBSColor *bgColor);
	uint16_t _mapSymbolFontCharacter(uint16_t character);
	uint16_t _mapDingbatsFontCharacter(uint16_t character);
};

#endif /* WPXCONTENTLISTENER_H */
