/* libwpd
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002 Marc Maurer (j.m.maurer@student.utwente.nl)
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

#ifndef WPXHLLISTENER_H
#define WPXHLLISTENER_H

#include "WPXLLListener.h"
#include "WPXHLListenerImpl.h"
#include "WPXTable.h"
#include "WPXVector.h"

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
	vector < WPXColumnDefinition > columns;
};

typedef struct _WPXParsingState WPXParsingState;
struct _WPXParsingState
{
	_WPXParsingState(bool sectionAttributesChanged=true);
	~_WPXParsingState();

/*
	WPXString m_bodyText;
	WPXString m_textBeforeNumber;
	WPXString m_textBeforeDisplayReference;
	WPXString m_numberText;
	WPXString m_textAfterDisplayReference;
	WPXString m_textAfterNumber;
*/
	uint32_t m_textAttributeBits;
	bool m_textAttributesChanged;
	float m_fontSize;
	WPXString *m_fontName;
	RGBSColor *m_fontColor;
	RGBSColor *m_highlightColor;

	bool m_isParagraphColumnBreak;
	bool m_isParagraphPageBreak;
	uint8_t m_paragraphJustification;
	uint8_t m_tempParagraphJustification; // TODO: remove this one after the tabs are properly implemented
	float m_paragraphLineSpacing;

	bool m_isSectionOpened;
	bool m_isPageSpanBreakDeferred;

	bool m_isParagraphOpened;
	bool m_isParagraphClosed;
	bool m_isListElementOpened;
	bool m_isListElementClosed;
	bool m_isSpanOpened;
	int m_numDeferredParagraphBreaks;

	WPXTableDefinition m_tableDefinition;
	int m_currentTableCol;
	int m_currentTableRow;
	bool m_isTableOpened;
	bool m_isTableRowOpened;
	bool m_isTableColumnOpened;
	bool m_isTableCellOpened;
	bool m_wasHeaderRow;
	bool m_isCellWithoutParagraph;
	uint32_t m_cellAttributeBits;
	uint8_t m_paragraphJustificationBeforeTable;
	
	bool m_isPageSpanOpened;
	int m_nextPageSpanIndice;
	int m_numPagesRemainingInSpan;

	bool m_sectionAttributesChanged;
	int m_numColumns;
	vector < WPXColumnDefinition > m_textColumns;
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

	float m_paragraphTextIndent; // resulting first line indent that is one of the paragraph properties
	float m_textIndentByParagraphIndentChange; // part of the indent due to the PARAGRAPH indent (WP6???)
	float m_textIndentByTabs; // part of the indent due to the "Back Tab"
	
/*	int32_t m_currentRow;
	int32_t m_currentColumn;

	stack<int> m_listLevelStack;
	uint16_t m_currentOutlineHash; // probably should replace Hash with Key in these sorts of cases
	uint8_t m_oldListLevel;
	uint8_t m_currentListLevel;
	WP6StyleStateSequence m_styleStateSequence;
	bool m_putativeListElementHasParagraphNumber;
	bool m_putativeListElementHasDisplayReferenceNumber;

	int m_noteTextPID;
*/
	uint16_t m_alignmentCharacter;
	vector<WPXTabStop> m_tabStops;
	bool m_isTabPositionRelative;
};

class WPXHLListener : public WPXLLListener
{
public:
	WPXHLListener(vector<WPXPageSpan *> *pageList, WPXHLListenerImpl *listenerImpl);
	WPXHLListener::~WPXHLListener();

	void startDocument();
	void handleSubDocument(uint16_t textPID, const bool isHeaderFooter, WPXTableList tableList, int nextTableIndice);
	virtual void insertBreak(const uint8_t breakType);
	virtual void lineSpacingChange(const float lineSpacing);
	virtual void justificationChange(const uint8_t justification);

	WPXParsingState *m_ps; // parse state
	WPXHLListenerImpl * m_listenerImpl;
	WPXPropertyList m_metaData;
	vector <WPXPageSpan *> *m_pageList;

protected:
	virtual void _handleSubDocument(uint16_t textPID, const bool isHeaderFooter, WPXTableList tableList, int nextTableIndice) = 0;
	virtual void _flushText(const bool fakeText=false) = 0;
	virtual void _flushList() = 0;

	void _openSection();
	void _closeSection();

	void _openPageSpan();
	void _closePageSpan();

	void _appendParagraphProperties(WPXPropertyList &propList);
	void _getTabStops(WPXVector<WPXPropertyList> &tabStops);
	void _appendJustification(WPXPropertyList &propList, int justification);
	virtual void _openParagraph();
	virtual void _resetParagraphState(const bool isListElement=false);
	void _closeParagraph();

	void _openListElement();
	void _closeListElement();	

	void _openSpan();
	void _closeSpan();

	void _openTable();
	void _closeTable();
	void _openTableRow(const float height, const bool isMinimumHeight, const bool isHeaderRow);
	void _closeTableRow();
	void _openTableCell(const uint8_t colSpan, const uint8_t rowSpan, const bool boundFromLeft, const bool boundFromAbove,
				const uint8_t borderBits, const RGBSColor * cellFgColor, const RGBSColor * cellBgColor,
				const RGBSColor * cellBorderColor,
				const WPXVerticalAlignment cellVerticalAlignment);
	void _closeTableCell();

	bool isUndoOn() { return m_isUndoOn; }
	bool m_isUndoOn;
	
private:
	WPXString _colorToString(const RGBSColor * color);
	WPXString _mergeColorsToString(const RGBSColor *fgColor, const RGBSColor *bgColor);
};

#endif /* WPXHLLISTENER_H */
