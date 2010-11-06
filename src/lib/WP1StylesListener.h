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

#ifndef WP1STYLESLISTENER_H
#define WP1STYLESLISTENER_H

#include "WP1Listener.h"
#include "WP1SubDocument.h"
#include "WPXStylesListener.h"
#include <vector>
#include "WPXPageSpan.h"
#include "WPXTable.h"

class WP1StylesListener : public WP1Listener, protected WPXStylesListener
{
public:
	WP1StylesListener(std::list<WPXPageSpan> &pageList, std::vector<WP1SubDocument *> &subDocuments);
	~WP1StylesListener() {}

	void startDocument() {}
	void startSubDocument() {}
	void insertCharacter(uint16_t /* character */) { if (!isUndoOn()) m_currentPageHasContent = true; }
	void insertExtendedCharacter(uint8_t /* extendedCharacter */) { if (!isUndoOn()) m_currentPageHasContent = true; }
	void insertTab() { if (!isUndoOn()) m_currentPageHasContent = true; }
	void insertEOL() { if (!isUndoOn()) m_currentPageHasContent = true; }
 	void insertBreak(uint8_t breakType);
	void insertNote(WPXNoteType /* noteType */, WP1SubDocument * /* subDocument */) {}
	void attributeChange(bool /* isOn */, uint8_t /* attribute */) {}
	void fontPointSize(uint8_t /* pointSize */) {}
	void fontId(uint16_t /* id */) {}
	void marginReset(uint16_t leftMargin, uint16_t rightMargin);
	void topMarginSet(uint16_t topMargin);
	void bottomMarginSet(uint16_t bottomMargin);
	void leftIndent(uint16_t /* leftMarginOffset */) { if (!isUndoOn()) m_currentPageHasContent = true; }
	void leftRightIndent(uint16_t /* leftRightMarginOffset */) { if (!isUndoOn()) m_currentPageHasContent = true; }
	void leftMarginRelease(uint16_t /* release */) {}
	void setTabs(const std::vector<WPXTabStop>& /* tabStops */) {}
	void headerFooterGroup(uint8_t headerFooterDefinition, WP1SubDocument *subDocument);
	void suppressPageCharacteristics(uint8_t suppressCode);
	void justificationChange(uint8_t /* justification */) {}
	void lineSpacingChange(uint8_t /* spacing */) {}
	void flushRightOn() {}
	void flushRightOff() {}
	void centerOn() {}
	void centerOff() {}
	void endDocument();
	void endSubDocument();
	void insertPicture(uint16_t /* width */, uint16_t /* height */, const WPXBinaryData & /* binaryData */) {}

protected:
	void _handleSubDocument(const WPXSubDocument *subDocument, WPXSubDocumentType subDocumentType, WPXTableList tableList, int nextTableIndice = 0);

private:
	WPXPageSpan m_currentPage, m_nextPage;
	std::vector<WP1SubDocument *> &m_subDocuments;
	double m_tempMarginLeft, m_tempMarginRight;
	bool m_currentPageHasContent;
	bool m_isSubDocument;
	std::list<WPXPageSpan>::iterator m_pageListHardPageMark;
};

#endif /* WP1STYLESLISTENER_H */
