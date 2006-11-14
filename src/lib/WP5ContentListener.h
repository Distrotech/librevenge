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

#ifndef WP5CONTENTLISTENER_H
#define WP5CONTENTLISTENER_H

#include "WP5Listener.h"
#include "WPXContentListener.h"
#include "WP5SubDocument.h"
#include "WPXString.h"
#include "libwpd_internal.h"
#include "WPXHLListenerImpl.h"
#// include "WPXListener.h"
#include <vector>

typedef struct _WP5ContentParsingState WP5ContentParsingState;
struct _WP5ContentParsingState
{
	_WP5ContentParsingState();
	~_WP5ContentParsingState();
	WPXString m_textBuffer;
	WPXString m_noteReference;

	WPXTableList m_tableList;
};

class WP5ContentListener : public WP5Listener, protected WPXContentListener
{
public:
	WP5ContentListener(std::list<WPXPageSpan> &pageList, std::vector<WP5SubDocument *> &subDocuments, WPXHLListenerImpl *listenerImpl);
	~WP5ContentListener();

	void startDocument() { WPXContentListener::startDocument(); };
	void setFont(const WPXString &fontName, const float fontSize);
	void insertCharacter(const uint16_t character);
	void insertTab(const uint8_t tabType, float tabPosition);
	void insertEOL();
	void insertBreak(const uint8_t breakType) { WPXContentListener::insertBreak(breakType); };
	void lineSpacingChange(const float lineSpacing) { WPXContentListener::lineSpacingChange(lineSpacing); };
	void justificationChange(const uint8_t justification) { WPXContentListener::justificationChange(justification); };
	void characterColorChange(const uint8_t red, const uint8_t green, const uint8_t blue);
	void attributeChange(const bool isOn, const uint8_t attribute);
	void pageMarginChange(const uint8_t /* side */, const uint16_t /* margin */) {};
	void pageFormChange(const uint16_t /* length */, const uint16_t /* width */, const WPXFormOrientation /* orientation */) {};
	void marginChange(const uint8_t side, const uint16_t margin);
	void paragraphMarginChange(const uint8_t /* side */, const int16_t /* margin */) {};
	void endDocument() { WPXContentListener::endDocument(); };

	void defineTable(const uint8_t position, const uint16_t leftOffset);
	void addTableColumnDefinition(const uint32_t width, const uint32_t leftGutter, const uint32_t rightGutter,
				const uint32_t attributes, const uint8_t alignment);
	void startTable();
 	void insertRow(const uint16_t rowHeight, const bool isMinimumHeight, const bool isHeaderRow);
 	void insertCell(const uint8_t colSpan, const uint8_t rowSpan, const uint8_t borderBits,
				const RGBSColor * cellFgColor, const RGBSColor * cellBgColor, 
				const RGBSColor * cellBorderColor, const WPXVerticalAlignment cellVerticalAlignment, 
				const bool useCellAttributes, const uint32_t cellAttributes);
 	void endTable();

	void insertNoteReference(const WPXString &noteReference);
	void insertNote(const WPXNoteType noteType, const WP5SubDocument *subDocument);
	void headerFooterGroup(const uint8_t headerFooterType, const uint8_t occurenceBits, WP5SubDocument *subDocument);
	void suppressPageCharacteristics(const uint8_t /* suppressCode */) {};
	
	void setDefaultFont(const WPXString &fontName, const float fontSize);
	
protected:
	void _handleSubDocument(const WPXSubDocument *subDocument, const bool isHeaderFooter, WPXTableList tableList, int nextTableIndice = 0);

	void _flushText();
	void _changeList() {};

private:
	WP5ContentParsingState *m_parseState;
	std::vector<WP5SubDocument *> &m_subDocuments;
	float m_defaultFontSize;
	WPXString m_defaultFontName;
};

#endif /* WP5CONTENTLISTENER_H */
