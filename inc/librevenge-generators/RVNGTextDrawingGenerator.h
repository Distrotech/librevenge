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

#ifndef RVNGTEXTDRAWINGGENERATOR_H
#define RVNGTEXTDRAWINGGENERATOR_H

#include <librevenge-stream/librevenge-stream.h>
#include <librevenge/librevenge.h>

namespace librevenge
{

struct RVNGTextDrawingGeneratorImpl;

class RVNGTextDrawingGenerator : public RVNGDrawingInterface
{
	// disable copying
	RVNGTextDrawingGenerator(const RVNGTextDrawingGenerator &other);
	RVNGTextDrawingGenerator &operator=(const RVNGTextDrawingGenerator &other);

public:
	explicit RVNGTextDrawingGenerator(RVNGStringVector &pages);
	~RVNGTextDrawingGenerator();

	void startDocument(const RVNGPropertyList &propList);
	void endDocument();
	void setDocumentMetaData(const RVNGPropertyList &propList);
	void startPage(const RVNGPropertyList &);
	void endPage();
	void startLayer(const RVNGPropertyList &);
	void endLayer();
	void startEmbeddedGraphics(const RVNGPropertyList &);
	void endEmbeddedGraphics();

	void setStyle(const RVNGPropertyList &, const RVNGPropertyListVector &);

	void drawRectangle(const RVNGPropertyList &);
	void drawEllipse(const RVNGPropertyList &);
	void drawPolyline(const RVNGPropertyListVector &);
	void drawPolygon(const RVNGPropertyListVector &);
	void drawPath(const RVNGPropertyList &);
	void drawGraphicObject(const RVNGPropertyList &);
	void startTextObject(const RVNGPropertyList &);
	void endTextObject();


	void openOrderedListLevel(const RVNGPropertyList &propList);
	void closeOrderedListLevel();

	void openUnorderedListLevel(const RVNGPropertyList &propList);
	void closeUnorderedListLevel();

	void openListElement(const RVNGPropertyList &propList, const RVNGPropertyListVector &tabStops);
	void closeListElement();

	void openParagraph(const RVNGPropertyList &propList, const RVNGPropertyListVector &tabStops);
	void closeParagraph();

	void openSpan(const RVNGPropertyList &propList);
	void closeSpan();

	void insertTab();
	void insertSpace();
	void insertText(const RVNGString &text);
	void insertLineBreak();
	void insertField(const RVNGString &type, const RVNGPropertyList &propList);

private:
	RVNGTextDrawingGeneratorImpl *m_impl;
};

}

#endif // RVNGTEXTDRAWINGGENERATOR_H

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
