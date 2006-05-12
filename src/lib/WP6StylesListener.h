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

// WP6StylesListener: creates an intermediate table representation, given a
// sequence of messages passed to it by the parser.

#ifndef WP6STYLESLISTENER_H
#define WP6STYLESLISTENER_H

#include "WP6Listener.h"
#include <vector>
#include <set>
#include <list>
#include "WPXPageSpan.h"
#include "WPXTable.h"

class WPXSubDocument;

class WP6StylesListener : public WP6Listener
{
public:
	WP6StylesListener(std::list<WPXPageSpan *> *pageList, WPXTableList tableList);

	virtual void setDate(const uint16_t year, const uint8_t month, const uint8_t day,
			     const uint8_t hour, const uint8_t minute, const uint8_t second,
			     const uint8_t dayOfWeek, const uint8_t timeZone, const uint8_t unused) {}
	virtual void setExtendedInformation(const uint16_t type, const WPXString &data) {}
	virtual void startDocument() {}
	virtual void setAlignmentCharacter(const uint16_t character) {}
	virtual void setLeaderCharacter(const uint16_t character, const uint8_t numberOfSpaces) {}
	virtual void defineTabStops(const bool isRelative, const std::vector<WPXTabStop> &tabStops, 
				    const std::vector<bool> &usePreWP9LeaderMethods) {}
	virtual void insertCharacter(const uint16_t character) { if (!isUndoOn()) m_currentPageHasContent = true; }
	virtual void insertTab(const uint8_t tabType, float tabPosition) { if (!isUndoOn()) m_currentPageHasContent = true; }
	virtual void handleLineBreak()  { if (!isUndoOn()) m_currentPageHasContent = true; }
	virtual void insertEOL() { if (!isUndoOn()) m_currentPageHasContent = true; }
 	virtual void insertBreak(const uint8_t breakType);
	virtual void characterColorChange(const uint8_t red, const uint8_t green, const uint8_t blue) {}
	virtual void characterShadingChange(const uint8_t shading) {}
	virtual void highlightChange(const bool isOn, const RGBSColor color) {}
	virtual void fontChange(const uint16_t matchedFontPointSize, const uint16_t fontPID) {}
	virtual void attributeChange(const bool isOn, const uint8_t attribute) {}
	virtual void lineSpacingChange(const float lineSpacing) {}
	virtual void spacingAfterParagraphChange(const float spacingRelative, const float spacingAbsolute) {}
	virtual void justificationChange(const uint8_t justification) {}
	virtual void pageMarginChange(const uint8_t side, const uint16_t margin);
	virtual void pageFormChange(const uint16_t length, const uint16_t width, const WPXFormOrientation orientation, const bool isPersistent);
	virtual void marginChange(const uint8_t side, const uint16_t margin);
	virtual void paragraphMarginChange(const uint8_t side, const int16_t margin) {}
	virtual void indentFirstLineChange(const int16_t offset) {}
	virtual void columnChange(const WPXTextColumnType columnType, const uint8_t numColumns, const std::vector<float> &columnWidth,
				  const std::vector<bool> &isFixedWidth) {}
	virtual void updateOutlineDefinition(const WP6OutlineLocation outlineLocation, const uint16_t outlineHash,
					     const uint8_t *numberingMethods, const uint8_t tabBehaviourFlag) {}

	virtual void paragraphNumberOn(const uint16_t outlineHash, const uint8_t level, const uint8_t flag) {}
	virtual void paragraphNumberOff() { if (!isUndoOn()) m_currentPageHasContent = true; }
	virtual void displayNumberReferenceGroupOn(const uint8_t subGroup, const uint8_t level) {}
	virtual void displayNumberReferenceGroupOff(const uint8_t subGroup) { if (!isUndoOn()) m_currentPageHasContent = true; }
	void styleGroupOn(const uint8_t subGroup) {}
	virtual void styleGroupOff(const uint8_t subGroup) {}
	virtual void globalOn(const uint8_t systemStyle) {}
	virtual void globalOff() {}
	virtual void noteOn(const uint16_t textPID);
	virtual void noteOff(const WPXNoteType noteType) {}
	virtual void headerFooterGroup(const uint8_t headerFooterType, const uint8_t occurenceBits, const uint16_t textPID);
	virtual void suppressPageCharacteristics(const uint8_t suppressCode);
	virtual void endDocument();

 	virtual void defineTable(const uint8_t position, const uint16_t leftOffset);
	virtual void addTableColumnDefinition(const uint32_t width, const uint32_t leftGutter, const uint32_t rightGutter,
				const uint32_t attributes, const uint8_t alignment) {}
	virtual void startTable();
 	virtual void insertRow(const uint16_t rowHeight, const bool isMinimumHeight, const bool isHeaderRow);
 	virtual void insertCell(const uint8_t colSpan, const uint8_t rowSpan, const uint8_t borderBits, 
				const RGBSColor * cellFgColor, const RGBSColor * cellBgColor,
				const RGBSColor * cellBorderColor, const WPXVerticalAlignment cellVerticalAlignment, 
				const bool useCellAttributes, const uint32_t cellAttributes);
 	virtual void endTable();

protected:
	virtual void _handleSubDocument(const WPXSubDocument *subDocument, const bool isHeaderFooter, WPXTableList tableList, int nextTableIndice = 0);

	virtual void _flushText() {}
	virtual void _openParagraph() {}
	virtual void _openPageSpan() { /* FIXME: REMOVE ME WHEN IMPLEMENTED IN WPXListener */ };
	virtual void _changeList() {}

private:
	WPXPageSpan *m_currentPage;

	WPXTableList m_tableList;
	WPXTable *m_currentTable;
	float m_tempMarginLeft, m_tempMarginRight;
	bool m_currentPageHasContent;
	bool m_isTableDefined;
	std::set <const WPXSubDocument *> m_subDocuments;
	std::list<WPXPageSpan *>::iterator m_pageListHardPageMark;
};

#endif /* WP6STYLESLISTENER_H */
