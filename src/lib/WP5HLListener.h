/* libwpd2
 * Copyright (C) 2003 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2003 Marc Maurer (j.m.maurer@student.utwente.nl)
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
 
#ifndef WP5HLLISTENER_H
#define WP5HLLISTENER_H
 
#include "WP5LLListener.h"
#include "WPXHLListener.h"
#include "WPXHLListenerImpl.h"

class WP5HLListener : public WP5LLListener
{
public:
	WP5HLListener(WPXHLListenerImpl *listenerImpl);

	virtual void startDocument();
	virtual void insertCharacter(const guint16 character);
	virtual void insertTab(const guint8 tabType);
	virtual void insertEOL();
 	virtual void insertBreak(const guint8 breakType) {};
	virtual void attributeChange(const bool isOn, const guint8 attribute);
	virtual void lineSpacingChange(const float lineSpacing) {};
	virtual void justificationChange(const guint8 justification) {};
	virtual void pageMarginChange(const guint8 side, const guint16 margin) {};
	virtual void marginChange(const guint8 side, const guint16 margin) {};
	virtual void columnChange(const guint8 numColumns) {};
	virtual void endDocument();
		
	virtual void defineTable(guint8 position, guint16 leftOffset) {};
	virtual void addTableColumnDefinition(guint32 width, guint32 leftGutter, guint32 rightGutter) {};
	virtual void startTable() {};
 	virtual void insertRow() {};
 	virtual void insertCell(const guint8 colSpan, const guint8 rowSpan, const bool boundFromLeft, const bool boundFromAbove, 
				const guint8 borderBits, 	
				const RGBSColor * cellFgColor, const RGBSColor * cellBgColor) {};
 	virtual void endTable() {};

private:
	void _flushText();
	void _openSpan();

	WPXHLListenerImpl *m_listenerImpl;

	UCSString m_textBuffer;

	bool m_textAttributesChanged;
	guint32 m_textAttributeBits;
};

#endif /* WP5HLLISTENER_H */
