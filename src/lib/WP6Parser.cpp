/* libwpd2
 * Copyright (C) 2002 William Lachance (wlach@interlog.com)
 * Copyright (C) 2002 Marc Maurer (j.m.maurer@student.utwente.nl)
 *  
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
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
#include "WP6LLListener.h"
#include "WP6Parser.h"
#include "WP6Part.h"
#include "UT_libwpd2.h"

WP6Parser::WP6Parser(FILE * stream, WPXLLListener *llListener/*,  WP6Header *header */)
	: WPXParser(stream, llListener)
{
}

// WP6Parser::parse() reads AND parses a wordperfect document, passing any retrieved low-level
// information to a low-level listener
void WP6Parser::parse()
{	
	try {
		WP6Header * header = new WP6Header(getStream());
		
		getLLListener()->startDocument();
		
		WPD_CHECK_FILE_SEEK_ERROR(fseek(getStream(), header->getDocumentOffset() - ftell(getStream()), SEEK_CUR));
	
		while (ftell(getStream()) < (long)((WP6Header *)header)->getDocumentSize())
			{
				guint8 readVal;
				WPD_CHECK_FILE_READ_ERROR(fread(&readVal, sizeof(guint8), 1, getStream()), 1);
				
				if (/*readVal >= (guint8)0x00 &&*/ readVal <= (guint8)0x20)
					{
						// Default Extended International Characters
					}
				else if (readVal >= (guint8)0x21 && readVal <= (guint8)0x7F)
					{
						// normal ASCII characters
						getLLListener()->insertCharacter( (guint16)readVal );
					}
				else 
					{
						WP6Part *part = WP6Part::constructPart(getStream(), readVal);
						if (part != NULL) {
							part->parse(dynamic_cast<WP6LLListener *>(getLLListener()));
							delete(part);
						}
					}
			}
	}
	catch(FileException) { WPD_DEBUG_MSG(("WordPerfect: File Seek Exception. Parse terminated prematurely.")); }
	getLLListener()->endDocument();
	WPD_DEBUG_MSG(("WordPerfect: Finished with document parse (position = %ld)\n",(long)ftell(getStream())));
}
