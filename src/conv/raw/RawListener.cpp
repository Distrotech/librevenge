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

#include <stdio.h>
#include "RawListener.h"
#include "WP6FileStructure.h" 

RawListener::RawListener() : 
	WP6LLListener(),
	m_isUndoOn(false)
{
}

void RawListener::insertCharacter(const guint16 character)
{
	if (!m_isUndoOn) {
		// first convert from ucs2 to ucs4
		gunichar characterUCS4 = (gunichar) character;
		
		// then convert from ucs4 to utf8 and write it
		gchar *characterUTF8 = g_ucs4_to_utf8(&characterUCS4, 1, NULL, NULL, NULL); // TODO: handle errors
		printf("%s", characterUTF8);
		
		// clean up our mess
		g_free(characterUTF8);
	}
}

void RawListener::undoChange(const guint8 undoType, const guint16 undoLevel)
{
	if (undoType == WP6_UNDO_GROUP_INVALID_TEXT_START) {
		//printf("<UNDO ON>");
		m_isUndoOn = true;
	}
	else if (undoType == WP6_UNDO_GROUP_INVALID_TEXT_END) {
		//printf("<UNDO OFF>");
		m_isUndoOn = false;
	}
}

void RawListener::lineSpacingChange(const float lineSpacing)
{
	printf("<line-spacing:%i>", lineSpacing);

}

void RawListener::justificationChange(const guint8 justification)
{
	printf("<justification:%i>", justification);

}

void RawListener::insertTab(const guint8 tabType)
{
	if (!m_isUndoOn) {
		printf("<TAB: %i>", tabType);
	}
}

void RawListener::insertEOL()
{
	if (!m_isUndoOn) {
		printf("<EOL>\n");
	}
}

void RawListener::insertBreak(const guint8 breakType)
{
	switch(breakType)
	{
	case WPX_PAGE_BREAK:
		printf("\n<HARD PAGE BREAK>\n");
		break;
	case WPX_SOFT_PAGE_BREAK:
		printf("\n<SOFT PAGE BREAK>\n");
		break;
	case WPX_COLUMN_BREAK:
		printf("\n<COLUMN BREAK>\n");
		break;
	}
}

void RawListener::setDate(const guint16 year, const guint8 month, const guint8 day, 
			  const guint8 hour, const guint8 minute, const guint8 second,
			  const guint8 dayOfWeek, const guint8 timeZone, const guint8 unused)
{
	printf("<DATE SET year: %i month: %i day: %i hour: %i minute: %i second: %i dayOfWeek: %i timeZone: %i unused: %i>\n");
}
void RawListener::setExtendedInformation(const guint16 type, const UCSString &data)
{
	UTF8String tempUTF8(data);
	printf("<EXTENDED INFO type: %i data: %s>\n", type, tempUTF8.getUTF8());
	
}
void RawListener::updateOutlineDefinition(const WP6OutlineLocation outlineLocation, const guint16 outlineHash, 
					  const guint8 *numberingMethods, const guint8 tabBehaviourFlag)
{
	if (!m_isUndoOn) {
		printf("<UPDATE OUTLINE DEFINITION (outlineHash:%i, numbering-methods:.., tab-behaviour-flag:%i)>\n", 
		       outlineHash, tabBehaviourFlag);
	}	
}

void RawListener::paragraphNumberOn(const guint16 outlineHash, const guint8 level, const guint8 flag)
{
	if (!m_isUndoOn) {
		printf("<PARAGRAPH NUMBER ON (outlineHash:%i, level:%i, flag:%i)>", 
		       outlineHash, level, flag);
	}	
}

void RawListener::paragraphNumberOff()
{
	if (!m_isUndoOn) {
		printf("<PARAGRAPH NUMBER OFF>");
	}	
}

void RawListener::displayNumberReferenceGroupOn(const guint8 subGroup, const guint8 level)
{
	if (!m_isUndoOn) {
		printf("<DISPLAY NUMBER REF ON (subgroup:%i, level:%i)>", subGroup, level);
	}	
}

void RawListener::displayNumberReferenceGroupOff(const guint8 subGroup)
{
	if (!m_isUndoOn) {
		printf("<DISPLAY NUMBER REF OFF (subgroup:%i)>", subGroup);
	}	
}

void RawListener::styleGroupOn(const guint8 subGroup)
{
	if (!m_isUndoOn) {
		printf("<STYLE GROUP ON (subgroup:%i)>", subGroup);
	}	
}

void RawListener::styleGroupOff(const guint8 subGroup)
{
	if (!m_isUndoOn) {
		printf("<STYLE GROUP OFF (subgroup:%i)>", subGroup);
	}	
}

void RawListener::suppressPageCharacteristics(const guint8 suppressCode)
{
	if (!m_isUndoOn) {
		printf("<SUPPRESS PAGE CHARACTERISTICS (suppressCode: %i)>", suppressCode);
	}	
}

void RawListener::insertCell(const guint8 colSpan, const guint8 rowSpan, const bool boundFromLeft, const bool boundFromAbove, 
			     const guint8 borderBits, const RGBSColor * cellFgColor, const RGBSColor * cellBgColor)
{
	if (!m_isUndoOn) {
		printf("<INSERT CELL (colSpan: %i, rowSpan: %i, borderBits: %x)>", colSpan, rowSpan, borderBits);
	}		
}
