 /* libwpd2
 * Copyright (C) 2002 William Lachance (wlach@interlog.com)
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

void HtmlListenerImpl::openParagraph(guint8 paragraphJustification, guint32 textAttributeBits)
{
	_closeCurrentParagraph();

	printf("<p style=\"");
	_appendTextAttributes(textAttributeBits);
	_appendParagraphJustification(paragraphJustification);
	printf("\">");

	m_isParagraphOpened = TRUE;
}

void HtmlListenerImpl::openSpan(guint32 textAttributeBits)
{
	_closeCurrentSpan();

	printf("<span style=\""); 
	_appendTextAttributes(textAttributeBits);
	printf("\">");
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

void HtmlListenerImpl::insertText(const guint16 *textArray, const guint len)
{
	for (guint i=0; i<len; i++) {
		printf("%c", textArray[i]);
	}
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

void HtmlListenerImpl::openCell(guint32 col, guint32 row, guint32 colSpan, guint32 rowSpan)
{
	_closeCurrentParagraph();	
	_closeCurrentCell();

	printf("<td rowspan=\"%ld\" colspan=\"%ld\">\n", rowSpan, colSpan);

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
