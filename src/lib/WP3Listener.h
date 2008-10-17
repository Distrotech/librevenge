/* libwpd
 * Copyright (C) 2004 Marc Maurer (uwog@uwog.net)
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

#ifndef WP3LISTENER_H
#define WP3LISTENER_H

#include "libwpd_internal.h"
#include <vector>

class WP3SubDocument;
class WP3ResourceFork;

class WP3Listener
{
public:
	WP3Listener();
	virtual ~WP3Listener() {}

	virtual void startDocument() = 0;
	virtual void startSubDocument() = 0;
	virtual void insertCharacter(const uint16_t character) = 0;
	virtual void insertTab() = 0;
	virtual void insertTab(const uint8_t tabType, const float tabPosition) = 0;
	virtual void insertBreak(const uint8_t breakType) = 0;
	virtual void insertEOL() = 0;
	virtual void lineSpacingChange(const float lineSpacing) = 0;
	virtual void attributeChange(const bool isOn, const uint8_t attribute) = 0;
	virtual void pageMarginChange(const uint8_t side, const uint16_t margin) = 0;
	virtual void pageFormChange(const uint16_t length, const uint16_t width, const WPXFormOrientation orientation) = 0;
	virtual void marginChange(const uint8_t side, const uint16_t margin) = 0;
	virtual void indentFirstLineChange(const int16_t offset) = 0;
	virtual void setTabs(const bool isRelative, const std::vector<WPXTabStop> tabStops) = 0;
	virtual void columnChange(const WPXTextColumnType columnType, const uint8_t numColumns, const std::vector<float> &columnWidth,
					const std::vector<bool> &isFixedWidth) = 0;
	virtual void endDocument() = 0;
	virtual void endSubDocument() = 0;

	virtual void defineTable(const uint8_t position, const uint16_t leftOffset) = 0;
	virtual void addTableColumnDefinition(const uint32_t width, const uint32_t leftGutter, const uint32_t rightGutter,
					const uint32_t attributes, const uint8_t alignment) = 0;
	virtual void startTable() = 0;
 	virtual void closeCell() = 0;
	virtual void closeRow() = 0;
	virtual void setTableCellSpan(const uint16_t colSpan, const uint16_t rowSpan) = 0;
	virtual void setTableCellFillColor(const RGBSColor * cellFillColor) = 0;
 	virtual void endTable() = 0;
	virtual void undoChange(const uint8_t undoType, const uint16_t undoLevel) = 0;
	virtual void justificationChange(const uint8_t justification) = 0;
	virtual void setTextColor(const RGBSColor * fontColor) = 0;
	virtual void setTextFont(const WPXString &fontName) = 0;
	virtual void setFontSize(const uint16_t fontSize) = 0;
	virtual void insertPageNumber(const WPXString &pageNumber) = 0;
	virtual void insertNoteReference(const WPXString &noteReference) = 0;
	virtual void insertNote(const WPXNoteType noteType, WP3SubDocument *subDocument) = 0;
	virtual void headerFooterGroup(const uint8_t headerFooterType, const uint8_t occurenceBits, WP3SubDocument *subDocument) = 0;
	virtual void suppressPage(const uint16_t suppressCode) = 0;
	virtual void backTab() = 0;
	virtual void leftIndent() = 0;
	virtual void leftIndent(const float offset) = 0;
	virtual void leftRightIndent() = 0;
	virtual void leftRightIndent(const float offset) = 0;

	void setResourceFork(WP3ResourceFork *resourceFork) { m_resourceFork = resourceFork; }
	const WP3ResourceFork *getResourceFork() const { return m_resourceFork; }

private:
	WP3ResourceFork *m_resourceFork;
};

#endif /* WP3LISTENER_H */
