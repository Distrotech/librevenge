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
#include <iostream>
#include "HtmlDocumentGenerator.h"

// use the BELL code to represent a TAB for now
#define UCS_TAB 0x0009

HtmlDocumentGenerator::HtmlDocumentGenerator() :
	m_ignore(false),
	m_pOutputStream(&std::cout),
	m_footNotesCount(0),
	m_endNotesCount(0),
	m_commentsCount(0),
	m_textBoxesCount(0),
	m_commentNumber(1),
	m_textBoxNumber(1)
{
}

HtmlDocumentGenerator::~HtmlDocumentGenerator()
{
}

void HtmlDocumentGenerator::setDocumentMetaData(const WPXPropertyList &propList)
{
	if (propList["author"])
		*m_pOutputStream << "<meta name=\"author\" content=\"" << propList["author"]->getStr().cstr() << "\">" << std::endl;
	if (propList["subject"])
		*m_pOutputStream << "<meta name=\"subject\" content=\"" << propList["subject"]->getStr().cstr() << "\">" << std::endl;
	if (propList["publisher"])
		*m_pOutputStream << "<meta name=\"publisher\" content=\"" << propList["publisher"]->getStr().cstr() << "\">" << std::endl;
	if (propList["keywords"])
		*m_pOutputStream << "<meta name=\"keywords\" content=\"" << propList["keywords"]->getStr().cstr() << "\">" << std::endl;
	if (propList["language"])
		*m_pOutputStream << "<meta name=\"language\" content=\"" << propList["language"]->getStr().cstr() << "\">" << std::endl;
	if (propList["abstract"])
		*m_pOutputStream << "<meta name=\"abstract\" content=\"" << propList["abstract"]->getStr().cstr() << "\">" << std::endl;
	if (propList["descriptive-name"])
		*m_pOutputStream << "<meta name=\"descriptive-name\" content=\"" << propList["descriptive-name"]->getStr().cstr() << "\">" << std::endl;
	if (propList["descriptive-type"])
		*m_pOutputStream << "<meta name=\"descriptive-type\" content=\"" << propList["descriptive-type"]->getStr().cstr() << "\">" << std::endl;
}

void HtmlDocumentGenerator::startDocument()
{
	*m_pOutputStream << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\" \"http://www.w3.org/TR/REC-html40/loose.dtd\">" << std::endl;
	*m_pOutputStream << "<html>" << std::endl;
	*m_pOutputStream << "<head>" << std::endl;
	*m_pOutputStream << "<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\" >" << std::endl;
	*m_pOutputStream << "</head>" << std::endl;
	*m_pOutputStream << "<body>" << std::endl;
}

void HtmlDocumentGenerator::endDocument()
{
	if (m_footNotesStream.str().length())
	{
		*m_pOutputStream << "<p><b>FOOTNOTES</b></p>" << std::endl;
		*m_pOutputStream << m_footNotesStream.str() << std::endl;
	}
	if (m_endNotesStream.str().length())
	{
		*m_pOutputStream << "<p><b>ENDNOTES</b></p>" << std::endl;
		*m_pOutputStream << m_endNotesStream.str() << std::endl;
	}
	if (m_commentsStream.str().length())
	{
		*m_pOutputStream << "<p><b>COMMENTS AND ANNOTATIONS</b></p>" << std::endl;
		*m_pOutputStream << m_commentsStream.str() << std::endl;
	}
	if (m_textBoxesStream.str().length())
	{
		*m_pOutputStream << "<p><b>TEXT BOXES</b></p>" << std::endl;
		*m_pOutputStream << m_textBoxesStream.str() << std::endl;
	}
	*m_pOutputStream << "</body>" << std::endl;
	*m_pOutputStream << "</html>" << std::endl;
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
		*m_pOutputStream << "<p style=\"";

		if (propList["fo:text-align"])
		{

			if (propList["fo:text-align"]->getStr() == WPXString("end")) // stupid OOo convention..
				*m_pOutputStream << "text-align:right;";
			else
				*m_pOutputStream << "text-align:" << propList["fo:text-align"]->getStr().cstr() << ";";
		}
		if (propList["fo:text-indent"])
			*m_pOutputStream << "text-indent:" << propList["fo:text-indent"]->getStr().cstr() << ";";

		if (propList["fo:line-height"] && propList["fo:line-height"]->getDouble() != 1.0)
			*m_pOutputStream << "line-height:" << propList["fo:line-height"]->getDouble() << ";";
		*m_pOutputStream << "\">";
	}
}

void HtmlDocumentGenerator::closeParagraph()
{
	if (!m_ignore)
	{
		*m_pOutputStream << "</p>" << std::endl;
	}
}

void HtmlDocumentGenerator::openSpan(const WPXPropertyList &propList)
{
	if (!m_ignore)
	{

		*m_pOutputStream << "<span style=\"";
		if (propList["style:font-name"])
			*m_pOutputStream << "font-family: \'" << propList["style:font-name"]->getStr().cstr() << "\';";
		if (propList["fo:font-size"])
			*m_pOutputStream << "font-size: " << propList["fo:font-size"]->getStr().cstr() << ";";
		if (propList["fo:font-weight"])
			*m_pOutputStream << "font-weight: " << propList["fo:font-weight"]->getStr().cstr() << ";";
		if (propList["fo:font-style"])
			*m_pOutputStream << "font-style: " << propList["fo:font-style"]->getStr().cstr() << ";";
		if (propList["style:text-crossing-out"] && propList["style:text-crossing-out"]->getStr() == WPXString("single-line"))
			*m_pOutputStream << "text-decoration:line-through;";
		if (propList["style:text-underline"]) // don't know if double underline is possible
			*m_pOutputStream << "text-decoration:underline;";
		if (propList["style:text-blinking"]) 
			*m_pOutputStream << "text-decoration:blink;";
		if (propList["fo:color"])
			*m_pOutputStream << "color:" << propList["fo:color"]->getStr().cstr() << ";";
		if (propList["style:text-background-color"])
			*m_pOutputStream << "background-color:" << propList["style:text-background-color"]->getStr().cstr() << ";";

		*m_pOutputStream << "\">";
	}
}

void HtmlDocumentGenerator::closeSpan()
{
	if (!m_ignore)
	{
		*m_pOutputStream << "</span>" << std::endl;
	}
}

void HtmlDocumentGenerator::insertTab()
{
	if (!m_ignore)
	{
		// Does not have a lot of effect since tabs in html are ignorable white-space
		*m_pOutputStream << "\t";
	}
}

void HtmlDocumentGenerator::insertLineBreak()
{
	if (!m_ignore)
	{

		*m_pOutputStream << "<br>" << std::endl;
	}
}

void HtmlDocumentGenerator::insertText(const WPXString &text)
{
	if (!m_ignore)
	{

		WPXString tempUTF8(text, true);
		*m_pOutputStream << tempUTF8.cstr();
	}
}

void HtmlDocumentGenerator::insertSpace()
{
	if (!m_ignore)
	{
            *m_pOutputStream << "&nbsp;";
        }
}

void HtmlDocumentGenerator::openOrderedListLevel(const WPXPropertyList & /* propList */)
{
	if (!m_ignore)
	{
		*m_pOutputStream << "<ol>" << std::endl;
	}
}

void HtmlDocumentGenerator::closeOrderedListLevel()
{
	if (!m_ignore)
	{
		*m_pOutputStream << "</ol>" << std::endl;
	}
}

void HtmlDocumentGenerator::openUnorderedListLevel(const WPXPropertyList & /* propList */)
{
	if (!m_ignore)
	{
		*m_pOutputStream << "<ul>" << std::endl;
	}
}

void HtmlDocumentGenerator::closeUnorderedListLevel()
{
	if (!m_ignore)
	{
		*m_pOutputStream << "</ul>" << std::endl;
	}
}


void HtmlDocumentGenerator::openListElement(const WPXPropertyList & /* propList */, const WPXPropertyListVector &/* tabStops */)
{
	if (!m_ignore)
	{
		*m_pOutputStream << "<li>";
	}
}

void HtmlDocumentGenerator::closeListElement()
{
	if (!m_ignore)
	{
		*m_pOutputStream << "</li>" << std::endl;
	}
}

void HtmlDocumentGenerator::openFootnote(const WPXPropertyList &propList)
{
	if (!m_ignore)
	{
		if (!m_footNotesCount++)
		{
			if (propList["libwpd:number"])
				*m_pOutputStream << "<sup>(footnote: " << propList["libwpd:number"]->getStr().cstr() << ")</sup>";
			m_pOutputStream = &m_footNotesStream;
			// Cheesey hack.. 
			if (propList["libwpd:number"])
				*m_pOutputStream << "<p>" << propList["libwpd:number"]->getStr().cstr() << ":</p>";
			else
				*m_pOutputStream << "<p/>";
		}
		else
			m_pOutputStream = &m_dummyStream;
	}
}

void HtmlDocumentGenerator::closeFootnote()
{
	if (!m_ignore)
	{
		if (!(--m_footNotesCount))
		{
			*m_pOutputStream << "<p/>" << std::endl;
			m_pOutputStream = &std::cout;
		}
	}
}

void HtmlDocumentGenerator::openEndnote(const WPXPropertyList &propList)
{
	if (!m_ignore)
	{
		if (!m_endNotesCount++)
		{
			if (propList["libwpd:number"])
				*m_pOutputStream << "<sup>(endnote: " << propList["libwpd:number"]->getStr().cstr() << ")</sup>";
			 m_pOutputStream = &m_footNotesStream;
			// Cheesey hack.. 
			if (propList["libwpd:number"])
				*m_pOutputStream << "<p>" << propList["libwpd:number"]->getStr().cstr() << ":</p>";
			else
				*m_pOutputStream << "<p/>";
		}
		else
			m_pOutputStream = &m_dummyStream;
	}
}

void HtmlDocumentGenerator::closeEndnote()
{
	if (!m_ignore)
	{
		if (!(--m_endNotesCount))
		{
			*m_pOutputStream << "<p/>" << std::endl;
			m_pOutputStream = &std::cout;
		}
	}
}

void HtmlDocumentGenerator::openComment(const WPXPropertyList & /*propList*/)
{
	if (!m_ignore)
	{
		if (!m_commentsCount++)
		{
			*m_pOutputStream << "<sup>(comment: " << m_commentNumber << ")</sup>";
			m_pOutputStream = &m_commentsStream;
			*m_pOutputStream << "<p>Comment " << m_commentNumber++ << ":</p>" << std::endl;
			*m_pOutputStream << "<p/>";
		}
		else
			m_pOutputStream = &m_dummyStream;
	}
}

void HtmlDocumentGenerator::closeComment()
{
	if (!m_ignore)
	{
		if (!(--m_commentsCount))
		{
			*m_pOutputStream << "<p/>" << std::endl;
			m_pOutputStream = &std::cout;
		}
	}
}

void HtmlDocumentGenerator::openTextBox(const WPXPropertyList & /*propList*/)
{
	if (!m_ignore)
	{
		if (!m_textBoxesCount)
		{
			*m_pOutputStream << "<sup>(text box: " << m_textBoxNumber << ")</sup>";
			m_pOutputStream = &m_commentsStream;
			*m_pOutputStream << "<p>Text Box " << m_textBoxNumber++ << ":</p>" << std::endl;
			m_pOutputStream = &m_textBoxesStream;
			*m_pOutputStream << "<p/>";
		}
		else
			m_pOutputStream = &m_dummyStream;
	}
}

void HtmlDocumentGenerator::closeTextBox()
{
	if (!m_ignore)
	{
		if (!(--m_textBoxesCount))
		{
			*m_pOutputStream << "<p/>" << std::endl;
			m_pOutputStream = &std::cout;
		}
	}
}

void HtmlDocumentGenerator::openTable(const WPXPropertyList & /* propList */, const WPXPropertyListVector & /* columns */)
{
	if (!m_ignore)
	{
		*m_pOutputStream << "<table border=\"1\">" << std::endl;
		*m_pOutputStream << "<tbody>" << std::endl;
	}
}

void HtmlDocumentGenerator::openTableRow(const WPXPropertyList & /* propList */)
{
	if (!m_ignore)
	{
		*m_pOutputStream << "<tr>" << std::endl;
	}
}

void HtmlDocumentGenerator::closeTableRow()
{
	if (!m_ignore)
	{
		*m_pOutputStream << "</tr>" << std::endl;
	}
}

void HtmlDocumentGenerator::openTableCell(const WPXPropertyList &propList)
{
	if (!m_ignore)
	{
		*m_pOutputStream << "<td style=\"";
		if (propList["fo:background-color"])
			*m_pOutputStream << "background-color:" << propList["fo:background-color"]->getStr().cstr() << ";";

		*m_pOutputStream << "\" ";
	
		if (propList["table:number-columns-spanned"])
			*m_pOutputStream << "colspan=\"" << propList["table:number-columns-spanned"]->getInt() << "\" ";
		if (propList["table:number-rows-spanned"])
			*m_pOutputStream << "rowspan=\"" << propList["table:number-rows-spanned"]->getInt() << "\" ";

		*m_pOutputStream << ">" << std::endl;
	}
}

void HtmlDocumentGenerator::closeTableCell()
{
	if (!m_ignore)
	{
		*m_pOutputStream << "</td>" << std::endl;
	}
}

void HtmlDocumentGenerator::closeTable()
{
	if (!m_ignore)
	{
		*m_pOutputStream << "</tbody>" << std::endl;
		*m_pOutputStream << "</table>" << std::endl;
	}
}
