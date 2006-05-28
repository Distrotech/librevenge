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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 * 
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by 
 * Corel Corporation or Corel Corporation Limited."
 */
 
#ifndef WP6LISTENER_H
#define WP6LISTENER_H
 
#include "WPXListener.h"
#include "WP6FileStructure.h"

#include <vector>
#include "WP6PrefixDataPacket.h"

class WPXString;
class WP6DefaultInitialFontPacket;
class WP6PrefixData;

enum WP6OutlineLocation { paragraphGroup, indexHeader };

class WP6Listener : public WPXListener
{
public:
	WP6Listener(std::list<WPXPageSpan> &pageList, WPXHLListenerImpl *listenerImpl);
	virtual ~WP6Listener() {};
	virtual void setDate(const uint16_t year, const uint8_t month, const uint8_t day,
			     const uint8_t hour, const uint8_t minute, const uint8_t second,
			     const uint8_t dayOfWeek, const uint8_t timeZone, const uint8_t unused) = 0;
	virtual void setExtendedInformation(const uint16_t type, const WPXString &data) = 0;
	virtual void setAlignmentCharacter(const uint16_t character) = 0;
	virtual void setLeaderCharacter(const uint16_t character, const uint8_t numSpaces) = 0;
	virtual void defineTabStops(const bool isRelative, const std::vector<WPXTabStop> &tabStops, 
				    const std::vector<bool> &usePreWP9LeaderMethods) = 0;
	virtual void insertCharacter(const uint16_t character) = 0;
	virtual void insertTab(const uint8_t tabType, float tabPosition) = 0;
	virtual void handleLineBreak() = 0;
	virtual void insertEOL() = 0;
	virtual void characterColorChange(const uint8_t red, const uint8_t green, const uint8_t blue) = 0;
	virtual void characterShadingChange(const uint8_t shading) = 0;
	virtual void highlightChange(const bool isOn, const RGBSColor color) = 0;
	virtual void fontChange(const uint16_t matchedFontPointSize, const uint16_t fontPID) = 0;
	virtual void attributeChange(const bool isOn, const uint8_t attribute) = 0;
	virtual void spacingAfterParagraphChange(const float spacingRelative, const float spacingAbsolute) = 0;
	virtual void pageMarginChange(const uint8_t side, const uint16_t margin) = 0;
	virtual void pageFormChange(const uint16_t length, const uint16_t width, const WPXFormOrientation orientation,
				const bool isPersistent) = 0;
	virtual void marginChange(const uint8_t side, const uint16_t margin) = 0;
	virtual void paragraphMarginChange(const uint8_t side, const int16_t margin) = 0;
	virtual void indentFirstLineChange(const int16_t offset) = 0;
	virtual void columnChange(const WPXTextColumnType columnType, const uint8_t numColumns,
				const std::vector<float> &columnWidth, const std::vector<bool> &isFixedWidth) = 0;
	virtual void updateOutlineDefinition(const WP6OutlineLocation outlineLocation, const uint16_t outlineHash,
					     const uint8_t *numberingMethods, const uint8_t tabBehaviourFlag) = 0;
	virtual void paragraphNumberOn(const uint16_t outlineHash, const uint8_t level, const uint8_t flag) = 0;
	virtual void paragraphNumberOff() = 0;
	virtual void displayNumberReferenceGroupOn(const uint8_t subGroup, const uint8_t level) = 0;
	virtual void displayNumberReferenceGroupOff(const uint8_t subGroup) = 0;
	virtual void styleGroupOn(const uint8_t subGroup) = 0;
	virtual void styleGroupOff(const uint8_t subGroup) = 0;
	virtual void globalOn(const uint8_t systemStyle) = 0;
	virtual void globalOff() = 0;
	virtual void noteOn(const uint16_t textPID) = 0;
	virtual void noteOff(const WPXNoteType noteType) = 0;
	virtual void headerFooterGroup(const uint8_t headerFooterType, const uint8_t occurenceBits, const uint16_t textPID) = 0;
	virtual void suppressPageCharacteristics(const uint8_t suppressCode) = 0;
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

	void setPrefixData(WP6PrefixData *prefixData) { m_prefixData = prefixData; }
	const WP6PrefixDataPacket * getPrefixDataPacket(const int prefixID) const;
		
	// for getting low-level messages from the parser
	virtual void undoChange(const uint8_t undoType, const uint16_t undoLevel);

private:
	WP6PrefixData *m_prefixData;
};

#endif /* WP6LISTENER_H */
