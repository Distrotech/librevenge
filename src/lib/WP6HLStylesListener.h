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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

// WP6HLStylesListener: creates an intermediate table representation, given a
// sequence of messages passed to it by the parser.

#ifndef WP6HLSTYLESLISTENER_H
#define WP6HLSTYLESLISTENER_H

#include "WP6HLListener.h"
#include <vector>
#include "WPXPageSpan.h"
#include "WPXTable.h"

class WP6HLStylesListener : public WP6HLListener
{
public:
	WP6HLStylesListener(vector<WPXPageSpan *> *pageList, vector<WPXTable *> *tableList);

	virtual void setDate(const guint16 year, const guint8 month, const guint8 day,
			     const guint8 hour, const guint8 minute, const guint8 second,
			     const guint8 dayOfWeek, const guint8 timeZone, const guint8 unused) {}
	virtual void setExtendedInformation(const guint16 type, const UCSString &data) {}
	virtual void startDocument() {}
	virtual void setAlignmentCharacter(const guint16 character) {}
	virtual void setDotLeaderCharacters(const guint16 character, const guint8 numberOfSpaces) {}
	virtual void defineTabStops(const int numberOfTabStops, const WPXTabStop *tabStops, const bool *usePreWP9LeaderMethods) {}
	virtual void insertCharacter(const guint16 character) { if (!isUndoOn()) m_currentPageHasContent = true; }
	virtual void insertTab(const guint8 tabType, const float tabPosition) { if (!isUndoOn()) m_currentPageHasContent = true; }
	virtual void handleLineBreak()  { if (!isUndoOn()) m_currentPageHasContent = true; }
	virtual void insertEOL() { if (!isUndoOn()) m_currentPageHasContent = true; }
 	virtual void insertBreak(const guint8 breakType);
	virtual void characterColorChange(const guint8 red, const guint8 green, const guint8 blue) {}
	virtual void characterShadingChange(const guint8 shading) {}
	virtual void highlightChange(const bool isOn, const RGBSColor color) {}
	virtual void fontChange(const guint16 matchedFontPointSize, const guint16 fontPID) {}
	virtual void attributeChange(const bool isOn, const guint8 attribute) {}
	virtual void lineSpacingChange(const float lineSpacing) {}
	virtual void spacingAfterParagraphChange(const float spacingRelative, const float spacingAbsolute) {}
	virtual void justificationChange(const guint8 justification) {}
	virtual void pageMarginChange(const guint8 side, const guint16 margin);
	virtual void pageFormChange(const guint16 length, const guint16 width, const WPXFormOrientation orientation);
	virtual void marginChange(const guint8 side, const guint16 margin);
	virtual void paragraphMarginChange(const guint8 side, const gint16 margin) {}
	virtual void indentFirstLineChange(const gint16 offset) {}
	virtual void columnChange(const guint8 numColumns) {}
	virtual void updateOutlineDefinition(const WP6OutlineLocation outlineLocation, const guint16 outlineHash,
					     const guint8 *numberingMethods, const guint8 tabBehaviourFlag) {}

	virtual void paragraphNumberOn(const guint16 outlineHash, const guint8 level, const guint8 flag) {}
	virtual void paragraphNumberOff() { if (!isUndoOn()) m_currentPageHasContent = true; }
	virtual void displayNumberReferenceGroupOn(const guint8 subGroup, const guint8 level) {}
	virtual void displayNumberReferenceGroupOff(const guint8 subGroup) { if (!isUndoOn()) m_currentPageHasContent = true; }
	void styleGroupOn(const guint8 subGroup) {}
	virtual void styleGroupOff(const guint8 subGroup) {}
	virtual void globalOn(const guint8 systemStyle) {}
	virtual void globalOff() {}
	virtual void noteOn(const guint16 textPID);
	virtual void noteOff(const WPXNoteType noteType) {}
	virtual void headerFooterGroup(const guint8 headerFooterType, const guint8 occurenceBits, const guint16 textPID);
	virtual void suppressPageCharacteristics(const guint8 suppressCode);
	virtual void endDocument();

 	virtual void defineTable(guint8 position, guint16 leftOffset);
	virtual void addTableColumnDefinition(guint32 width, guint32 leftGutter, guint32 rightGutter) {}
	virtual void startTable();
 	virtual void insertRow();
 	virtual void insertCell(const guint8 colSpan, const guint8 rowSpan, const bool boundFromLeft, const bool boundFromAbove,
				const guint8 borderBits,
				const RGBSColor * cellFgColor, const RGBSColor * cellBgColor);
 	virtual void endTable();

protected:
	virtual void _handleSubDocument(guint16 textPID, const bool isHeaderFooter);
	virtual void _flushText(const bool fakeText=false) {}
	virtual void _openPageSpan() { /* FIXME: REMOVE ME WHEN IMPLEMENTED IN WPXHLListener */ };

private:
	WPXPageSpan *m_currentPage;

	vector<WPXTable *> *m_tableList;
	WPXTable *m_currentTable;
	float m_tempMarginLeft, m_tempMarginRight;
	bool m_currentPageHasContent;
	bool m_isTableDefined;
};

#endif /* WP6HLSTYLESLISTENER_H */
