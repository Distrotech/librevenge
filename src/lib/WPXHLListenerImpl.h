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
	\param propList Property list for the metadata. May contain:
	\li \c author The document's author
	\li \c subject The document's subject
	\li \c publisher The document's publisher
	\li \c keywords The document's keywords
	\li \c abstract Abstract of the document's contents
	\li \c descriptive-name The descriptive name for the document
	\li \c descriptive-type The descriptive type for the document
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
	\param propList Property list for the page span. May contain:
	\li \c num-pages The length of this span, in number of pages
	\li \c is-last-page-span Whether this is the start of the last page span in the document
	\li \c page-height The height of the page, in inches (portrait or landscape)
	\li \c page-width The width of the page, in inches
	\li \c orientation The orientation of the page, encoded as an integer
	\li \c margin-left The left margin for each page in the span, in inches
	\li \c margin-right The right margin for each page in the span, in inches
	\li \c margin-top The top margin for each page in the span, in inches
	\li \c margin-bottom The bottom margin for each page in the span, in inches
	*/
	virtual void openPageSpan(const WPXPropertyList &propList) = 0;
	/**
	Called when a page span is closed.
	*/
	virtual void closePageSpan() = 0;
	/**
	Called when a header or footer should be opened (a sub-document will be placed inside of it).
	\param occurrence Determines on which pages the header will occur, encoded as an integer
	*/
	virtual void openHeaderFooter(const WPXPropertyList &propList) = 0;
	virtual void closeHeaderFooter(const WPXPropertyList &propList) = 0;

	/**
	Called when a new paragraph is opened. This (or openListElement) will always be called before any text or span is placed into the document.
	\param propList Property list for the paragraph. May contain:
	\li \c justification The justification (left, center, right, full, full all lines, or decimal aligned) encoded as an unsigned 8-bit integer
	\li \c margin-left The left indentation of this paragraph, in inches
	\li \c margin-right The right indentation of this paragraph, in inches
	\li \c margin-top The amount of extra spacing to be placed before the paragraph, in inches
	\li \c margin-bottom The amount of extra spacing to be placed after the paragraph, in inches
	\li \c text-indent The indentation of first line, in inches (difference relative to margin-left)
	\li \c line-spacing The amount of spacing between lines, in number of lines (1.0 is single spacing)
	\li \c column-break Whether this paragraph should be placed in a new column
	\li \c page-break Whether this paragraph should start a new page
	\param tabStops List of tabstop definitions for the paragraph. If the list is empty, default tabstop definition should be used.
	*/
	virtual void openParagraph(const WPXPropertyList &propList, const vector<WPXTabStop> &tabStops) = 0;
	/**
	Called when a paragraph is closed.
	*/
	virtual void closeParagraph() = 0;
	
	/**
	Called when a text span is opened
	\param propList Property list for the span. May contain:
	\li \c text-attribute-bits  Font style (bold, italic, etc.), encoded as a WP6 attribute state integer
	\li \c font-name The name of the font used in the span, a text string in ascii
	\li \c font-size The size of the font used in the span, in points (72 points per inch)
	\li \c color The color of the font used in the span
	\li \c text-background-color The background color of the text in the span
	*/
	virtual void openSpan(const WPXPropertyList &propList) = 0;
	/**
	Called when a text span is closed
	*/
	virtual void closeSpan() = 0;
	/**
	Called when a new section is opened
	\param propList Property list for the section. May contain: 
	\li \c num-columns  Number of text columns on the page. If equal to 1, the section does not have columns.
	\li \c margin-bottom  Extra space to add after the section, in inches 
	\param columns List of definitions of each column: left gutter, right gutter, and width (includes the gutters). Empty if num-columns is equal to 1.
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
	Called when a string of text should be inserted
	\param text A textbuffer encoded in UCS4
	*/
	virtual void insertText(const UCSString &text) = 0;
	/**
	Called when a line break should be inserted
	*/
 	virtual void insertLineBreak() = 0;

	/**
	Defines an ordered (enumerated) list level
	\param propList Defines a set of properties for the list. May contain:
	\li \c id A unique integer identifier for the list
	\li \c level Level of indentation
	\li \c type Type of list encoded as an integer
	\li \c text-before-number Text that comes before the number in the list
	\li \c text-after-number Text that comes after the number in the list
	\li \c starting-number Text that comes after the number in the list
	*/
	virtual void defineOrderedListLevel(const WPXPropertyList &propList) = 0;
	/**
	Defines an unordered (unenumerated) list level
	\param propList Defines a set of properties for the list level. May contain:
	\li \c id A unique integer identifier for the list
	\li \c level Level of indentation
	\li \c bullet The string that should be used as a bullet
	*/
	virtual void defineUnorderedListLevel(const WPXPropertyList &propList) = 0;	
	/**
	Called when a new ordered list level should be opened
	\param propList Defines a set of properties for the list level. May contain:	
	\li \c id Which list level definition should be used
	*/
	virtual void openOrderedListLevel(const WPXPropertyList &propList) = 0;
	/**
	Called when a new unordered list level should be opened
	\param propList Defines a set of properties for the list level. May contain:	
	\li \c id Which list level definition should be used
	*/
	virtual void openUnorderedListLevel(const WPXPropertyList &propList) = 0;
	/**
	Called when an unordered list level should be closed
	*/
	virtual void closeOrderedListLevel() = 0;
	/**
	Called when an ununordered list level should be closed
	*/
	virtual void closeUnorderedListLevel() = 0;
	/**
	Called when a list element should be opened
	\param propList Defines a set of properties for the list element. May contain:	
	\li \c justification The justification (left, center, right, full, full all lines, or decimal aligned) encoded as an integer
	\li \c margin-left The left indentation of this list element, in inches
	\li \c margin-right The right indentation of this list element, in inches
	\li \c margin-top The amount of extra spacing to be placed before the list element, in inches
	\li \c margin-bottom The amount of extra spacing to be placed after the list element, in inches
	\li \c text-indent The indentation of first line, in inches (difference relative to margin-left)
	\li \c line-spacing The amount of spacing between lines, in number of lines (1.0 is single spacing)
	\param tabStops List of tabstop definitions for the list element. If the list is empty, default tabstop definition should be used.
	*/
	virtual void openListElement(const WPXPropertyList &propList, const vector<WPXTabStop> &tabStops) = 0;
	/**
	Called when a list element should be closed
	*/
	virtual void closeListElement() = 0;       

	/**
	Called when a footnote should be opened (a sub-document will be placed inside of it)
	\param propList Defines a set of properties for the footnote. May contain:
	\li \c number The footnote's number
	*/
	virtual void openFootnote(const WPXPropertyList &propList) = 0;
	virtual void closeFootnote() = 0;
	/**
	Called when a endnote should be opened (a sub-document will be placed inside of it)
	\param propList Defines a set of properties for the endnote. May contain:
	\li \c number The endnote's number
	*/
	virtual void openEndnote(const WPXPropertyList &propList) = 0;
	/**
	Called when a endnote should be closed
	*/
	virtual void closeEndnote() = 0;

	/**
	Called when a table should be opened
	\param propList Defines a set of properties for the table. May contain:
	\li \c alignment The alignment (left, right, between margins, full, absolute position from left margin) encoded as an integer
	\li \c margin-left The left indentation of the table, in inches
	\li \c margin-right The right indentation of the table, in inches
	\li \c left-offset The offset of the table from the left margin
	\param columns Column definitions for the table
	*/
 	virtual void openTable(const WPXPropertyList &propList, const vector < WPXColumnDefinition > &columns) = 0;
	/**
	Called when a new table row is opened
	\param propList Defines a set of properties for the table row. May contain:
	\li \c height The row's height, in inches
	\li \c is-minimum-height Whether the height is a minimum height or an exact height (if height is 0 and isMinimumHeight is true, the row has automatic height) 
	\li \c is-header-row This row contains headings of columns and should repeat at every page (for tables that span several pages)
	*/
 	virtual void openTableRow(const WPXPropertyList &propList) = 0;
	/**
	Called when the current table row is closed
	*/
	virtual void closeTableRow() = 0;
	/**
	Called when a new table cell is opened
	\param propList Property list for the table cell. May contain:
	\li \c col Number of the column in which the cell is positioned
	\li \c row Number of the row in which the cell is positioned
	\li \c col-span Number of columns that the cell spans
	\li \c row-span Number of rows that the cell spans
	\li \c border-bits Which borders should be turned on, encoded as a bitfield
	\li \c foreground-color Foreground color of the cell, encoded as an integer
	\li \c background-color Background color of the cell, encoded as an integer
	\li \c border-color Color of the border lines of the cell encoded as an integer
	\li \c vertical-alignment Vertical alignment of the content in the cell (top, middle, bottom, or full) encoded as an integer
	*/
 	virtual void openTableCell(const WPXPropertyList &propList) = 0;
	/**
	Called when the current table cell is closed
	*/
	virtual void closeTableCell() = 0;
	/**
	Called when a covered (spanned by another cell in the table) table cell is opened
	\param propList Property list for the table cell. May contain:
	\li \c col Number of the column in which the cell is positioned
	\li \c row Number of the row in which the cell is positioned
	*/
	virtual void insertCoveredTableCell(const WPXPropertyList &propList) = 0;
	/**
	Called when the current table is closed
	*/
 	virtual void closeTable() = 0;
};

#endif /* WPXHLLISTENERIMPL_H */
