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
 
#ifndef WP6HLLISTENER_H
#define WP6HLLISTENER_H
 
#include "WP6LLListener.h"
#include "WPXHLListener.h"

class WPXHLListenerImpl;

class WP6HLListener : public WP6LLListener
{
 public:
	WP6HLListener(WPXHLListenerImpl *listenerImpl);
	~WP6HLListener();

	// for getting low-level messages from the parser
	virtual void startDocument();
	virtual void insertCharacter(guint16 character);
	virtual void insertEOL();
 	virtual void insertBreak(guint8 breakType); 
	virtual void undoChange(const guint8 undoType, const guint16 undoLevel);
	virtual void fontChange(const guint16 matchedFontPointSize, const guint16 fontPID);
	virtual void attributeChange(gboolean isOn, guint8 attribute);
	virtual void justificationChange(guint8 justification);
	virtual void marginChange(guint8 side, guint16 margin);
	virtual void columnChange(guint8 numColumns); 
	virtual void endDocument();
 
	virtual void startTable();
 	virtual void insertRow();
 	virtual void insertCell(guint8 colSpan, guint8 rowSpan, gboolean boundFromLeft, gboolean boundFromAbove, RGBSColor * cellFgColor, RGBSColor * cellBgColor);
 	virtual void endTable(); 

 protected:
	void _flushText();

 private:
	WPXHLListenerImpl * m_listenerImpl;

	GArray *m_textArray;
	
	guint32 m_textAttributeBits;
	gboolean m_textAttributesChanged;

	gfloat m_currentFontSize;
	GString * m_currentFontName;

	gboolean m_isParagraphColumnBreak;
	gboolean m_isParagraphPageBreak;
	guint8 m_paragraphJustification;
 	gboolean m_paragraphJustificationChanged;
	gboolean m_isParagraphOpened;
	gboolean m_isParagraphClosed;
	guint m_numDeferredParagraphBreaks;
	
	gboolean m_isTableOpened;

	gboolean m_sectionAttributesChanged;
	guint m_numColumns;
	gboolean m_isLeftMarginSet;
	gboolean m_isRightMarginSet;
	gfloat m_marginLeft;
	gfloat m_marginRight;
	
	gint32 m_currentRow;
	gint32 m_currentColumn;
	
	gboolean m_isUndoOn;
};

#endif /* WP6HLLISTENER_H */
