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

HtmlListenerImpl::HtmlListenerImpl() :
	m_isSectionOpened(FALSE),
	m_isParagraphOpened(FALSE),
	m_isSpanOpened(FALSE),
 	m_isRowOpened(FALSE),
 	m_isCellOpened(FALSE)
{
}

HtmlListenerImpl::~HtmlListenerImpl()
{
}


void HtmlListenerImpl::startDocument()
{
    printf("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\" \"http://www.w3.org/TR/REC-html40/loose.dtd\">\n");
    printf("<html>\n");
    printf("<head>\n");
    printf("<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\" >\n");
    printf("</head>\n");
    printf("<body>\n");
}

void HtmlListenerImpl::endDocument()
{
	_closeCurrentParagraph();

	if (m_isSectionOpened)
		printf("</section>\n");

	printf("\n");
	printf("</body>\n");
	printf("</html>\n");
}

void HtmlListenerImpl::openParagraph(const guint8 paragraphJustification, const guint32 textAttributeBits, 
				     const gchar *fontName, const gfloat fontSize,
				     const gboolean isColumnBreak, const gboolean isPageBreak)
{
	_closeCurrentParagraph();

	printf("<p style=\"");
	_appendTextAttributes(textAttributeBits);
	_appendParagraphJustification(paragraphJustification);
	printf("\" font-name=\"%s\" font-size=\"%f\">", fontName, fontSize);

	m_isParagraphOpened = TRUE;
}

void HtmlListenerImpl::openSpan(guint32 textAttributeBits, const gchar *fontName, const gfloat fontSize)
{
	_closeCurrentSpan();

	printf("<span style=\""); 
	_appendTextAttributes(textAttributeBits);
	printf("\" font-name=\"%s\" font-size=\"%f\">", fontName, fontSize);
	m_isSpanOpened = TRUE;
}

void HtmlListenerImpl::openSection(guint numColumns, gfloat marginLeft, gfloat marginRight)
{
	_closeCurrentParagraph();

	if (m_isSectionOpened)
		printf("\n</section>\n");

	m_isSectionOpened = TRUE;

	printf("<section columns:%i margin-left:%4.4fin margin-right:%4.4fin>\n", numColumns, marginLeft, marginRight);
}

void HtmlListenerImpl::insertText(const UCSString &text)
{
	UTF8String tempUTF8(text);
	printf("%s", tempUTF8.getUTF8());
}

void HtmlListenerImpl::openOrderedListLevel(const gint listID)
{
	_closeCurrentParagraph();

	printf("<ol>\n");
}

void HtmlListenerImpl::closeOrderedListLevel()
{
	printf("</ol>\n");
}

void HtmlListenerImpl::openUnorderedListLevel(const gint listID)
{
	_closeCurrentParagraph();

	printf("<ul>\n");
}

void HtmlListenerImpl::closeUnorderedListLevel()
{
	printf("</ul>\n");
}


void HtmlListenerImpl::openListElement()
{
	printf("<li>");
}

void HtmlListenerImpl::closeListElement()
{
	printf("</li>\n");
}

void HtmlListenerImpl::openTable()
{
	_closeCurrentParagraph();

	printf("<table border=\"1\">\n");
	printf("<tbody>\n");
}

void HtmlListenerImpl::openRow()
{
	_closeCurrentParagraph();
	_closeCurrentCell();
	_closeCurrentRow();

	printf("<tr>\n");

	m_isRowOpened = TRUE;
}

void HtmlListenerImpl::openCell(const guint32 col, const guint32 row, const guint32 colSpan, const guint32 rowSpan, 
				const RGBSColor * cellFgColor, const RGBSColor * cellBgColor)
{
	_closeCurrentParagraph();	
	_closeCurrentCell();
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
	m_isCellOpened = TRUE;
}

void HtmlListenerImpl::closeTable()
{
	_closeCurrentParagraph();
	_closeCurrentCell();
	_closeCurrentRow();
	
	printf("</tbody>\n");
	printf("</table>\n");
}

void HtmlListenerImpl::_appendTextAttributes(guint32 textAttributeBits)
{
	if (textAttributeBits & WPX_BOLD_BIT)
		printf("font-weight: bold;");
	if (textAttributeBits & WPX_ITALICS_BIT)
		printf("font-style: italic;");
}

void HtmlListenerImpl::_appendParagraphJustification(guint32 paragraphJustification)
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

void HtmlListenerImpl::_closeCurrentSpan()
{
	if (m_isSpanOpened)
		printf("</span>");

	m_isSpanOpened = FALSE;
}

void HtmlListenerImpl::_closeCurrentParagraph()
{
	_closeCurrentSpan();
	if (m_isParagraphOpened)
		printf("</p>");

	m_isParagraphOpened = FALSE;
}

void HtmlListenerImpl::_closeCurrentCell()
{
	if (m_isCellOpened)
		printf("</td>\n");

	m_isCellOpened = FALSE;
}

void HtmlListenerImpl::_closeCurrentRow()
{
	if (m_isRowOpened)
		printf("</tr>\n");

	m_isRowOpened = FALSE;
}
