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

#ifndef WPXLLLISTENER_H
#define WPXLLLISTENER_H
#include <glib.h>
#include "libwpd_support.h"

class WPXLLListener
{
 public:
	WPXLLListener() {}
	virtual ~WPXLLListener() {}
	virtual void startDocument() = 0;
	virtual void setAlignmentCharacter(const guint16 character) = 0;
	virtual void setDotLeaderCharacters(const guint16 character, const guint8 numberOfSpaces) = 0;
	virtual void insertCharacter(const guint16 character) = 0;
	virtual void insertTab(const guint8 tabType, const guint16 tabPosition) = 0;
	virtual void handleLineBreak() = 0;
	virtual void insertEOL() = 0;
 	virtual void insertBreak(const guint8 breakType) = 0;
	virtual void attributeChange(const bool isOn, const guint8 attribute) = 0;
	virtual void lineSpacingChange(const float lineSpacing) = 0;
	virtual void spacingAfterParagraphChange(const float spacingRelative, const float spacingAbsolute) = 0;
	virtual void justificationChange(const guint8 justification) = 0;
	virtual void pageMarginChange(const guint8 side, const guint16 margin) = 0;
	virtual void pageFormChange(const guint16 length, const guint16 width, const WPXFormOrientation orientation) = 0;
	virtual void marginChange(const guint8 side, const guint16 margin) = 0;
	virtual void paragraphMarginChange(const guint8 side, const gint16 margin) = 0;
	virtual void indentFirstLineChange(const gint16 offset) = 0;
	virtual void columnChange(const guint8 numColumns) = 0;
	virtual void endDocument() = 0;

	virtual void defineTable(guint8 position, guint16 leftOffset) = 0;
	virtual void addTableColumnDefinition(guint32 width, guint32 leftGutter, guint32 rightGutter) = 0;
	virtual void startTable() = 0;
 	virtual void insertRow() = 0;
 	virtual void insertCell(const guint8 colSpan, const guint8 rowSpan, const bool boundFromLeft, const bool boundFromAbove,
				const guint8 borderBits,
				const RGBSColor * cellFgColor, const RGBSColor * cellBgColor) = 0;
 	virtual void endTable() = 0;
};

#endif /* WPXLLLISTENER_H */
