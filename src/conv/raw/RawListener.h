/* libwpd2
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
	virtual void insertCharacter(guint16 character);
	virtual void insertEOL();
	virtual void insertBreak(guint8 breakType) {}
	virtual void undoChange(guint8 undoType, guint16 undoLevel);
	virtual void setDate(const guint16 year, const guint8 month, const guint8 day, 
			     const guint8 hour, const guint8 minute, const guint8 second,
			     const guint8 dayOfWeek, const guint8 timeZone, const guint8 unused);
	virtual void setExtendedInformation(const guint16 type, const UCSString &data);
	virtual void fontChange(guint16 matchedFontPointSize, guint16 fontPID) {}
	virtual void attributeChange(gboolean isOn, guint8 attribute) {}
	virtual void justificationChange(guint8 justification);
	virtual void marginChange(guint8 side, guint16 margin) {}
	virtual void columnChange(guint8 numColumns) {}
	virtual void updateOutlineDefinition(const WP6OutlineLocation outlineLocation, const guint16 outlineHash, 
					     const guint8 *numberingMethods, const guint8 tabBehaviourFlag);

	virtual void paragraphNumberOn(const guint16 outlineHash, const guint8 level, const guint8 flag);
	virtual void paragraphNumberOff();
	virtual void displayNumberReferenceGroupOn(const guint8 subGroup, const guint8 level);
	virtual void displayNumberReferenceGroupOff(const guint8 subGroup);
	virtual void styleGroupOn(const guint8 subGroup);
	virtual void styleGroupOff(const guint8 subGroup);
	virtual void endDocument() {}
		
	virtual void startTable() {}
 	virtual void insertRow() {}
 	virtual void insertCell(guint8 colSpan, guint8 rowSpan, gboolean boundFromLeft, gboolean boundFromAbove, RGBSColor * cellFgColor, RGBSColor * cellBgColor) {}
 	virtual void endTable() {}

 private:
	gboolean m_isUndoOn;
};

#endif /* RAWLISTENER_H */
