/* libwpd
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002-2003 Marc Maurer (uwog@uwog.net)
 *  
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
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
