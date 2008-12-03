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

#ifndef WP42CONTENTLISTENER_H
#define WP42CONTENTLISTENER_H

#include "WP42Listener.h"
#include "WP42SubDocument.h"
#include "WPXContentListener.h"
#include "WPXDocumentInterface.h"

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
	WP42ContentListener(std::list<WPXPageSpan> &pageList, std::vector<WP42SubDocument *> &subDocuments, WPXDocumentInterface *documentInterface);
	~WP42ContentListener();

	void startDocument() { WPXContentListener::startDocument(); }
	void startSubDocument() { WPXContentListener::startSubDocument(); }
	void insertCharacter(uint16_t character);
	void insertTab(uint8_t tabType, double tabPosition);
	void insertBreak(uint8_t breakType) { WPXContentListener::insertBreak(breakType); }
	void insertEOL();
	void attributeChange(bool isOn, uint8_t attribute);
	void marginReset(uint8_t leftMargin, uint8_t rightMargin);
	void headerFooterGroup(uint8_t headerFooterDefinition, WP42SubDocument *subDocument);
	void suppressPageCharacteristics(uint8_t /* suppressCode */) {}
	void endDocument() { WPXContentListener::endDocument(); };
	void endSubDocument() { WPXContentListener::endSubDocument(); };

protected:
	void _handleSubDocument(const WPXSubDocument *subDocument, WPXSubDocumentType subDocumentType, WPXTableList tableList, int nextTableIndice = 0);

	void _flushText();
	void _changeList() {};

private:
	WP42ContentListener(const WP42ContentListener&);
	WP42ContentListener& operator=(const WP42ContentListener&);
	WP42ContentParsingState *m_parseState;
	std::vector<WP42SubDocument *> &m_subDocuments;
};

#endif /* WP42LISTENER_H */
