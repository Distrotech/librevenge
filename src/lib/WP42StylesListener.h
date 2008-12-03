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

#ifndef WP42STYLESLISTENER_H
#define WP42STYLESLISTENER_H

#include "WP42Listener.h"
#include "WP42SubDocument.h"
#include "WPXStylesListener.h"
#include <vector>
#include "WPXPageSpan.h"
#include "WPXTable.h"

class WP42StylesListener : public WP42Listener, protected WPXStylesListener
{
public:
	WP42StylesListener(std::list<WPXPageSpan> &pageList, std::vector<WP42SubDocument *> &subDocuments);

	void startDocument() {}
	void startSubDocument() {}
	void insertCharacter(uint16_t /* character */) { if (!isUndoOn()) m_currentPageHasContent = true; }
	void insertTab(uint8_t /* tabType */, double /* tabPosition */) { if (!isUndoOn()) m_currentPageHasContent = true; }
	void insertEOL() { if (!isUndoOn()) m_currentPageHasContent = true; }
 	void insertBreak(uint8_t breakType);
	void attributeChange(bool /* isOn */, uint8_t /* attribute */) {}
	void marginReset(uint8_t /* leftMargin */, uint8_t /* rightMargin */) {}
	void headerFooterGroup(uint8_t headerFooterDefinition, WP42SubDocument *subDocument);
	void suppressPageCharacteristics(uint8_t suppressCode);
	void endDocument();
	void endSubDocument();

protected:
	void _handleSubDocument(const WPXSubDocument *subDocument, WPXSubDocumentType subDocumentType, WPXTableList tableList, int nextTableIndice = 0);

private:
	WPXPageSpan m_currentPage, m_nextPage;
	std::vector<WP42SubDocument *> &m_subDocuments;
	double m_tempMarginLeft, m_tempMarginRight;
	bool m_currentPageHasContent;
	bool m_isSubDocument;
	std::list<WPXPageSpan>::iterator m_pageListHardPageMark;
};

#endif /* WP42STYLESLISTENER_H */
