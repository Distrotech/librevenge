/* libwpd
 * Copyright (C) 2004 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#ifndef WP3STYLESLISTENER_H
#define WP3STYLESLISTENER_H

#include "WP3Listener.h"
#include "WPXStylesListener.h"
#include <vector>
#include <set>
#include "WPXPageSpan.h"
#include "WPXTable.h"

class WP3StylesListener : public WP3Listener, protected WPXStylesListener
{
public:
	WP3StylesListener(std::list<WPXPageSpan> &pageList, WPXTableList tableList, std::vector<WP3SubDocument *> &subDocuments);

	void startDocument() {}
	void startSubDocument() {}
	void insertCharacter(uint16_t /* character */) { if (!isUndoOn()) m_currentPageHasContent = true; }
	void insertTab() { if (!isUndoOn()) m_currentPageHasContent = true; }
	void insertTab(uint8_t /* tabType */, double /* tabPosition */) { if (!isUndoOn()) m_currentPageHasContent = true; }
	void insertEOL() { if (!isUndoOn()) m_currentPageHasContent = true; }
 	void insertBreak(uint8_t breakType);
	void attributeChange(bool /* isOn */, uint8_t /* attribute */) {}
	void lineSpacingChange(double /* lineSpacing */) {}
	void justificationChange(uint8_t /* justification */) {}
	void pageMarginChange(uint8_t side, uint16_t margin);
	void pageFormChange(uint16_t length, uint16_t width, WPXFormOrientation orientation);
	void marginChange(uint8_t side, uint16_t margin);
	void indentFirstLineChange(int16_t /* offset */) {}
	void setTabs(bool /* isRelative */, const std::vector<WPXTabStop> /* tabStops */) {}
	void columnChange(WPXTextColumnType /* columnType */, uint8_t /* numColumns */,
			const std::vector<double> & /* columnWidth */, const std::vector<bool> & /* isFixedWidth */) {}
	void endDocument();
	void endSubDocument();

	void defineTable(uint8_t /* position */, uint16_t /* leftOffset */) {}
	void addTableColumnDefinition(uint32_t /* width */, uint32_t /* leftGutter */, uint32_t /* rightGutter */,
					uint32_t /* attributes */, uint8_t /* alignment */) {}
	void startTable();
 	void insertRow();
 	void insertCell();
 	void closeCell() {}
	void closeRow() {}
	void setTableCellSpan(uint16_t /* colSpan */, uint16_t /* rowSpan */) {}
	void setTableCellFillColor(const RGBSColor * /* cellFillColor */) {}
 	void endTable() {}
	void undoChange(uint8_t undoType, uint16_t undoLevel);
	void setTextColor(const RGBSColor * /* fontColor */) {}
	void setTextFont(const WPXString& /* fontName */) {}
	void setFontSize(uint16_t /* fontSize */) {}
	void insertPageNumber(const WPXString & /* pageNumber */) {}
	void insertNoteReference(const WPXString & /* noteReference */) {}
	void insertNote(WPXNoteType /* noteType */, const WP3SubDocument * /* subDocument */) { if (!isUndoOn()) m_currentPageHasContent = true; }
	void headerFooterGroup(uint8_t headerFooterType, uint8_t occurenceBits, WP3SubDocument *subDocument);
	void suppressPage(uint16_t suppressCode);
	void backTab() { if (!isUndoOn()) m_currentPageHasContent = true; }
	void leftIndent() {}
	void leftIndent(double /* offset */) {}
	void leftRightIndent() {}
	void leftRightIndent(double /* offset */) {}
	void insertPicture(double /* height */, double /* width */, double /* verticalOffset */, double /* horizontalOffset */, uint8_t /* leftColumn */, uint8_t /* rightColumn */,
			uint16_t /* figureFlags */, const WPXBinaryData & /* binaryData */) {}
	void insertTextBox(double /* height */, double /* width */, double /* verticalOffset */, double /* horizontalOffset */, uint8_t /* leftColumn */, uint8_t /* rightColumn */,
			uint16_t /* figureFlags */, const WP3SubDocument * /* subDocument */, const WP3SubDocument * /* caption */) {}
	void insertWP51Table(double /* height */, double /* width */, double /* verticalOffset */, double /* horizontalOffset */, uint8_t /* leftColumn */, uint8_t /* rightColumn */,
			uint16_t /* figureFlags */, const WP3SubDocument * /* subDocument */, const WP3SubDocument * /* caption */) {}

protected:
	void _handleSubDocument(const WPXSubDocument *subDocument, WPXSubDocumentType subDocumentType, WPXTableList tableList, int nextTableIndice = 0);

private:
	WP3StylesListener(const WP3StylesListener&);
	WP3StylesListener& operator=(const WP3StylesListener&);
	WPXPageSpan m_currentPage;

	WPXTableList m_tableList;
	WPXTable *m_currentTable;
	double m_tempMarginLeft, m_tempMarginRight;
	bool m_currentPageHasContent;
	bool m_isSubDocument;
	std::vector<WP3SubDocument *> &m_subDocuments;
	std::list<WPXPageSpan>::iterator m_pageListHardPageMark;
};

#endif /* WP3STYLESLISTENER_H */
