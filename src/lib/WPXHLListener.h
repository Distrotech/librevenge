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

class WPXPageSpan;

typedef struct _WPXDocumentMetaData WPXDocumentMetaData;
struct _WPXDocumentMetaData
{
	UCSString m_author;
	UCSString m_subject;
	UCSString m_publisher;
	UCSString m_category;
	UCSString m_keywords;
	UCSString m_language;
	UCSString m_abstract;
	UCSString m_descriptiveName;
	UCSString m_descriptiveType;
};

typedef struct _WPXParsingState WPXParsingState;
struct _WPXParsingState
{
	_WPXParsingState(bool sectionAttributesChanged=true);
	~_WPXParsingState() {}

/*
	UCSString m_bodyText;
	UCSString m_textBeforeNumber;
	UCSString m_textBeforeDisplayReference;
	UCSString m_numberText;
	UCSString m_textAfterDisplayReference;
	UCSString m_textAfterNumber;
*/
	guint32 m_textAttributeBits;
	bool m_textAttributesChanged;
	float m_fontSize;
	GString *m_fontName;
	RGBSColor *m_fontColor;

	bool m_isParagraphColumnBreak;
	bool m_isParagraphPageBreak;
/*	guint8 m_paragraphJustification;
	guint8 m_tempParagraphJustification;
	float m_paragraphLineSpacing;
*/
	bool m_isSectionOpened;

	bool m_isParagraphOpened;
	bool m_isParagraphClosed;
	bool m_isSpanOpened;
	guint m_numDeferredParagraphBreaks;
/*	guint m_numRemovedParagraphBreaks;

	WPXTable *m_currentTable;
	int m_nextTableIndice;
	int m_currentTableCol;
	int m_currentTableRow;
	bool m_isTableOpened;
	bool m_isTableRowOpened;
	bool m_isTableColumnOpened;
	bool m_isTableCellOpened;
*/
	bool m_isPageSpanOpened;
	int m_nextPageSpanIndice;
	int m_numPagesRemainingInSpan;

	bool m_sectionAttributesChanged;
	guint m_numColumns;

	float m_pageFormLength;
	float m_pageFormWidth;
	WPXFormOrientation m_pageFormOrientation;

	float m_pageMarginLeft;
	float m_pageMarginRight;
	float m_paragraphMarginLeft;
	float m_paragraphMarginRight;
	float m_paragraphTextIndent;

	/*gint32 m_currentRow;
	gint32 m_currentColumn;

	stack<int> m_listLevelStack;
	guint16 m_currentOutlineHash; // probably should replace Hash with Key in these sorts of cases
	guint8 m_oldListLevel;
	guint8 m_currentListLevel;
	WP6StyleStateSequence m_styleStateSequence;
	bool m_putativeListElementHasParagraphNumber;
	bool m_putativeListElementHasDisplayReferenceNumber;

	int m_noteTextPID;

	*/

};

class WPXHLListener : public WPXLLListener
{
public:
	WPXHLListener(vector<WPXPageSpan *> *pageList, WPXHLListenerImpl *listenerImpl);
	WPXHLListener::~WPXHLListener();

	void startDocument();
	void handleSubDocument(guint16 textPID);
	virtual void insertBreak(const guint8 breakType);

	WPXParsingState *m_ps; // parse state
	WPXHLListenerImpl * m_listenerImpl;
	WPXDocumentMetaData m_metaData;
	vector <WPXPageSpan *> *m_pageList;

protected:
	virtual void _handleSubDocument(guint16 textPID) = 0;
	virtual void _flushText(const bool fakeText=false) = 0;

	void _openSection();
	void _closeSection();

	void _openPageSpan();
	void _closePageSpan();

	// void _openParagraph();
	void _closeParagraph();

	void _openSpan();
	void _closeSpan();

	bool isUndoOn() { return m_isUndoOn; }
	bool m_isUndoOn;
};

#endif /* WPXHLLISTENER_H */
