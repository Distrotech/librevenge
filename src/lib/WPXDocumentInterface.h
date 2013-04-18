/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2002-2005 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#ifndef WPXDOCUMENTINTERFACEIMPL_H
#define WPXDOCUMENTINTERFACEIMPL_H
#include "WPXPropertyList.h"
#include "WPXPropertyListVector.h"
#include "WPXBinaryData.h"

/**
Pure virtual class containing all the callback functions that can be made by
the parser. An application using this library should implement all the function
definitions listed here.
*/

class WPXInputStream;

class WPXDocumentInterface
{
public:
	virtual ~WPXDocumentInterface() {}

	/**
	Called when all document metadata should be set. This is always the first callback made.
	\param propList Property list for the metadata. May contain:
	\li \c dc:creator
	\li \c dc:language The document's keywords
	\li \c dc:publisher The document's publisher
	\li \c dc:source
	\li \c dc:subject The document's subject
	\li \c dc:type The document's type
	\li \c dcterms:available Date when the document was completed
	\li \c dcterms:issued: Date of the version of the document
	\li \c libwpd:abstract Abstract of the document's contents
	\li \c libwpd:account Account
	\li \c libwpd:address Address
	\li \c libwpd:attachments
	\li \c libwpd:authorization
	\li \c libwpd:bill-to
	\li \c libwpd:blind-copy
	\li \c libwpd:carbon-copy
	\li \c libwpd:checked-by
	\li \c libwpd:client
	\li \c libwpd:comments
	\li \c libwpd:department
	\li \c libwpd:descriptive-name The descriptive name for the document
	\li \c libwpd:descriptive-type The descriptive type for the document
	\li \c libwpd:destination
	\li \c libwpd:disposition
	\li \c libwpd:division
	\li \c libwpd:document-number
	\li \c libwpd:editor
	\li \c libwpd:forward-to
	\li \c libwpd:group
	\li \c libwpd:mail-stop
	\li \c libwpd:matter
	\li \c libwpd:office
	\li \c libwpd:owner
	\li \c libwpd:project
	\li \c libwpd:purpose
	\li \c libwpd:received-from
	\li \c libwpd:recorded-by
	\li \c libwpd:recorded-date Date when the document was recorded
	\li \c libwpd:reference
	\li \c libwpd:revision-notes
	\li \c libwpd:revision-number
	\li \c libwpd:section
	\li \c libwpd:security
	\li \c libwpd:status
	\li \c libwpd:telephone-number
	\li \c libwpd:version-notes
	\li \c libwpd:version-number
	\li \c meta:creation-date Document creation date
	\li \c meta:initial-creator The document's author
	\li \c meta:keyword The document's keywords
	\li \c
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

	virtual void definePageStyle(const WPXPropertyList &propList) = 0;

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

	virtual void defineParagraphStyle(const WPXPropertyList &propList, const WPXPropertyListVector &tabStops) = 0;

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
	\li \c style:type Type of tab (left, right, center, or char)
	\li \c style:char Alingnment character for char aligned tabs
	\li \c style:leader-text The leader character
	\li \c style:position Position of the tab
	*/
	virtual void openParagraph(const WPXPropertyList &propList, const WPXPropertyListVector &tabStops) = 0;
	/**
	Called when a paragraph is closed.
	*/
	virtual void closeParagraph() = 0;

	virtual void defineCharacterStyle(const WPXPropertyList &propList) = 0;

	/**
	Called when a text span is opened
	\param propList Property list for the span. May contain:
	\li \c fo:font-style Font style (italic or normal)
	\li \c fo:font-weight Font style (bold or normal)
	\li \c style:text-line-through-type (double or single, if present)
	\li \c style:text-underline-type (double or single, if present)
	\li \c style:text-outline (true or false)
	\li \c fo:font-variant (small-caps, if present)
	\li \c style:font-name The name of the font used in the span, a text string in ascii
	\li \c fo:font-size The size of the font used in the span, in points (72 points per inch)
	\li \c fo:color The color of the font used in the span (encoded in hex: \#RRGGBB)
	\li \c fo:background-color The background color of the text in the span (encoded in hex: \#RRGGBB)
	\li \c style:text-blinking Whether the text should blink (true or false)
	\li \c fo:text-shadow
	*/
	virtual void openSpan(const WPXPropertyList &propList) = 0;
	/**
	Called when a text span is closed
	*/
	virtual void closeSpan() = 0;

	virtual void defineSectionStyle(const WPXPropertyList &propList, const WPXPropertyListVector &columns) = 0;

	/**
	Called when a new section is opened
	\param propList Property list for the section. May contain:
	\li \c fo:margin-left The left indentation of this section, in inches
	\li \c fo:margin-right The right indentation of this section, in inches
	\li \c libwpd:margin-bottom  Extra space to add after the section, in inches
	\li \c text:dont-balance-text-columns Whether or not to balance text columns
	\param columns List of definitions of each column: left gutter, right gutter, and width (includes the gutters). Empty if there is only one column in the section. Each column may contain:
	\li \c style:rel-width
	\li \c fo:start-indent The left indentation of the margin, in inches
	\li \c fo:end-indent The right indentation of the margin, in inches
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
	Called when an explicit space should be inserted
	*/
	virtual void insertSpace() = 0;
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
	Called when a field should be inserted. Field types may include:
	\li \c text:page-number Current page number
	\li \c text:page-count Total # of pages in document
	\param propList Defines a set of properties for the field. May contain:
	\li \c style:num-format Type of page number (for page number)
	*/
	virtual void insertField(const WPXString &type, const WPXPropertyList &propList) = 0;

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
	\li \c style:type Type of tab (left, right, center, or char)
	\li \c style:char Alingnment character for char aligned tabs
	\li \c style:leader-text The leader character
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
	/**
	Called when a footnote should be closed
	*/
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
	Called when a comment or annotation should be opened (a sub-document will be placed inside of it)
	\param propList Defines a set of properties for the comment or annotation.
	*/
	virtual void openComment(const WPXPropertyList &propList) = 0;
	/**
	Called when a comment or annotation should be closed
	*/
	virtual void closeComment() = 0;

	/**
	Called when a text box should be opened (a sub-document will be placed inside of it)
	\param propList Defines a set of properties for the text box.
	*/
	virtual void openTextBox(const WPXPropertyList &propList) = 0;
	/**
	Called when a text box should be closed
	*/
	virtual void closeTextBox() = 0;

	/**
	Called when a table should be opened
	\param propList Defines a set of properties for the table. May contain:
	\li \c table:align The alignment (left, right, center, or margins)
	\li \c fo:margin-left The left indentation of the table, in inches
	\li \c fo:margin-right The right indentation of the table, in inches
	\li \c style:width Total width of the table, in inches
	\li \c fo:break-before Whether this table should be placed in a new column or page (the value is set to column or page if so)
	\param columns Column definitions for the table. May contain
	\li \c style:column-width Width of a column, in inches
	*/
	virtual void openTable(const WPXPropertyList &propList, const WPXPropertyListVector &columns) = 0;
	/**
	Called when a new table row is opened
	\param propList Defines a set of properties for the table row. May contain:
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
	\li \c fo:background-color Color of the cell (encoded in hex: \#RRGGBB)
	\li \c style:vertical-align Vertical alignment of the content in the cell (top, middle, or bottom)
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
	/**
	Called when a positioned box should be opened
	\param propList Defines a set of properties for the box. May contain:
	\li \c text:anchor-type How the Box is anchored (page, frame, paragraph, char, as-char)
	\li \c svg:x Horizontal position of the box (in inches)
	\li \c svg:y Vertical position of the box (in inches)
	\li \c style:horizontal-pos Horizontal alignment of the box (left, center, right, from-left, inside, outside, from-inside)
	\li \c style:horizontal-rel From where the horizontal position of the box is measured (page, page-content, page-start-margin, page-end-margin, frame, frame-content, frame-start-margin, frame-end-margin, paragraph, paragraph-content, paragraph-start-margin, paragraph-end-margin, char)
	\li \c style:vertical-pos Vertical alignment of the box
	\li \c style:vertical-rel From where the vertical position of the box is measured
	\li \c svg:width Width of the box (in inches)
	\li \c style:relative-width If the width should scale to keep the width/height ratio of the original object, this property is set to &quot;scale&quot;.
	\li \c svg:height Height of the box (in inches)
	\li \c style:relative-height If the height should scale to keep the width/height ratio of the original object, this property is set to &quot;scale&quot;.
	*/
	virtual void openFrame(const WPXPropertyList &propList) = 0;
	/**
	Called when the current positioned box is closed
	*/
	virtual void closeFrame() = 0;
	/**
	Called when a binary object should be inserted
	\param propList Defines a set of properties for the object. May contain:
	\li \c libwpd:mimetype The mimetype of the object
	\param data Reference to the binary object
	*/
	virtual void insertBinaryObject(const WPXPropertyList &propList, const WPXBinaryData &data) = 0;
	/**
	Called when a mathml object should be inserted
	\param propList Defines a set of properties for the object.
	\li \c libwpd:mimetype The mimetype of the object
	\param data Reference to the string containing the equation representation
	*/
	virtual void insertEquation(const WPXPropertyList &propList, const WPXString &data) = 0;
};

#endif /* WPXDOCUMENTINTERFACEIMPL_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
