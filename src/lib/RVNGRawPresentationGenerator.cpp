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

#include <stdio.h>

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

struct RVNGRawPresentationGeneratorImpl
{
};

RVNGRawPresentationGenerator::RVNGRawPresentationGenerator()
	: m_impl(0)
{
}

RVNGRawPresentationGenerator::~RVNGRawPresentationGenerator()
{
}

void RVNGRawPresentationGenerator::startDocument(const RVNGPropertyList &propList)
{
	printf("RVNGRawPresentationGenerator::startDocument(%s)\n", getPropString(propList).cstr());
}

void RVNGRawPresentationGenerator::endDocument()
{
	printf("RVNGRawPresentationGenerator::endDocument\n");
}

void RVNGRawPresentationGenerator::setDocumentMetaData(const RVNGPropertyList &propList)
{
	printf("RVNGRawPresentationGenerator::setDocumentMetaData(%s)\n", getPropString(propList).cstr());
}

void RVNGRawPresentationGenerator::startSlide(const RVNGPropertyList &propList)
{
	printf("RVNGRawPresentationGenerator::startSlide(%s)\n", getPropString(propList).cstr());
}

void RVNGRawPresentationGenerator::endSlide()
{
	printf("RVNGRawPresentationGenerator::endSlide\n");
}

void RVNGRawPresentationGenerator::startLayer(const RVNGPropertyList &propList)
{
	printf("RVNGRawPresentationGenerator::startLayer (%s)\n", getPropString(propList).cstr());
}

void RVNGRawPresentationGenerator::endLayer()
{
	printf("RVNGRawPresentationGenerator::endLayer\n");
}

void RVNGRawPresentationGenerator::startEmbeddedGraphics(const RVNGPropertyList &propList)
{
	printf("RVNGRawPresentationGenerator::startEmbeddedGraphics (%s)\n", getPropString(propList).cstr());
}

void RVNGRawPresentationGenerator::endEmbeddedGraphics()
{
	printf("RVNGRawPresentationGenerator::endEmbeddedGraphics \n");
}

void RVNGRawPresentationGenerator::startGroup(const RVNGPropertyList &propList)
{
	printf("RVNGRawPresentationGenerator::startGroup(%s)\n", getPropString(propList).cstr());
}

void RVNGRawPresentationGenerator::endGroup()
{
	printf("RVNGRawPresentationGenerator::endGroup\n");
}

void RVNGRawPresentationGenerator::setStyle(const RVNGPropertyList &propList, const RVNGPropertyListVector &gradient)
{
	printf("RVNGRawPresentationGenerator::setStyle(%s, gradient: (%s))\n", getPropString(propList).cstr(), getPropString(gradient).cstr());
}

void RVNGRawPresentationGenerator::drawRectangle(const RVNGPropertyList &propList)
{
	printf("RVNGRawPresentationGenerator::drawRectangle (%s)\n", getPropString(propList).cstr());
}

void RVNGRawPresentationGenerator::drawEllipse(const RVNGPropertyList &propList)
{
	printf("RVNGRawPresentationGenerator::drawEllipse (%s)\n", getPropString(propList).cstr());
}

void RVNGRawPresentationGenerator::drawPolyline(const RVNGPropertyListVector &vertices)
{
	printf("RVNGRawPresentationGenerator::drawPolyline (%s)\n", getPropString(vertices).cstr());
}

void RVNGRawPresentationGenerator::drawPolygon(const RVNGPropertyListVector &vertices)
{
	printf("RVNGRawPresentationGenerator::drawPolygon (%s)\n", getPropString(vertices).cstr());
}

void RVNGRawPresentationGenerator::drawPath(const RVNGPropertyListVector &path)
{
	printf("RVNGRawPresentationGenerator::drawPath (%s)\n", getPropString(path).cstr());
}

void RVNGRawPresentationGenerator::drawGraphicObject(const RVNGPropertyList &propList, const RVNGBinaryData & /*binaryData*/)
{
	printf("RVNGRawPresentationGenerator::drawGraphicObject (%s)\n", getPropString(propList).cstr());
}

void RVNGRawPresentationGenerator::drawConnector(const RVNGPropertyList &propList, const RVNGPropertyListVector &path)
{
	printf("RVNGRawPresentationGenerator::drawConnector(%s, path: (%s))\n", getPropString(propList).cstr(), getPropString(path).cstr());
}

void RVNGRawPresentationGenerator::startTextObject(const RVNGPropertyList &propList, const RVNGPropertyListVector &path)
{
	printf("RVNGRawPresentationGenerator::startTextObject (%s, path: (%s))\n", getPropString(propList).cstr(), getPropString(path).cstr());
}

void RVNGRawPresentationGenerator::endTextObject()
{
	printf("RVNGRawPresentationGenerator::endTextObject\n");
}

void RVNGRawPresentationGenerator::openParagraph(const RVNGPropertyList &propList, const RVNGPropertyListVector &tabStops)
{
	printf("RVNGRawPresentationGenerator::openParagraph (%s, tabStops: (%s))\n", getPropString(propList).cstr(), getPropString(tabStops).cstr());
}

void RVNGRawPresentationGenerator::closeParagraph()
{
	printf("RVNGRawPresentationGenerator::closeParagraph\n");
}

void RVNGRawPresentationGenerator::openSpan(const RVNGPropertyList &propList)
{
	printf("RVNGRawPresentationGenerator::openSpan (%s)\n", getPropString(propList).cstr());
}

void RVNGRawPresentationGenerator::closeSpan()
{
	printf("RVNGRawPresentationGenerator::closeSpan\n");
}

void RVNGRawPresentationGenerator::insertText(const RVNGString &str)
{
	printf("RVNGRawPresentationGenerator::insertText (%s)\n", str.cstr());
}

void RVNGRawPresentationGenerator::insertTab()
{
	printf("RVNGRawPresentationGenerator::insertTab\n");
}

void RVNGRawPresentationGenerator::insertSpace()
{
	printf("RVNGRawPresentationGenerator::insertSpace\n");
}

void RVNGRawPresentationGenerator::insertLineBreak()
{
	printf("RVNGRawPresentationGenerator::insertLineBreak\n");
}

void RVNGRawPresentationGenerator::insertField(const RVNGString &type, const RVNGPropertyList &propList)
{
	printf("RVNGRawPresentationGenerator::insertField(%s, %s)\n", type.cstr(), getPropString(propList).cstr());
}

void RVNGRawPresentationGenerator::openOrderedListLevel(const RVNGPropertyList &propList)
{
	printf("RVNGRawPresentationGenerator::openOrderedListLevel(%s)\n", getPropString(propList).cstr());
}

void RVNGRawPresentationGenerator::openUnorderedListLevel(const RVNGPropertyList &propList)
{
	printf("RVNGRawPresentationGenerator::openUnorderedListLevel(%s)\n", getPropString(propList).cstr());
}

void RVNGRawPresentationGenerator::closeOrderedListLevel()
{
	printf("RVNGRawPresentationGenerator::closeOrderedListLevel\n");
}

void RVNGRawPresentationGenerator::closeUnorderedListLevel()
{
	printf("RVNGRawPresentationGenerator::closeUnorderedListLevel\n");
}

void RVNGRawPresentationGenerator::openListElement(const RVNGPropertyList &propList, const RVNGPropertyListVector &tabStops)
{
	printf("RVNGRawPresentationGenerator::openListElement(%s, tabStops: (%s))\n", getPropString(propList).cstr(), getPropString(tabStops).cstr());
}

void RVNGRawPresentationGenerator::closeListElement()
{
	printf("RVNGRawPresentationGenerator::closeListElement\n");
}

void RVNGRawPresentationGenerator::openTable(const RVNGPropertyList &propList, const RVNGPropertyListVector &columns)
{
	printf("RVNGRawPresentationGenerator::openTable(%s, columns: (%s))\n", getPropString(propList).cstr(), getPropString(columns).cstr());
}

void RVNGRawPresentationGenerator::openTableRow(const RVNGPropertyList &propList)
{
	printf("RVNGRawPresentationGenerator::openTableRow(%s)\n", getPropString(propList).cstr());
}

void RVNGRawPresentationGenerator::closeTableRow()
{
	printf("RVNGRawPresentationGenerator::closeTableRow\n");
}

void RVNGRawPresentationGenerator::openTableCell(const RVNGPropertyList &propList)
{
	printf("RVNGRawPresentationGenerator::openTableCell(%s)\n", getPropString(propList).cstr());
}

void RVNGRawPresentationGenerator::closeTableCell()
{
	printf("RVNGRawPresentationGenerator::closeTableCell\n");
}

void RVNGRawPresentationGenerator::insertCoveredTableCell(const RVNGPropertyList &propList)
{
	printf("RVNGRawPresentationGenerator::insertCoveredTableCell(%s)\n", getPropString(propList).cstr());
}

void RVNGRawPresentationGenerator::closeTable()
{
	printf("RVNGRawPresentationGenerator::closeTable\n");
}

void RVNGRawPresentationGenerator::startComment(const RVNGPropertyList &propList)
{
	printf("RVNGRawPresentationGenerator::startComment(%s)\n", getPropString(propList).cstr());
}

void RVNGRawPresentationGenerator::endComment()
{
	printf("RVNGRawPresentationGenerator::endComment\n");
}

void RVNGRawPresentationGenerator::startNotes(const RVNGPropertyList &propList)
{
	printf("RVNGRawPresentationGenerator::startNotes(%s)\n", getPropString(propList).cstr());
}

void RVNGRawPresentationGenerator::endNotes()
{
	printf("RVNGRawPresentationGenerator::endNotes\n");
}

}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
