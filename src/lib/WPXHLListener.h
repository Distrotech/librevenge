/* libwpd2
 * Copyright (C) 2002 William Lachance (wlach@interlog.com)
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
 
#ifndef WPXHLLISTENER_H
#define WPXHLLISTENER_H
 
#include "WPXLLListener.h"

// ATTRIBUTE bits
#define WPX_EXTRALARGE_BIT 1
#define WPX_VERYLARGE_BIT 2
#define WPX_LARGE_BIT 4
#define WPX_SMALLPRINT_BIT 8
#define WPX_FINEPRINT_BIT 16
#define WPX_SUPERSCRIPT_BIT 32
#define WPX_SUBSCRIPT_BIT 64
#define WPX_OUTLINE_BIT 128
#define WPX_ITALICS_BIT 256
#define WPX_SHADOW_BIT 512
#define WPX_REDLINE_BIT 1024
#define WPX_DOUBLEUNDERLINE_BIT 2048
#define WPX_BOLD_BIT 4096
#define WPX_STRIKEOUT_BIT 8192
#define WPX_UNDERLINE_BIT 16384
#define WPX_SMALLCAPS_BIT 32768
#define WPX_BLINK_BIT 65536
#define WPX_REVERSEVIDEO_BIT 131072

// BREAK bits
// TODO: I wouldn't have a clue what defines we need, so I figured we need at least the following ones:
#define WPX_PAGE_BREAK 1
#define WPX_SECTION_BREAK 2
#define WPX_COLUMN_BREAK 3

class WPXHLListener
{
	// not sure if this is needed yet
};

#endif /* WPXHLLISTENER_H */
