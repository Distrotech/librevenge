/* libwpd
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002-2003 Marc Maurer (j.m.maurer@student.utwente.nl)
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by 
 * Corel Corporation or Corel Corporation Limited."
 */

#include <stdio.h>
#include "TextListenerImpl.h"

// use the BELL code to represent a TAB for now
#define UCS_TAB 0x0009 

TextListenerImpl::TextListenerImpl()
{
}

TextListenerImpl::~TextListenerImpl()
{
}

void TextListenerImpl::closeParagraph()
{
	printf("\n");
}

void TextListenerImpl::insertTab()
{
	printf("%c", UCS_TAB);
}

void TextListenerImpl::insertText(const WPXString &text)
{
	printf("%s", text.cstr());
}

void TextListenerImpl::insertLineBreak()
{
	printf("\n");
}
