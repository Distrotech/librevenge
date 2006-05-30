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

#ifndef WP5LISTENER_H
#define WP5LISTENER_H

#include "WP5PrefixData.h"
//#include "WP5SubDocument.h"
#include "WPXPageSpan.h"
#include "WPXHLListenerImpl.h"
#include <list>
#include <vector>

class WP5SubDocument;

class WP5Listener
{
public:
	WP5Listener(std::list<WPXPageSpan> &pageList, WPXHLListenerImpl *listenerImpl);
	virtual ~WP5Listener() {};

	virtual void startDocument() = 0;
	virtual void setFont(const WPXString fontName, const float fontSize) = 0;
	virtual void insertCharacter(const uint16_t character) = 0;
	virtual void insertTab(const uint8_t tabType, float tabPosition) = 0;
	virtual void handleLineBreak() = 0;
	virtual void insertEOL() = 0;
	virtual void insertBreak(const uint8_t breakType) = 0;
	virtual void lineSpacingChange(const float lineSpacing) = 0;
	virtual void justificationChange(const uint8_t justification) = 0;
	virtual void characterColorChange(const uint8_t red, const uint8_t green, const uint8_t blue) = 0;
	virtual void attributeChange(const bool isOn, const uint8_t attribute) = 0;
	virtual void pageMarginChange(const uint8_t side, const uint16_t margin) = 0;
	virtual void pageFormChange(const uint16_t length, const uint16_t width, const WPXFormOrientation orientation, const bool isPersistent) = 0;
	virtual void marginChange(const uint8_t side, const uint16_t margin) = 0;
	virtual void endDocument() = 0;

	virtual void defineTable(const uint8_t position, const uint16_t leftOffset) = 0;
	virtual void addTableColumnDefinition(const uint32_t width, const uint32_t leftGutter, const uint32_t rightGutter,
				const uint32_t attributes, const uint8_t alignment) = 0;
	virtual void startTable() = 0;
 	virtual void insertRow(const uint16_t rowHeight, const bool isMinimumHeight, const bool isHeaderRow) = 0;
 	virtual void insertCell(const uint8_t colSpan, const uint8_t rowSpan, const uint8_t borderBits,
				const RGBSColor * cellFgColor, const RGBSColor * cellBgColor, 
				const RGBSColor * cellBorderColor, const WPXVerticalAlignment cellVerticalAlignment, 
				const bool useCellAttributes, const uint32_t cellAttributes) = 0;
 	virtual void endTable() = 0;

	virtual void insertNoteReference(const WPXString noteReference) = 0;
	virtual void insertNote(const WPXNoteType noteType, const WP5SubDocument *subDocument) = 0;
	virtual void headerFooterGroup(const uint8_t headerFooterType, const uint8_t occurenceBits, WP5SubDocument *subDocument) = 0;
	virtual void suppressPageCharacteristics(const uint8_t suppressCode) = 0;

	void setPrefixData(WP5PrefixData *prefixData) { m_prefixData = prefixData; }
	const WP5GeneralPacketData * getGeneralPacketData(const int type) const;

private:
	WP5PrefixData *m_prefixData;
};

#endif /* WP5LISTENER_H */
