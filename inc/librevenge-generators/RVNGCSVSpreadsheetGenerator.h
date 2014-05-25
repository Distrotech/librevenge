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

#ifndef RVNGCSVSPREADSHEETGENERATOR_H
#define RVNGCSVSPREADSHEETGENERATOR_H

#include "librevenge-generators-api.h"

#include <librevenge/librevenge.h>
#include <librevenge-stream/librevenge-stream.h>

namespace librevenge
{

struct RVNGCSVSpreadsheetGeneratorImpl;
/** A CSV generator for spreadsheet documents.
  *
  * See \c ::librevenge::RVNGSpreadsheetInterface for documentation of the basic interface.
  */
class REVENGE_GENERATORS_API RVNGCSVSpreadsheetGenerator : public RVNGSpreadsheetInterface
{
	//! unimplemented copy constructor to prevent copy
	RVNGCSVSpreadsheetGenerator(const RVNGCSVSpreadsheetGenerator &other);
	//! unimplemented copy operator to prevent copy
	RVNGCSVSpreadsheetGenerator &operator=(const RVNGCSVSpreadsheetGenerator &other);

public:
	/** constructor given:
		\li \c sheets a vector of string to fill
		\li \c generateFormula a flag to known if we need to generate the formula or not

		\note if generateFormula is true, the formula will be
		generated with english name, so a localized version of Excel,
		... will not recognize them
	*/
	explicit RVNGCSVSpreadsheetGenerator(RVNGStringVector &sheets, bool generateFormula=false);
	//! destructor
	~RVNGCSVSpreadsheetGenerator();
	/** set the separators:
		- \c fieldSep the field separator, default: ,
		- \c textSep the text separator, default: "
		- \c decimalSep the decimal separator, default: .
	 */
	void setSeparators(char fieldSep=',', char textSep='"', char decimalSep='.');
	//! defines the format used to export the date and the time (see strftime)
	void setDTFormats(RVNGString const &date="%m/%d/%y", RVNGString const &time="%H:%M:%S");
	void setDocumentMetaData(const RVNGPropertyList &propList);
	void defineEmbeddedFont(const RVNGPropertyList &propList);

	void startDocument(const RVNGPropertyList &propList);
	void endDocument();

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

	void defineParagraphStyle(const RVNGPropertyList &propList);
	void openParagraph(const RVNGPropertyList &propList);
	void closeParagraph();

	void defineCharacterStyle(const RVNGPropertyList &propList);
	void openSpan(const RVNGPropertyList &propList);
	void closeSpan();

	void openLink(const RVNGPropertyList &propList);
	void closeLink();

	void defineSectionStyle(const RVNGPropertyList &propList);
	void openSection(const RVNGPropertyList &propList);
	void closeSection();

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
	//! the internal storage data
	RVNGCSVSpreadsheetGeneratorImpl *m_impl;
};

}

#endif /* RVNGCSVSPREADSHEETGENERATOR_H */

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
