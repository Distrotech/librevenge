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
#include "libwpd_support.h"
#include "WPXProperty.h"
#include <vector>
#include <map>
using namespace std;

/**
Pure virtual class containing all the callback functions that can be made by
the parser. An application using this library should implement all the function
definitions listed here.
*/

class WPXHLListenerImpl
{
 public:
	/** 
	Called when all document metadata should be set. This is always the first callback made.
	*/
 	virtual void setDocumentMetaData(const WPXPropertyList &propList) = 0;

	/**
	Called at the start of the parsing process. This is always the second callback made.
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
	\param formLength The height of the page, in inches (portrait or landscape)
	\param formWidth The width of the page, in inches
	\param orientation The orientation of the page 
	\param marginLeft The left margin for each page in the span, in inches
	\param marginRight The right margin for each page in the span, in inches
	\param marginTop The top margin for each page in the span, in inches
	\param marginBottom The bottom margin for each page in the span, in inches
	*/
	virtual void openPageSpan(const WPXPropertyList &propList) = 0;
	/**
	Called when a page span is closed.
	*/
	virtual void closePageSpan() = 0;
	virtual void openHeaderFooter(const WPXPropertyList &propList) = 0;
	virtual void closeHeaderFooter(const WPXPropertyList &propList) = 0;

	/**
	Called when a new paragraph is opened. This (or openListElement) will always be called before any text or span is placed into the document.
	\param paragraphJustification The justification (left, center, right, full, full all lines, or decimal aligned) encoded as an unsigned 8-bit integer
	\param marginLeftOffset The left indentation of this paragraph, in inches
	\param marginRightOffset The right indentation of this paragraph, in inches
	\param textIndent The indentation of first line, in inches (difference relative to marginLeftOffset)
	\param lineSpacing The amount of spacing between lines, in number of lines (1.0 is single spacing)
	\param spacingBeforeParagraph The amount of extra spacing to be placed before the paragraph, in inches
	\param spacingAfterParagraph The amount of extra spacing to be placed after the paragraph, in inches
	\param tabStops List of tabstop definitions for the paragraph. If the list is empty, default tabstop definition should be used.
	\param isColumnBreak Whether this paragraph should be placed in a new column
	\param isPageBreak Whether this paragraph should start a new page
	*/
	virtual void openParagraph(const WPXPropertyList &propList, const vector<WPXTabStop> &tabStops) = 0;
	/**
	Called when a paragraph is closed.
	*/
	virtual void closeParagraph() = 0;
	
	/**
	Called when a text span is opened
	\param textAttributeBits Corresponds to the WP6 Attribute state variable
	\param fontName The name of the font used in the span, a text string in ascii
	\param fontSize The size of the font used in the span, in points (72 points per inch)
	\param fontColor The color of the font used in the span
	\param highlightColor The color of highlight (the pointer is empty if there is no highlight)
	*/
	virtual void openSpan(const WPXPropertyList &propList) = 0;
	/**
	Called when a text span is closed
	*/
	virtual void closeSpan() = 0;
	/**
	Called when a new section is opened
	\param numColums Number of text columns on the page. If numColumns>=1, the section does not have columns.
	\param columns List of definitions of each column: left gutter, right gutter, and width (includes the gutters). If numColumns>=1 the list is empty.
	\param spaceAfter Extra space to add after the section, in inches 
	*/
	virtual void openSection(const WPXPropertyList &propList, const vector <WPXColumnDefinition> &columns) = 0;
	/**
	Called when a section is closed
	*/
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

	virtual void defineOrderedListLevel(const WPXPropertyList &propList) = 0;
	virtual void defineUnorderedListLevel(const WPXPropertyList &propList) = 0;
	virtual void openOrderedListLevel(const WPXPropertyList &propList) = 0;
	virtual void openUnorderedListLevel(const WPXPropertyList &propList) = 0;
	virtual void closeOrderedListLevel() = 0;
	virtual void closeUnorderedListLevel() = 0;
	virtual void openListElement(const WPXPropertyList &propList, const vector<WPXTabStop> &tabStops) = 0;
	virtual void closeListElement() = 0;

	virtual void openFootnote(const WPXPropertyList &propList) = 0;
	virtual void closeFootnote() = 0;
	virtual void openEndnote(const WPXPropertyList &propList) = 0;
	virtual void closeEndnote() = 0;

 	virtual void openTable(const WPXPropertyList &propList, const vector < WPXColumnDefinition > &columns) = 0;
	/**
	Called when a new table row is opened
	\param height The row's height, in inches
	\param isMinimumHeight Whether the height is a minimum height or an exact height (if height is 0 and isMinimumHeight is true, the row has automatic height) 
	\param isHeaderRow This row contains headings of columns and should repeat at every page (for tables that span several pages)
	*/
 	virtual void openTableRow(const WPXPropertyList &propList) = 0;
	/**
	Called when the current table row is closed
	*/
	virtual void closeTableRow() = 0;
	/**
	Called when a new table cell is opened
	\param col Number of the column in which the cell is positioned
	\param row Number of the row in which the cell is positioned
	\param colSpan Number of columns that the cell spans
	\param rowSpan Number of rows that the cell spans
	\param borderBits
	\param cellFgColor
	\param cellBgColor
	\param cellBorderColor Color of the border lines of the cell
	\param cellVerticalAlignment Vertical alignment of the content in the cell (top, middle, bottom, or full)
	*/
 	virtual void openTableCell(const WPXPropertyList &propList) = 0;
	/**
	Called when the current table cell is closed
	*/
	virtual void closeTableCell() = 0;
	virtual void insertCoveredTableCell(const WPXPropertyList &propList) = 0;
	/**
	Called when the current table is closed
	*/
 	virtual void closeTable() = 0;
};

#endif /* WPXHLLISTENERIMPL_H */
