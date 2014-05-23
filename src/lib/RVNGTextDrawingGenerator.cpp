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

#include <sstream>

#include <librevenge/librevenge.h>
#include <librevenge-generators/librevenge-generators.h>

namespace librevenge
{

struct RVNGTextDrawingGeneratorImpl
{
	RVNGTextDrawingGeneratorImpl(RVNGStringVector &pages);

	RVNGStringVector &m_pages;
	std::ostringstream m_stream;
};

RVNGTextDrawingGeneratorImpl::RVNGTextDrawingGeneratorImpl(RVNGStringVector &pages)
	: m_pages(pages)
	, m_stream()
{
}

RVNGTextDrawingGenerator::RVNGTextDrawingGenerator(RVNGStringVector &pages)
	: m_impl(new RVNGTextDrawingGeneratorImpl(pages))
{
}

RVNGTextDrawingGenerator::~RVNGTextDrawingGenerator()
{
	delete m_impl;
}

void RVNGTextDrawingGenerator::startDocument(const RVNGPropertyList & /*propList*/) {}
void RVNGTextDrawingGenerator::endDocument() {}
void RVNGTextDrawingGenerator::setDocumentMetaData(const RVNGPropertyList & /*propList*/) {}
void RVNGTextDrawingGenerator::defineEmbeddedFont(const RVNGPropertyList & /*propList*/) {}
void RVNGTextDrawingGenerator::startPage(const RVNGPropertyList &) {}

void RVNGTextDrawingGenerator::endPage()
{
	m_impl->m_pages.append(m_impl->m_stream.str().c_str());
	m_impl->m_stream.str("");
}

void RVNGTextDrawingGenerator::startMasterPage(const RVNGPropertyList &) {}

void RVNGTextDrawingGenerator::endMasterPage()
{
	// we don't do anything with master pages yet, so just reset the content
	m_impl->m_stream.str("");
}

void RVNGTextDrawingGenerator::startLayer(const RVNGPropertyList &) {}
void RVNGTextDrawingGenerator::endLayer() {}
void RVNGTextDrawingGenerator::startEmbeddedGraphics(const RVNGPropertyList &) {}
void RVNGTextDrawingGenerator::endEmbeddedGraphics() {}

void RVNGTextDrawingGenerator::openGroup(const RVNGPropertyList & /*propList*/) {}
void RVNGTextDrawingGenerator::closeGroup() {}

void RVNGTextDrawingGenerator::setStyle(const RVNGPropertyList &) {}

void RVNGTextDrawingGenerator::drawRectangle(const RVNGPropertyList &) {}
void RVNGTextDrawingGenerator::drawEllipse(const RVNGPropertyList &) {}
void RVNGTextDrawingGenerator::drawPolyline(const RVNGPropertyList &) {}
void RVNGTextDrawingGenerator::drawPolygon(const RVNGPropertyList &) {}
void RVNGTextDrawingGenerator::drawPath(const RVNGPropertyList &) {}
void RVNGTextDrawingGenerator::drawGraphicObject(const RVNGPropertyList &) {}
void RVNGTextDrawingGenerator::drawConnector(const RVNGPropertyList &) {}
void RVNGTextDrawingGenerator::startTextObject(const RVNGPropertyList &) {}
void RVNGTextDrawingGenerator::endTextObject() {}

void RVNGTextDrawingGenerator::openOrderedListLevel(const RVNGPropertyList & /*propList*/) {}
void RVNGTextDrawingGenerator::closeOrderedListLevel() {}

void RVNGTextDrawingGenerator::openUnorderedListLevel(const RVNGPropertyList & /*propList*/) {}
void RVNGTextDrawingGenerator::closeUnorderedListLevel() {}

void RVNGTextDrawingGenerator::openListElement(const RVNGPropertyList & /*propList*/) {}
void RVNGTextDrawingGenerator::closeListElement() {}

void RVNGTextDrawingGenerator::defineParagraphStyle(const RVNGPropertyList & /*propList*/) {}
void RVNGTextDrawingGenerator::openParagraph(const RVNGPropertyList & /*propList*/) {}

void RVNGTextDrawingGenerator::closeParagraph()
{
	m_impl->m_stream << "\n";
}

void RVNGTextDrawingGenerator::defineCharacterStyle(const RVNGPropertyList & /* propList */) {}
void RVNGTextDrawingGenerator::openSpan(const RVNGPropertyList & /* propList */) {}
void RVNGTextDrawingGenerator::closeSpan() {}

void RVNGTextDrawingGenerator::openLink(const RVNGPropertyList & /* propList */) {}
void RVNGTextDrawingGenerator::closeLink() {}

void RVNGTextDrawingGenerator::insertTab() {}
void RVNGTextDrawingGenerator::insertSpace() {}

void RVNGTextDrawingGenerator::insertText(const RVNGString &str)
{
	m_impl->m_stream << str.cstr();
}

void RVNGTextDrawingGenerator::insertLineBreak() {}
void RVNGTextDrawingGenerator::insertField(const RVNGPropertyList & /*propList*/) {}

void RVNGTextDrawingGenerator::startTableObject(const RVNGPropertyList &) {}
void RVNGTextDrawingGenerator::openTableRow(const RVNGPropertyList &) {}
void RVNGTextDrawingGenerator::closeTableRow() {}
void RVNGTextDrawingGenerator::openTableCell(const RVNGPropertyList &) {}
void RVNGTextDrawingGenerator::closeTableCell() {}
void RVNGTextDrawingGenerator::insertCoveredTableCell(const RVNGPropertyList &) {}
void RVNGTextDrawingGenerator::endTableObject() {}
}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
