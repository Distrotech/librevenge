/* libwpd
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002 Marc Maurer (j.m.maurer@student.utwente.nl)
 *  
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
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

#ifndef RAWLISTENER_H
#define RAWLISTENER_H

#include <glib.h>
#include "WP6LLListener.h"

class RawListener : public WP6LLListener
{
public:
	RawListener();
	~RawListener() {}
	virtual void startDocument() {}
	virtual void insertCharacter(const guint16 character);
	virtual void insertTab(const guint8 tabType);
	virtual void insertEOL();
	virtual void insertBreak(const guint8 breakType);
	virtual void undoChange(const guint8 undoType, const guint16 undoLevel);
	virtual void setDate(const guint16 year, const guint8 month, const guint8 day, 
			     const guint8 hour, const guint8 minute, const guint8 second,
			     const guint8 dayOfWeek, const guint8 timeZone, const guint8 unused);
	virtual void setExtendedInformation(const guint16 type, const UCSString &data);
	virtual void fontChange(const guint16 matchedFontPointSize, const guint16 fontPID) {}
	virtual void attributeChange(const bool isOn, const guint8 attribute) {}
	virtual void lineSpacingChange(const float lineSpacing);
	virtual void justificationChange(const guint8 justification);
	virtual void pageMarginChange(const guint8 side, const guint16 margin) {}
	virtual void marginChange(const guint8 side, const guint16 margin) {}
	virtual void columnChange(const guint8 numColumns) {}
	virtual void updateOutlineDefinition(const WP6OutlineLocation outlineLocation, const guint16 outlineHash, 
					     const guint8 *numberingMethods, const guint8 tabBehaviourFlag);

	virtual void paragraphNumberOn(const guint16 outlineHash, const guint8 level, const guint8 flag);
	virtual void paragraphNumberOff();
	virtual void displayNumberReferenceGroupOn(const guint8 subGroup, const guint8 level);
	virtual void displayNumberReferenceGroupOff(const guint8 subGroup);
	virtual void styleGroupOn(const guint8 subGroup);
	virtual void styleGroupOff(const guint8 subGroup);
	virtual void globalOn(const guint8 systemStyle) {}
	virtual void globalOff() {}
	virtual void noteOn(const guint16 textPID) {}
	virtual void noteOff(const WPXNoteType noteType) {}
	virtual void headerFooterGroup(const guint8 headerFooterType, const guint8 occurenceBits, const guint16 textPID) {}
	virtual void suppressPageCharacteristics(const guint8 suppressCode);
	virtual void endDocument() {}
		
	virtual void defineTable(guint8 position, guint16 leftOffset) {}
	virtual void addTableColumnDefinition(guint32 width, guint32 leftGutter, guint32 rightGutter) {}
	virtual void startTable() {}
 	virtual void insertRow() {}
 	virtual void insertCell(const guint8 colSpan, const guint8 rowSpan, const bool boundFromLeft, const bool boundFromAbove, 
				const guint8 borderBits, 
				const RGBSColor * cellFgColor, const RGBSColor * cellBgColor);
 	virtual void endTable() {}

 private:
	bool m_isUndoOn;
};

#endif /* RAWLISTENER_H */
