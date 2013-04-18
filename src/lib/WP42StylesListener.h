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
	void insertCharacter(uint32_t /* character */)
	{
		if (!isUndoOn()) m_currentPageHasContent = true;
	}
	void insertTab(uint8_t /* tabType */, double /* tabPosition */)
	{
		if (!isUndoOn()) m_currentPageHasContent = true;
	}
	void insertEOL()
	{
		if (!isUndoOn()) m_currentPageHasContent = true;
	}
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
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
