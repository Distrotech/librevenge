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
 * Copyright (C) 2002-2004 Marc Maurer (uwog@uwog.net)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 */

#ifndef RVNGRAWSPREADSHEETGENERATOR_H
#define RVNGRAWSPREADSHEETGENERATOR_H

#include <librevenge/librevenge.h>
#include <librevenge-stream/librevenge-stream.h>

namespace librevenge
{

struct RVNGRawSpreadsheetGeneratorImpl;

class RVNGRawSpreadsheetGenerator : public RVNGSpreadsheetInterface
{
	// disable copying
	RVNGRawSpreadsheetGenerator(const RVNGRawSpreadsheetGenerator &other);
	RVNGRawSpreadsheetGenerator &operator=(const RVNGRawSpreadsheetGenerator &other);

public:
	explicit RVNGRawSpreadsheetGenerator(bool printCallgraphScore);
	virtual ~RVNGRawSpreadsheetGenerator();

	virtual void setDocumentMetaData(const RVNGPropertyList &propList);

	virtual void startDocument();
	virtual void endDocument();

	// sheet
	virtual void defineSheetNumberingStyle(const RVNGPropertyList &propList);
	virtual void insertSheetConditionInNumberingStyle(const RVNGPropertyList &propList);
	virtual void openSheet(const RVNGPropertyList &propList);
	virtual void closeSheet();
	virtual void openSheetRow(const RVNGPropertyList &propList);
	virtual void closeSheetRow();
	virtual void openSheetCell(const RVNGPropertyList &propList);
	virtual void closeSheetCell();

	// chart
	virtual void openChart(const RVNGPropertyList &propList);
	virtual void closeChart();
	virtual void insertChartSerie(const RVNGPropertyList &series);

	virtual void definePageStyle(const RVNGPropertyList &propList);
	virtual void openPageSpan(const RVNGPropertyList &propList);
	virtual void closePageSpan();
	virtual void openHeader(const RVNGPropertyList &propList);
	virtual void closeHeader();
	virtual void openFooter(const RVNGPropertyList &propList);
	virtual void closeFooter();

	virtual void defineParagraphStyle(const RVNGPropertyList &propList);
	virtual void openParagraph(const RVNGPropertyList &propList);
	virtual void closeParagraph();

	virtual void defineCharacterStyle(const RVNGPropertyList &propList);
	virtual void openSpan(const RVNGPropertyList &propList);
	virtual void closeSpan();

	virtual void defineSectionStyle(const RVNGPropertyList &propList);
	virtual void openSection(const RVNGPropertyList &propList);
	virtual void closeSection();

	virtual void insertTab();
	virtual void insertSpace();
	virtual void insertText(const RVNGString &text);
	virtual void insertLineBreak();
	virtual void insertField(const RVNGPropertyList &propList);

	virtual void defineOrderedListLevel(const RVNGPropertyList &propList);
	virtual void defineUnorderedListLevel(const RVNGPropertyList &propList);
	virtual void openOrderedListLevel(const RVNGPropertyList &propList);
	virtual void openUnorderedListLevel(const RVNGPropertyList &propList);
	virtual void closeOrderedListLevel();
	virtual void closeUnorderedListLevel();
	virtual void openListElement(const RVNGPropertyList &propList);
	virtual void closeListElement();

	virtual void openFootnote(const RVNGPropertyList &propList);
	virtual void closeFootnote();
	virtual void openEndnote(const RVNGPropertyList &propList);
	virtual void closeEndnote();
	virtual void openComment(const RVNGPropertyList &propList);
	virtual void closeComment();

	virtual void openTextBox(const RVNGPropertyList &propList);
	virtual void closeTextBox();

	virtual void openTable(const RVNGPropertyList &propList);
	virtual void openTableRow(const RVNGPropertyList &propList);
	virtual void closeTableRow();
	virtual void openTableCell(const RVNGPropertyList &propList);
	virtual void closeTableCell();
	virtual void insertCoveredTableCell(const RVNGPropertyList &propList);
	virtual void closeTable();

	virtual void openFrame(const RVNGPropertyList &propList);
	virtual void closeFrame();

	virtual void insertBinaryObject(const RVNGPropertyList &propList);

	// drawing function
	virtual void startGraphic(const RVNGPropertyList &propList);
	virtual void endGraphic();
	virtual void startGraphicPage(const RVNGPropertyList &propList);
	virtual void endGraphicPage();
	virtual void setGraphicStyle(const RVNGPropertyList &propList);
	virtual void startGraphicLayer(const RVNGPropertyList &propList);
	virtual void endGraphicLayer();
	virtual void drawRectangle(const RVNGPropertyList &propList);
	virtual void drawEllipse(const RVNGPropertyList &propList);
	virtual void drawPolygon(const RVNGPropertyList &propList);
	virtual void drawPolyline(const RVNGPropertyList &propList);
	virtual void drawPath(const RVNGPropertyList &propList);

	virtual void insertEquation(const RVNGPropertyList &propList);

private:
	RVNGRawSpreadsheetGeneratorImpl *m_impl;
};

}

#endif /* RVNGRAWSPREADSHEETGENERATOR_H */

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
