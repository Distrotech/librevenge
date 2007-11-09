/* libwpd
 * Copyright (C) 2002-2004 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2004 Fridrich Strba (fridrich.strba@bluewin.ch)
 * Copyright (C) 2005 Net Integration Technologies (http://www.net-itech.com)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#include <stdio.h>
#include "HtmlDocumentGenerator.h"

// use the BELL code to represent a TAB for now
#define UCS_TAB 0x0009

HtmlDocumentGenerator::HtmlDocumentGenerator() :
	m_ignore(false)
{
}

HtmlDocumentGenerator::~HtmlDocumentGenerator()
{
}

void HtmlDocumentGenerator::setDocumentMetaData(const WPXPropertyList &propList)
{
	if (propList["author"])
		printf("<meta name=\"author\" content=\"%s\">\n", propList["author"]->getStr().cstr());
	if (propList["subject"])
		printf("<meta name=\"subject\" content=\"%s\">\n", propList["subject"]->getStr().cstr());
	if (propList["publisher"])
		printf("<meta name=\"publisher\" content=\"%s\">\n", propList["publisher"]->getStr().cstr());
	if (propList["keywords"])
		printf("<meta name=\"keywords\" content=\"%s\">\n", propList["keywords"]->getStr().cstr());
	if (propList["language"])
		printf("<meta name=\"language\" content=\"%s\">\n", propList["language"]->getStr().cstr());
	if (propList["abstract"])
		printf("<meta name=\"abstract\" content=\"%s\">\n", propList["abstract"]->getStr().cstr());
	if (propList["descriptive-name"])
		printf("<meta name=\"descriptive-name\" content=\"%s\">\n", propList["descriptive-name"]->getStr().cstr());
	if (propList["descriptive-type"])
		printf("<meta name=\"descriptive-type\" content=\"%s\">\n", propList["descriptive-type"]->getStr().cstr());
}

void HtmlDocumentGenerator::startDocument()
{
	printf("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\" \"http://www.w3.org/TR/REC-html40/loose.dtd\">\n");
	printf("<html>\n");
	printf("<head>\n");
	printf("<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\" >\n");
	printf("</head>\n");
	printf("<body>\n");
}

void HtmlDocumentGenerator::endDocument()
{
	printf("\n");
	printf("</body>\n");
	printf("</html>\n");
}

void HtmlDocumentGenerator::openHeader(const WPXPropertyList & /* propList */)
{
	m_ignore = true;
}

void HtmlDocumentGenerator::closeHeader()
{
	m_ignore = false;
}


void HtmlDocumentGenerator::openFooter(const WPXPropertyList & /* propList */)
{
	m_ignore = true;
}

void HtmlDocumentGenerator::closeFooter()
{
	m_ignore = false;
}

void HtmlDocumentGenerator::openParagraph(const WPXPropertyList &propList, const WPXPropertyListVector & /* tabStops */)
{
	if (!m_ignore)
	{
		printf("<p style=\"");

		if (propList["fo:text-align"])
		{

			if (propList["fo:text-align"]->getStr() == WPXString("end")) // stupid OOo convention..
				printf("text-align:right;");
			else
				printf("text-align:%s;", propList["fo:text-align"]->getStr().cstr());
		}
		if (propList["fo:text-indent"])
			printf("text-indent:%s;", propList["fo:text-indent"]->getStr().cstr());

		if (propList["fo:line-height"] && propList["fo:line-height"]->getFloat() != 1.0f)
			printf("line-height:%f;", propList["fo:line-height"]->getFloat());
		printf("\">");
	}
}

void HtmlDocumentGenerator::closeParagraph()
{
	if (!m_ignore)
	{
		printf("</p>\n");
	}
}

void HtmlDocumentGenerator::openSpan(const WPXPropertyList &propList)
{
	if (!m_ignore)
	{

		printf("<span style=\"");
		if (propList["style:font-name"])
			printf("font-family: \'%s\';", propList["style:font-name"]->getStr().cstr());
		if (propList["fo:font-size"])
			printf("font-size: %s;", propList["fo:font-size"]->getStr().cstr());
		if (propList["fo:font-weight"])
			printf("font-weight: %s;", propList["fo:font-weight"]->getStr().cstr());
		if (propList["fo:font-style"])
			printf("font-style: %s;", propList["fo:font-style"]->getStr().cstr());
		if (propList["style:text-crossing-out"] && propList["style:text-crossing-out"]->getStr() == WPXString("single-line"))
			printf("text-decoration:line-through;");
		if (propList["style:text-underline"]) // don't know if double underline is possible
			printf("text-decoration:underline;");
		if (propList["style:text-blinking"]) 
			printf("text-decoration:blink;");
		if (propList["fo:color"])
			printf("color:%s;", propList["fo:color"]->getStr().cstr());
		if (propList["style:text-background-color"])
			printf("background-color:%s;", propList["style:text-background-color"]->getStr().cstr());

		printf("\">");
	}
}

void HtmlDocumentGenerator::closeSpan()
{
	if (!m_ignore)
	{
		printf("</span>\n");
	}
}

void HtmlDocumentGenerator::insertTab()
{
	if (!m_ignore)
	{

		printf("%c", UCS_TAB);
	}
}

void HtmlDocumentGenerator::insertLineBreak()
{
	if (!m_ignore)
	{

		printf("<br>\n");
	}
}

void HtmlDocumentGenerator::insertText(const WPXString &text)
{
	if (!m_ignore)
	{

		WPXString tempUTF8(text, true);
		printf("%s", tempUTF8.cstr());
	}
}

void HtmlDocumentGenerator::openOrderedListLevel(const WPXPropertyList & /* propList */)
{
	if (!m_ignore)
	{
		printf("<ol>\n");
	}
}

void HtmlDocumentGenerator::closeOrderedListLevel()
{
	if (!m_ignore)
	{
		printf("</ol>\n");
	}
}

void HtmlDocumentGenerator::openUnorderedListLevel(const WPXPropertyList & /* propList */)
{
	if (!m_ignore)
	{
		printf("<ul>\n");
	}
}

void HtmlDocumentGenerator::closeUnorderedListLevel()
{
	if (!m_ignore)
	{
		printf("</ul>\n");
	}
}


void HtmlDocumentGenerator::openListElement(const WPXPropertyList & /* propList */, const WPXPropertyListVector &/* tabStops */)
{
	if (!m_ignore)
	{
		printf("<li>");
	}
}

void HtmlDocumentGenerator::closeListElement()
{
	if (!m_ignore)
	{
		printf("</li>\n");
	}
}

void HtmlDocumentGenerator::openFootnote(const WPXPropertyList &propList)
{
	if (!m_ignore)
	{
		// Cheesey hack.. 
		if (propList["libwpd:number"])
			printf("<p>%s:</p>", propList["libwpd:number"]->getStr().cstr());
		else
			printf("<p/>");
	}
}

void HtmlDocumentGenerator::closeFootnote()
{
	if (!m_ignore)
	{
		printf("<p/>\n");
	}
}

void HtmlDocumentGenerator::openEndnote(const WPXPropertyList &propList)
{
	if (!m_ignore)
	{
		// Cheesey hack.. 
		if (propList["libwpd:number"])
			printf("<p>%s:</p>", propList["libwpd:number"]->getStr().cstr());
		else
			printf("<p/>");
	}
}

void HtmlDocumentGenerator::closeEndnote()
{
	if (!m_ignore)
	{
		printf("<p/>\n");
	}
}

void HtmlDocumentGenerator::openComment(const WPXPropertyList & /*propList*/)
{
	if (!m_ignore)
	{
		printf("<p/>");
	}
}

void HtmlDocumentGenerator::closeComment()
{
	if (!m_ignore)
	{
		printf("<p/>\n");
	}
}

void HtmlDocumentGenerator::openTextBox(const WPXPropertyList & /*propList*/)
{
	if (!m_ignore)
	{
		printf("<p/>");
	}
}

void HtmlDocumentGenerator::closeTextBox()
{
	if (!m_ignore)
	{
		printf("<p/>\n");
	}
}

void HtmlDocumentGenerator::openTable(const WPXPropertyList & /* propList */, const WPXPropertyListVector & /* columns */)
{
	if (!m_ignore)
	{
		printf("<table border=\"1\">\n");
		printf("<tbody>\n");
	}
}

void HtmlDocumentGenerator::openTableRow(const WPXPropertyList & /* propList */)
{
	if (!m_ignore)
	{
		printf("<tr>\n");
	}
}

void HtmlDocumentGenerator::closeTableRow()
{
	if (!m_ignore)
	{
		printf("</tr>\n");
	}
}

void HtmlDocumentGenerator::openTableCell(const WPXPropertyList &propList)
{
	if (!m_ignore)
	{
		printf("<td style=\"");
		if (propList["fo:background-color"])
			printf("background-color:%s;", propList["fo:background-color"]->getStr().cstr());

		printf("\" ");
	
		if (propList["table:number-columns-spanned"])
			printf("colspan=\"%d\" ", propList["table:number-columns-spanned"]->getInt());
		if (propList["table:number-rows-spanned"])
			printf("rowspan=\"%d\" ", propList["table:number-rows-spanned"]->getInt());

		printf(">\n");
	}
}

void HtmlDocumentGenerator::closeTableCell()
{
	if (!m_ignore)
	{
		printf("</td>\n");
	}
}

void HtmlDocumentGenerator::closeTable()
{
	if (!m_ignore)
	{
		printf("</tbody>\n");
		printf("</table>\n");
	}
}
