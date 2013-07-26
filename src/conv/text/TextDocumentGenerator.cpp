/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
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
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#include <stdio.h>
#include "TextDocumentGenerator.h"

// use the BELL code to represent a TAB for now
#define UCS_TAB 0x0009

TextDocumentGenerator::TextDocumentGenerator(const bool isInfo) :
	m_isInfo(isInfo)
{
}

TextDocumentGenerator::~TextDocumentGenerator()
{
}

void TextDocumentGenerator::setDocumentMetaData(const WPXPropertyList &propList)
{
	if (!m_isInfo)
		return;
	WPXPropertyList::Iter propIter(propList);
	for (propIter.rewind(); propIter.next(); )
	{
		printf("%s %s\n", propIter.key(), propIter()->getStr().cstr());
	}
}

void TextDocumentGenerator::closeParagraph()
{
	if (m_isInfo)
		return;
	printf("\n");
}

void TextDocumentGenerator::insertTab()
{
	if (m_isInfo)
		return;
	printf("%c", UCS_TAB);
}

void TextDocumentGenerator::insertText(const WPXString &text)
{
	if (m_isInfo)
		return;
	printf("%s", text.cstr());
}

void TextDocumentGenerator::insertSpace()
{
	if (m_isInfo)
		return;
	printf(" ");
}

void TextDocumentGenerator::insertLineBreak()
{
	if (m_isInfo)
		return;
	printf("\n");
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
