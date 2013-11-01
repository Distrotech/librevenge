/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2003 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2004 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2006-2007 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 *
 * For further information visit http://librevenge.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#ifndef WP5STYLESLISTENER_H
#define WP5STYLESLISTENER_H

#include "WP5Listener.h"
#include "RVNGStylesListener.h"
#include <vector>
#include <list>
#include "RVNGPageSpan.h"
#include "RVNGTable.h"
#include "WP5SubDocument.h"

class WP5StylesListener : public WP5Listener, protected RVNGStylesListener
{
public:
	WP5StylesListener(std::list<RVNGPageSpan> &pageList, RVNGTableList tableList, std::vector<WP5SubDocument *> &subDocuments);

	void startDocument() {}
	void startSubDocument() {}
	void setFont(const RVNGString & /* fontName */, double /* fontSize */) {}
	void setTabs(const std::vector<RVNGTabStop> & /* tabStops */, uint16_t /* tabOffset */) {}
	void insertCharacter(uint32_t /* character */)
	{
		/*if (!isUndoOn())*/ m_currentPageHasContent = true;
	}
	void insertTab(uint8_t /* tabType */, double /* tabPosition */)
	{
		/*if (!isUndoOn())*/ m_currentPageHasContent = true;
	}
	virtual void insertIndent(uint8_t /* indentType */, double /* indentPosition */)
	{
		/*if (!isUndoOn())*/ m_currentPageHasContent = true;
	}
	void characterColorChange(uint8_t /* red */, uint8_t /* green */, uint8_t /* blue */) {}
	void insertEOL()
	{
		/*if (!isUndoOn())*/ m_currentPageHasContent = true;
	}
	void insertBreak(uint8_t breakType);
	void attributeChange(bool /* isOn */, uint8_t /* attribute */) {}
	void lineSpacingChange(double /* lineSpacing */) {}
	void justificationChange(uint8_t /* justification */) {}
	void pageMarginChange(uint8_t side, uint16_t margin);
	void pageFormChange(uint16_t length, uint16_t width, RVNGFormOrientation orientation);
	void marginChange(uint8_t side, uint16_t margin);
	void endDocument();
	void endSubDocument();

	void defineTable(uint8_t /* position */, uint16_t /* leftOffset */) {}
	void addTableColumnDefinition(uint32_t /* width */, uint32_t /* leftGutter */, uint32_t /* rightGutter */,
	                              uint32_t /* attributes */, uint8_t /* alignment */) {}
	void startTable();
	void insertRow(uint16_t rowHeight, bool isMinimumHeight, bool isHeaderRow);
	void insertCell(uint8_t colSpan, uint8_t rowSpan, uint8_t borderBits,
	                const RGBSColor *cellFgColor, const RGBSColor *cellBgColor,
	                const RGBSColor *cellBorderColor, RVNGVerticalAlignment cellVerticalAlignment,
	                bool useCellAttributes, uint32_t cellAttributes);
	void endTable() {}

	void insertNoteReference(const RVNGString & /* noteReference */) {}
	void insertNote(RVNGNoteType /* noteType */, const WP5SubDocument * /* subDocument */) {}
	void headerFooterGroup(uint8_t headerFooterType, uint8_t occurenceBits, WP5SubDocument *subDocument);
	void suppressPageCharacteristics(uint8_t suppressCode);

	void boxOn(uint8_t /* positionAndType */, uint8_t /* alignment */, uint16_t /* width */, uint16_t /* height */, uint16_t /* x */, uint16_t /* y */) {}
	virtual void boxOff() {}
	virtual void insertGraphicsData(const RVNGBinaryData * /* data */) {}

protected:
	void _handleSubDocument(const RVNGSubDocument *subDocument, RVNGSubDocumentType subDocumentType, RVNGTableList tableList, int nextTableIndice = 0);

private:
	WP5StylesListener(const WP5StylesListener &);
	WP5StylesListener &operator=(const WP5StylesListener &);
	RVNGPageSpan m_currentPage, m_nextPage;

	RVNGTableList m_tableList;
	RVNGTable *m_currentTable;
	double m_tempMarginLeft, m_tempMarginRight;
	bool m_currentPageHasContent;
	bool m_isSubDocument;
	std::vector<WP5SubDocument *> &m_subDocuments;
	std::list<RVNGPageSpan>::iterator m_pageListHardPageMark;
};

#endif /* WP5STYLESLISTENER_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
