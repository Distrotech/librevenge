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

#include <stdio.h>
#include "TextListener.h"
#include "WP6FileStructure.h" 

// use the BELL code to represent a TAB for now
#define UCS_TAB 0x0009 

TextListener::TextListener() : 
	WP6LLListener(),
	m_isUndoOn(FALSE)
{
}

void TextListener::insertCharacter(const guint16 character)
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

void TextListener::undoChange(const guint8 undoType, const guint16 undoLevel)
{
	if (undoType == WP6_UNDO_GROUP_INVALID_TEXT_START)
		m_isUndoOn = TRUE;
	else if (undoType == WP6_UNDO_GROUP_INVALID_TEXT_END)
		m_isUndoOn = FALSE;
}

void TextListener::insertTab()
{
	if (!m_isUndoOn) {
		printf("%c", UCS_TAB);
	}
}

void TextListener::insertEOL()
{
	if (!m_isUndoOn) {
		printf("\n");
	}
}
