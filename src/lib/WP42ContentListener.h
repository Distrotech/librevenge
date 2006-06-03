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

#ifndef WP42CONTENTLISTENER_H
#define WP42CONTENTLISTENER_H

#include "WP42Listener.h"
#include "WPXContentListener.h"
#include "WPXHLListenerImpl.h"

typedef struct _WP42ContentParsingState WP42ContentParsingState;
struct _WP42ContentParsingState
{
	_WP42ContentParsingState();
	~_WP42ContentParsingState();
	WPXString m_textBuffer;
};

class WP42ContentListener : public WP42Listener, protected WPXContentListener
{
public:
	WP42ContentListener(std::list<WPXPageSpan> &pageList, WPXHLListenerImpl *listenerImpl);
	~WP42ContentListener();

	void startDocument() { WPXContentListener::startDocument(); };
	void insertCharacter(const uint16_t character);
	void insertTab(const uint8_t tabType, float tabPosition);
	void insertBreak(const uint8_t breakType) { WPXContentListener::insertBreak(breakType); };
	void insertEOL();
	void attributeChange(const bool isOn, const uint8_t attribute);
	void endDocument();

protected:
	void _handleSubDocument(const WPXSubDocument *subDocument, const bool isHeaderFooter, WPXTableList tableList, int nextTableIndice = 0) {}

	void _flushText();
	void _changeList() {};

private:
	WP42ContentParsingState *m_parseState;
};

#endif /* WP42LISTENER_H */
