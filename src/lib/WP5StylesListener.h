/* libwpd
 * Copyright (C) 2003 William Lachance (wrlach@gmail.com)
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

#ifndef WP5STYLESLISTENER_H
#define WP5STYLESLISTENER_H

#include "WP5Listener.h"
#include "WPXStylesListener.h"
#include <vector>
#include <list>
#include <set>
#include "WPXPageSpan.h"
#include "WPXTable.h"
#include "WP5SubDocument.h"

class WP5StylesListener : public WP5Listener, protected WPXStylesListener
{
public:
	WP5StylesListener(std::list<WPXPageSpan> &pageList, WPXTableList tableList, std::vector<WP5SubDocument*> &subDocuments);

	void startDocument() {}
	void setFont(const WPXString /* fontName */, const float /* fontSize */) {}
	void insertCharacter(const uint16_t /* character */) { /*if (!isUndoOn())*/ m_currentPageHasContent = true; }
	void insertTab(const uint8_t /* tabType */, float /* tabPosition */) { /*if (!isUndoOn())*/ m_currentPageHasContent = true; }
	void characterColorChange(const uint8_t /* red */, const uint8_t /* green */, const uint8_t /* blue */) {};
	void insertEOL() { /*if (!isUndoOn())*/ m_currentPageHasContent = true; }
 	void insertBreak(const uint8_t breakType);
	void attributeChange(const bool /* isOn */, const uint8_t /* attribute */) {}
	void lineSpacingChange(const float /* lineSpacing */) {}
	void justificationChange(const uint8_t /* justification */) {}
	void pageMarginChange(const uint8_t side, const uint16_t margin);
	void pageFormChange(const uint16_t length, const uint16_t width, const WPXFormOrientation orientation, const bool isPersistent);
	void marginChange(const uint8_t side, const uint16_t margin);
	void endDocument();

	void defineTable(const uint8_t /* position */, const uint16_t /* leftOffset */) {}
	void addTableColumnDefinition(const uint32_t /* width */, const uint32_t /* leftGutter */, const uint32_t /* rightGutter */,
				const uint32_t /* attributes */, const uint8_t /* alignment */) {}
	void startTable();
 	void insertRow(const uint16_t rowHeight, const bool isMinimumHeight, const bool isHeaderRow);
 	void insertCell(const uint8_t colSpan, const uint8_t rowSpan, const uint8_t borderBits,
				const RGBSColor * cellFgColor, const RGBSColor * cellBgColor, 
				const RGBSColor * cellBorderColor, const WPXVerticalAlignment cellVerticalAlignment, 
				const bool useCellAttributes, const uint32_t cellAttributes);
 	void endTable() {}

	void insertNoteReference(const WPXString /* noteReference */) {};
	void insertNote(const WPXNoteType /* noteType */, const WP5SubDocument * /* subDocument */) {};
	void headerFooterGroup(const uint8_t headerFooterType, const uint8_t occurenceBits, WP5SubDocument *subDocument);
	void suppressPageCharacteristics(const uint8_t suppressCode);

protected:
	void _handleSubDocument(const WPXSubDocument *subDocument, const bool isHeaderFooter, WPXTableList tableList, int nextTableIndice = 0);

private:
	WPXPageSpan m_currentPage, m_nextPage;

	WPXTableList m_tableList;
	WPXTable *m_currentTable;
	float m_tempMarginLeft, m_tempMarginRight;
	bool m_currentPageHasContent;
	bool m_isSubDocument;
	std::vector<WP5SubDocument *> &m_subDocuments;
	std::list<WPXPageSpan>::iterator m_pageListHardPageMark;
};

#endif /* WP5STYLESLISTENER_H */
