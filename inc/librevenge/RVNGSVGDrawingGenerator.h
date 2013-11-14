/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2006 Ariya Hidayat (ariya@kde.org)
 * Copyright (C) 2005 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 */

#ifndef RVNGSVGDRAWINGGENERATOR_H
#define RVNGSVGDRAWINGGENERATOR_H

#include "RVNGDrawingInterface.h"
#include "RVNGStringVector.h"

namespace librevenge
{

struct RVNGSVGDrawingGeneratorPrivate;

class RVNGSVGDrawingGenerator : public RVNGDrawingInterface
{
public:
	RVNGSVGDrawingGenerator(RVNGStringVector &vec, const RVNGString &nmspace);
	~RVNGSVGDrawingGenerator();

	void startDocument(const RVNGPropertyList &propList);
	void endDocument();
	void setDocumentMetaData(const RVNGPropertyList &propList);
	void startPage(const RVNGPropertyList &propList);
	void endPage();
	void startLayer(const RVNGPropertyList &propList);
	void endLayer();
	void startEmbeddedGraphics(const RVNGPropertyList &propList);
	void endEmbeddedGraphics();

	void setStyle(const RVNGPropertyList &propList, const RVNGPropertyListVector &gradient);

	void drawRectangle(const RVNGPropertyList &propList);
	void drawEllipse(const RVNGPropertyList &propList);
	void drawPolyline(const RVNGPropertyListVector &vertices);
	void drawPolygon(const RVNGPropertyListVector &vertices);
	void drawPath(const RVNGPropertyListVector &path);
	void drawGraphicObject(const RVNGPropertyList &propList);
	void startTextObject(const RVNGPropertyList &propList, const RVNGPropertyListVector &path);
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
	RVNGSVGDrawingGenerator(const RVNGSVGDrawingGenerator &);
	RVNGSVGDrawingGenerator &operator=(const RVNGSVGDrawingGenerator &);
	RVNGSVGDrawingGeneratorPrivate *m_pImpl;
};

}

#endif // RVNGSVGDRAWINGGENERATOR_H

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
