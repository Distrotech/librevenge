/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
* Copyright (C) 2002 William Lachance (wrlach@gmail.com)
* Copyright (C) 2002-2003 Marc Maurer (uwog@uwog.net)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
*/

#ifndef RVNGTEXTSPREADSHEETGENERATOR_H
#define RVNGTEXTSPREADSHEETGENERATOR_H

#include "librevenge-generators-api.h"

#include <librevenge/librevenge.h>
#include <librevenge-stream/librevenge-stream.h>

namespace librevenge
{

struct RVNGTextSpreadsheetGeneratorImpl;
/** A text generator for spreadsheet documents.
  *
  * See \c ::librevenge::RVNGSpreadsheetInterface for documentation of the basic interface.
  */
class REVENGE_GENERATORS_API RVNGTextSpreadsheetGenerator : public RVNGSpreadsheetInterface
{
	//! unimplemented copy constructor to prevent copy
	RVNGTextSpreadsheetGenerator(const RVNGTextSpreadsheetGenerator &other);
	//! unimplemented copy operator to prevent copy
	RVNGTextSpreadsheetGenerator &operator=(const RVNGTextSpreadsheetGenerator &other);

public:
	explicit RVNGTextSpreadsheetGenerator(RVNGStringVector &sheets, const bool isInfo=false);
	~RVNGTextSpreadsheetGenerator();

	void setDocumentMetaData(const RVNGPropertyList &propList);

	void startDocument(const RVNGPropertyList &propList);
	void endDocument();

	void defineEmbeddedFont(const RVNGPropertyList &propList);

	// sheet
	void defineSheetNumberingStyle(const RVNGPropertyList &propList);
	void openSheet(const RVNGPropertyList &propList);
	void closeSheet();
	void openSheetRow(const RVNGPropertyList &propList);
	void closeSheetRow();
	void openSheetCell(const RVNGPropertyList &propList);
	void closeSheetCell();

	// chart
	void defineChartStyle(const RVNGPropertyList &propList);
	void openChart(const RVNGPropertyList &propList);
	void closeChart();
	void openChartTextObject(const RVNGPropertyList &propList);
	void closeChartTextObject();
	void openChartPlotArea(const RVNGPropertyList &propList);
	void closeChartPlotArea();
	void insertChartAxis(const RVNGPropertyList &axis);
	void openChartSerie(const librevenge::RVNGPropertyList &series);
	void closeChartSerie();

	void definePageStyle(const RVNGPropertyList &propList);
	void openPageSpan(const RVNGPropertyList &propList);
	void closePageSpan();
	void openHeader(const RVNGPropertyList &propList);
	void closeHeader();
	void openFooter(const RVNGPropertyList &propList);
	void closeFooter();

	void defineSectionStyle(const RVNGPropertyList &propList);
	void openSection(const RVNGPropertyList &propList);
	void closeSection();

	void defineParagraphStyle(const RVNGPropertyList &propList);
	void openParagraph(const RVNGPropertyList &propList);
	void closeParagraph();

	void defineCharacterStyle(const RVNGPropertyList &propList);
	void openSpan(const RVNGPropertyList &propList);
	void closeSpan();

	void openLink(const RVNGPropertyList &propList);
	void closeLink();

	void insertTab();
	void insertSpace();
	void insertText(const RVNGString &text);
	void insertLineBreak();
	void insertField(const RVNGPropertyList &propList);

	void openOrderedListLevel(const RVNGPropertyList &propList);
	void openUnorderedListLevel(const RVNGPropertyList &propList);
	void closeOrderedListLevel();
	void closeUnorderedListLevel();
	void openListElement(const RVNGPropertyList &propList);
	void closeListElement();

	void openFootnote(const RVNGPropertyList &propList);
	void closeFootnote();
	void openEndnote(const RVNGPropertyList &propList);
	void closeEndnote();
	void openComment(const RVNGPropertyList &propList);
	void closeComment();

	void openTextBox(const RVNGPropertyList &propList);
	void closeTextBox();

	void openTable(const RVNGPropertyList &propList);
	void openTableRow(const RVNGPropertyList &propList);
	void closeTableRow();
	void openTableCell(const RVNGPropertyList &propList);
	void closeTableCell();
	void insertCoveredTableCell(const RVNGPropertyList &propList);
	void closeTable();

	void openFrame(const RVNGPropertyList &propList);
	void closeFrame();

	void insertBinaryObject(const RVNGPropertyList &propList);

	// drawing function
	void openGroup(const RVNGPropertyList &propList);
	void closeGroup();

	void defineGraphicStyle(const RVNGPropertyList &propList);
	void drawRectangle(const RVNGPropertyList &propList);
	void drawEllipse(const RVNGPropertyList &propList);
	void drawPolygon(const RVNGPropertyList &propList);
	void drawPolyline(const RVNGPropertyList &propList);
	void drawPath(const RVNGPropertyList &propList);
	void drawConnector(const RVNGPropertyList &propList);

	void insertEquation(const RVNGPropertyList &propList);
private:
	RVNGTextSpreadsheetGeneratorImpl *m_impl;
};

}

#endif /* RVNGTEXTSPREADSHEETGENERATOR_H */

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
