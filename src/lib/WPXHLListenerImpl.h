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
#include "libwpd.h"
#include <glib.h>

class WPXHLListenerImpl
{
 public:
	virtual void startDocument() = 0;
	virtual void endDocument() = 0;
	virtual void openParagraph(const guint8 paragraphJustification, const guint32 textAttributeBits,
				   const gchar *fontName, const gfloat fontSize, 
				   const gboolean isColumnBreak, const gboolean isPageBreak) = 0;
	virtual void openSpan(const guint32 textAttributeBits, const gchar *fontName, const gfloat fontSize) = 0;
	virtual void openSection(const guint numColumns, const gfloat marginLeft, const gfloat marginRight) = 0;
	virtual void insertText(const GArray *text) = 0;
 	virtual void insertLineBreak() = 0;

	virtual void defineOrderedListLevel(const gint listID, const guint16 listLevel, const WPXListType listType, 
					    const GArray *textBeforeNumber, const GArray *textAfterNumber, 
					    const gint startingNum) = 0;
	virtual void defineUnorderedListLevel(const gint listID, const guint16 listLevel, const GArray *bullet) = 0;
	virtual void openOrderedListLevel(const gint listID) = 0;
	virtual void openUnorderedListLevel(const gint listID) = 0;
	virtual void closeOrderedListLevel() = 0;
	virtual void closeUnorderedListLevel() = 0;
	virtual void openListElement(const gint listID) = 0;
	virtual void closeListElement() = 0;
 
 	virtual void openTable() = 0;
 	virtual void openRow() = 0;
 	virtual void openCell(const guint32 col, const guint32 row, const guint32 colSpan, const guint32 rowSpan, 
			      const RGBSColor * cellFgColor, const RGBSColor * cellBgColor) = 0;
 	virtual void closeTable() = 0;
};

#endif /* WPXHLLISTENERIMPL_H */
