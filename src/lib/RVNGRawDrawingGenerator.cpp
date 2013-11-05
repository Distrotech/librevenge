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
#include <stdarg.h>
#include <string.h>
#include <stack>

#include <librevenge-generators/librevenge-generators.h>

#ifdef _U
#undef _U
#endif

#define _U(M, L) \
	if (!m_impl->m_printCallgraphScore) \
			m_impl->iuprintf M; \
	else \
		m_impl->m_callStack.push(L);

#ifdef _D
#undef _D
#endif

#define _D(M, L) \
	if (!m_impl->m_printCallgraphScore) \
			m_impl->idprintf M; \
	else \
	{ \
		RVNGRawDrawingGeneratorCallback lc = m_impl->m_callStack.top(); \
		if (lc != L) \
			m_impl->m_callbackMisses++; \
		m_impl->m_callStack.pop(); \
	}

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

} // anonymous namespace

struct RVNGRawDrawingGeneratorImpl
{
	RVNGRawDrawingGeneratorImpl(bool printCallgraphScore);

	int m_indent;
	int m_callbackMisses;
	bool m_printCallgraphScore;
	std::stack<RVNGRawDrawingGeneratorCallback> m_callStack;

	void indentUp()
	{
		m_indent++;
	}
	void indentDown()
	{
		if (m_indent > 0) m_indent--;
	}

	void iprintf(const char *format, ...);
	void iuprintf(const char *format, ...);
	void idprintf(const char *format, ...);
};

RVNGRawDrawingGeneratorImpl::RVNGRawDrawingGeneratorImpl(const bool printCallgraphScore)
	: m_indent(0)
	, m_callbackMisses(0)
	, m_printCallgraphScore(printCallgraphScore)
	, m_callStack()
{
}

RVNGRawDrawingGenerator::RVNGRawDrawingGenerator(bool printCallgraphScore):
	RVNGDrawingInterface(),
	m_impl(new RVNGRawDrawingGeneratorImpl(printCallgraphScore))
{
}

RVNGRawDrawingGenerator::~RVNGRawDrawingGenerator()
{
	if (m_impl->m_printCallgraphScore)
		printf("%d\n", (int)(m_impl->m_callStack.size() + m_impl->m_callbackMisses));

	delete m_impl;
}

void RVNGRawDrawingGeneratorImpl::iprintf(const char *format, ...)
{
	if (m_printCallgraphScore) return;

	va_list args;
	va_start(args, format);
	for (int i=0; i<m_indent; i++)
		printf("  ");
	vprintf(format, args);
	va_end(args);
}

void RVNGRawDrawingGeneratorImpl::iuprintf(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	for (int i=0; i<m_indent; i++)
		printf("  ");
	vprintf(format, args);
	indentUp();
	va_end(args);
}

void RVNGRawDrawingGeneratorImpl::idprintf(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	indentDown();
	for (int i=0; i<m_indent; i++)
		printf("  ");
	vprintf(format, args);
	va_end(args);
}

void RVNGRawDrawingGenerator::startDocument(const librevenge::RVNGPropertyList & /*propList*/) {}
void RVNGRawDrawingGenerator::endDocument() {}
void RVNGRawDrawingGenerator::setDocumentMetaData(const librevenge::RVNGPropertyList & /*propList*/) {}

void RVNGRawDrawingGenerator::startPage(const RVNGPropertyList &propList)
{
	_U(("startPage(%s)\n", getPropString(propList).cstr()), PC_START_GRAPHICS);
}

void RVNGRawDrawingGenerator::endPage()
{
	_D(("endPage\n"), PC_START_GRAPHICS);
}

void RVNGRawDrawingGenerator::startLayer(const RVNGPropertyList &propList)
{
	_U(("startLayer (%s)\n", getPropString(propList).cstr()), PC_START_LAYER);
}

void RVNGRawDrawingGenerator::endLayer()
{
	_D(("endLayer\n"), PC_START_LAYER);
}

void RVNGRawDrawingGenerator::startEmbeddedGraphics(const RVNGPropertyList &propList)
{
	_U(("startEmbeddedGraphics (%s)\n", getPropString(propList).cstr()), PC_START_EMBEDDED_GRAPHICS);
}

void RVNGRawDrawingGenerator::endEmbeddedGraphics()
{
	_D(("endEmbeddedGraphics \n"), PC_START_EMBEDDED_GRAPHICS);
}

void RVNGRawDrawingGenerator::setStyle(const RVNGPropertyList &propList, const RVNGPropertyListVector &gradient)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("setStyle(%s, gradient: (%s))\n", getPropString(propList).cstr(), getPropString(gradient).cstr());
}

void RVNGRawDrawingGenerator::drawRectangle(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("drawRectangle (%s)\n", getPropString(propList).cstr());
}

void RVNGRawDrawingGenerator::drawEllipse(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("drawEllipse (%s)\n", getPropString(propList).cstr());
}

void RVNGRawDrawingGenerator::drawPolyline(const RVNGPropertyListVector &vertices)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("drawPolyline (%s)\n", getPropString(vertices).cstr());
}

void RVNGRawDrawingGenerator::drawPolygon(const RVNGPropertyListVector &vertices)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("drawPolygon (%s)\n", getPropString(vertices).cstr());
}

void RVNGRawDrawingGenerator::drawPath(const RVNGPropertyListVector &path)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("drawPath (%s)\n", getPropString(path).cstr());
}

void RVNGRawDrawingGenerator::drawGraphicObject(const RVNGPropertyList &propList, const RVNGBinaryData & /*binaryData*/)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("drawGraphicObject (%s)\n", getPropString(propList).cstr());
}

void RVNGRawDrawingGenerator::startTextObject(const RVNGPropertyList &propList, const RVNGPropertyListVector &path)
{
	_U(("startTextObject (%s, path: (%s))\n", getPropString(propList).cstr(), getPropString(path).cstr()), PC_START_TEXT_OBJECT);
}

void RVNGRawDrawingGenerator::endTextObject()
{
	_D(("endTextObject\n"), PC_START_TEXT_OBJECT);
}

void RVNGRawDrawingGenerator::openOrderedListLevel(const librevenge::RVNGPropertyList & /*propList*/) {}
void RVNGRawDrawingGenerator::closeOrderedListLevel() {}

void RVNGRawDrawingGenerator::openUnorderedListLevel(const librevenge::RVNGPropertyList & /*propList*/) {}
void RVNGRawDrawingGenerator::closeUnorderedListLevel() {}

void RVNGRawDrawingGenerator::openListElement(const librevenge::RVNGPropertyList & /*propList*/, const librevenge::RVNGPropertyListVector & /* tabStops */) {}
void RVNGRawDrawingGenerator::closeListElement() {}

void RVNGRawDrawingGenerator::openParagraph(const RVNGPropertyList &propList, const RVNGPropertyListVector &tabStops)
{
	_U(("openParagraph (%s, tabStops: (%s))\n", getPropString(propList).cstr(), getPropString(tabStops).cstr()), PC_START_TEXT_LINE);
}

void RVNGRawDrawingGenerator::closeParagraph()
{
	_D(("closeParagraph\n"), PC_START_TEXT_LINE);
}

void RVNGRawDrawingGenerator::openSpan(const RVNGPropertyList &propList)
{
	_U(("openSpan (%s)\n", getPropString(propList).cstr()), PC_START_TEXT_SPAN);
}

void RVNGRawDrawingGenerator::closeSpan()
{
	_D(("closeSpan\n"), PC_START_TEXT_SPAN);
}

void RVNGRawDrawingGenerator::insertTab() {}
void RVNGRawDrawingGenerator::insertSpace() {}

void RVNGRawDrawingGenerator::insertText(const RVNGString &str)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("insertText (%s)\n", str.cstr());
}

void RVNGRawDrawingGenerator::insertLineBreak() {}
void RVNGRawDrawingGenerator::insertField(const librevenge::RVNGString & /* type */, const librevenge::RVNGPropertyList & /*propList*/) {}

}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
