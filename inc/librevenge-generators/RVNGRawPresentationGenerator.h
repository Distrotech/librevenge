/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 */

#ifndef RVNGRAWPRESENTATIONGENERATOR_H
#define RVNGRAWPRESENTATIONGENERATOR_H

#include "librevenge-generators-api.h"

#include <librevenge/librevenge.h>
#include <librevenge-stream/librevenge-stream.h>

namespace librevenge
{

struct RVNGRawPresentationGeneratorImpl;

class REVENGE_GENERATORS_API RVNGRawPresentationGenerator : public RVNGPresentationInterface
{
	// disable copying
	RVNGRawPresentationGenerator(const RVNGRawPresentationGenerator &other);
	RVNGRawPresentationGenerator &operator=(const RVNGRawPresentationGenerator &other);

public:
	explicit RVNGRawPresentationGenerator(bool printCallgraphScore);
	~RVNGRawPresentationGenerator();

	void startDocument(const RVNGPropertyList &propList);
	void endDocument();
	void setDocumentMetaData(const RVNGPropertyList &propList);
	void defineEmbeddedFont(const RVNGPropertyList &propList);
	void startSlide(const RVNGPropertyList &propList);
	void endSlide();
	void startMasterSlide(const RVNGPropertyList &propList);
	void endMasterSlide();
	void setSlideTransition(const RVNGPropertyList &propList);
	void startLayer(const RVNGPropertyList &propList);
	void endLayer();
	void startEmbeddedGraphics(const RVNGPropertyList &propList);
	void endEmbeddedGraphics();
	void openGroup(const RVNGPropertyList &propList);
	void closeGroup();

	void setStyle(const RVNGPropertyList &propList);

	void drawRectangle(const RVNGPropertyList &propList);
	void drawEllipse(const RVNGPropertyList &propList);
	void drawPolyline(const RVNGPropertyList &propList);
	void drawPolygon(const RVNGPropertyList &propList);
	void drawPath(const RVNGPropertyList &propList);
	void drawGraphicObject(const RVNGPropertyList &propList);
	void drawConnector(const RVNGPropertyList &propList);

	void startTextObject(const RVNGPropertyList &propList);
	void endTextObject();
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
	void insertText(const RVNGString &str);
	void insertLineBreak();

	void insertField(const RVNGPropertyList &propList);

	void openOrderedListLevel(const RVNGPropertyList &propList);
	void openUnorderedListLevel(const RVNGPropertyList &propList);
	void closeOrderedListLevel();
	void closeUnorderedListLevel();
	void openListElement(const RVNGPropertyList &propList);
	void closeListElement();

	void startTableObject(const RVNGPropertyList &propList);
	void openTableRow(const RVNGPropertyList &propList);
	void closeTableRow();
	void openTableCell(const RVNGPropertyList &propList);
	void closeTableCell();
	void insertCoveredTableCell(const RVNGPropertyList &propList);
	void endTableObject();

	void startComment(const RVNGPropertyList &propList);
	void endComment();

	void startNotes(const RVNGPropertyList &propList);
	void endNotes();

	void defineChartStyle(const RVNGPropertyList &propList);
	void openChart(const RVNGPropertyList &propList);
	void closeChart();
	void openChartTextObject(const RVNGPropertyList &propList);
	void closeChartTextObject();
	void openChartPlotArea(const RVNGPropertyList &propList);
	void closeChartPlotArea();
	void insertChartAxis(const RVNGPropertyList &propList);
	void openChartSeries(const librevenge::RVNGPropertyList &propList);
	void closeChartSeries();

	void openAnimationSequence(const RVNGPropertyList &propList);
	void closeAnimationSequence();
	void openAnimationGroup(const RVNGPropertyList &propList);
	void closeAnimationGroup();
	void openAnimationIteration(const RVNGPropertyList &propList);
	void closeAnimationIteration();
	void insertMotionAnimation(const RVNGPropertyList &propList);
	void insertColorAnimation(const RVNGPropertyList &propList);
	void insertAnimation(const RVNGPropertyList &propList);
	void insertEffect(const RVNGPropertyList &propList);

private:
	RVNGRawPresentationGeneratorImpl *m_impl;
};

}

#endif // RVNGRAWPRESENTATIONGENERATOR_H

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
