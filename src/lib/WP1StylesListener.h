/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2003 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2004 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
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
	void insertCharacter(uint32_t /* character */)
	{
		if (!isUndoOn()) m_currentPageHasContent = true;
	}
	void insertExtendedCharacter(uint8_t /* extendedCharacter */)
	{
		if (!isUndoOn()) m_currentPageHasContent = true;
	}
	void insertTab()
	{
		if (!isUndoOn()) m_currentPageHasContent = true;
	}
	void insertEOL()
	{
		if (!isUndoOn()) m_currentPageHasContent = true;
	}
	void insertBreak(uint8_t breakType);
	void insertNote(WPXNoteType /* noteType */, WP1SubDocument * /* subDocument */) {}
	void attributeChange(bool /* isOn */, uint8_t /* attribute */) {}
	void fontPointSize(uint8_t /* pointSize */) {}
	void fontId(uint16_t /* id */) {}
	void marginReset(uint16_t leftMargin, uint16_t rightMargin);
	void topMarginSet(uint16_t topMargin);
	void bottomMarginSet(uint16_t bottomMargin);
	void leftIndent(uint16_t /* leftMarginOffset */)
	{
		if (!isUndoOn()) m_currentPageHasContent = true;
	}
	void leftRightIndent(uint16_t /* leftRightMarginOffset */)
	{
		if (!isUndoOn()) m_currentPageHasContent = true;
	}
	void leftMarginRelease(uint16_t /* release */) {}
	void setTabs(const std::vector<WPXTabStop> & /* tabStops */) {}
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
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
