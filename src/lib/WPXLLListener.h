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

#ifndef WPXLLLISTENER_H
#define WPXLLLISTENER_H
#include "libwpd_support.h"
#include <vector>

using namespace std;

class WPXLLListener
{
 public:
	WPXLLListener() {}
	virtual ~WPXLLListener() {}
	virtual void startDocument() = 0;
	virtual void setAlignmentCharacter(const uint16_t character) = 0;
	virtual void setLeaderCharacter(const uint16_t character, const uint8_t numberOfSpaces) = 0;
	virtual void defineTabStops(const bool isRelative, const vector<WPXTabStop> &tabStops, 
				    const vector<bool> &usePreWP9LeaderMethods) = 0;
	virtual void insertCharacter(const uint16_t character) = 0;
	virtual void insertTab(const uint8_t tabType, const float tabPosition) = 0;
	virtual void handleLineBreak() = 0;
	virtual void insertEOL() = 0;
 	virtual void insertBreak(const uint8_t breakType) = 0;
	virtual void attributeChange(const bool isOn, const uint8_t attribute) = 0;
	virtual void lineSpacingChange(const float lineSpacing) = 0;
	virtual void spacingAfterParagraphChange(const float spacingRelative, const float spacingAbsolute) = 0;
	virtual void justificationChange(const uint8_t justification) = 0;
	virtual void pageMarginChange(const uint8_t side, const uint16_t margin) = 0;
	virtual void pageFormChange(const uint16_t length, const uint16_t width, const WPXFormOrientation orientation,
					const bool isPersistent) = 0;
	virtual void marginChange(const uint8_t side, const uint16_t margin) = 0;
	virtual void paragraphMarginChange(const uint8_t side, const int16_t margin) = 0;
	virtual void indentFirstLineChange(const int16_t offset) = 0;
	virtual void columnChange(const uint8_t numColumns) = 0;
	virtual void endDocument() = 0;

	virtual void defineTable(uint8_t position, uint16_t leftOffset) = 0;
	virtual void addTableColumnDefinition(uint32_t width, uint32_t leftGutter, uint32_t rightGutter) = 0;
	virtual void startTable() = 0;
 	virtual void insertRow(const bool isHeaderRow, const bool isFixedHeightRow, const bool hasMinimumHeight, const uint16_t rowHeight) = 0;
 	virtual void insertCell(const uint8_t colSpan, const uint8_t rowSpan, const bool boundFromLeft, const bool boundFromAbove,
				const uint8_t borderBits,
				const RGBSColor * cellFgColor, const RGBSColor * cellBgColor, const uint32_t cellAttributes) = 0;
 	virtual void endTable() = 0;
};

#endif /* WPXLLLISTENER_H */
