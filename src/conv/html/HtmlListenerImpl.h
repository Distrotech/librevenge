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

#ifndef HTMLLISTENERIMPL_H
#define HTMLLISTENERIMPL_H

#include <glib.h>
#include "WPXHLListenerImpl.h"
#include "WPXHLListener.h"

class HtmlListenerImpl : public WPXHLListenerImpl
{
public:
	HtmlListenerImpl();
	virtual ~HtmlListenerImpl();
    
	virtual void startHeader();
	virtual void endHeader();
 	virtual void setDocumentMetaData(const UCSString &author, const UCSString &subject,
 					 const UCSString &publisher, const UCSString &category,
 					 const UCSString &keywords, const UCSString &language,
 					 const UCSString &abstract, const UCSString &descriptiveName,
					 const UCSString &descriptiveType)
;
	virtual void startDocument();
	virtual void endDocument();
	virtual void openSection(const guint numColumns, gfloat marginLeft, gfloat marginRight);
	virtual void closeSection();
	virtual void openParagraph(const guint8 paragraphJustification, const guint32 textAttributeBits,
				   const gchar *fontName, gfloat fontSize, 
				   const float lineSpacing,
				   gboolean isColumnBreak, gboolean isPageBreak);
	virtual void closeParagraph();
	virtual void openSpan(const guint32 textAttributeBits, const gchar *fontName, const gfloat fontSize);
	virtual void closeSpan();

	virtual void insertTab();
	virtual void insertText(const UCSString &text);
	virtual void insertLineBreak() {}

	virtual void defineOrderedListLevel(const gint listID, const guint16 listLevel, const NumberingType listType, 
					    const UCSString &textBeforeNumber, const UCSString &textAfterNumber,
					    const gint startingNumber) {}
	virtual void defineUnorderedListLevel(const gint listID, const guint16 listLevel, const UCSString &bullet) {}
	virtual void openOrderedListLevel(const gint listID);
	virtual void openUnorderedListLevel(const gint listID);
	virtual void closeOrderedListLevel();
	virtual void closeUnorderedListLevel();
	virtual void openListElement(const guint8 paragraphJustification, const guint32 textAttributeBits,
				     const gchar *fontName, const gfloat fontSize, 
				     const float lineSpacing);
	virtual void closeListElement();
#if 0
	virtual void openFootnote();
	virtual void closeFootnote();
	virtual void openEndnote();
	virtual void closeEndnote();
#endif
	virtual void openTable();
	virtual void openTableRow();
	virtual void closeTableRow();
	virtual void openTableCell(const guint32 col, const guint32 row, 
				   const guint32 colSpan, const guint32 rowSpan, 
				   const RGBSColor * cellFgColor, const RGBSColor * cellBgColor);
	virtual void closeTableCell();
	virtual void closeTable();
 
protected:
	void _appendTextAttributes(const guint32 textAttributeBits);
	void _appendParagraphJustification(const guint32 paragraphJustification);

private:
	guint m_currentListLevel;
};

#endif /* HTMLLISTENERIMPL_H */
