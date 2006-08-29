/* libwpd
 * Copyright (C) 2003 William Lachance (william.lachance@sympatico.ca)
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
#include "WP1SubDocument.h"
#include "WPXContentListener.h"
#include "WPXHLListenerImpl.h"

typedef struct _WP1ContentParsingState WP1ContentParsingState;
struct _WP1ContentParsingState
{
	_WP1ContentParsingState();
	~_WP1ContentParsingState();
	WPXString m_textBuffer;
	int m_numDeferredTabs;
};

class WP1ContentListener : public WP1Listener, protected WPXContentListener
{
public:
	WP1ContentListener(std::list<WPXPageSpan> &pageList, std::vector<WP1SubDocument *> &subDocuments, WPXHLListenerImpl *listenerImpl);
	~WP1ContentListener();

	void startDocument() { WPXContentListener::startDocument(); }
	void insertCharacter(const uint16_t character);
	void insertExtendedCharacter(const uint8_t extendedCharacter);
	void insertTab();
	void insertBreak(const uint8_t breakType) { WPXContentListener::insertBreak(breakType); }
	void insertEOL();
	void attributeChange(const bool isOn, const uint8_t attribute);
	void fontPointSize(const uint8_t pointSize);
	void marginReset(const uint16_t leftMargin, const uint16_t rightMargin);
	void topMarginSet(const uint16_t topMargin) {}
	void bottomMarginSet(const uint16_t bottomMargin) {}
	void leftIndent(const uint16_t leftMarginOffset);
	void leftRightIndent(const uint16_t leftRightMarginOffset);
	void leftMarginRelease(const uint16_t release);
	void setTabs(const std::vector<WPXTabStop> tabStops);
	void headerFooterGroup(const uint8_t headerFooterDefinition, WP1SubDocument *subDocument);
	void suppressPageCharacteristics(const uint8_t suppressCode) {}
	void justificationChange(const uint8_t justification);
	void lineSpacingChange(const uint8_t spacing) { WPXContentListener::lineSpacingChange((float)((double)spacing/2.0f)); }
	void endDocument() { WPXContentListener::endDocument(); };

protected:
	void _handleSubDocument(const WPXSubDocument *subDocument, const bool isHeaderFooter, WPXTableList tableList, int nextTableIndice = 0);

	void _flushText();
	void _changeList() {};

private:
	WP1ContentParsingState *m_parseState;
	std::vector<WP1SubDocument *> &m_subDocuments;
};

#endif /* WP1LISTENER_H */
