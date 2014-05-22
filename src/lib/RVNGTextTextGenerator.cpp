/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002-2003 Marc Maurer (uwog@uwog.net)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 */

#include <sstream>

#include <librevenge-generators/librevenge-generators.h>

// use the BELL code to represent a TAB for now
#define UCS_TAB 0x0009

namespace librevenge
{

struct RVNGTextTextGeneratorImpl
{
	explicit RVNGTextTextGeneratorImpl(RVNGString &document, bool isInfo);

	RVNGString &m_document;
	std::ostringstream m_stream;
	bool m_isInfo;
};

RVNGTextTextGeneratorImpl::RVNGTextTextGeneratorImpl(RVNGString &document, const bool isInfo)
	: m_document(document)
	, m_stream()
	, m_isInfo(isInfo)
{
}

RVNGTextTextGenerator::RVNGTextTextGenerator(RVNGString &document, const bool isInfo) :
	m_impl(new RVNGTextTextGeneratorImpl(document, isInfo))
{
}

RVNGTextTextGenerator::~RVNGTextTextGenerator()
{
	delete m_impl;
}

void RVNGTextTextGenerator::setDocumentMetaData(const RVNGPropertyList &propList)
{
	if (!m_impl->m_isInfo)
		return;
	RVNGPropertyList::Iter propIter(propList);
	for (propIter.rewind(); propIter.next();)
	{
		m_impl->m_stream << propIter.key() << ' ' << propIter()->getStr().cstr() << '\n';
	}
}

void RVNGTextTextGenerator::startDocument(const RVNGPropertyList &) {}
void RVNGTextTextGenerator::endDocument()
{
	m_impl->m_document=m_impl->m_stream.str().c_str();
}

void RVNGTextTextGenerator::definePageStyle(const RVNGPropertyList &) {}
void RVNGTextTextGenerator::defineEmbeddedFont(const RVNGPropertyList &) {}
void RVNGTextTextGenerator::openPageSpan(const RVNGPropertyList & /* propList */) {}
void RVNGTextTextGenerator::closePageSpan() {}
void RVNGTextTextGenerator::openHeader(const RVNGPropertyList & /* propList */) {}
void RVNGTextTextGenerator::closeHeader() {}
void RVNGTextTextGenerator::openFooter(const RVNGPropertyList & /* propList */) {}
void RVNGTextTextGenerator::closeFooter() {}

void RVNGTextTextGenerator::defineSectionStyle(const RVNGPropertyList &) {}
void RVNGTextTextGenerator::openSection(const RVNGPropertyList & /* propList */) {}
void RVNGTextTextGenerator::closeSection() {}

void RVNGTextTextGenerator::defineParagraphStyle(const RVNGPropertyList &) {}
void RVNGTextTextGenerator::openParagraph(const RVNGPropertyList & /* propList */) {}

void RVNGTextTextGenerator::closeParagraph()
{
	if (m_impl->m_isInfo)
		return;
	m_impl->m_stream << '\n';
}

void RVNGTextTextGenerator::defineCharacterStyle(const RVNGPropertyList &) {}
void RVNGTextTextGenerator::openSpan(const RVNGPropertyList & /* propList */) {}
void RVNGTextTextGenerator::closeSpan() {}

void RVNGTextTextGenerator::openLink(const RVNGPropertyList & /* propList */) {}
void RVNGTextTextGenerator::closeLink() {}

void RVNGTextTextGenerator::insertTab()
{
	if (m_impl->m_isInfo)
		return;
	m_impl->m_stream << static_cast<char>(UCS_TAB);
}

void RVNGTextTextGenerator::insertText(const RVNGString &text)
{
	if (m_impl->m_isInfo)
		return;
	m_impl->m_stream << text.cstr();
}

void RVNGTextTextGenerator::insertSpace()
{
	if (m_impl->m_isInfo)
		return;
	m_impl->m_stream << ' ';
}

void RVNGTextTextGenerator::insertLineBreak()
{
	if (m_impl->m_isInfo)
		return;
	m_impl->m_stream << '\n';
}

void RVNGTextTextGenerator::insertField(const RVNGPropertyList & /* propList */) {}

void RVNGTextTextGenerator::openOrderedListLevel(const RVNGPropertyList & /* propList */) {}
void RVNGTextTextGenerator::openUnorderedListLevel(const RVNGPropertyList & /* propList */) {}
void RVNGTextTextGenerator::closeOrderedListLevel() {}
void RVNGTextTextGenerator::closeUnorderedListLevel() {}
void RVNGTextTextGenerator::openListElement(const RVNGPropertyList & /* propList */) {}
void RVNGTextTextGenerator::closeListElement() {}

void RVNGTextTextGenerator::openFootnote(const RVNGPropertyList & /* propList */) {}
void RVNGTextTextGenerator::closeFootnote() {}
void RVNGTextTextGenerator::openEndnote(const RVNGPropertyList & /* propList */) {}
void RVNGTextTextGenerator::closeEndnote() {}
void RVNGTextTextGenerator::openComment(const RVNGPropertyList & /* propList */) {}
void RVNGTextTextGenerator::closeComment() {}
void RVNGTextTextGenerator::openTextBox(const RVNGPropertyList & /* propList */) {}
void RVNGTextTextGenerator::closeTextBox() {}

void RVNGTextTextGenerator::openTable(const RVNGPropertyList & /* propList */) {}
void RVNGTextTextGenerator::openTableRow(const RVNGPropertyList & /* propList */) {}
void RVNGTextTextGenerator::closeTableRow() {}
void RVNGTextTextGenerator::openTableCell(const RVNGPropertyList & /* propList */) {}
void RVNGTextTextGenerator::closeTableCell() {}
void RVNGTextTextGenerator::insertCoveredTableCell(const RVNGPropertyList & /* propList */) {}
void RVNGTextTextGenerator::closeTable() {}

void RVNGTextTextGenerator::openFrame(const RVNGPropertyList & /* propList */) {}
void RVNGTextTextGenerator::closeFrame() {}

void RVNGTextTextGenerator::openGroup(const librevenge::RVNGPropertyList & /* propList */) {}
void RVNGTextTextGenerator::closeGroup() {}

void RVNGTextTextGenerator::defineGraphicStyle(const librevenge::RVNGPropertyList & /* propList */) {}
void RVNGTextTextGenerator::drawRectangle(const librevenge::RVNGPropertyList & /* propList */) {}
void RVNGTextTextGenerator::drawEllipse(const librevenge::RVNGPropertyList & /* propList */) {}
void RVNGTextTextGenerator::drawPolygon(const librevenge::RVNGPropertyList & /* propList */) {}
void RVNGTextTextGenerator::drawPolyline(const librevenge::RVNGPropertyList & /* propList */) {}
void RVNGTextTextGenerator::drawPath(const librevenge::RVNGPropertyList & /* propList */) {}
void RVNGTextTextGenerator::drawConnector(const librevenge::RVNGPropertyList & /* propList */) {}

void RVNGTextTextGenerator::insertBinaryObject(const RVNGPropertyList & /* propList */) {}
void RVNGTextTextGenerator::insertEquation(const RVNGPropertyList & /* propList */) {}

}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
