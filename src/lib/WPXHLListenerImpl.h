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

#ifndef WPXHLLISTENERIMPL_H
#define WPXHLLISTENERIMPL_H
#include "libwpd.h"
#include <glib.h>
#include <vector>
using namespace std;

/**
Pure virtual class containing all the callback functions that can be made by
the parser. An application using this library should implement all the function
definitions listed here.
*/

class WPXHLListenerImpl
{
 public:
	// setDocumentMeta: should be used to set all document metadata
	// this function is guaranteed to be called before any subsequent function
	// which deals with document elements
	// completely optional. if your listener implementation doesn't
	// need/support these functions, don't implement them
 	virtual void setDocumentMetaData(const UCSString &author, const UCSString &subject,
					 const UCSString &publisher, const UCSString &category,
					 const UCSString &keywords, const UCSString &language,
					 const UCSString &abstract, const UCSString &descriptiveName,
					 const UCSString &descriptiveType) = 0;

	/**
	Called at the start of the parsing process. This is always the first callback made.
	*/
	virtual void startDocument() = 0;
	 /**
	Called at the end of the parsing process. This is always the last callback made.
	 */
	virtual void endDocument() = 0;

	/**
	Called when a new page span is opened. This will always be called before any actual content is placed into
	the document.
	\param span The length of this span, in number of pages
	\param isLastPageSpan true if this is the start of the last page span in the document
	\param formLength The height of the page, in inches
	\param formWidth The width of the page, in inches
	\param orientation The orientation of the page (portrait or landscape)
	\param marginLeft The left margin for each page in the span, in inches
	\param marginRight The right margin for each page in the span, in inches
	\param marginTop The top margin for each page in the span, in inches
	\param marginBottom The bottom margin for each page in the span, in inches
	*/
	virtual void openPageSpan(const int span, const bool isLastPageSpan,
				  const float formLength, const float formWidth, const WPXFormOrientation orientation,
				  const float marginLeft, const float marginRight,
				  const float marginTop, const float marginBottom) = 0;
	/**
	Called when a page span is closed.
	*/
	virtual void closePageSpan() = 0;
	virtual void openHeaderFooter(const WPXHeaderFooterType headerFooterType, const WPXHeaderFooterOccurence headerFooterOccurence) = 0;
	virtual void closeHeaderFooter(const WPXHeaderFooterType headerFooterType, const WPXHeaderFooterOccurence headerFooterOccurence) = 0;

	/**
	Called when a new paragraph is opened. This (or openListElement) will always be called before any text or span is placed into the document.
	\param paragraphJustification The justification (left, center, right, full, full all lines, or reserved) encoded as an unsigned 8-bit integer
	\param marginLeftOffset The left indentation of this paragraph, in inches
	\param marginLeftOffset The right indentation of this paragraph, in inches
	\param textIndent The left indentation of first line, in inches
	\param lineSpacing The amount of spacing between lines, in number of lines (1.0 is single spacing)
        \param spacingBeforeParagraph The amount of extra spacing to be placed before the paragraph, in inches
	\param spacingAfterParagraph The amount of extra spacing to be placed after the paragraph, in inches
	\param isColumnBreak Whether this paragraph should be placed in a new column
	\param isPageBreak Whether this paragraph should start a new page
	*/
	virtual void openParagraph(const guint8 paragraphJustification,
				   const float marginLeftOffset, const float marginRightOffset, const float textIndent,
				   const float lineSpacing, const float spacingBeforeParagraph, const float spacingAfterParagraph,
				   const bool isColumnBreak, const bool isPageBreak) = 0;
	/**
	Called when a paragraph is closed.
	*/
	virtual void closeParagraph() = 0;
	virtual void openSpan(const guint32 textAttributeBits, const gchar *fontName, const float fontSize,
			      const RGBSColor *fontColor, const RGBSColor *highlightColor) = 0;
	virtual void closeSpan() = 0;
	virtual void openSection(const unsigned int numColumns, const float spaceAfter) = 0;
	virtual void closeSection() = 0;

	/**
	Called when a TAB character should be inserted
	*/
	virtual void insertTab() = 0;
	/**
	Called when a string of text should be inserted. The textbuffer contains only
	characters which all have the same set of properties.
	\param text A textbuffer in the UCS4 encoding
	*/
	virtual void insertText(const UCSString &text) = 0;
	/**
	Called when a line break should be inserted
	*/
 	virtual void insertLineBreak() = 0;

	virtual void defineOrderedListLevel(const int listID, const guint16 listLevel, const WPXNumberingType listType,
					    const UCSString &textBeforeNumber, const UCSString &textAfterNumber,
					    const int startingNumber) = 0;
	virtual void defineUnorderedListLevel(const int listID, const guint16 listLevel, const UCSString &bullet) = 0;
	virtual void openOrderedListLevel(const int listID) = 0;
	virtual void openUnorderedListLevel(const int listID) = 0;
	virtual void closeOrderedListLevel() = 0;
	virtual void closeUnorderedListLevel() = 0;
	virtual void openListElement(const guint8 paragraphJustification,
				     const float marginLeftOffset, const float marginRightOffset, const float textIndent,
				     const float lineSpacing, const float spacingBeforeParagraph, const float spacingAfterParagraph) = 0;
	virtual void closeListElement() = 0;

	virtual void openFootnote(int number) = 0;
	virtual void closeFootnote() = 0;
	virtual void openEndnote(int number) = 0;
	virtual void closeEndnote() = 0;

 	virtual void openTable(const guint8 tablePositionBits,
			       const float marginLeftOffset, const float marginRightOffset,
			       const float leftOffset, const vector < WPXColumnDefinition > &columns) = 0;
	/**
	Called when a new table row should be started
	*/
 	virtual void openTableRow() = 0;
	/**
	Called when the current table row is closed
	*/
	virtual void closeTableRow() = 0;
 	virtual void openTableCell(const guint32 col, const guint32 row, const guint32 colSpan, const guint32 rowSpan,
				   const guint8 borderBits,
				   const RGBSColor * cellFgColor, const RGBSColor * cellBgColor) = 0;
	/**
	Called when the current table cell is closed
	*/
	virtual void closeTableCell() = 0;
	virtual void insertCoveredTableCell(const guint32 col, const guint32 row) = 0;
	/**
	Called when the current table is closed
	*/
 	virtual void closeTable() = 0;
};

#endif /* WPXHLLISTENERIMPL_H */
