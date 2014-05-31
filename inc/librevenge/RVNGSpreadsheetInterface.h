/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
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
 */

#ifndef RVNGSPREADSHEETINTERFACE_H
#define RVNGSPREADSHEETINTERFACE_H

#include "RVNGString.h"
#include "RVNGPropertyList.h"
#include "RVNGPropertyListVector.h"

namespace librevenge
{

/**
Pure virtual class containing all the callback functions that can be made by
a spreadsheet parser. An application using this library should implement all the function
definitions listed here.

\note in the following, the formula are defined using RVNGPropertyListVector which must contains the list of instruction to build the formula. Each instruction must contain \c librevenge:type with:
		- \c librevenge:operator with the operator "(", "+", .... must be in propList[librevenge:operator]
		- \c librevenge:function with the function in propList[librevenge:function]
		- \c librevenge:number with the number in propList[librevenge:number]
		- \c librevenge:text with the text in propList[librevenge:text]
		- \c librevenge:cell with the coordinate in propList[librevenge:row] and propList[librevenge:column] ;
		   if needed the sheet name must be in propList[librevenge:sheet-name]
		- \c librevenge:cells with the coordinate in propList[librevenge:start-row],propList[librevenge:start-column]
		    and propList[librevenge:end-row], propList[librevenge:end-column]
			if neededlibrevenge:name the numbering style name
*/

class RVNGSpreadsheetInterface
{
public:


	/** destructor */
	virtual ~RVNGSpreadsheetInterface() {}

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
	\li \c librevenge:abstract Abstract of the document's contents
	\li \c librevenge:account Account
	\li \c librevenge:address Address
	\li \c librevenge:attachments
	\li \c librevenge:authorization
	\li \c librevenge:bill-to
	\li \c librevenge:blind-copy
	\li \c librevenge:carbon-copy
	\li \c librevenge:checked-by
	\li \c librevenge:client
	\li \c librevenge:comments
	\li \c librevenge:department
	\li \c librevenge:descriptive-name The descriptive name for the document
	\li \c librevenge:descriptive-type The descriptive type for the document
	\li \c librevenge:destination
	\li \c librevenge:disposition
	\li \c librevenge:division
	\li \c librevenge:document-number
	\li \c librevenge:editor
	\li \c librevenge:forward-to
	\li \c librevenge:group
	\li \c librevenge:mail-stop
	\li \c librevenge:matter
	\li \c librevenge:office
	\li \c librevenge:owner
	\li \c librevenge:project
	\li \c librevenge:purpose
	\li \c librevenge:received-from
	\li \c librevenge:recorded-by
	\li \c librevenge:recorded-date Date when the document was recorded
	\li \c librevenge:reference
	\li \c librevenge:revision-notes
	\li \c librevenge:revision-number
	\li \c librevenge:section
	\li \c librevenge:security
	\li \c librevenge:status
	\li \c librevenge:telephone-number
	\li \c librevenge:version-notes
	\li \c librevenge:version-number
	\li \c meta:creation-date Document creation date
	\li \c meta:initial-creator The document's author
	\li \c meta:keyword The document's keywords
	\li \c
	*/
	virtual void setDocumentMetaData(const RVNGPropertyList &propList) = 0;

	/**
	Called at the start of the parsing process. This is always the second callback made.
	*/
	virtual void startDocument(const RVNGPropertyList &propList) = 0;
	/**
	    Called at the end of the parsing process. This is always the last callback made.
	*/
	virtual void endDocument() = 0;

	virtual void definePageStyle(const RVNGPropertyList &propList) = 0;

	/**
	Called when an embedded font should be defined
	\param propList A set of properties that specify the font. May contain:
	\li \c librevenge:name The name of the font
	\li \c librevenge:mime-type The mimetype of the font
	\li \c office:binary-data The font data
	\li \c librevenge:replacement-objects A property list vector containing alternative formats of the font. Every element is a property list containing:
	    -# \c librevenge:mime-type The mimetype of the replacement object
	    -# \c office:binary-data The replacement object data
	*/
	virtual void defineEmbeddedFont(const RVNGPropertyList &propList) = 0;

	/**
	Called when a new page span is opened. This will always be called before any actual content is placed into
	the document.
	\param propList Property list for the page span. May contain:
	\li \c librevenge:num-pages The length of this span, in number of pages
	\li \c librevenge:is-last-page-span Whether this is the start of the last page span in the document
	\li \c fo:page-width The width of the page, in inches
	\li \c fo:page-height The height of the page, in inches
	\li \c style:print-orientation The orientation of the page (portrait or landscape)
	\li \c fo:margin-left The left margin for each page in the span, in inches
	\li \c fo:margin-right The right margin for each page in the span, in inches
	\li \c fo:margin-top The top margin for each page in the span, in inches
	\li \c fo:margin-bottom The bottom margin for each page in the span, in inches
	*/
	virtual void openPageSpan(const RVNGPropertyList &propList) = 0;
	/**
	Called when a page span is closed.
	*/
	virtual void closePageSpan() = 0;

	/**
	Called when a header should be opened (a sub-document will be placed inside of it).
	\param propList Property list for the header. May contain:
	\li \c librevenge:occurrence Determines on which pages the header will occur (odd, even, or all)
	*/
	virtual void openHeader(const RVNGPropertyList &propList) = 0;
	/**
	Called when a header should be closed.
	*/
	virtual void closeHeader() = 0;

	/**
	Called when a footer should be opened (a sub-document will be placed inside of it).
	\param propList Property list for the footer. May contain:
	\li \c librevenge:occurrence Determines on which pages the footer will occur (odd, even, or all)
	*/
	virtual void openFooter(const RVNGPropertyList &propList) = 0;
	/**
	Called when a footer should be closed.
	*/
	virtual void closeFooter() = 0;


	/**
	 Called when a numbering style must be defined
	 \param propList Property list for the style. Must contain:
	 - \c librevenge:name the numbering style name
	 - \c librevenge:value_type the numbering type
	 - \c librevenge:format a vector containing a list of format instructions
	 - \c librevenge:conditions a vector containing
	    -# \c librevenge:formula the condition
	    -# \c librevenge:name the style to apply when the condition is verified
	 */
	virtual void defineSheetNumberingStyle(const RVNGPropertyList &propList) = 0;
	/**
	Called when a sheet should be opened
	\param propList Property list for the sheet. May contain:
	- \c librevenge:name Determines the sheet name
	- \c librevenge:columns a vector of property list which must contain:
	    -# style:column-width: the width of a column, in inches
	*/
	virtual void openSheet(const RVNGPropertyList &propList) = 0;
	/**
	Called when a sheet should be closed.
	*/
	virtual void closeSheet() = 0;
	/**
	Called when a new sheet row is opened
	\param propList Defines a set of properties for the table row. May contain:
	\li \c style:row-height The row's height, in inches
	\li \c style:min-row-height The row's minimum height, in inches
	\li \c librevenge:is-header-row This row contains headings of columns and should repeat at every page (for tables that span several pages)
	*/
	virtual void openSheetRow(const RVNGPropertyList &propList) = 0;
	/**
	Called when a sheet row should be closed.
	*/
	virtual void closeSheetRow() = 0;
	/**
	Called when a cell should be opened
	\param propList Property list for the sheet. May contain:
	\li \c librevenge:column the cell column (between 0 and ... )
	\li \c librevenge:row the cell row (between 0 and ... )

	\li \c librevenge:formula a formula
	\li \c librevenge:numbering-name a numbering style name
	\li \c librevenge:double for a number cell
	\li \c librevenge:bool for a boolean cell
	\li \c librevenge:year, librevenge:month, librevenge:day, librevenge:hour, libwp:minute, librevenge:second
	  for a date and/or time cell
	*/
	virtual void openSheetCell(const RVNGPropertyList &propList) = 0;
	/**
	Called when a sheet should be closed.
	*/
	virtual void closeSheetCell() = 0;

	virtual void defineChartStyle(const RVNGPropertyList &propList) = 0;

	/**
	Called when a chart should be opened
	\param propList Property list for the sheet. May contain:
	\li \c librevenge:name Determines the sheet name
	*/
	virtual void openChart(const RVNGPropertyList &propList) = 0;
	/**
	Called when a chart should be closed.
	*/
	virtual void closeChart() = 0;

	/**
	Called when a chart text zone:label/legend/title/subtitle/footer should be opened.
	*/
	virtual void openChartTextObject(const RVNGPropertyList &propList) = 0;
	/**
	Called when a chart text zone:legend/title/subtitle/footer should be closed.
	*/
	virtual void closeChartTextObject() = 0;

	/**
	Called when a chart plot area should be opened.
	*/
	virtual void openChartPlotArea(const RVNGPropertyList &propList) = 0;
	/**
	Called when a chart plot arre should be closed.
	*/
	virtual void closeChartPlotArea() = 0;
	/**
	 Called when a axis should be add in a plot area
	 */
	virtual void insertChartAxis(const RVNGPropertyList &axis) = 0;
	/**
	 Called when a serie should be opened (in a plot area)
	 */
	virtual void openChartSerie(const librevenge::RVNGPropertyList &series) = 0;
	/**
	 Called when a serie should be closed (in a plot area)
	 */
	virtual void closeChartSerie() = 0;

	virtual void defineParagraphStyle(const RVNGPropertyList &propList) = 0;

	/**
	Called when a new paragraph is opened. This (or openListElement) will always be called before any text or span is placed into the document.
	\param propList Property list for the paragraph. May contain:
	\paragraphproperties
	\li \c librevenge:tab-stops List of tabstop definitions for the paragraph. If the list is empty, default tabstop definition should be used. Each tab stop may contain:
	    -# \c style:type Type of tab (left, right, center, or char)
	    -# \c style:char Alingnment character for char aligned tabs
	    -# \c style:leader-text The leader character
	    -# \c style:position Position of the tab
	*/
	virtual void openParagraph(const RVNGPropertyList &propList) = 0;
	/**
	Called when a paragraph is closed.
	*/
	virtual void closeParagraph() = 0;

	virtual void defineCharacterStyle(const RVNGPropertyList &propList) = 0;

	/**
	Called when a text span is opened
	\param propList Property list for the span. May contain:
	\textproperties
	\li \c fo:text-shadow
	*/
	virtual void openSpan(const RVNGPropertyList &propList) = 0;
	/**
	Called when a text span is closed
	*/
	virtual void closeSpan() = 0;
	/**
	Called when a link should be opened
	\param propList Defines a set of properties for the link. May contain:
	\li \c xlink:type .
	\li \c xlink:href .
	*/
	virtual void openLink(const RVNGPropertyList &propList) = 0;
	/**
	Called when the current link is closed
	*/
	virtual void closeLink() = 0;

	virtual void defineSectionStyle(const RVNGPropertyList &propList) = 0;

	/**
	Called when a new section is opened
	\param propList Property list for the section. May contain:
	\li \c fo:margin-left The left indentation of this section, in inches
	\li \c fo:margin-right The right indentation of this section, in inches
	\li \c librevenge:margin-bottom  Extra space to add after the section, in inches
	\li \c text:dont-balance-text-columns Whether or not to balance text columns
	\li \c style:columns List of definitions of each column: left gutter, right gutter, and width (includes the gutters). Empty if there is only one column in the section. Each column may contain:
		-# \c style:rel-width
		-# \c fo:start-indent The left indentation of the margin, in inches
		-# \c fo:end-indent The right indentation of the margin, in inches
	*/
	virtual void openSection(const RVNGPropertyList &propList) = 0;
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
	virtual void insertText(const RVNGString &text) = 0;
	/**
	Called when a line break should be inserted
	*/
	virtual void insertLineBreak() = 0;

	/**
	Called when a field should be inserted.
	- \c librevenge:field-type field types may include:
	   -# \c text:page-number Current page number
	   -# \c text:page-count Total # of pages in document
	- \c style:num-format Type of page number (for page number)
	*/
	virtual void insertField(const RVNGPropertyList &propList) = 0;

	/**
	Called when a new ordered list level should be opened
	\param propList Defines a set of properties for the list level. May contain:
	\li \c librevenge:list-id A unique integer identifier for the list
	\li \c librevenge:level The level of the list in the hierarchy
	\li \c style:num-format Type of list
	\li \c style:num-prefix Text that comes before the number in the list
	\li \c style:num-suffix Text that comes after the number in the list
	\li \c text:start-value The starting number of the list
	\li \c text:min-label-width The distance between the list label and the actual text, stored in inches
	\li \c text:space-before The indentation level of the lists, stored in inches
	*/
	virtual void openOrderedListLevel(const RVNGPropertyList &propList) = 0;
	/**
	Called when a new unordered list level should be opened
	\param propList Defines a set of properties for the list level. May contain:
	\li \c librevenge:list-id A unique integer identifier for the list
	\li \c librevenge:level The level of the list in the hierarchy
	\li \c text:bullet-char The string that should be used as a bullet
	\li \c text:min-label-width The distance between the bullet and the actual text, stored in inches
	\li \c text:space-before The indentation level of the lists, stored in inches
	*/
	virtual void openUnorderedListLevel(const RVNGPropertyList &propList) = 0;
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
	\li \c librevenge:tab-stops List of tabstop definitions for the paragraph. If the list is empty, default tabstop definition should be used. Each tab stop may contain:
	    -# \c style:type Type of tab (left, right, center, or char)
	    -# \c style:char Alingnment character for char aligned tabs
	    -# \c style:leader-text The leader character
	    -# \c style:position Position of the tab
	*/
	virtual void openListElement(const RVNGPropertyList &propList) = 0;
	/**
	Called when a list element should be closed
	*/
	virtual void closeListElement() = 0;

	/**
	Called when a footnote should be opened (a sub-document will be placed inside of it)
	\param propList Defines a set of properties for the footnote. May contain:
	\li \c librevenge:number The footnote's number
	*/
	virtual void openFootnote(const RVNGPropertyList &propList) = 0;
	/**
	Called when a footnote should be closed
	*/
	virtual void closeFootnote() = 0;

	/**
	Called when a comment or annotation should be opened (a sub-document will be placed inside of it)
	\param propList Defines a set of properties for the comment or annotation.
	*/
	virtual void openComment(const RVNGPropertyList &propList) = 0;
	/**
	Called when a comment or annotation should be closed
	*/
	virtual void closeComment() = 0;

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
	virtual void openFrame(const RVNGPropertyList &propList) = 0;
	/**
	Called when the current positioned box is closed
	*/
	virtual void closeFrame() = 0;
	/**
	Called when a binary object should be inserted
	\param propList Defines a set of properties for the object. May contain:
	\li \c librevenge:mimetype The mimetype of the object
	\li \c office:binary-data The object data
	\li \c librevenge:replacement-objects A property list vector containing alternative representations of the content. Every element is a property list containing:
	    -# \c librevenge:mime-type The mimetype of the replacement object
	    -# \c office:binary-data The replacement object data
	\li \c librevenge:replacement-text A string containing a text representation of the content.
	*/
	virtual void insertBinaryObject(const RVNGPropertyList &propList) = 0;

	//
	// specific text
	//

	/**
	Called when a text box should be opened (a sub-document will be placed inside of it)
	\param propList Defines a set of properties for the text box.
	*/
	virtual void openTextBox(const RVNGPropertyList &propList) = 0;
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
	\li \c librevenge:table-columns Column definitions for the table. May contain
	    -# \c style:column-width Width of a column, in inches
	*/
	virtual void openTable(const RVNGPropertyList &propList) = 0;
	/**
	Called when a new table row is opened
	\param propList Defines a set of properties for the table row. May contain:
	\li \c style:row-height The row's height, in inches
	\li \c style:min-row-height The row's minimum height, in inches
	\li \c librevenge:is-header-row This row contains headings of columns and should repeat at every page (for tables that span several pages)
	*/
	virtual void openTableRow(const RVNGPropertyList &propList) = 0;
	/**
	Called when the current table row is closed
	*/
	virtual void closeTableRow() = 0;
	/**
	Called when a new table cell is opened
	\param propList Property list for the table cell. May contain:
	\li \c librevenge:column Number of the column in which the cell is positioned
	\li \c librevenge:row Number of the row in which the cell is positioned
	\li \c table:number-columns-spanned Number of columns that the cell spans
	\li \c table:number-rows-spanned Number of rows that the cell spans
	\li \c fo:border-left Properties of the left border of the cell
	\li \c fo:border-right Properties of the right border of the cell
	\li \c fo:border-top Properties of the top border of the cell
	\li \c fo:border-bottom Properties of the bottom border of the cell
	\li \c fo:background-color Color of the cell (encoded in hex: \#RRGGBB)
	\li \c style:vertical-align Vertical alignment of the content in the cell (top, middle, or bottom)
	*/
	virtual void openTableCell(const RVNGPropertyList &propList) = 0;
	/**
	Called when the current table cell is closed
	*/
	virtual void closeTableCell() = 0;
	/**
	Called when a covered (spanned by another cell in the table) table cell is opened
	\param propList Property list for the table cell. May contain:
	\li \c librevenge:column Number of the column in which the cell is positioned
	\li \c librevenge:row Number of the row in which the cell is positioned
	*/
	virtual void insertCoveredTableCell(const RVNGPropertyList &propList) = 0;
	/**
	Called when the current table is closed
	*/
	virtual void closeTable() = 0;

	//
	// simple Graphic
	//

	virtual void openGroup(const RVNGPropertyList &propList) = 0;
	virtual void closeGroup() = 0;

	virtual void defineGraphicStyle(const RVNGPropertyList &propList) = 0;

	// Different primitive shapes
	virtual void drawRectangle(const RVNGPropertyList &propList) = 0;

	virtual void drawEllipse(const RVNGPropertyList &propList) = 0;

	virtual void drawPolygon(const RVNGPropertyList &propList) = 0;

	virtual void drawPolyline(const RVNGPropertyList &propList) = 0;

	virtual void drawPath(const RVNGPropertyList &propList) = 0;

	/** Draw a connector.
	  The connector will be typically between two shapes, but free-standing connectors are allowed too.
	  \param propList Defines a set of properties for the object. May contain:
	  \li \c draw:end-shape The \c ID of the shape attached at the end. Should not be present if the connector's end is not attached to any object.
	  \li \c draw:start-shape The \c ID of the shape attached at the beginning. Should not be present if the connector's start is not attached to any object.
	  \li \c svg:d The connector's geometric path. This is optional; if missing, the consumer is free to render it any way it likes.
	  \li \c svg:x1 The \c X coordinate of the start point.
	  \li \c svg:y1 The \c Y coordinate of the start point.
	  \li \c svg:x2 The \c X coordinate of the end point.
	  \li \c svg:y2 The \c Y coordinate of the end point.
	  */
	virtual void drawConnector(const RVNGPropertyList &propList) = 0;

	//
	// Equation
	//

	/**
	Called when a mathml object should be inserted
	\param propList Defines a set of properties for the object.
	\li \c librevenge:mimetype The mimetype of the object
	\li \c librevenge:data reference to the string containing the equation representation
	*/
	virtual void insertEquation(const RVNGPropertyList &propList) = 0;
};
}

#endif /* DI_SPREADSHEET_INTERFACE_HXX */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
