/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#ifndef WP6LISTENER_H
#define WP6LISTENER_H

#include "libwpd_internal.h"
#include <vector>

class WPXString;
class WP6DefaultInitialFontPacket;
class WP6PrefixData;
class WP6PrefixDataPacket;
class WP6SubDocument;

enum WP6OutlineLocation { paragraphGroup, indexHeader };

class WP6Listener
{
public:
	WP6Listener();
	virtual ~WP6Listener() {}

	virtual void startDocument() = 0;
	virtual void startSubDocument() = 0;
	virtual void setDate(const uint16_t type, const uint16_t year,
	                     const uint8_t month, const uint8_t day,
	                     const uint8_t hour, const uint8_t minute,
	                     const uint8_t second, const uint8_t dayOfWeek,
	                     const uint8_t timeZone, const uint8_t unused) = 0;
	virtual void setExtendedInformation(const uint16_t type, const WPXString &data) = 0;
	virtual void setAlignmentCharacter(const uint32_t character) = 0;
	virtual void setLeaderCharacter(const uint32_t character, const uint8_t numSpaces) = 0;
	virtual void defineTabStops(const bool isRelative, const std::vector<WPXTabStop> &tabStops,
	                            const std::vector<bool> &usePreWP9LeaderMethods) = 0;
	virtual void insertCharacter(uint32_t character) = 0;
	virtual void insertTab(const uint8_t tabType, double tabPosition) = 0;
	virtual void handleLineBreak() = 0;
	virtual void insertEOL() = 0;
	virtual void insertBreak(const uint8_t breakType) = 0;
	virtual void lineSpacingChange(const double lineSpacing) = 0;
	virtual void justificationChange(const uint8_t justification) = 0;
	virtual void characterColorChange(const uint8_t red, const uint8_t green, const uint8_t blue) = 0;
	virtual void characterShadingChange(const uint8_t shading) = 0;
	virtual void highlightChange(const bool isOn, const RGBSColor &color) = 0;
	virtual void fontChange(const uint16_t matchedFontPointSize, const uint16_t fontPID, const WPXString &fontName) = 0;
	virtual void attributeChange(const bool isOn, const uint8_t attribute) = 0;
	virtual void spacingAfterParagraphChange(const double spacingRelative, const double spacingAbsolute) = 0;
	virtual void pageMarginChange(const uint8_t side, const uint16_t margin) = 0;
	virtual void pageFormChange(const uint16_t length, const uint16_t width, const WPXFormOrientation orientation) = 0;
	virtual void pageNumberingChange(const WPXPageNumberPosition pageNumberPosition, const uint16_t pageNumberFontPointSize, const uint16_t pageNumberFontPID) = 0;
	virtual void marginChange(const uint8_t side, const uint16_t margin) = 0;
	virtual void paragraphMarginChange(const uint8_t side, const int16_t margin) = 0;
	virtual void indentFirstLineChange(const int16_t offset) = 0;
	virtual void columnChange(const WPXTextColumnType columnType, const uint8_t numColumns,
	                          const std::vector<double> &columnWidth, const std::vector<bool> &isFixedWidth) = 0;
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
	virtual void setPageNumber(const uint16_t pageNumber) = 0;
	virtual void setPageNumberingType(const WPXNumberingType pageNumberingType) = 0;
	virtual void endDocument() = 0;
	virtual void endSubDocument() = 0;
	virtual void defineTable(const uint8_t position, const uint16_t leftOffset) = 0;
	virtual void addTableColumnDefinition(const uint32_t width, const uint32_t leftGutter, const uint32_t rightGutter,
	                                      const uint32_t attributes, const uint8_t alignment) = 0;
	virtual void startTable() = 0;
	virtual void insertRow(const uint16_t rowHeight, const bool isMinimumHeight, const bool isHeaderRow) = 0;
	virtual void insertCell(const uint8_t colSpan, const uint8_t rowSpan, const uint8_t borderBits,
	                        const RGBSColor *cellFgColor, const RGBSColor *cellBgColor,
	                        const RGBSColor *cellBorderColor, const WPXVerticalAlignment cellVerticalAlignment,
	                        const bool useCellAttributes, const uint32_t cellAttributes) = 0;
	virtual void endTable() = 0;

	virtual void undoChange(const uint8_t undoType, const uint16_t undoLevel) = 0;
	virtual void boxOn(const uint8_t anchoringType, const uint8_t generalPositioningFlags, const uint8_t horizontalPositioningFlags,
	                   const int16_t horizontalOffset, const uint8_t leftColumn, const uint8_t rightColumn,
	                   const uint8_t verticalPositioningFlags, const int16_t verticalOffset, const uint8_t widthFlags, const uint16_t width,
	                   const uint8_t heightFlags, const uint16_t height, const uint8_t boxContentType, const uint16_t nativeWidth,
	                   const uint16_t nativeHeight) = 0;
	virtual void boxOff() = 0;
	virtual void insertGraphicsData(const uint16_t packetId) = 0;
	virtual void insertTextBox(const WP6SubDocument *subDocument) = 0;
	virtual void commentAnnotation(const uint16_t textPID) = 0;

	void setPrefixData(WP6PrefixData *prefixData)
	{
		m_prefixData = prefixData;
	}
	const WP6PrefixDataPacket *getPrefixDataPacket(const int prefixID) const;
	WPXString getFontNameForPID(const int prefixID) const;

private:
	WP6Listener(const WP6Listener &);
	WP6Listener &operator=(const WP6Listener &);
	WP6PrefixData *m_prefixData;
};

#endif /* WP6LISTENER_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
