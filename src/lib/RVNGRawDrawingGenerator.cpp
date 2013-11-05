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
 * Copyright (C) 2004 Marc Oude Kotte (marc@solcon.nl)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 */

#include <stdio.h>
#include <string.h>

#include <librevenge-generators/librevenge-generators.h>

namespace librevenge
{

namespace
{

RVNGString getPropString(const RVNGPropertyList &propList)
{
	RVNGString propString;
	RVNGPropertyList::Iter i(propList);
	if (!i.last())
	{
		propString.append(i.key());
		propString.append(": ");
		propString.append(i()->getStr().cstr());
		for (; i.next(); )
		{
			propString.append(", ");
			propString.append(i.key());
			propString.append(": ");
			propString.append(i()->getStr().cstr());
		}
	}

	return propString;
}

RVNGString getPropString(const RVNGPropertyListVector &itemList)
{
	RVNGString propString;

	propString.append("(");
	RVNGPropertyListVector::Iter i(itemList);

	if (!i.last())
	{
		propString.append("(");
		propString.append(getPropString(i()));
		propString.append(")");

		for (; i.next();)
		{
			propString.append(", (");
			propString.append(getPropString(i()));
			propString.append(")");
		}

	}
	propString.append(")");

	return propString;
}

}

RVNGRawDrawingGenerator::RVNGRawDrawingGenerator(): RVNGDrawingInterface()
{
}

void RVNGRawDrawingGenerator::startDocument(const RVNGPropertyList &propList)
{
	printf("RVNGRawDrawingGenerator::startDocument(%s)\n", getPropString(propList).cstr());
}

void RVNGRawDrawingGenerator::endDocument()
{
	printf("RVNGRawDrawingGenerator::endDocument\n");
}

void RVNGRawDrawingGenerator::setDocumentMetaData(const RVNGPropertyList &propList)
{
	printf("RVNGRawDrawingGenerator::setDocumentMetaData((%s)\n", getPropString(propList).cstr());
}

void RVNGRawDrawingGenerator::startPage(const RVNGPropertyList &propList)
{
	printf("RVNGRawDrawingGenerator::startPage(%s)\n", getPropString(propList).cstr());
}

void RVNGRawDrawingGenerator::endPage()
{
	printf("RVNGRawDrawingGenerator::endPage\n");
}

void RVNGRawDrawingGenerator::startLayer(const RVNGPropertyList &propList)
{
	printf("RVNGRawDrawingGenerator::startLayer (%s)\n", getPropString(propList).cstr());
}

void RVNGRawDrawingGenerator::endLayer()
{
	printf("RVNGRawDrawingGenerator::endLayer\n");
}

void RVNGRawDrawingGenerator::startEmbeddedGraphics(const RVNGPropertyList &propList)
{
	printf("RVNGRawDrawingGenerator::startEmbeddedGraphics (%s)\n", getPropString(propList).cstr());
}

void RVNGRawDrawingGenerator::endEmbeddedGraphics()
{
	printf("RVNGRawDrawingGenerator::endEmbeddedGraphics \n");
}

void RVNGRawDrawingGenerator::setStyle(const RVNGPropertyList &propList, const RVNGPropertyListVector &gradient)
{
	printf("RVNGRawDrawingGenerator::setStyle(%s, gradient: (%s))\n", getPropString(propList).cstr(), getPropString(gradient).cstr());
}

void RVNGRawDrawingGenerator::drawRectangle(const RVNGPropertyList &propList)
{
	printf("RVNGRawDrawingGenerator::drawRectangle (%s)\n", getPropString(propList).cstr());
}

void RVNGRawDrawingGenerator::drawEllipse(const RVNGPropertyList &propList)
{
	printf("RVNGRawDrawingGenerator::drawEllipse (%s)\n", getPropString(propList).cstr());
}

void RVNGRawDrawingGenerator::drawPolyline(const RVNGPropertyListVector &vertices)
{
	printf("RVNGRawDrawingGenerator::drawPolyline (%s)\n", getPropString(vertices).cstr());
}

void RVNGRawDrawingGenerator::drawPolygon(const RVNGPropertyListVector &vertices)
{
	printf("RVNGRawDrawingGenerator::drawPolygon (%s)\n", getPropString(vertices).cstr());
}

void RVNGRawDrawingGenerator::drawPath(const RVNGPropertyListVector &path)
{
	printf("RVNGRawDrawingGenerator::drawPath (%s)\n", getPropString(path).cstr());
}

void RVNGRawDrawingGenerator::drawGraphicObject(const RVNGPropertyList &propList, const RVNGBinaryData & /*binaryData*/)
{
	printf("RVNGRawDrawingGenerator::drawGraphicObject (%s)\n", getPropString(propList).cstr());
}

void RVNGRawDrawingGenerator::startTextObject(const RVNGPropertyList &propList, const RVNGPropertyListVector &path)
{
	printf("RVNGRawDrawingGenerator::startTextObject (%s, path: (%s))\n", getPropString(propList).cstr(), getPropString(path).cstr());
}

void RVNGRawDrawingGenerator::endTextObject()
{
	printf("RVNGRawDrawingGenerator::endTextObject\n");
}

void RVNGRawDrawingGenerator::openParagraph(const RVNGPropertyList &propList, const RVNGPropertyListVector &tabStops)
{
	printf("RVNGRawDrawingGenerator::openParagraph (%s, tabs (%s))\n", getPropString(propList).cstr(), getPropString(tabStops).cstr());
}

void RVNGRawDrawingGenerator::closeParagraph()
{
	printf("RVNGRawDrawingGenerator::closeParagraph\n");
}

void RVNGRawDrawingGenerator::openListElement(const RVNGPropertyList &propList, const RVNGPropertyListVector &tabStops)
{
	printf("RVNGRawDrawingGenerator::openListElement (%s, tabs (%s))\n", getPropString(propList).cstr(), getPropString(tabStops).cstr());
}

void RVNGRawDrawingGenerator::closeListElement()
{
	printf("RVNGRawDrawingGenerator::closeListElement\n");
}

void RVNGRawDrawingGenerator::openSpan(const RVNGPropertyList &propList)
{
	printf("RVNGRawDrawingGenerator::openSpan (%s)\n", getPropString(propList).cstr());
}

void RVNGRawDrawingGenerator::closeSpan()
{
	printf("RVNGRawDrawingGenerator::closeSpan\n");
}

void RVNGRawDrawingGenerator::insertTab()
{
	printf("RVNGRawDrawingGenerator::insertTab\n");
}

void RVNGRawDrawingGenerator::insertSpace()
{
	printf("RVNGRawDrawingGenerator::insertSpace\n");
}

void RVNGRawDrawingGenerator::insertLineBreak()
{
	printf("RVNGRawDrawingGenerator::insertLineBreak\n");
}

void RVNGRawDrawingGenerator::insertText(const RVNGString &str)
{
	printf("RVNGRawDrawingGenerator::insertText (%s)\n", str.cstr());
}

void RVNGRawDrawingGenerator::insertField(const RVNGString &type, const RVNGPropertyList &propList)
{
	printf("RVNGRawDrawingGenerator::insertField(%s, type %s)\n", getPropString(propList).cstr(), type.cstr());
}

void RVNGRawDrawingGenerator::openOrderedListLevel(const RVNGPropertyList &propList)
{
	printf("RVNGRawDrawingGenerator::openOrderedListLevel(%s)\n", getPropString(propList).cstr());
}

void RVNGRawDrawingGenerator::closeOrderedListLevel()
{
	printf("RVNGRawDrawingGenerator::closeOrderedListLevel\n");
}

void RVNGRawDrawingGenerator::openUnorderedListLevel(const RVNGPropertyList &propList)
{
	printf("RVNGRawDrawingGenerator::openUnorderedListLevel(%s)\n", getPropString(propList).cstr());
}

void RVNGRawDrawingGenerator::closeUnorderedListLevel()
{
	printf("RVNGRawDrawingGenerator::closeUnorderedListLevel\n");
}

}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
