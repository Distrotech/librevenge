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

#ifndef RVNGRAWDRAWINGGENERATOR_H
#define RVNGRAWDRAWINGGENERATOR_H

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stack>
#include <librevenge-stream/librevenge-stream.h>
#include <librevenge/librevenge.h>

namespace librevenge
{

enum RVNGRawDrawingGeneratorCallback
{
    PC_START_GRAPHICS = 0,
    PC_START_LAYER,
    PC_START_EMBEDDED_GRAPHICS,
    PC_START_TEXT_OBJECT,
    PC_START_TEXT_LINE,
    PC_START_TEXT_SPAN
};

#ifdef _U
#undef _U
#endif

#define _U(M, L) \
	if (!m_printCallgraphScore) \
			__iuprintf M; \
	else \
		m_callStack.push(L);

#ifdef _D
#undef _D
#endif

#define _D(M, L) \
	if (!m_printCallgraphScore) \
			__idprintf M; \
	else \
	{ \
		RVNGRawDrawingGeneratorCallback lc = m_callStack.top(); \
		if (lc != L) \
			m_callbackMisses++; \
		m_callStack.pop(); \
	}

class RVNGRawDrawingGenerator : public RVNGDrawingInterface
{
public:
	RVNGRawDrawingGenerator(bool printCallgraphScore = false);

	~RVNGRawDrawingGenerator();

	void startDocument(const librevenge::RVNGPropertyList & /*propList*/) {}
	void endDocument() {}
	void setDocumentMetaData(const librevenge::RVNGPropertyList & /*propList*/) {}
	void startPage(const librevenge::RVNGPropertyList &propList);
	void endPage();
	void startLayer(const librevenge::RVNGPropertyList &propList);
	void endLayer();
	void startEmbeddedGraphics(const librevenge::RVNGPropertyList &propList);
	void endEmbeddedGraphics();

	void setStyle(const librevenge::RVNGPropertyList &propList, const librevenge::RVNGPropertyListVector &gradient);

	void drawRectangle(const librevenge::RVNGPropertyList &propList);
	void drawEllipse(const librevenge::RVNGPropertyList &propList);
	void drawPolyline(const librevenge::RVNGPropertyListVector &vertices);
	void drawPolygon(const librevenge::RVNGPropertyListVector &vertices);
	void drawPath(const librevenge::RVNGPropertyListVector &path);
	void drawGraphicObject(const librevenge::RVNGPropertyList &propList, const librevenge::RVNGBinaryData &binaryData);
	void startTextObject(const librevenge::RVNGPropertyList &propList, const librevenge::RVNGPropertyListVector &path);
	void endTextObject();


	void openOrderedListLevel(const librevenge::RVNGPropertyList & /*propList*/) {}
	void closeOrderedListLevel() {}

	void openUnorderedListLevel(const librevenge::RVNGPropertyList & /*propList*/) {}
	void closeUnorderedListLevel() {}

	void openListElement(const librevenge::RVNGPropertyList & /*propList*/, const librevenge::RVNGPropertyListVector & /* tabStops */) {}
	void closeListElement() {}

	void openParagraph(const librevenge::RVNGPropertyList &propList, const librevenge::RVNGPropertyListVector &tabStops);
	void closeParagraph();

	void openSpan(const librevenge::RVNGPropertyList &propList);
	void closeSpan();

	void insertTab() {}
	void insertSpace() {}
	void insertText(const librevenge::RVNGString &text);
	void insertLineBreak() {}
	void insertField(const librevenge::RVNGString & /* type */, const librevenge::RVNGPropertyList & /*propList*/) {}


private:
	int m_indent;
	int m_callbackMisses;
	bool m_printCallgraphScore;
	std::stack<RVNGRawDrawingGeneratorCallback> m_callStack;

	void __indentUp()
	{
		m_indent++;
	}
	void __indentDown()
	{
		if (m_indent > 0) m_indent--;
	}

	void __iprintf(const char *format, ...);
	void __iuprintf(const char *format, ...);
	void __idprintf(const char *format, ...);
};


}

#endif // RVNGRAWDRAWINGGENERATOR_H

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
