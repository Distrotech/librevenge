/* libwpd
 * Copyright (C) 2003 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2003 Marc Maurer (uwog@uwog.net)
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

#ifndef WP1CONTENTLISTENER_H
#define WP1CONTENTLISTENER_H

#include "WP1Listener.h"
#include "WPXContentListener.h"

class WP1SubDocument;

typedef struct _WP1ContentParsingState WP1ContentParsingState;
struct _WP1ContentParsingState
{
	_WP1ContentParsingState();
	~_WP1ContentParsingState();
	WPXString m_textBuffer;
	int m_numDeferredTabs;
	int m_footNoteNumber, m_endNoteNumber;
private:
	_WP1ContentParsingState(const _WP1ContentParsingState&);
	_WP1ContentParsingState& operator=(const _WP1ContentParsingState&);
};

class WP1ContentListener : public WP1Listener, protected WPXContentListener
{
public:
	WP1ContentListener(std::list<WPXPageSpan> &pageList, std::vector<WP1SubDocument *> &subDocuments, WPXDocumentInterface *documentInterface);
	~WP1ContentListener();

	void startDocument() { WPXContentListener::startDocument(); }
	void startSubDocument() { WPXContentListener::startSubDocument(); }
	void insertCharacter(uint16_t character);
	void insertExtendedCharacter(uint8_t extendedCharacter);
	void insertTab();
	void insertBreak(uint8_t breakType) { WPXContentListener::insertBreak(breakType); }
	void insertEOL();
	void insertNote(WPXNoteType noteType, WP1SubDocument *subDocument);
	void attributeChange(bool isOn, uint8_t attribute);
	void fontPointSize(uint8_t pointSize);
	void fontId(uint16_t id);
	void marginReset(uint16_t leftMargin, uint16_t rightMargin);
	void topMarginSet(uint16_t /* topMargin */) {}
	void bottomMarginSet(uint16_t /* bottomMargin */) {}
	void leftIndent(uint16_t leftMarginOffset);
	void leftRightIndent(uint16_t leftRightMarginOffset);
	void leftMarginRelease(uint16_t release);
	void setTabs(const std::vector<WPXTabStop> &tabStops);
	void headerFooterGroup(uint8_t headerFooterDefinition, WP1SubDocument *subDocument);
	void suppressPageCharacteristics(uint8_t /* suppressCode */) {}
	void justificationChange(uint8_t justification);
	void lineSpacingChange(uint8_t spacing) { WPXContentListener::lineSpacingChange((double)((double)spacing/2.0)); }
	void flushRightOn();
	void flushRightOff() {}
	void centerOn();
	void centerOff() {}
	void endDocument() { WPXContentListener::endDocument(); };
	void endSubDocument() { WPXContentListener::endSubDocument(); };
	void insertPicture(uint16_t width, uint16_t height, const WPXBinaryData &binaryData);

protected:
	using WPXContentListener::lineSpacingChange;
	void _handleSubDocument(const WPXSubDocument *subDocument, WPXSubDocumentType subDocumentType, WPXTableList tableList, int nextTableIndice = 0);

	void _flushText();
	void _changeList() {};

private:
	WP1ContentParsingState *m_parseState;
	std::vector<WP1SubDocument *> &m_subDocuments;
	WP1ContentListener(const WP1ContentListener&);
	WP1ContentListener& operator=(WP1ContentListener&);
};

#endif /* WP1LISTENER_H */
