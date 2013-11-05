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

struct RVNGTextPresentationGeneratorImpl
{
};

RVNGTextPresentationGenerator::RVNGTextPresentationGenerator()
	: m_impl(0)
{
}

RVNGTextPresentationGenerator::~RVNGTextPresentationGenerator()
{
}

void RVNGTextPresentationGenerator::startDocument(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::endDocument()
{
}

void RVNGTextPresentationGenerator::setDocumentMetaData(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::startSlide(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::endSlide()
{
	printf("\n");
}

void RVNGTextPresentationGenerator::startLayer(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::endLayer()
{
}

void RVNGTextPresentationGenerator::startEmbeddedGraphics(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::endEmbeddedGraphics()
{
}

void RVNGTextPresentationGenerator::startGroup(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::endGroup()
{
}

void RVNGTextPresentationGenerator::setStyle(const RVNGPropertyList &, const RVNGPropertyListVector &)
{
}

void RVNGTextPresentationGenerator::drawRectangle(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::drawEllipse(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::drawPolyline(const RVNGPropertyListVector &)
{
}

void RVNGTextPresentationGenerator::drawPolygon(const RVNGPropertyListVector &)
{
}

void RVNGTextPresentationGenerator::drawPath(const RVNGPropertyListVector &)
{
}

void RVNGTextPresentationGenerator::drawGraphicObject(const RVNGPropertyList &, const RVNGBinaryData &)
{
}

void RVNGTextPresentationGenerator::drawConnector(const RVNGPropertyList &, const RVNGPropertyListVector &)
{
}

void RVNGTextPresentationGenerator::startTextObject(const RVNGPropertyList &, const RVNGPropertyListVector &)
{
}

void RVNGTextPresentationGenerator::endTextObject()
{
	printf("\n");
}

void RVNGTextPresentationGenerator::openParagraph(const RVNGPropertyList &, const RVNGPropertyListVector &)
{
}

void RVNGTextPresentationGenerator::closeParagraph()
{
	printf("\n");
}

void RVNGTextPresentationGenerator::openSpan(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::closeSpan()
{
}

void RVNGTextPresentationGenerator::insertText(const RVNGString &str)
{
	printf("%s", str.cstr());
}

void RVNGTextPresentationGenerator::insertTab()
{
	printf("\t");
}

void RVNGTextPresentationGenerator::insertSpace()
{
	printf(" ");
}

void RVNGTextPresentationGenerator::insertLineBreak()
{
	printf("\n");
}

void RVNGTextPresentationGenerator::insertField(const RVNGString &, const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::openOrderedListLevel(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::openUnorderedListLevel(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::closeOrderedListLevel()
{
}

void RVNGTextPresentationGenerator::closeUnorderedListLevel()
{
}

void RVNGTextPresentationGenerator::openListElement(const RVNGPropertyList &propList, const RVNGPropertyListVector &tabStops)
{
	openParagraph(propList, tabStops);
}

void RVNGTextPresentationGenerator::closeListElement()
{
	closeParagraph();
}

void RVNGTextPresentationGenerator::openTable(const RVNGPropertyList &, const RVNGPropertyListVector &)
{
}

void RVNGTextPresentationGenerator::openTableRow(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::closeTableRow()
{
}

void RVNGTextPresentationGenerator::openTableCell(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::closeTableCell()
{
}

void RVNGTextPresentationGenerator::insertCoveredTableCell(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::closeTable()
{
}

void RVNGTextPresentationGenerator::startComment(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::endComment()
{
}

void RVNGTextPresentationGenerator::startNotes(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::endNotes()
{
}

}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
