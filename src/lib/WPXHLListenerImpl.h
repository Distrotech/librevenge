/* libwpd2
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
 
#ifndef WPXHLLISTENERIMPL_H
#define WPXHLLISTENERIMPL_H
#include "libwpd.h"
#include <glib.h>
#include <vector>
using namespace std;

class WPXHLListenerImpl
{
 public:
	// startHeader/endHeader: self explanatory and completely optional. if your listener
	// implementation doesn't need these functions, don't implement them
	virtual void startHeader() {}
	virtual void endHeader() {}

	// setDocumentMeta: should be used to set all document metadata
	// this function is guaranteed to be called before any subsequent function
	// which deals with document elements
	// completely optional. if your listener implementation doesn't 
	// need/support these functions, don't implement them
 	virtual void setDocumentMetaData(const UCSString &author, const UCSString &subject,
 					 const UCSString &publisher, const UCSString &category,
 					 const UCSString &keywords, const UCSString &language,
 					 const UCSString &abstract, const UCSString &descriptiveName,
					 const UCSString &descriptiveType) {}

	virtual void startDocument() = 0;
	virtual void endDocument() = 0;

	virtual void openParagraph(const guint8 paragraphJustification, const guint32 textAttributeBits,
				   const gchar *fontName, const gfloat fontSize, 
				   const gfloat lineSpacing, 
				   const gboolean isColumnBreak, const gboolean isPageBreak) = 0;
	virtual void closeParagraph() = 0;
	virtual void openSpan(const guint32 textAttributeBits, const gchar *fontName, const gfloat fontSize) = 0;
	virtual void closeSpan() = 0;
	virtual void openSection(const guint numColumns, const gfloat marginLeft, const gfloat marginRight) = 0;
	virtual void closeSection() = 0;

	virtual void insertTab() = 0;
	virtual void insertText(const UCSString &text) = 0;
 	virtual void insertLineBreak() = 0;

	virtual void defineOrderedListLevel(const gint listID, const guint16 listLevel, const NumberingType listType, 
					    const UCSString &textBeforeNumber, const UCSString &textAfterNumber,
					    const gint startingNumber) = 0;
	virtual void defineUnorderedListLevel(const gint listID, const guint16 listLevel, const UCSString &bullet) = 0;
	virtual void openOrderedListLevel(const gint listID) = 0;
	virtual void openUnorderedListLevel(const gint listID) = 0;
	virtual void closeOrderedListLevel() = 0;
	virtual void closeUnorderedListLevel() = 0;
	virtual void openListElement(const guint8 paragraphJustification, const guint32 textAttributeBits,
				     const gchar *fontName, const gfloat fontSize, 
				     const float lineSpacing) = 0;
	virtual void closeListElement() = 0;
	
	virtual void openFootnote(int number) = 0;
	virtual void closeFootnote() = 0;
	virtual void openEndnote(int number) = 0;
	virtual void closeEndnote() = 0;

 	virtual void openTable(const guint8 tablePositionBits, const gfloat leftOffset, const vector < WPXColumnDefinition > columns) = 0;
 	virtual void openTableRow() = 0;
	virtual void closeTableRow() = 0;
 	virtual void openTableCell(const guint32 col, const guint32 row, const guint32 colSpan, const guint32 rowSpan, 
				   const RGBSColor * cellFgColor, const RGBSColor * cellBgColor) = 0;
	virtual void closeTableCell() = 0;
 	virtual void closeTable() = 0;
};

#endif /* WPXHLLISTENERIMPL_H */
