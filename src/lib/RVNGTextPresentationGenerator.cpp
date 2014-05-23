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

#include <librevenge-generators/librevenge-generators.h>

namespace librevenge
{

struct RVNGTextPresentationGeneratorImpl
{
	explicit RVNGTextPresentationGeneratorImpl(RVNGStringVector &pages);

	RVNGStringVector &m_pages;
	std::ostringstream m_stream;
};

RVNGTextPresentationGeneratorImpl::RVNGTextPresentationGeneratorImpl(RVNGStringVector &pages)
	: m_pages(pages)
	, m_stream()
{
}

RVNGTextPresentationGenerator::RVNGTextPresentationGenerator(RVNGStringVector &pages)
	: m_impl(new RVNGTextPresentationGeneratorImpl(pages))
{
}

RVNGTextPresentationGenerator::~RVNGTextPresentationGenerator()
{
	delete m_impl;
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

void RVNGTextPresentationGenerator::defineEmbeddedFont(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::startSlide(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::endSlide()
{
	m_impl->m_pages.append(m_impl->m_stream.str().c_str());
	m_impl->m_stream.str("");
}

void RVNGTextPresentationGenerator::startMasterSlide(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::endMasterSlide()
{
	// we don't do anything with master slides yet, so just reset the content
	m_impl->m_stream.str("");
}

void RVNGTextPresentationGenerator::setSlideTransition(const RVNGPropertyList &)
{
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

void RVNGTextPresentationGenerator::openGroup(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::closeGroup()
{
}

void RVNGTextPresentationGenerator::setStyle(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::drawRectangle(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::drawEllipse(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::drawPolyline(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::drawPolygon(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::drawPath(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::drawGraphicObject(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::drawConnector(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::startTextObject(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::endTextObject()
{
	m_impl->m_stream << '\n';
}

void RVNGTextPresentationGenerator::defineParagraphStyle(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::openParagraph(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::closeParagraph()
{
	m_impl->m_stream << '\n';
}

void RVNGTextPresentationGenerator::defineCharacterStyle(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::openSpan(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::closeSpan()
{
}

void RVNGTextPresentationGenerator::openLink(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::closeLink()
{
}

void RVNGTextPresentationGenerator::insertText(const RVNGString &str)
{
	m_impl->m_stream << str.cstr();
}

void RVNGTextPresentationGenerator::insertTab()
{
	m_impl->m_stream << '\t';
}

void RVNGTextPresentationGenerator::insertSpace()
{
	m_impl->m_stream << ' ';
}

void RVNGTextPresentationGenerator::insertLineBreak()
{
	m_impl->m_stream << '\n';
}

void RVNGTextPresentationGenerator::insertField(const RVNGPropertyList &)
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

void RVNGTextPresentationGenerator::openListElement(const RVNGPropertyList &propList)
{
	openParagraph(propList);
}

void RVNGTextPresentationGenerator::closeListElement()
{
	closeParagraph();
}

void RVNGTextPresentationGenerator::startTableObject(const RVNGPropertyList &)
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

void RVNGTextPresentationGenerator::endTableObject()
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

void RVNGTextPresentationGenerator::defineChartStyle(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::openChart(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::closeChart()
{
}

void RVNGTextPresentationGenerator::openChartTextObject(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::closeChartTextObject()
{
}

void RVNGTextPresentationGenerator::openChartPlotArea(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::closeChartPlotArea()
{
}

void RVNGTextPresentationGenerator::insertChartAxis(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::openChartSeries(const librevenge::RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::closeChartSeries()
{
}

void RVNGTextPresentationGenerator::openAnimationSequence(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::closeAnimationSequence()
{
}

void RVNGTextPresentationGenerator::openAnimationGroup(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::closeAnimationGroup()
{
}

void RVNGTextPresentationGenerator::openAnimationIteration(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::closeAnimationIteration()
{
}

void RVNGTextPresentationGenerator::insertMotionAnimation(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::insertColorAnimation(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::insertAnimation(const RVNGPropertyList &)
{
}

void RVNGTextPresentationGenerator::insertEffect(const RVNGPropertyList &)
{
}

}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
