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
 
#ifndef WPXHLLISTENERIMPL_H
#define WPXHLLISTENERIMPL_H
#include <glib.h>

class WPXHLListenerImpl
{
 public:
	//virtual void insertSpan() = 0;
	virtual void startDocument() = 0;
	virtual void endDocument() = 0;
	virtual void openParagraph(guint32 textAttributeBits) = 0;
	virtual void openSpan(guint32 textAttributeBits) = 0;
	virtual void openSection(guint numColumns, gfloat marginLeft, gfloat marginRight) = 0;
	virtual void insertText(const guint16 *textArray, const guint len) = 0;
 	virtual void insertBreak(guint8 breakType) = 0;
 
 	virtual void startTable() = 0;
 	virtual void insertRow() = 0;
 	virtual void insertCell(guint32 numRow, guint numColumn, guint32 rowSpan, guint32 colSpan) = 0;
 	virtual void endTable() = 0;
};

#endif /* WPXHLLISTENERIMPL_H */
