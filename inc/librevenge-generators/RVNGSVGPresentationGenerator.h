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

#ifndef RVNGSVGPRESENTATIONGENERATOR_H
#define RVNGSVGPRESENTATIONGENERATOR_H

#include <librevenge/librevenge.h>
#include <librevenge-stream/librevenge-stream.h>

namespace librevenge
{

struct RVNGSVGPresentationGeneratorImpl;

class RVNGSVGPresentationGenerator : public RVNGPresentationInterface
{
	// disable copying
	RVNGSVGPresentationGenerator(const RVNGSVGPresentationGenerator &other);
	RVNGSVGPresentationGenerator &operator=(const RVNGSVGPresentationGenerator &other);

public:
	explicit RVNGSVGPresentationGenerator(RVNGStringVector &vec);
	~RVNGSVGPresentationGenerator();

	void startDocument(const RVNGPropertyList &propList);
	void endDocument();

	void setDocumentMetaData(const RVNGPropertyList &propList);

	void startSlide(const RVNGPropertyList &propList);
	void endSlide();

	void setStyle(const RVNGPropertyList &propList);

	void startLayer(const RVNGPropertyList &propList);
	void endLayer();

	void startEmbeddedGraphics(const RVNGPropertyList &propList);
	void endEmbeddedGraphics();

	void startGroup(const RVNGPropertyList &propList);
	void endGroup();

	void drawRectangle(const RVNGPropertyList &propList);
	void drawEllipse(const RVNGPropertyList &propList);
	void drawPolyline(const RVNGPropertyList &propList);
	void drawPolygon(const RVNGPropertyList &propList);
	void drawPath(const RVNGPropertyList &propList);
	void drawGraphicObject(const RVNGPropertyList &propList);

	void drawConnector(const RVNGPropertyList &propList);

	void startTextObject(const RVNGPropertyList &propList);
	void endTextObject();
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

	void openParagraph(const RVNGPropertyList &propList);
	void closeParagraph();
	void openSpan(const RVNGPropertyList &propList);
	void closeSpan();

	void openLink(const RVNGPropertyList &propList);
	void closeLink();

	void openTable(const RVNGPropertyList &propList);
	void openTableRow(const RVNGPropertyList &propList);
	void closeTableRow();
	void openTableCell(const RVNGPropertyList &propList);
	void closeTableCell();
	void insertCoveredTableCell(const RVNGPropertyList &propList);
	void closeTable();

	void startComment(const RVNGPropertyList &propList);
	void endComment();

	void startNotes(const RVNGPropertyList &propList);
	void endNotes();

private:
	RVNGSVGPresentationGeneratorImpl *m_impl;
};

} // namespace libetonyek

#endif // RVNGSVGPRESENTATIONGENERATOR_H

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
