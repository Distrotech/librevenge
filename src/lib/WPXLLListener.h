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

#ifndef WPXLLLISTENER_H
#define WPXLLLISTENER_H

#include "glib.h"

class WPXLLListener
{
 public:
	WPXLLListener() {}
	virtual ~WPXLLListener() {}
	virtual void startDocument() = 0;
	virtual void insertCharacter(guint16 character) = 0;
	virtual void insertEOL() = 0;
	virtual void attributeChange(gboolean isOn, guint8 attribute) = 0;
	virtual void marginChange(guint8 side, guint16 margin) = 0;
	virtual void endDocument() = 0;
};

#endif /* WPXLLLISTENER_H */
