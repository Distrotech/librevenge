/* libwpd
 * Copyright (C) 2002-2005 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#ifndef WPXHLLISTENERIMPL_H
#define WPXHLLISTENERIMPL_H
#include "WPXPropertyList.h"
#include "WPXPropertyListVector.h"

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
	\li \c dc:author The document's author
	\li \c dc:subject The document's subject
	\li \c dc:publisher The document's publisher
	\li \c dc:type The document's type
	\li \c dc:language The document's keywords
	\li \c libwpd:keywords The document's keywords
	\li \c libwpd:abstract Abstract of the document's contents
	\li \c libwpd:descriptive-name The descriptive name for the document
	\li \c libwpd:descriptive-type The descriptive type for the document
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
	\li \c libwpd:num-pages The length of this span, in number of pages
	\li \c libwpd:is-last-page-span Whether this is the start of the last page span in the document
	\li \c fo:page-width The width of the page, in inches
	\li \c fo:page-height The height of the page, in inches
	\li \c style:print-orientation The orientation of the page (portrait or landscape)
	\li \c fo:margin-left The left margin for each page in the span, in inches
	\li \c fo:margin-right The right margin for each page in the span, in inches
	\li \c fo:margin-top The top margin for each page in the span, in inches
	\li \c fo:margin-bottom The bottom margin for each page in the span, in inches
	*/
	virtual void openPageSpan(const WPXPropertyList &propList) = 0;
	/**
	Called when a page span is closed.
	*/
	virtual void closePageSpan() = 0;

	/**
	Called when a header should be opened (a sub-document will be placed inside of it).
	\param propList Property list for the header. May contain: 
	\li \c libwpd:occurrence Determines on which pages the header will occur (odd, even, or all)
	*/
	virtual void openHeader(const WPXPropertyList &propList) = 0;
	/**
	Called when a header should be closed.
	*/
	virtual void closeHeader() = 0;

	/**
	Called when a footer should be opened (a sub-document will be placed inside of it).
	\param propList Property list for the footer. May contain: 
	\li \c libwpd:occurrence Determines on which pages the footer will occur (odd, even, or all)
	*/
	virtual void openFooter(const WPXPropertyList &propList) = 0;
	/**
	Called when a footer should be closed.
	*/
	virtual void closeFooter() = 0;

	/**
	Called when a new paragraph is opened. This (or openListElement) will always be called before any text or span is placed into the document.
	\param propList Property list for the paragraph. May contain:
	\li \c fo:text-align The justification of this paragraph (left, center, end, full, or justify)
	\li \c fo:margin-left The left indentation of this paragraph, in inches
	\li \c fo:margin-right The right indentation of this paragraph, in inches
	\li \c fo:margin-top The amount of extra spacing to be placed before the paragraph, in inches
	\li \c fo:margin-bottom The amount of extra spacing to be placed after the paragraph, in inches
	\li \c fo:text-indent The indentation of first line, in inches (difference relative to margin-left)
	\li \c fo:line-height The amount of spacing between lines, in number of lines (1.0 is single spacing)
	\li \c fo:break-before Whether this paragraph should be placed in a new column or page (the value is set to column or page if so)
	\param tabStops List of tabstop definitions for the paragraph. If the list is empty, default tabstop definition should be used. Each tab stop may contain:
	\li \c style:type Type of tab (left, right, center, char, or .)
	\li \c style:leader-char The leader character
	\li \c style:position Position of the tab
	*/
	virtual void openParagraph(const WPXPropertyList &propList, const WPXPropertyListVector &tabStops) = 0;
	/**
	Called when a paragraph is closed.
	*/
	virtual void closeParagraph() = 0;
	
	/**
	Called when a text span is opened
	\param propList Property list for the span. May contain:
	\li \c fo:font-style Font style (italic or normal)
	\li \c fo:font-weight Font style (bold or normal)
	\li \c style:text-crossing-out (single-line, if present)
	\li \c style:text-underline (double or single)
	\li \c style:text-outline (true or false)
	\li \c fo:font-variant (small-caps, if present)
	\li \c style:font-name The name of the font used in the span, a text string in ascii
	\li \c fo:font-size The size of the font used in the span, in points (72 points per inch)
	\li \c fo:color The color of the font used in the span (encoded in hex: #RRGGBB)
	\li \c style:text-background-color The background color of the text in the span
	*/
	virtual void openSpan(const WPXPropertyList &propList) = 0;
	/**
	Called when a text span is closed
	*/
	virtual void closeSpan() = 0;
	/**
	Called when a new section is opened
	\param propList Property list for the section. May contain: 
	\li \c fo:margin-bottom  Extra space to add after the section, in inches 
	\li \c text:dont-balance-text-columns Whether or not to balance text columns
	\param columns List of definitions of each column: left gutter, right gutter, and width (includes the gutters). Empty if there is only one column in the section. Each column may contain:
	\li \c style:rel-width
	\li \c fo:margin-left The left indentation of the margin, in inches
	\li \c fo:margin-right The right indentation of the margin, in inches
	*/
	virtual void openSection(const WPXPropertyList &propList, const WPXPropertyListVector &columns) = 0;
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
	\param text A textbuffer encoded as a UTF8 string
	*/
	virtual void insertText(const WPXString &text) = 0;
	/**
	Called when a line break should be inserted
	*/
 	virtual void insertLineBreak() = 0;

	/**
	Defines an ordered (enumerated) list level
	\param propList Defines a set of properties for the list. May contain:
	\li \c libwpd:id A unique integer identifier for the list
	\li \c libwpd:level The level of the list in the hierarchy
	\li \c style:num-format Type of list
	\li \c style:num-prefix Text that comes before the number in the list
	\li \c style:num-suffix Text that comes after the number in the list
	\li \c text:start-value The starting number of the list
	\li \c text:min-label-width The distance between the list label and the actual text, stored in inches
	\li \c text:space-before The indentation level of the lists, stored in inches
	*/
	virtual void defineOrderedListLevel(const WPXPropertyList &propList) = 0;
	/**
	Defines an unordered (unenumerated) list level
	\param propList Defines a set of properties for the list level. May contain:
	\li \c libwpd:id A unique integer identifier for the list
	\li \c libwpd:level The level of the list in the hierarchy
	\li \c text:bullet-char The string that should be used as a bullet
	\li \c text:min-label-width The distance between the bullet and the actual text, stored in inches
	\li \c text:space-before The indentation level of the lists, stored in inches
	*/
	virtual void defineUnorderedListLevel(const WPXPropertyList &propList) = 0;	
	/**
	Called when a new ordered list level should be opened
	\param propList Defines a set of properties for the list level. May contain:	
	\li \c libwpd:id Which list level definition should be used
	*/
	virtual void openOrderedListLevel(const WPXPropertyList &propList) = 0;
	/**
	Called when a new unordered list level should be opened
	\param propList Defines a set of properties for the list level. May contain:	
	\li \c libwpd:id Which list level definition should be used
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
	\param propList Property list for the paragraph. May contain:
	\li \c fo:text-align The justification of this paragraph (left, center, end, full, or justify)
	\li \c fo:margin-left The left indentation of this paragraph, in inches
	\li \c fo:margin-right The right indentation of this paragraph, in inches
	\li \c fo:margin-top The amount of extra spacing to be placed before the paragraph, in inches
	\li \c fo:margin-bottom The amount of extra spacing to be placed after the paragraph, in inches
	\li \c fo:text-indent The indentation of first line, in inches (difference relative to margin-left)
	\li \c fo:line-height The amount of spacing between lines, in number of lines (1.0 is single spacing)
	\li \c fo:break-before Whether this paragraph should be placed in a new column or page (the value is set to column or page if so)
	\param tabStops List of tabstop definitions for the paragraph. If the list is empty, default tabstop definition should be used. Each tab stop may contain:
	\li \c style:type Type of tab (left, right, center, char, or .)
	\li \c style:leader-char The leader character
	\li \c style:position Position of the tab
	*/
	virtual void openListElement(const WPXPropertyList &propList, const WPXPropertyListVector &tabStops) = 0;
	/**
	Called when a list element should be closed
	*/
	virtual void closeListElement() = 0;       

	/**
	Called when a footnote should be opened (a sub-document will be placed inside of it)
	\param propList Defines a set of properties for the footnote. May contain:
	\li \c libwpd:number The footnote's number
	*/
	virtual void openFootnote(const WPXPropertyList &propList) = 0;
	virtual void closeFootnote() = 0;
	/**
	Called when a endnote should be opened (a sub-document will be placed inside of it)
	\param propList Defines a set of properties for the endnote. May contain:
	\li \c libwpd:number The endnote's number
	*/
	virtual void openEndnote(const WPXPropertyList &propList) = 0;
	/**
	Called when a endnote should be closed
	*/
	virtual void closeEndnote() = 0;

	/**
	Called when a table should be opened
	\param propList Defines a set of properties for the table. May contain:
	\li \c table:align The alignment (left, right, center, or margins)
	\li \c fo:margin-left The left indentation of the table, in inches
	\li \c fo:margin-right The right indentation of the table, in inches
	\li \c style:width Total width of the table, in inches
	\param columns Column definitions for the table. May contain
	\li \c style:column-width Width of a column, in inches
	*/
 	virtual void openTable(const WPXPropertyList &propList, const WPXPropertyListVector &columns) = 0;
	/**
	Called when a new table row is opened
	\param propList Defines a set of properties for the table row. May contain:
	\li \c fo:break-before Whether this table should be placed in a new column or page (the value is set to column or page if so)
	\li \c style:row-height The row's height, in inches
	\li \c style:min-row-height The row's minimum height, in inches
	\li \c libwpd:is-header-row This row contains headings of columns and should repeat at every page (for tables that span several pages)
	*/
 	virtual void openTableRow(const WPXPropertyList &propList) = 0;
	/**
	Called when the current table row is closed
	*/
	virtual void closeTableRow() = 0;
	/**
	Called when a new table cell is opened
	\param propList Property list for the table cell. May contain:
	\li \c libwpd:column Number of the column in which the cell is positioned
	\li \c libwpd:row Number of the row in which the cell is positioned
	\li \c table:number-columns-spanned Number of columns that the cell spans
	\li \c table:number-rows-spanned Number of rows that the cell spans
	\li \c fo:border-left Properties of the left border of the cell
	\li \c fo:border-right Properties of the right border of the cell
	\li \c fo:border-top Properties of the top border of the cell
	\li \c fo:border-bottom Properties of the bottom border of the cell
	\li \c fo:background-color Color of the cell
	\li \c fo:vertical-align Vertical alignment of the content in the cell (top, middle, or bottom)
	*/
 	virtual void openTableCell(const WPXPropertyList &propList) = 0;
	/**
	Called when the current table cell is closed
	*/
	virtual void closeTableCell() = 0;
	/**
	Called when a covered (spanned by another cell in the table) table cell is opened
	\param propList Property list for the table cell. May contain:
	\li \c libwpd:column Number of the column in which the cell is positioned
	\li \c libwpd:row Number of the row in which the cell is positioned
	*/
	virtual void insertCoveredTableCell(const WPXPropertyList &propList) = 0;
	/**
	Called when the current table is closed
	*/
 	virtual void closeTable() = 0;

protected:
	~WPXHLListenerImpl() {}
};

#endif /* WPXHLLISTENERIMPL_H */
