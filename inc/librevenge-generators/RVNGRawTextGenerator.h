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
 * Copyright (C) 2002-2004 Marc Maurer (uwog@uwog.net)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 */

#ifndef RVNGRAWTEXTGENERATOR_H
#define RVNGRAWTEXTGENERATOR_H

#include "librevenge-generators-api.h"

#include <librevenge/librevenge.h>
#include <librevenge-stream/librevenge-stream.h>

namespace librevenge
{

struct RVNGRawTextGeneratorImpl;

class REVENGE_GENERATORS_API RVNGRawTextGenerator : public RVNGTextInterface
{
	// disable copying
	RVNGRawTextGenerator(const RVNGRawTextGenerator &other);
	RVNGRawTextGenerator &operator=(const RVNGRawTextGenerator &other);

public:
	explicit RVNGRawTextGenerator(bool printCallgraphScore);
	~RVNGRawTextGenerator();

	void setDocumentMetaData(const RVNGPropertyList &propList);

	void startDocument(const RVNGPropertyList &propList);
	void endDocument();

	void defineEmbeddedFont(const RVNGPropertyList &propList);

	void definePageStyle(const RVNGPropertyList &propList);
	void openPageSpan(const RVNGPropertyList &propList);
	void closePageSpan();
	void openHeader(const RVNGPropertyList &propList);
	void closeHeader();
	void openFooter(const RVNGPropertyList &propList);
	void closeFooter();

	void defineParagraphStyle(const RVNGPropertyList &propList);
	void openParagraph(const RVNGPropertyList &propList);
	void closeParagraph();

	void defineCharacterStyle(const RVNGPropertyList &propList);
	void openSpan(const RVNGPropertyList &propList);
	void closeSpan();

	void openLink(const RVNGPropertyList &propList);
	void closeLink();

	void defineSectionStyle(const RVNGPropertyList &propList);
	void openSection(const RVNGPropertyList &propList);
	void closeSection();

	void insertTab();
	void insertSpace();
	void insertText(const RVNGString &text);
	void insertLineBreak();
	void insertField(const RVNGPropertyList &propList);

	void openOrderedListLevel(const RVNGPropertyList &propList);
	void openUnorderedListLevel(const RVNGPropertyList &propList);
	void closeOrderedListLevel();
	void closeUnorderedListLevel();
	void openListElement(const RVNGPropertyList &propList);
	void closeListElement();

	void openFootnote(const RVNGPropertyList &propList);
	void closeFootnote();
	void openEndnote(const RVNGPropertyList &propList);
	void closeEndnote();
	void openComment(const RVNGPropertyList &propList);
	void closeComment();
	void openTextBox(const RVNGPropertyList &propList);
	void closeTextBox();

	void openTable(const RVNGPropertyList &propList);
	void openTableRow(const RVNGPropertyList &propList);
	void closeTableRow();
	void openTableCell(const RVNGPropertyList &propList);
	void closeTableCell();
	void insertCoveredTableCell(const RVNGPropertyList &propList);
	void closeTable();

	void openFrame(const RVNGPropertyList &propList);
	void closeFrame();

	void openGroup(const librevenge::RVNGPropertyList &propList);
	void closeGroup();

	void defineGraphicStyle(const librevenge::RVNGPropertyList &propList);
	void drawRectangle(const librevenge::RVNGPropertyList &propList);
	void drawEllipse(const librevenge::RVNGPropertyList &propList);
	void drawPolygon(const librevenge::RVNGPropertyList &propList);
	void drawPolyline(const librevenge::RVNGPropertyList &propList);
	void drawPath(const librevenge::RVNGPropertyList &propList);
	void drawConnector(const RVNGPropertyList &propList);

	void insertBinaryObject(const RVNGPropertyList &propList);
	void insertEquation(const RVNGPropertyList &propList);

private:
	RVNGRawTextGeneratorImpl *m_impl;
};

}

#endif /* RVNGRAWTEXTGENERATOR_H */

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
