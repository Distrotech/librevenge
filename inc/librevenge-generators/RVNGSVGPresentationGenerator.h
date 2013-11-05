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
	virtual ~RVNGSVGPresentationGenerator();

	virtual void startDocument(const RVNGPropertyList &propList);
	virtual void endDocument();

	virtual void setDocumentMetaData(const RVNGPropertyList &propList);

	virtual void startSlide(const RVNGPropertyList &propList);
	virtual void endSlide();

	virtual void setStyle(const RVNGPropertyList &propList, const RVNGPropertyListVector &gradient);

	virtual void startLayer(const RVNGPropertyList &propList);
	virtual void endLayer();

	virtual void startEmbeddedGraphics(const RVNGPropertyList & /*propList*/) {}
	virtual void endEmbeddedGraphics() {}

	virtual void startGroup(const RVNGPropertyList &propList);
	virtual void endGroup();

	virtual void drawRectangle(const RVNGPropertyList &propList);
	virtual void drawEllipse(const RVNGPropertyList &propList);
	virtual void drawPolyline(const RVNGPropertyListVector &vertices);
	virtual void drawPolygon(const RVNGPropertyListVector &vertices);
	virtual void drawPath(const RVNGPropertyListVector &path);
	virtual void drawGraphicObject(const RVNGPropertyList &propList, const RVNGBinaryData &binaryData);

	virtual void drawConnector(const RVNGPropertyList &propList, const RVNGPropertyListVector &path);

	virtual void startTextObject(const RVNGPropertyList &propList, const RVNGPropertyListVector &path);
	virtual void endTextObject();
	virtual void insertTab();
	virtual void insertSpace();
	virtual void insertText(const RVNGString &text);
	virtual void insertLineBreak();

	virtual void insertField(const RVNGString &type, const RVNGPropertyList &propList);

	virtual void openOrderedListLevel(const RVNGPropertyList &propList);
	virtual void openUnorderedListLevel(const RVNGPropertyList &propList);
	virtual void closeOrderedListLevel();
	virtual void closeUnorderedListLevel();
	virtual void openListElement(const RVNGPropertyList &propList, const RVNGPropertyListVector &tabStops);
	virtual void closeListElement();

	virtual void openParagraph(const RVNGPropertyList &propList, const RVNGPropertyListVector &tabStops);
	virtual void closeParagraph();
	virtual void openSpan(const RVNGPropertyList &propList);
	virtual void closeSpan();

	virtual void openTable(const RVNGPropertyList &propList, const RVNGPropertyListVector &columns);
	virtual void openTableRow(const RVNGPropertyList &propList);
	virtual void closeTableRow();
	virtual void openTableCell(const RVNGPropertyList &propList);
	virtual void closeTableCell();
	virtual void insertCoveredTableCell(const RVNGPropertyList &propList);
	virtual void closeTable();

	virtual void startComment(const RVNGPropertyList &propList);
	virtual void endComment();

	virtual void startNotes(const RVNGPropertyList &propList);
	virtual void endNotes();

private:
	RVNGSVGPresentationGeneratorImpl *m_impl;
};

} // namespace libetonyek

#endif // RVNGSVGPRESENTATIONGENERATOR_H

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
