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

#include <librevenge/librevenge.h>
#include <librevenge-stream/librevenge-stream.h>

namespace librevenge
{

struct RVNGTextPresentationGeneratorImpl;

class RVNGTextPresentationGenerator : public RVNGPresentationInterface
{
	// disable copying
	RVNGTextPresentationGenerator(const RVNGTextPresentationGenerator &other);
	RVNGTextPresentationGenerator &operator=(const RVNGTextPresentationGenerator &other);

public:
	explicit RVNGTextPresentationGenerator(RVNGStringVector &pages);
	virtual ~RVNGTextPresentationGenerator();

	virtual void startDocument(const RVNGPropertyList &propList);
	virtual void endDocument();
	virtual void setDocumentMetaData(const RVNGPropertyList &propList);
	virtual void startSlide(const RVNGPropertyList &propList);
	virtual void endSlide();
	virtual void startLayer(const RVNGPropertyList &propList);
	virtual void endLayer();
	virtual void startEmbeddedGraphics(const RVNGPropertyList &propList);
	virtual void endEmbeddedGraphics();
	virtual void startGroup(const RVNGPropertyList &propList);
	virtual void endGroup();

	virtual void setStyle(const RVNGPropertyList &propList, const RVNGPropertyListVector &gradient);

	virtual void drawRectangle(const RVNGPropertyList &propList);
	virtual void drawEllipse(const RVNGPropertyList &propList);
	virtual void drawPolyline(const RVNGPropertyListVector &vertices);
	virtual void drawPolygon(const RVNGPropertyListVector &vertices);
	virtual void drawPath(const RVNGPropertyList &propList);
	virtual void drawGraphicObject(const RVNGPropertyList &propList);
	virtual void drawConnector(const RVNGPropertyList &propList);

	virtual void startTextObject(const RVNGPropertyList &propList);
	virtual void endTextObject();
	virtual void openParagraph(const RVNGPropertyList &propList);
	virtual void closeParagraph();
	virtual void openSpan(const RVNGPropertyList &propList);
	virtual void closeSpan();
	virtual void insertTab();
	virtual void insertSpace();
	virtual void insertText(const RVNGString &str);
	virtual void insertLineBreak();

	virtual void insertField(const RVNGPropertyList &propList);

	virtual void openOrderedListLevel(const RVNGPropertyList &propList);
	virtual void openUnorderedListLevel(const RVNGPropertyList &propList);
	virtual void closeOrderedListLevel();
	virtual void closeUnorderedListLevel();
	virtual void openListElement(const RVNGPropertyList &propList);
	virtual void closeListElement();

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
	RVNGTextPresentationGeneratorImpl *m_impl;
};

}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
