 /* libwpd2
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002 Marc Maurer (j.m.maurer@student.utwente.nl)
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

#include <stdio.h>
#include "HtmlListenerImpl.h"

// use the BELL code to represent a TAB for now
#define UCS_TAB 0x0009 

HtmlListenerImpl::HtmlListenerImpl()
{
}

HtmlListenerImpl::~HtmlListenerImpl()
{
}

void HtmlListenerImpl::startHeader()
{
    printf("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\" \"http://www.w3.org/TR/REC-html40/loose.dtd\">\n");
    printf("<html>\n");
    printf("<head>\n");
    printf("<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\" >\n");
}

void HtmlListenerImpl::endHeader()
{
    printf("</head>\n");
}

void HtmlListenerImpl::setDocumentMetaData(const UCSString &author, const UCSString &subject,
 					 const UCSString &publisher, const UCSString &category,
 					 const UCSString &keywords, const UCSString &language,
 					 const UCSString &abstract, const UCSString &descriptiveName,
					 const UCSString &descriptiveType)
{
	if (author.getLen() > 0)
		printf("<meta name=\"author\" content=\"%s\">\n", UTF8String(author).getUTF8());
	if (subject.getLen() > 0)
		printf("<meta name=\"subject\" content=\"%s\">\n", UTF8String(subject).getUTF8());
	if (publisher.getLen() > 0)
		printf("<meta name=\"publisher\" content=\"%s\">\n", UTF8String(publisher).getUTF8());
	if (keywords.getLen() > 0)
		printf("<meta name=\"keywords\" content=\"%s\">\n", UTF8String(keywords).getUTF8());
	if (language.getLen() > 0)
		printf("<meta name=\"language\" content=\"%s\">\n", UTF8String(language).getUTF8());
	if (abstract.getLen() > 0)
		printf("<meta name=\"abstract\" content=\"%s\">\n", UTF8String(abstract).getUTF8());
	if (descriptiveName.getLen() > 0)
		printf("<meta name=\"descriptive-name\" content=\"%s\">\n", UTF8String(descriptiveName).getUTF8());
	if (descriptiveType.getLen() > 0)
		printf("<meta name=\"descriptive-type\" content=\"%s\">\n", UTF8String(descriptiveType).getUTF8());
}

void HtmlListenerImpl::startDocument()
{
    printf("<body>\n");
}

void HtmlListenerImpl::endDocument()
{
	printf("\n");
	printf("</body>\n");
	printf("</html>\n");
}

void HtmlListenerImpl::openParagraph(const guint8 paragraphJustification, const guint32 textAttributeBits, 
				     const gchar *fontName, const gfloat fontSize,
				     const float lineSpacing,
				     const gboolean isColumnBreak, const gboolean isPageBreak)
{
	printf("<p style=\"");
	_appendTextAttributes(textAttributeBits);
	_appendParagraphJustification(paragraphJustification);
	printf("\" font-name=\"%s\" font-size=\"%f\">", fontName, fontSize);
}

void HtmlListenerImpl::closeParagraph()
{
	printf("</p>\n");
}

void HtmlListenerImpl::openSpan(guint32 textAttributeBits, const gchar *fontName, const gfloat fontSize)
{
	printf("<span style=\""); 
	_appendTextAttributes(textAttributeBits);
	printf("\" font-name=\"%s\" font-size=\"%f\">", fontName, fontSize);
}

void HtmlListenerImpl::closeSpan()
{
	printf("</span>");
}

void HtmlListenerImpl::openSection(guint numColumns, gfloat marginLeft, gfloat marginRight)
{
	printf("<section columns:%i margin-left:%4.4fin margin-right:%4.4fin>\n", numColumns, marginLeft, marginRight);
}

void HtmlListenerImpl::closeSection()
{
	printf("\n</section>\n");
}

void HtmlListenerImpl::insertTab()
{
	printf("%c", UCS_TAB);
}

void HtmlListenerImpl::insertText(const UCSString &text)
{
	UTF8String tempUTF8(text);
	printf("%s", tempUTF8.getUTF8());
}

void HtmlListenerImpl::openOrderedListLevel(const gint listID)
{
	printf("<ol>\n");
}

void HtmlListenerImpl::closeOrderedListLevel()
{
	printf("</ol>\n");
}

void HtmlListenerImpl::openUnorderedListLevel(const gint listID)
{
	printf("<ul>\n");
}

void HtmlListenerImpl::closeUnorderedListLevel()
{
	printf("</ul>\n");
}


void HtmlListenerImpl::openListElement(const guint8 paragraphJustification, const guint32 textAttributeBits,
				       const gchar *fontName, const gfloat fontSize, 
				       const float lineSpacing)
{
	printf("<li style=\"");
	_appendTextAttributes(textAttributeBits);
	_appendParagraphJustification(paragraphJustification);
	printf("\" font-name=\"%s\" font-size=\"%f\">", fontName, fontSize);
}

void HtmlListenerImpl::closeListElement()
{
	printf("</li>\n");
}

#if 0
void HtmlListenerImpl::openFootnote()
{
	printf("<footnote>\n");
}

void HtmlListenerImpl::closeFootnote()
{
	printf("</footnote>\n");
}

void HtmlListenerImpl::openEndnote()
{
	printf("<endnote>\n");
}

void HtmlListenerImpl::closeEndnote()
{
	printf("</endnote>\n");
}
#endif

void HtmlListenerImpl::openTable()
{
	printf("<table border=\"1\">\n");
	printf("<tbody>\n");
}

void HtmlListenerImpl::openTableRow()
{
	printf("<tr>\n");
}

void HtmlListenerImpl::closeTableRow()
{
	printf("</tr>\n");
}

void HtmlListenerImpl::openTableCell(const guint32 col, const guint32 row, const guint32 colSpan, const guint32 rowSpan, 
				const RGBSColor * cellFgColor, const RGBSColor * cellBgColor)
{
	printf("<td ");
	
	if (cellFgColor || cellBgColor)
	{
		printf("style=\"");
		// NOTE: The WordPerfect Cell Foreground Color maps onto the HTML Background Color
		if (cellFgColor)
			printf("background-color: rgb(%d, %d, %d);", cellFgColor->m_r, cellFgColor->m_g, cellFgColor->m_b);
		// TODO: Handle the Cell Background Color here, but I don't think HTML supports that
		// ...
		printf("\"");
	}
	printf(" rowspan=\"%ld\" colspan=\"%ld\"", rowSpan, colSpan);

	printf(">\n");
}

void HtmlListenerImpl::closeTableCell()
{
	printf("</td>\n");
}

void HtmlListenerImpl::closeTable()
{	
	printf("</tbody>\n");
	printf("</table>\n");
}

void HtmlListenerImpl::_appendTextAttributes(const guint32 textAttributeBits)
{
	if (textAttributeBits & WPX_BOLD_BIT)
		printf("font-weight: bold;");
	if (textAttributeBits & WPX_ITALICS_BIT)
		printf("font-style: italic;");
}

void HtmlListenerImpl::_appendParagraphJustification(const guint32 paragraphJustification)
{
	switch (paragraphJustification)
	{
		case WPX_PARAGRAPH_JUSTIFICATION_LEFT:
			printf("text-align: left;");
			break;
		case WPX_PARAGRAPH_JUSTIFICATION_RIGHT:
			printf("text-align: right;");
			break;
		case WPX_PARAGRAPH_JUSTIFICATION_CENTER:
			printf("text-align: center;");
			break;
		case WPX_PARAGRAPH_JUSTIFICATION_FULL:
		case WPX_PARAGRAPH_JUSTIFICATION_FULL_ALL_LINES:
			printf("text-align: jusitify;");
			break;
	}
}
