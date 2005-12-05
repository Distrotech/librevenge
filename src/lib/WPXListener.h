/* libwpd
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002 Marc Maurer (j.m.maurer@student.utwente.nl)
 * Copyright (C) 2005 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#ifndef WPXLISTENER_H
#define WPXLISTENER_H

#include "WPXHLListenerImpl.h"
#include "WPXTable.h"
#include "WPXPropertyListVector.h"
#include "libwpd_internal.h"
#include "WPXSubDocument.h"
#include <vector>
#include <set>

class WPXPageSpan;

typedef struct _WPXDocumentMetaData WPXDocumentMetaData;
struct _WPXDocumentMetaData
{
	WPXString m_author;
	WPXString m_subject;
	WPXString m_publisher;
	WPXString m_category;
	WPXString m_keywords;
	WPXString m_language;
	WPXString m_abstract;
	WPXString m_descriptiveName;
	WPXString m_descriptiveType;
};

typedef struct _WPXTableDefinition WPXTableDefinition;
struct _WPXTableDefinition
{
	uint8_t m_positionBits;
	float m_leftOffset;
	std::vector < WPXColumnDefinition > columns;
	std::vector < WPXColumnProperties > columnsProperties;
};

typedef struct _WPXParsingState WPXParsingState;
struct _WPXParsingState
{
	_WPXParsingState();
	~_WPXParsingState();

	uint32_t m_textAttributeBits;
	float m_fontSize;
	WPXString *m_fontName;
	RGBSColor *m_fontColor;
	RGBSColor *m_highlightColor;

	bool m_isParagraphColumnBreak;
	bool m_isParagraphPageBreak;
	uint8_t m_paragraphJustification;
	uint8_t m_tempParagraphJustification; // TODO: remove this one after the tabs are properly implemented
	float m_paragraphLineSpacing;

	bool m_isDocumentStarted;
	bool m_isPageSpanOpened;
	bool m_isSectionOpened;
	bool m_isPageSpanBreakDeferred;

	bool m_isSpanOpened;
	bool m_isParagraphOpened;
	bool m_isListElementOpened;

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
	uint32_t m_cellAttributeBits;
	uint8_t m_paragraphJustificationBeforeTable;
	
	int m_nextPageSpanIndice;
	int m_numPagesRemainingInSpan;

	bool m_sectionAttributesChanged;
	int m_numColumns;
	std::vector < WPXColumnDefinition > m_textColumns;
	bool m_isTextColumnWithoutParagraph;

	float m_pageFormLength;
	float m_pageFormWidth;
	WPXFormOrientation m_pageFormOrientation;

	float m_pageMarginLeft;
	float m_pageMarginRight;
	float m_paragraphMarginLeft;  // resulting paragraph margin that is one of the paragraph
	float m_paragraphMarginRight; // properties
	float m_paragraphMarginTop;
	float m_paragraphMarginBottom;
	float m_leftMarginByPageMarginChange;  // part of the margin due to the PAGE margin change
	float m_rightMarginByPageMarginChange; // inside a page that already has content.
	float m_leftMarginByParagraphMarginChange;  // part of the margin due to the PARAGRAPH
	float m_rightMarginByParagraphMarginChange; // margin change (in WP6)
	float m_leftMarginByTabs;  // part of the margin due to the LEFT or LEFT/RIGHT Indent; the
	float m_rightMarginByTabs; // only part of the margin that is reset at the end of a paragraph

	float m_listReferencePosition; // position from the left page margin of the list number/bullet
	float m_listBeginPosition; // position from the left page margin of the beginning of the list

	float m_paragraphTextIndent; // resulting first line indent that is one of the paragraph properties
	float m_textIndentByParagraphIndentChange; // part of the indent due to the PARAGRAPH indent (WP6???)
	float m_textIndentByTabs; // part of the indent due to the "Back Tab" or "Left Tab"

	uint8_t m_currentListLevel;
	
	uint16_t m_alignmentCharacter;
	std::vector<WPXTabStop> m_tabStops;
	bool m_isTabPositionRelative;

	std::set <const WPXSubDocument *> m_subDocuments;

	bool m_inSubDocument;
	bool m_isNote;
};

class WPXListener
{
public:
	WPXListener(std::vector<WPXPageSpan *> *pageList, WPXHLListenerImpl *listenerImpl);
	virtual ~WPXListener();

	virtual void startDocument();
	void handleSubDocument(const WPXSubDocument *subDocument, const bool isHeaderFooter, WPXTableList tableList, int nextTableIndice);
	virtual void insertBreak(const uint8_t breakType);
	virtual void lineSpacingChange(const float lineSpacing);
	virtual void justificationChange(const uint8_t justification);
	virtual void setAlignmentCharacter(const uint16_t character) = 0;
	virtual void setLeaderCharacter(const uint16_t character, const uint8_t numberOfSpaces) = 0;
	virtual void defineTabStops(const bool isRelative, const std::vector<WPXTabStop> &tabStops, 
				    const std::vector<bool> &usePreWP9LeaderMethods) = 0;
	virtual void insertCharacter(const uint16_t character) = 0;
	virtual void insertTab(const uint8_t tabType, const float tabPosition) = 0;
	virtual void handleLineBreak() = 0;
	virtual void insertEOL() = 0;
	virtual void attributeChange(const bool isOn, const uint8_t attribute) = 0;
	virtual void spacingAfterParagraphChange(const float spacingRelative, const float spacingAbsolute) = 0;
	virtual void pageMarginChange(const uint8_t side, const uint16_t margin) = 0;
	virtual void pageFormChange(const uint16_t length, const uint16_t width, const WPXFormOrientation orientation,
					const bool isPersistent) = 0;
	virtual void marginChange(const uint8_t side, const uint16_t margin) = 0;
	virtual void paragraphMarginChange(const uint8_t side, const int16_t margin) = 0;
	virtual void indentFirstLineChange(const int16_t offset) = 0;
	virtual void columnChange(const WPXTextColumnType columnType, const uint8_t numColumns, const std::vector<float> &columnWidth,
					const std::vector<bool> &isFixedWidth) = 0;
	virtual void endDocument() = 0;

	virtual void defineTable(const uint8_t position, const uint16_t leftOffset) = 0;
	virtual void addTableColumnDefinition(const uint32_t width, const uint32_t leftGutter, const uint32_t rightGutter,
					const uint32_t attributes, const uint8_t alignment) = 0;
	virtual void startTable() = 0;
 	virtual void endTable() = 0;

	WPXParsingState *m_ps; // parse state
	WPXHLListenerImpl * m_listenerImpl;
	WPXPropertyList m_metaData;
	std::vector <WPXPageSpan *> *m_pageList;

protected:
	virtual void _handleSubDocument(const WPXSubDocument *subDocument, const bool isHeaderFooter, WPXTableList tableList, int nextTableIndice) = 0;
	virtual void _flushText() = 0;
	virtual void _changeList() = 0;

	void _openSection();
	void _closeSection();

	void _openPageSpan();
	void _closePageSpan();

	virtual void _appendParagraphProperties(WPXPropertyList &propList, const bool isListElement=false);
	virtual void _getTabStops(WPXPropertyListVector &tabStops);
	virtual void _appendJustification(WPXPropertyList &propList, int justification);
	virtual void _resetParagraphState(const bool isListElement=false);
	virtual void _openParagraph();
	void _closeParagraph();

	void _openListElement();
	void _closeListElement();

	void _openSpan();
	void _closeSpan();

	void _openTable();
	void _closeTable();
	void _openTableRow(const float height, const bool isMinimumHeight, const bool isHeaderRow);
	void _closeTableRow();
	void _openTableCell(const uint8_t colSpan, const uint8_t rowSpan, const uint8_t borderBits, 
				const RGBSColor * cellFgColor, const RGBSColor * cellBgColor,
				const RGBSColor * cellBorderColor,
				const WPXVerticalAlignment cellVerticalAlignment);
	void _closeTableCell();

	bool isUndoOn() { return m_isUndoOn; }
	bool m_isUndoOn;
	
private:
	WPXString _colorToString(const RGBSColor * color);
	WPXString _mergeColorsToString(const RGBSColor *fgColor, const RGBSColor *bgColor);
};

#endif /* WPXLISTENER_H */
