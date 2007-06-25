/* libwpd
 * Copyright (C) 2004 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2005-2006 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#ifndef WP3CONTENTLISTENER_H
#define WP3CONTENTLISTENER_H

#include "WP3Listener.h"
#include "WPXContentListener.h"
#include "WP3SubDocument.h"

typedef struct _WP3ContentParsingState WP3ContentParsingState;
struct _WP3ContentParsingState
{
	_WP3ContentParsingState();
	~_WP3ContentParsingState();
	uint16_t m_colSpan;
	uint16_t m_rowSpan;
	WPXString m_textBuffer;
	RGBSColor * m_cellFillColor;
	WPXString m_noteReference;

	WPXTableList m_tableList;
private:
	_WP3ContentParsingState(const _WP3ContentParsingState&);
	_WP3ContentParsingState& operator=(const _WP3ContentParsingState&);
};

class WP3ContentListener : public WP3Listener, protected WPXContentListener
{
public:
	WP3ContentListener(std::list<WPXPageSpan> &pageList, std::vector<WP3SubDocument *> &subDocuments, WPXDocumentInterface *listenerImpl);
	~WP3ContentListener();

	void startDocument() { WPXContentListener::startDocument(); };
	void insertCharacter(const uint16_t character);
	void insertTab(const uint8_t tabType, float tabPosition);
	void insertBreak(const uint8_t breakType) { WPXContentListener::insertBreak(breakType); };
	void insertEOL();
	void attributeChange(const bool isOn, const uint8_t attribute);
	void lineSpacingChange(const float lineSpacing) { WPXContentListener::lineSpacingChange(lineSpacing); };
	void pageMarginChange(const uint8_t /* side */, const uint16_t /* margin */) {};
	void pageFormChange(const uint16_t /* length */, const uint16_t /* width */, const WPXFormOrientation /* orientation */) {};
	void marginChange(const uint8_t side, const uint16_t margin);
	void indentFirstLineChange(const int16_t offset);
	void setTabs(const bool isRelative, const std::vector<WPXTabStop> tabStops);
	void columnChange(const WPXTextColumnType columnType, const uint8_t numColumns, const std::vector<float> &columnWidth,
					const std::vector<bool> &isFixedWidth);
	void endDocument() { WPXContentListener::endDocument(); };

	void defineTable(const uint8_t position, const uint16_t leftOffset);
	void addTableColumnDefinition(const uint32_t width, const uint32_t leftGutter, const uint32_t rightGutter,
					const uint32_t attributes, const uint8_t alignment);
	void startTable();
 	void insertRow();
 	void insertCell();
 	void closeCell();
	void closeRow();
	void setTableCellSpan(const uint16_t colSpan, const uint16_t rowSpan);
	void setTableCellFillColor(const RGBSColor * cellFillColor);
 	void endTable();
	void undoChange(const uint8_t undoType, const uint16_t undoLevel);
	void justificationChange(const uint8_t justification);
	void setTextColor(const RGBSColor * fontColor);
	void setTextFont(const WPXString &fontName);
	void setFontSize(const uint16_t fontSize);
	void insertPageNumber(const WPXString &pageNumber);
	void insertNoteReference(const WPXString &noteReference);
	void insertNote(const WPXNoteType noteType, WP3SubDocument *subDocument);
	void headerFooterGroup(const uint8_t headerFooterType, const uint8_t occurenceBits, WP3SubDocument *subDocument);
	void suppressPage(const uint16_t /* suppressCode */) {};
	
protected:
	void _handleSubDocument(const WPXSubDocument *subDocument, const bool isHeaderFooter, WPXTableList tableList, int nextTableIndice = 0);
	void _openParagraph();

	void _flushText();
	void _changeList() {};

private:
	WP3ContentListener(const WP3ContentListener&);
	WP3ContentListener& operator=(const WP3ContentListener&);
	WP3ContentParsingState *m_parseState;
	std::vector<WP3SubDocument *> &m_subDocuments;
};

#endif /* WP3CONTENTLISTENER_H */
