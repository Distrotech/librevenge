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
#include "WP5SubDocument.h"
#include "WPXString.h"
#include "libwpd_internal.h"
#include "WPXHLListenerImpl.h"
#include "WPXListener.h"
#include <vector>

typedef struct _WP5ParsingState WP5ParsingState;
struct _WP5ParsingState
{
	_WP5ParsingState();
	~_WP5ParsingState();
	WPXString m_textBuffer;
	WPXString m_noteReference;

	WPXTableList m_tableList;
};

class WP5ContentListener : public WP5Listener
{
public:
	WP5ContentListener(std::vector<WPXPageSpan *> *pageList, std::vector<WP5SubDocument *> &subDocuments, WPXHLListenerImpl *listenerImpl);
	~WP5ContentListener();

	void setAlignmentCharacter(const uint16_t character) {};
	void setLeaderCharacter(const uint16_t character, const uint8_t numberOfSpaces) {};
	void defineTabStops(const bool isRelative, const std::vector<WPXTabStop> &tabStops, 
				const std::vector<bool> &usePreWP9LeaderMethods) {};
	void setFont(const WPXString fontName, const float fontSize);
	void insertCharacter(const uint16_t character);
	void insertTab(const uint8_t tabType, const float tabPosition);
	void handleLineBreak() {};
	void insertEOL();
	void attributeChange(const bool isOn, const uint8_t attribute);
	void spacingAfterParagraphChange(const float spacingRelative, const float spacingAbsolute) {};
	void pageMarginChange(const uint8_t side, const uint16_t margin) {};
	void pageFormChange(const uint16_t length, const uint16_t width, const WPXFormOrientation orientation, const bool isPersistent) {};
	void marginChange(const uint8_t side, const uint16_t margin);
	void paragraphMarginChange(const uint8_t side, const int16_t margin) {};
	void indentFirstLineChange(const int16_t offset) {};
	void columnChange(const WPXTextColumnType columnType, const uint8_t numColumns, const std::vector<float> &columnWidth,
				const std::vector<bool> &isFixedWidth) {};
	void endDocument();

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

	void insertNoteReference(const WPXString noteReference);
	void insertNote(const WPXNoteType noteType, const WP5SubDocument *subDocument);
	void headerFooterGroup(const uint8_t headerFooterType, const uint8_t occurenceBits, WP5SubDocument *subDocument);
	void suppressPageCharacteristics(const uint8_t suppressCode) {};
	
protected:
	void _handleSubDocument(const WPXSubDocument *subDocument, const bool isHeaderFooter, WPXTableList tableList, int nextTableIndice = 0);

private:
	void _flushText();
	void _changeList() {};

	WP5ParsingState *m_parseState;
	std::vector<WP5SubDocument *> &m_subDocuments;
};

#endif /* WP5CONTENTLISTENER_H */
