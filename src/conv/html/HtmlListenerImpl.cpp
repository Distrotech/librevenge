/* libwpd
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002 Marc Maurer (j.m.maurer@student.utwente.nl)
 * Copyright (C) 2004 Fridrich Strba (fridrich.strba@bluewin.ch)
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

HtmlListenerImpl::HtmlListenerImpl():
	m_isSuperscript(false),
	m_isSubscript(false),
	m_isBold(false),
	m_isItalic(false),
	m_isStrikeout(false),
	m_isUnderline(false),
	m_isDoubleUnderline(false),
	m_isOutline(false),
	m_isSmallCaps(false),
	m_isBlink(false),
	m_isShadow(false),
	m_isHighlightColor(false)
{
}

HtmlListenerImpl::~HtmlListenerImpl()
{
}

void HtmlListenerImpl::setDocumentMetaData(const WPXPropertyList &propList)
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
	printf("\n");
	printf("</body>\n");
	printf("</html>\n");
}

void HtmlListenerImpl::openPageSpan(const WPXPropertyList &propList)
{
	printf("<page-span span:%i margin-left:%fin margin-right:%fin margin-top:%fin margin-bottom:%fin>\n", propList["span"]->getInt(),
	       propList["margin-left"]->getFloat(), propList["margin-right"]->getFloat(), 
	       propList["margin-top"]->getFloat(), propList["margin-bottom"]->getFloat());
}

void HtmlListenerImpl::closePageSpan()
{
	printf("</page-span>\n");
}

void HtmlListenerImpl::openParagraph(const WPXPropertyList &propList, const vector<WPXPropertyList> &tabStops)
{
	printf("<p style=\"");
	_appendParagraphJustification(propList["justification"]->getInt());
	if (propList["margin-left"]->getFloat() != 0.0f)
		printf(" margin-left: %.4fin;", propList["margin-left"]->getFloat());
	if (propList["margin-right"]->getFloat() != 0.0f)
		printf(" margin-right: %.4fin;", propList["margin-right"]->getFloat());
	if (propList["margin-top"]->getFloat() != 0.0f)
		printf(" margin-top: /.4in;", propList["margin-top"]->getFloat());
	if (propList["margin-bottom"]->getFloat() != 0.0f)
		printf(" margin-bottom: %.4fin;", propList["margin-bottom"]->getFloat());
	if (propList["line-spacing"]->getFloat() != 1.0f)
		printf(" line-height: %.2f%%;", propList["line-spacing"]->getFloat()*100.0f);
	printf("\">");
}

void HtmlListenerImpl::closeParagraph()
{
	printf("</p>\n");
}

void HtmlListenerImpl::openSpan(const WPXPropertyList &propList)
{
	printf("<span style=\"font-family: \'%s\'\">", propList["font-name"]->getStr().cstr());
	printf("<span style=\"font-size: %.1fpt\">", propList["font-size"]->getFloat());
	if (propList["text-attribute-bits"]->getInt() & WPX_REDLINE_BIT)
		printf("<span style=\"color: #ff3333\">");
	else
		printf("<span style=\"color: #%.6x\">", _rgbsToRGB(propList["font-color"]->getInt()));
	if ((propList["highlight-color"]->getInt() & 0x000000ff) != 0xff)
	{
		printf("<span style=\"background-color: #%.6x\">", _rgbsToRGB(propList["highlight-color"]->getInt()));
		m_isHighlightColor = true;
	}
	_addTextAttributes(propList["text-attribute-bits"]->getInt());
}

void HtmlListenerImpl::closeSpan()
{
	_removeTextAttributes();
	printf("</span></span></span>");
	if (m_isHighlightColor)
	{
		printf("</span>");
		m_isHighlightColor = false;
	}
}

void HtmlListenerImpl::insertTab()
{
	printf("%c", UCS_TAB);
}

void HtmlListenerImpl::insertLineBreak()
{
	printf("<br>\n");
}

void HtmlListenerImpl::insertText(const WPXString &text)
{
	WPXString tempUTF8(text, true);
	printf("%s", tempUTF8.cstr());
}

void HtmlListenerImpl::openOrderedListLevel(const WPXPropertyList &propList)
{
	printf("<ol>\n");
}

void HtmlListenerImpl::closeOrderedListLevel()
{
	printf("</ol>\n");
}

void HtmlListenerImpl::openUnorderedListLevel(const WPXPropertyList &propList)
{
	printf("<ul>\n");
}

void HtmlListenerImpl::closeUnorderedListLevel()
{
	printf("</ul>\n");
}


void HtmlListenerImpl::openListElement(const WPXPropertyList &propList, const vector<WPXPropertyList> &tabStops)
{
	printf("<li style=\"");
	_appendParagraphJustification(propList["justification"]->getInt());
	if (propList["margin-left"]->getFloat() != 0.0f)
		printf(" margin-left: %.4fin;", propList["margin-left"]->getFloat());
	if (propList["margin-right"]->getFloat() != 0.0f)
		printf(" margin-right: %.4fin;", propList["margin-right"]->getFloat());
	if (propList["margin-top"]->getFloat() != 0.0f)
		printf(" margin-top: %.4in;", propList["margin-top"]->getFloat());
	if (propList["margin-bottom"]->getFloat() != 0.0f)
		printf(" margin-bottom: %.4fin;", propList["margin-bottom"]->getFloat());
	if (propList["line-spacing"]->getFloat() != 1.0f)
		printf(" line-height: %.2f%%;", propList["line-spacing"]->getFloat()*100.0f);
	printf("\">");
}

void HtmlListenerImpl::closeListElement()
{
	printf("</li>\n");
}

void HtmlListenerImpl::openFootnote(const WPXPropertyList &propList)
{
	printf("<footnote num=\"%i\">\n", propList["number"]->getInt());
}

void HtmlListenerImpl::closeFootnote()
{
	printf("</footnote>\n");
}

void HtmlListenerImpl::openEndnote(const WPXPropertyList &propList)
{
	printf("<endnote num=\"%i\">\n", propList["number"]->getInt());
}

void HtmlListenerImpl::closeEndnote()
{
	printf("</endnote>\n");
}

void HtmlListenerImpl::openTable(const WPXPropertyList &propList, const vector <WPXPropertyList> &columns)
{
	printf("<table border=\"1\">\n");
	printf("<tbody>\n");
}

void HtmlListenerImpl::openTableRow(const WPXPropertyList &propList)
{
	printf("<tr>\n");
}

void HtmlListenerImpl::closeTableRow()
{
	printf("</tr>\n");
}

void HtmlListenerImpl::openTableCell(const WPXPropertyList &propList)
{
	printf("<td ");

/*	if (((proplist["foreground-color"]->getInt()) && 0x000000ff) != 0xff) || ((proplist["background-color"]->getInt()) && 0x000000ff) != 0xff))
	{
		printf("style=\"");
		// NOTE: The WordPerfect Cell Foreground Color maps onto the HTML Background Color
		if (cellFgColor)
			printf("background-color: #%.6x;", cellFgColor->m_r, cellFgColor->m_g, cellFgColor->m_b);
		// TODO: Handle the Cell Background Color here, but I don't think HTML supports that
		// ...
		printf("\"");
	}*/
	printf(" rowspan=\"%ld\" colspan=\"%ld\"", propList["row-span"]->getInt(), propList["col-span"]->getInt());

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

void HtmlListenerImpl::_addTextAttributes(const int textAttributeBits)
{
	if (textAttributeBits & WPX_SUPERSCRIPT_BIT)
	{
		printf("<sup>");
		m_isSuperscript = true;
	}
	if (textAttributeBits & WPX_SUBSCRIPT_BIT)
	{
		printf("<sub>");
		m_isSubscript = true;
	}
	if (textAttributeBits & WPX_ITALICS_BIT)
	{
		printf("<i>");
		m_isItalic = true;
	}
	if (textAttributeBits & WPX_BOLD_BIT)
	{
		printf("<b>");
		m_isBold = true;
	}
	if (textAttributeBits & WPX_STRIKEOUT_BIT)
	{
		printf("<s>");
		m_isStrikeout = true;
	}
	if (textAttributeBits & WPX_UNDERLINE_BIT)
	{
		printf("<u>");
		m_isUnderline = true;
	}
	if (textAttributeBits & WPX_DOUBLE_UNDERLINE_BIT)
	{
		printf("<u>"); // I did not find double underline in HTML
		m_isDoubleUnderline = true;
	}
	// Following are just placeholders. I do not know how to convert them into a standard html
	if (textAttributeBits & WPX_OUTLINE_BIT)
	{
		m_isOutline = true;
	}
	if (textAttributeBits & WPX_SMALL_CAPS_BIT)
	{
		m_isSmallCaps = true;
	}
	if (textAttributeBits & WPX_BLINK_BIT)
	{
		m_isBlink = true;
	}
	if (textAttributeBits & WPX_SHADOW_BIT)
	{
		m_isShadow = true;
	}
}

void HtmlListenerImpl::_removeTextAttributes()
{
	if (m_isShadow)
	{
		m_isShadow = false;
	}
	if (m_isBlink)
	{
		m_isBlink = false;
	}
	if (m_isSmallCaps)
	{
		m_isSmallCaps = false;
	}
	if (m_isOutline)
	{
		m_isOutline = false;
	}
	if (m_isDoubleUnderline)
	{
		printf("</u>");
		m_isDoubleUnderline = false;
	}
	if (m_isUnderline)
	{
		printf("</u>");
		m_isUnderline = false;
	}
	if (m_isStrikeout)
	{
		printf("</s>");
		m_isStrikeout = false;
	}
	if (m_isBold)
	{
		printf("</b>");
		m_isBold = false;
	}
	if (m_isItalic)
	{
		printf("</i>");
		m_isItalic = false;
	}
	if (m_isSubscript)
	{
		printf("</sub>");
		m_isSubscript = false;
	}
	if (m_isSuperscript)
	{
		printf("</sup>");
		m_isSuperscript = false;
	}
}

void HtmlListenerImpl::_appendParagraphJustification(const int justification)
{
	switch (justification)
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

int HtmlListenerImpl::_rgbsToRGB(const int rgbsColor)
{
	int tmpRGBColor;
	tmpRGBColor = 0x00000000;
	float shading = ((float)(rgbsColor & 0x000000ff)/100.0f); //convert the percents to float between 0 and 1
	// Mix shading amount of given color with (1-shading) of White (#ffffff)
	int red = (int)0xFF + (int)(((float)((rgbsColor & 0xff000000) >> 24))*shading) - (int)((float)0xFF*shading);
	tmpRGBColor = red;
	int green = (int)0xFF + (int)(((float)((rgbsColor & 0x00ff0000) >> 16))*shading) - (int)((float)0xFF*shading);
	tmpRGBColor = (tmpRGBColor << 8) + green;
	int blue = (int)0xFF + (int)(((float)((rgbsColor & 0x0000ff00) >> 8))*shading) - (int)((float)0xFF*shading);
	tmpRGBColor = (tmpRGBColor << 8) + blue;
	return tmpRGBColor;
} 
