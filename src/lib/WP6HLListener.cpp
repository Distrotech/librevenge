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

#include <math.h>
#include <ctype.h>
#include "WP6HLListener.h"
#include "WP6FileStructure.h"
#include "WPXFileStructure.h"
#include "libwpd_internal.h"

WP6HLListener::WP6HLListener() :
	WP6LLListener(), 
	m_isUndoOn(false)
{
}

void WP6HLListener::undoChange(const guint8 undoType, const guint16 undoLevel)
{
	if (undoType == WP6_UNDO_GROUP_INVALID_TEXT_START)
		m_isUndoOn = true;
	else if (undoType == WP6_UNDO_GROUP_INVALID_TEXT_END)
		m_isUndoOn = false;		
}
