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

#include <stdio.h>
#include <librevenge-generators/librevenge-generators.h>

// use the BELL code to represent a TAB for now
#define UCS_TAB 0x0009

namespace librevenge
{

struct RVNGTextTextGeneratorImpl
{
	explicit RVNGTextTextGeneratorImpl(bool isInfo);

	bool m_isInfo;
};

RVNGTextTextGeneratorImpl::RVNGTextTextGeneratorImpl(const bool isInfo) :
	m_isInfo(isInfo)
{
}

RVNGTextTextGenerator::RVNGTextTextGenerator(const bool isInfo) :
	m_impl(new RVNGTextTextGeneratorImpl(isInfo))
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
	for (propIter.rewind(); propIter.next(); )
	{
		printf("%s %s\n", propIter.key(), propIter()->getStr().cstr());
	}
}

void RVNGTextTextGenerator::closeParagraph()
{
	if (m_impl->m_isInfo)
		return;
	printf("\n");
}

void RVNGTextTextGenerator::insertTab()
{
	if (m_impl->m_isInfo)
		return;
	printf("%c", UCS_TAB);
}

void RVNGTextTextGenerator::insertText(const RVNGString &text)
{
	if (m_impl->m_isInfo)
		return;
	printf("%s", text.cstr());
}

void RVNGTextTextGenerator::insertSpace()
{
	if (m_impl->m_isInfo)
		return;
	printf(" ");
}

void RVNGTextTextGenerator::insertLineBreak()
{
	if (m_impl->m_isInfo)
		return;
	printf("\n");
}

}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
