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
 * Copyright (C) 2003 Marc Maurer (uwog@uwog.net)
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

// WP6StylesListener: creates an intermediate table representation, given a
// sequence of messages passed to it by the parser.

#ifndef WP6STYLESLISTENER_H
#define WP6STYLESLISTENER_H

#include "WP6Listener.h"
#include "WPXStylesListener.h"
#include <vector>
#include <set>
#include <list>
#include "WPXPageSpan.h"
#include "WPXTable.h"

class WPXSubDocument;

class WP6StylesListener : public WP6Listener, protected WPXStylesListener
{
public:
	WP6StylesListener(std::list<WPXPageSpan> &pageList, WPXTableList tableList);

	void setDate(const uint16_t /* type */, const uint16_t /* year */,
	             const uint8_t /* month */, const uint8_t /* day */,
	             const uint8_t /* hour */, const uint8_t /* minute */,
	             const uint8_t /* second */, const uint8_t /* dayOfWeek */,
	             const uint8_t /* timeZone */, const uint8_t /* unused */) {}
	void setExtendedInformation(const uint16_t /* type */, const WPXString & /*data*/) {}
	void startDocument() {}
	void startSubDocument() {}
	void setAlignmentCharacter(const uint32_t /* character */) {}
	void setLeaderCharacter(const uint32_t /* character */, const uint8_t /* numberOfSpaces */) {}
	void defineTabStops(const bool /* isRelative */, const std::vector<WPXTabStop> & /* tabStops */,
	                    const std::vector<bool> & /* usePreWP9LeaderMethods */) {}
	void insertCharacter(uint32_t /* character */)
	{
		if (!isUndoOn()) m_currentPageHasContent = true;
	}
	void insertTab(const uint8_t /* tabType */, double /* tabPosition */)
	{
		if (!isUndoOn()) m_currentPageHasContent = true;
	}
	void handleLineBreak()
	{
		if (!isUndoOn()) m_currentPageHasContent = true;
	}
	void insertEOL()
	{
		if (!isUndoOn()) m_currentPageHasContent = true;
	}
	void insertBreak(const uint8_t breakType);
	void characterColorChange(const uint8_t /* red */, const uint8_t /* green */, const uint8_t /* blue */) {}
	void characterShadingChange(const uint8_t /* shading */) {}
	void highlightChange(const bool /* isOn */, const RGBSColor & /* color */) {}
	void fontChange(const uint16_t /* matchedFontPointSize */, const uint16_t /* fontPID */, const WPXString & /* fontName */) {}
	void attributeChange(const bool /* isOn */, const uint8_t /* attribute */) {}
	void lineSpacingChange(const double /* lineSpacing */) {}
	void spacingAfterParagraphChange(const double /* spacingRelative */, const double /* spacingAbsolute */) {}
	void justificationChange(const uint8_t /* justification */) {}
	void pageNumberingChange(const WPXPageNumberPosition /* page numbering position */, const uint16_t /* matchedFontPointSize */, const uint16_t /* fontPID */);
	void pageMarginChange(const uint8_t side, const uint16_t margin);
	void pageFormChange(const uint16_t length, const uint16_t width, const WPXFormOrientation orientation);
	void marginChange(const uint8_t side, const uint16_t margin);
	void paragraphMarginChange(const uint8_t /* side */, const int16_t /* margin */) {}
	void indentFirstLineChange(const int16_t /* offset */) {}
	void columnChange(const WPXTextColumnType /* columnType */, const uint8_t /* numColumns */,
	                  const std::vector<double> & /* columnWidth */, const std::vector<bool> & /* isFixedWidth */) {}
	void updateOutlineDefinition(const WP6OutlineLocation /* outlineLocation */, const uint16_t /* outlineHash */,
	                             const uint8_t * /* numberingMethods */, const uint8_t /* tabBehaviourFlag */) {}

	void paragraphNumberOn(const uint16_t /* outlineHash */, const uint8_t /* level */, const uint8_t /* flag */) {}
	void paragraphNumberOff()
	{
		if (!isUndoOn()) m_currentPageHasContent = true;
	}
	void displayNumberReferenceGroupOn(const uint8_t /* subGroup */, const uint8_t /* level */) {}
	void displayNumberReferenceGroupOff(const uint8_t /* subGroup */)
	{
		if (!isUndoOn()) m_currentPageHasContent = true;
	}
	void styleGroupOn(const uint8_t /* subGroup */) {}
	void styleGroupOff(const uint8_t /* subGroup */) {}
	void globalOn(const uint8_t /* systemStyle */) {}
	void globalOff() {}
	void noteOn(const uint16_t textPID);
	void noteOff(const WPXNoteType /* noteType */) {}
	void headerFooterGroup(const uint8_t headerFooterType, const uint8_t occurenceBits, const uint16_t textPID);
	void suppressPageCharacteristics(const uint8_t suppressCode);
	void setPageNumber(const uint16_t pageNumber);
	void setPageNumberingType(const WPXNumberingType pageNumberingType);

	void endDocument();
	void endSubDocument();

	void defineTable(const uint8_t position, const uint16_t leftOffset);
	void addTableColumnDefinition(const uint32_t /* width */, const uint32_t /* leftGutter */, const uint32_t /* rightGutter */,
	                              const uint32_t /* attributes */, const uint8_t /* alignment */) {}
	void startTable();
	void insertRow(const uint16_t rowHeight, const bool isMinimumHeight, const bool isHeaderRow);
	void insertCell(const uint8_t colSpan, const uint8_t rowSpan, const uint8_t borderBits,
	                const RGBSColor *cellFgColor, const RGBSColor *cellBgColor,
	                const RGBSColor *cellBorderColor, const WPXVerticalAlignment cellVerticalAlignment,
	                const bool useCellAttributes, const uint32_t cellAttributes);
	void endTable();
	void boxOn(const uint8_t /* anchoringType */, const uint8_t /* generalPositioningFlags */, const uint8_t /* horizontalPositioningFlags */,
	           const int16_t /* horizontalOffset */, const uint8_t /* leftColumn */, const uint8_t /* rightColumn */,
	           const uint8_t /* verticalPositioningFlags */, const int16_t /* verticalOffset */, const uint8_t /* widthFlags */,
	           const uint16_t /* width */, const uint8_t /* heightFlags */, const uint16_t /* height */, const uint8_t /* boxContentType */,
	           const uint16_t /* nativeWidth */, const uint16_t /* nativeHeight */) {}
	void boxOff() {}
	void insertGraphicsData(const uint16_t /* packetId */) {}
	void insertTextBox(const WP6SubDocument *subDocument);
	void commentAnnotation(const uint16_t textPID);

	void undoChange(const uint8_t undoType, const uint16_t undoLevel);

protected:
	void _handleSubDocument(const WPXSubDocument *subDocument, WPXSubDocumentType subDocumentType, WPXTableList tableList, int nextTableIndice = 0);

	void _flushText() {}
	void _changeList() {}

private:
	WP6StylesListener(const WP6StylesListener &);
	WP6StylesListener &operator=(const WP6StylesListener &);
	WPXPageSpan m_currentPage;

	WPXTableList m_tableList;
	WPXTable *m_currentTable;
	double m_tempMarginLeft, m_tempMarginRight;
	bool m_currentPageHasContent;
	bool m_isTableDefined;
	bool m_isSubDocument;
	std::set <const WPXSubDocument *> m_subDocuments;
	std::list<WPXPageSpan>::iterator m_pageListHardPageMark;
};

#endif /* WP6STYLESLISTENER_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
