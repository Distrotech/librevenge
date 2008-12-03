/* libwpd
 * Copyright (C) 2003 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2004 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2006-2007 Fridrich Strba (fridrich.strba@bluewin.ch)
 * Copyright (C) 2007 Novell, Inc. (http://www.novell.com)
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

#ifndef WP5STYLESLISTENER_H
#define WP5STYLESLISTENER_H

#include "WP5Listener.h"
#include "WPXStylesListener.h"
#include <vector>
#include <list>
#include "WPXPageSpan.h"
#include "WPXTable.h"
#include "WP5SubDocument.h"

class WP5StylesListener : public WP5Listener, protected WPXStylesListener
{
public:
	WP5StylesListener(std::list<WPXPageSpan> &pageList, WPXTableList tableList, std::vector<WP5SubDocument*> &subDocuments);

	void startDocument() {}
	void startSubDocument() {}
	void setFont(const WPXString& /* fontName */, double /* fontSize */) {}
	void setTabs(const std::vector<WPXTabStop>& /* tabStops */, uint16_t /* tabOffset */) {} 
	void insertCharacter(uint16_t /* character */) { /*if (!isUndoOn())*/ m_currentPageHasContent = true; }
	void insertTab(uint8_t /* tabType */, double /* tabPosition */) { /*if (!isUndoOn())*/ m_currentPageHasContent = true; }
	virtual void insertIndent(uint8_t /* indentType */, double /* indentPosition */) { /*if (!isUndoOn())*/ m_currentPageHasContent = true; }
	void characterColorChange(uint8_t /* red */, uint8_t /* green */, uint8_t /* blue */) {};
	void insertEOL() { /*if (!isUndoOn())*/ m_currentPageHasContent = true; }
 	void insertBreak(uint8_t breakType);
	void attributeChange(bool /* isOn */, uint8_t /* attribute */) {}
	void lineSpacingChange(double /* lineSpacing */) {}
	void justificationChange(uint8_t /* justification */) {}
	void pageMarginChange(uint8_t side, uint16_t margin);
	void pageFormChange(uint16_t length, uint16_t width, WPXFormOrientation orientation);
	void marginChange(uint8_t side, uint16_t margin);
	void endDocument();
	void endSubDocument();

	void defineTable(uint8_t /* position */, uint16_t /* leftOffset */) {}
	void addTableColumnDefinition(uint32_t /* width */, uint32_t /* leftGutter */, uint32_t /* rightGutter */,
				uint32_t /* attributes */, uint8_t /* alignment */) {}
	void startTable();
 	void insertRow(uint16_t rowHeight, bool isMinimumHeight, bool isHeaderRow);
 	void insertCell(uint8_t colSpan, uint8_t rowSpan, uint8_t borderBits,
				const RGBSColor * cellFgColor, const RGBSColor * cellBgColor, 
				const RGBSColor * cellBorderColor, WPXVerticalAlignment cellVerticalAlignment, 
				bool useCellAttributes, uint32_t cellAttributes);
 	void endTable() {}

	void insertNoteReference(const WPXString& /* noteReference */) {};
	void insertNote(WPXNoteType /* noteType */, const WP5SubDocument * /* subDocument */) {};
	void headerFooterGroup(uint8_t headerFooterType, uint8_t occurenceBits, WP5SubDocument *subDocument);
	void suppressPageCharacteristics(uint8_t suppressCode);

	void boxOn(uint8_t /* positionAndType */, uint8_t /* alignment */, uint16_t /* width */, uint16_t /* height */, uint16_t /* x */, uint16_t /* y */) {};
	virtual void boxOff() {};
	virtual void insertGraphicsData(const WPXBinaryData * /* data */) {};

protected:
	void _handleSubDocument(const WPXSubDocument *subDocument, WPXSubDocumentType subDocumentType, WPXTableList tableList, int nextTableIndice = 0);

private:
	WP5StylesListener(const WP5StylesListener&);
	WP5StylesListener& operator=(const WP5StylesListener&);
	WPXPageSpan m_currentPage, m_nextPage;

	WPXTableList m_tableList;
	WPXTable *m_currentTable;
	double m_tempMarginLeft, m_tempMarginRight;
	bool m_currentPageHasContent;
	bool m_isSubDocument;
	std::vector<WP5SubDocument *> &m_subDocuments;
	std::list<WPXPageSpan>::iterator m_pageListHardPageMark;
};

#endif /* WP5STYLESLISTENER_H */
