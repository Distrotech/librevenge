 /* libwpd
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002-2003 Marc Maurer (j.m.maurer@student.utwente.nl)
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

#ifndef TEXTLISTENERIMPL_H
#define TEXTLISTENERIMPL_H

#include <glib.h>
#include "WPXHLListenerImpl.h"
#include "WPXHLListener.h"

class TextListenerImpl : public WPXHLListenerImpl
{
public:
	TextListenerImpl();
	virtual ~TextListenerImpl();

 	virtual void setDocumentMetaData(const UCSString &author, const UCSString &subject,
					 const UCSString &publisher, const UCSString &category,
					 const UCSString &keywords, const UCSString &language,
					 const UCSString &abstract, const UCSString &descriptiveName,
					 const UCSString &descriptiveType) {}

	virtual void startDocument() {}
	virtual void endDocument() {}

	virtual void openPageSpan(const int span, const bool isLastPageSpan,
				  const float formLength, const float formWidth, const WPXFormOrientation orientation,
				  const float marginLeft, const float marginRight,
				  const float marginTop, const float marginBottom) {}
	virtual void closePageSpan() {}
	virtual void openHeaderFooter(const WPXHeaderFooterType headerFooterType, const WPXHeaderFooterOccurence headerFooterOccurence) {}
	virtual void closeHeaderFooter(const WPXHeaderFooterType headerFooterType, const WPXHeaderFooterOccurence headerFooterOccurence) {}

	virtual void openSection(const unsigned int numColumns, const float spaceAfter) {}
	virtual void closeSection() {}
	virtual void openParagraph(const guint8 paragraphJustification, const guint32 textAttributeBits,
				   const float marginLeftOffset, const float marginRightOffset, const float textIndent,
				   const gchar *fontName, const float fontSize, const RGBSColor *fontColor,
				   const RGBSColor *highlightColor, const float lineSpacing, const float spacingAfterParagraph,
				   bool isColumnBreak, bool isPageBreak) {}
	virtual void closeParagraph();
	virtual void openSpan(const guint32 textAttributeBits, const gchar *fontName, const float fontSize,
				   const RGBSColor *fontColor, const RGBSColor *highlightColor) {}
	virtual void closeSpan() {}

	virtual void insertTab();
	virtual void insertText(const UCSString &text);
	virtual void insertLineBreak() {}

	virtual void defineOrderedListLevel(const int listID, const guint16 listLevel, const WPXNumberingType listType,
					    const UCSString &textBeforeNumber, const UCSString &textAfterNumber,
					    const int startingNumber) {}
	virtual void defineUnorderedListLevel(const int listID, const guint16 listLevel, const UCSString &bullet) {}
	virtual void openOrderedListLevel(const int listID) {}
	virtual void openUnorderedListLevel(const int listID) {}
	virtual void closeOrderedListLevel() {}
	virtual void closeUnorderedListLevel() {}
	virtual void openListElement(const guint8 paragraphJustification, const guint32 textAttributeBits,
				   const float marginLeftOffset, const float marginRightOffset, const float textIndent,
				   const gchar *fontName, const float fontSize, const RGBSColor *fontColor,
			       const RGBSColor *highlightColor, const float lineSpacing, const float spacingAfterParagraph) {}
	virtual void closeListElement() {}

	virtual void openFootnote(int number) {}
	virtual void closeFootnote() {}
	virtual void openEndnote(int number) {}
	virtual void closeEndnote() {}


	virtual void openTable(const guint8 tablePositionBits,
			       const float marginLeftOffset, const float marginRightOffset,
			       const float leftOffset, const vector < WPXColumnDefinition > &columns) {}
	virtual void openTableRow() {}
	virtual void closeTableRow() {}
	virtual void openTableCell(const guint32 col, const guint32 row, const guint32 colSpan, const guint32 rowSpan,
				   const guint8 borderBits,
				   const RGBSColor * cellFgColor, const RGBSColor * cellBgColor) {}
	virtual void closeTableCell() {}
	virtual void insertCoveredTableCell(const guint32 col, const guint32 row) {}
	virtual void closeTable() {}

private:
	guint m_currentListLevel;
};

#endif /* TEXTLISTENERIMPL_H */
