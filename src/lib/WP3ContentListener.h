/* libwpd
 * Copyright (C) 2004 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2005-2006 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
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

#ifndef WP3CONTENTLISTENER_H
#define WP3CONTENTLISTENER_H

#include "WP3Listener.h"
#include "WPXContentListener.h"
#include "WP3SubDocument.h"

typedef struct _WP3ContentParsingState WP3ContentParsingState;
struct _WP3ContentParsingState
{
	_WP3ContentParsingState();
	~_WP3ContentParsingState();
	uint16_t m_colSpan;
	uint16_t m_rowSpan;
	WPXString m_textBuffer;
	RGBSColor * m_cellFillColor;
	WPXString m_noteReference;

	WPXTableList m_tableList;
private:
	_WP3ContentParsingState(const _WP3ContentParsingState&);
	_WP3ContentParsingState& operator=(const _WP3ContentParsingState&);
};

class WP3ContentListener : public WP3Listener, protected WPXContentListener
{
public:
	WP3ContentListener(std::list<WPXPageSpan> &pageList, std::vector<WP3SubDocument *> &subDocuments, WPXDocumentInterface *documentInterface);
	~WP3ContentListener();

	void startDocument() { WPXContentListener::startDocument(); };
	void startSubDocument() { WPXContentListener::startSubDocument(); };
	void insertCharacter(uint16_t character);
	void insertTab();
	void insertTab(uint8_t tabType, double tabPosition);
	void insertBreak(uint8_t breakType) { WPXContentListener::insertBreak(breakType); };
	void insertEOL();
	void attributeChange(bool isOn, uint8_t attribute);
	void lineSpacingChange(double lineSpacing) { WPXContentListener::lineSpacingChange(lineSpacing); };
	void pageMarginChange(uint8_t /* side */, uint16_t /* margin */) {};
	void pageFormChange(uint16_t /* length */, uint16_t /* width */, WPXFormOrientation /* orientation */) {};
	void marginChange(uint8_t side, uint16_t margin);
	void indentFirstLineChange(int16_t offset);
	void setTabs(bool isRelative, const std::vector<WPXTabStop> tabStops);
	void columnChange(WPXTextColumnType columnType, uint8_t numColumns, const std::vector<double> &columnWidth,
					const std::vector<bool> &isFixedWidth);
	void endDocument() { WPXContentListener::endDocument(); };
	void endSubDocument() { WPXContentListener::endSubDocument(); };

	void defineTable(uint8_t position, uint16_t leftOffset);
	void addTableColumnDefinition(uint32_t width, uint32_t leftGutter, uint32_t rightGutter,
					uint32_t attributes, uint8_t alignment);
	void startTable();
 	void insertRow();
 	void insertCell();
 	void closeCell();
	void closeRow();
	void setTableCellSpan(uint16_t colSpan, uint16_t rowSpan);
	void setTableCellFillColor(const RGBSColor * cellFillColor);
 	void endTable();
	void undoChange(uint8_t undoType, uint16_t undoLevel);
	void justificationChange(uint8_t justification);
	void setTextColor(const RGBSColor * fontColor);
	void setTextFont(const WPXString &fontName);
	void setFontSize(uint16_t fontSize);
	void insertPageNumber(const WPXString &pageNumber);
	void insertNoteReference(const WPXString &noteReference);
	void insertNote(WPXNoteType noteType, const WP3SubDocument *subDocument);
	void headerFooterGroup(uint8_t headerFooterType, uint8_t occurenceBits, WP3SubDocument *subDocument);
	void suppressPage(uint16_t /* suppressCode */) {};
	void backTab();
	void leftIndent();
	void leftIndent(double offset);
	void leftRightIndent();
	void leftRightIndent(double offset);
	void insertPicture(double height, double width, double verticalOffset, double horizontalOffset, uint8_t leftColumn, uint8_t rightColumn,
			uint16_t figureFlags, const WPXBinaryData &binaryData);
	void insertTextBox(double height, double width, double verticalOffset, double horizontalOffset, uint8_t leftColumn, uint8_t rightColumn,
			uint16_t figureFlags, const WP3SubDocument *subDocument, const WP3SubDocument *caption);
	void insertWP51Table(double height, double width, double verticalOffset, double horizontalOffset, uint8_t leftColumn, uint8_t rightColumn,
			uint16_t figureFlags, const WP3SubDocument *subDocument, const WP3SubDocument *caption);
	
protected:
	void _handleSubDocument(const WPXSubDocument *subDocument, WPXSubDocumentType subDocumentType, WPXTableList tableList, int nextTableIndice = 0);
	void _openParagraph();

	void _flushText();
	void _changeList() {};
	
	void _handleFrameParameters( WPXPropertyList &propList, double height, double width, double verticalOffset, double horizontalOffset, uint8_t leftColumn, uint8_t rightColumn,
			uint16_t figureFlags );

private:
	WP3ContentListener(const WP3ContentListener&);
	WP3ContentListener& operator=(const WP3ContentListener&);
	WP3ContentParsingState *m_parseState;
	std::vector<WP3SubDocument *> &m_subDocuments;
};

#endif /* WP3CONTENTLISTENER_H */
