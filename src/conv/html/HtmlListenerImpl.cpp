/* libwpd
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
	m_isRedline(false)
{
}

HtmlListenerImpl::~HtmlListenerImpl()
{
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

void HtmlListenerImpl::openPageSpan(const int span, const bool isLastPageSpan,
				    const float marginLeft, const float marginRight,
				    const float marginTop, const float marginBottom)
{
	printf("<page-span span:%i margin-left:%fin margin-right:%fin margin-top:%fin margin-bottom:%fin>\n", span, 
	       marginLeft, marginRight, marginTop, marginBottom);
}

void HtmlListenerImpl::closePageSpan()
{
	printf("</page-span>\n");
}

void HtmlListenerImpl::openHeaderFooter(const WPXHeaderFooterType headerFooterType, const WPXHeaderFooterOccurence headerFooterOccurence)
{
	switch (headerFooterType)
	{
	case HEADER:
		printf("<header>\n");
		break;
	case FOOTER:
		printf("<footer>\n");
		break;
	}
}

void HtmlListenerImpl::closeHeaderFooter(const WPXHeaderFooterType headerFooterType, const WPXHeaderFooterOccurence headerFooterOccurence)
{
	switch (headerFooterType)
	{
	case HEADER:
		printf("</header>\n");
		break;
	case FOOTER:
		printf("</footer>\n");
		break;
	}
}

void HtmlListenerImpl::openParagraph(const guint8 paragraphJustification, const guint32 textAttributeBits, 
				     const float marginLeft, const float marginRight,
				     const gchar *fontName, const float fontSize,
				     const float lineSpacing,
				     const bool isColumnBreak, const bool isPageBreak)
{
	printf("<p style=\"");
	_appendParagraphJustification(paragraphJustification);
	if (marginLeft != 0.0f)
		printf(" margin-left: %.4fin;", marginLeft);
	if (marginRight != 0.0f)
		printf(" margin-right: %.4fin;", marginRight);
	if (lineSpacing != 1.0f)
		printf(" line-height: %.2f%%;", lineSpacing*100.0f);
	printf("\">");
}

void HtmlListenerImpl::closeParagraph()
{
	printf("</p>\n");
}

void HtmlListenerImpl::openSpan(guint32 textAttributeBits, const gchar *fontName, const float fontSize)
{
	printf("<span style=\"font-family: \'%s\'\">", fontName);
	printf("<span style=\"font-size: %.1fpt\">", fontSize);
	_addTextAttributes(textAttributeBits);
}

void HtmlListenerImpl::closeSpan()
{
	_removeTextAttributes();
	printf("</span></span>");
}

void HtmlListenerImpl::openSection(const unsigned int numColumns, const float spaceBefore)
{
	printf("<section columns:%i>\n", numColumns);
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

void HtmlListenerImpl::insertLineBreak()
{
	printf("<br>\n");
}

void HtmlListenerImpl::openOrderedListLevel(const int listID)
{
	printf("<ol>\n");
}

void HtmlListenerImpl::closeOrderedListLevel()
{
	printf("</ol>\n");
}

void HtmlListenerImpl::openUnorderedListLevel(const int listID)
{
	printf("<ul>\n");
}

void HtmlListenerImpl::closeUnorderedListLevel()
{
	printf("</ul>\n");
}


void HtmlListenerImpl::openListElement(const guint8 paragraphJustification, const guint32 textAttributeBits,
				       const float marginLeftOffset, const float marginRightOffset,
				       const gchar *fontName, const float fontSize, 
				       const float lineSpacing)
{
	printf("<li style=\"");
	_appendParagraphJustification(paragraphJustification);
	if (marginLeftOffset != 0.0f)
		printf(" margin-left: %.4fin;", marginLeftOffset);
	if (marginRightOffset != 0.0f)
		printf(" margin-right: %.4fin;", marginRightOffset);
	if (lineSpacing != 1.0f)
		printf(" line-height: %.2f%%;", lineSpacing*100.0f);
	printf("\">");
}

void HtmlListenerImpl::closeListElement()
{
	printf("</li>\n");
}

void HtmlListenerImpl::openFootnote(int number)
{
	printf("<footnote num=\"%i\">\n", number);
}

void HtmlListenerImpl::closeFootnote()
{
	printf("</footnote>\n");
}

void HtmlListenerImpl::openEndnote(int number)
{
	printf("<endnote num=\"%i\">\n", number);
}

void HtmlListenerImpl::closeEndnote()
{
	printf("</endnote>\n");
}

void HtmlListenerImpl::openTable(const guint8 tablePositionBits, const float marginLeftOffset, const float marginRightOffset,
				 const float leftOffset, const vector < WPXColumnDefinition > &columns)
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
						const guint8 borderBits,
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

void HtmlListenerImpl::_addTextAttributes(const guint32 textAttributeBits)
{
	if (textAttributeBits & WPX_REDLINE_BIT)
	{
		printf("<span style=\"color: #ff3333\">");
		m_isRedline = true;
	}
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
	if (m_isRedline)
	{
		printf("</span>");
		m_isRedline = false;
	}
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
