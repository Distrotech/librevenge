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

#ifndef HTMLLISTENERIMPL_H
#define HTMLLISTENERIMPL_H

#include <glib.h>
#include "WPXHLListenerImpl.h"
#include "WPXHLListener.h"

class HtmlListenerImpl : public WPXHLListenerImpl
{
public:
	HtmlListenerImpl();
	virtual ~HtmlListenerImpl();

 	virtual void setDocumentMetaData(const UCSString &author, const UCSString &subject,
					 const UCSString &publisher, const UCSString &category,
					 const UCSString &keywords, const UCSString &language,
					 const UCSString &abstract, const UCSString &descriptiveName,
					 const UCSString &descriptiveType);

	virtual void startDocument();
	virtual void endDocument();

	virtual void openPageSpan(const int span, const bool isLastPageSpan,
				  const float formLength, const float formWidth, const WPXFormOrientation orientation,
				  const float marginLeft, const float marginRight,
				  const float marginTop, const float marginBottom);
	virtual void closePageSpan();
	virtual void openHeaderFooter(const WPXHeaderFooterType headerFooterType, const WPXHeaderFooterOccurence headerFooterOccurence);
	virtual void closeHeaderFooter(const WPXHeaderFooterType headerFooterType, const WPXHeaderFooterOccurence headerFooterOccurence);

	virtual void openSection(const unsigned int numColumns, const vector< WPXColumnDefinition> &columns, const float spaceAfter);
	virtual void closeSection();
	virtual void openParagraph(const uint8_t paragraphJustification, 
				   const float marginLeftOffset, const float marginRightOffset, const float textIndent,
				   const float lineSpacing, const float spacingBeforeParagraph, const float spacingAfterParagraph,
				   const vector<WPXTabStop> &tabStops, const bool isColumnBreak, const bool isPageBreak);
	virtual void closeParagraph();
	virtual void openSpan(const uint32_t textAttributeBits, const char *fontName, const float fontSize,
				   const RGBSColor *fontColor, const RGBSColor *highlightColor);
	virtual void closeSpan();

	virtual void insertTab();
	virtual void insertText(const UCSString &text);
	virtual void insertLineBreak();

	virtual void defineOrderedListLevel(const int listID, const int listLevel, const WPXNumberingType listType,
					    const UCSString &textBeforeNumber, const UCSString &textAfterNumber,
					    const int startingNumber) {}
	virtual void defineUnorderedListLevel(const int listID, const int listLevel, const UCSString &bullet) {}
	virtual void openOrderedListLevel(const int listID);
	virtual void openUnorderedListLevel(const int listID);
	virtual void closeOrderedListLevel();
	virtual void closeUnorderedListLevel();
	virtual void openListElement(const uint8_t paragraphJustification, 
				     const float marginLeftOffset, const float marginRightOffset, const float textIndent,
				     const float lineSpacing, const float spacingBeforeParagraph, const float spacingAfterParagraph,
				     const vector<WPXTabStop> &tabStops);
	virtual void closeListElement();

	virtual void openFootnote(int number);
	virtual void closeFootnote();
	virtual void openEndnote(int number);
	virtual void closeEndnote();


	virtual void openTable(const uint8_t tablePositionBits,
			       const float marginLeftOffset, const float marginRightOffset,
			       const float leftOffset, const vector < WPXColumnDefinition > &columns);
	virtual void openTableRow(const float height, const bool isMinimumHeight, const bool isHeaderRow);
	virtual void closeTableRow();
	virtual void openTableCell(const uint32_t col, const uint32_t row, const uint32_t colSpan, const uint32_t rowSpan,
				   const uint8_t borderBits, const RGBSColor * cellFgColor, const RGBSColor * cellBgColor,
				   const RGBSColor * cellBorderColor,
				   const WPXVerticalAlignment cellVerticalAlignment);
	virtual void closeTableCell();
	virtual void insertCoveredTableCell(const uint32_t col, const uint32_t row) {}
	virtual void closeTable();

protected:
	void _addTextAttributes(const uint32_t textAttributeBits);
	void _removeTextAttributes();
	void _appendParagraphJustification(const uint32_t paragraphJustification);
	RGBSColor _convertRGBStoRGB(const RGBSColor color);

private:
	unsigned int m_currentListLevel;
// textAttributeBits
	bool m_isSuperscript;
	bool m_isSubscript;
	bool m_isBold;
	bool m_isItalic;
	bool m_isStrikeout;
	bool m_isUnderline;
	bool m_isDoubleUnderline;
	bool m_isOutline;
	bool m_isSmallCaps;
	bool m_isBlink;
	bool m_isShadow;

	bool m_isHighlightColor;

};

#endif /* HTMLLISTENERIMPL_H */
