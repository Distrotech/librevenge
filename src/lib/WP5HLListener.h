/* libwpd
 * Copyright (C) 2003 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2003 Marc Maurer (j.m.maurer@student.utwente.nl)
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
 
#ifndef WP5HLLISTENER_H
#define WP5HLLISTENER_H
 
#include "WP5LLListener.h"
#include "WPXHLListener.h"
#include "WPXHLListenerImpl.h"

typedef struct _WP5ParsingState WP5ParsingState;
struct _WP5ParsingState
{
	_WP5ParsingState(bool sectionAttributesChanged=true);
	~_WP5ParsingState();
	UCSString m_bodyText;
	UCSString m_textBeforeNumber;
	UCSString m_textBeforeDisplayReference;
	UCSString m_numberText;
	UCSString m_textAfterDisplayReference;
	UCSString m_textAfterNumber;

	guint32 m_textAttributeBits;
	bool m_textAttributesChanged;
	float m_fontSize;
	GString * m_fontName;

	bool m_isParagraphColumnBreak;
	bool m_isParagraphPageBreak;
	guint8 m_paragraphJustification;
	guint8 m_tempParagraphJustification;
	float m_paragraphLineSpacing;

	bool m_isSectionOpened;

	bool m_isParagraphOpened;
	bool m_isParagraphClosed;
	bool m_isSpanOpened;
	guint m_numDeferredParagraphBreaks;
	guint m_numRemovedParagraphBreaks;

	/*WPXTable *m_currentTable;
	int m_nextTableIndice;
	int m_currentTableCol;
	int m_currentTableRow;
	bool m_isTableOpened;
	bool m_isTableRowOpened;
	bool m_isTableColumnOpened;
	bool m_isTableCellOpened;

	bool m_isPageSpanOpened;
	int m_nextPageSpanIndice;
	int m_numPagesRemainingInSpan;

	bool m_sectionAttributesChanged;
	guint m_numColumns;
	bool m_isLeftMarginSet;
	bool m_isRightMarginSet;
	float m_pageMarginLeft;
	float m_pageMarginRight;
	float m_marginLeft;
	float m_marginRight;
	
	gint32 m_currentRow;
	gint32 m_currentColumn;

	stack<int> m_listLevelStack;
	guint16 m_currentOutlineHash; // probably should replace Hash with Key in these sorts of cases
	guint8 m_oldListLevel;
	guint8 m_currentListLevel;
	WP6StyleStateSequence m_styleStateSequence;
	bool m_putativeListElementHasParagraphNumber;
	bool m_putativeListElementHasDisplayReferenceNumber;

	int m_noteTextPID;
	bool m_inSubDocument;*/
};


class WP5HLListener : public WPXHLListener, public WP5LLListener
{
public:
	WP5HLListener(WPXHLListenerImpl *listenerImpl);

	virtual void startDocument();
	virtual void insertCharacter(const guint16 character);
	virtual void insertTab(const guint8 tabType);
	virtual void insertEOL();
 	virtual void insertBreak(const guint8 breakType) {};
	virtual void attributeChange(const bool isOn, const guint8 attribute);
	virtual void lineSpacingChange(const float lineSpacing) {};
	virtual void justificationChange(const guint8 justification) {};
	virtual void pageMarginChange(const guint8 side, const guint16 margin) {};
	virtual void marginChange(const guint8 side, const guint16 margin) {};
	virtual void columnChange(const guint8 numColumns) {};
	virtual void endDocument();
		
	virtual void defineTable(guint8 position, guint16 leftOffset) {};
	virtual void addTableColumnDefinition(guint32 width, guint32 leftGutter, guint32 rightGutter) {};
	virtual void startTable() {};
 	virtual void insertRow() {};
 	virtual void insertCell(const guint8 colSpan, const guint8 rowSpan, const bool boundFromLeft, const bool boundFromAbove, 
				const guint8 borderBits, 	
				const RGBSColor * cellFgColor, const RGBSColor * cellBgColor) {};
 	virtual void endTable() {};

private:
	void _openParagraph();
	void _closeParagraph();
	void _openSpan();
	void _closeSpan();
	void _flushText();

	WPXHLListenerImpl *m_listenerImpl;

	UCSString m_textBuffer;

	WP5ParsingState *m_parseState;
};

#endif /* WP5HLLISTENER_H */
