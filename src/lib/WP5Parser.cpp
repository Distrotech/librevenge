/* libwpd
 * Copyright (C) 2003 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2003 Marc Maurer (j.m.maurer@student.utwente.nl)
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
 
#include "WP5Parser.h"
#include "WPXHeader.h"
#include "WP5Part.h"
#include "WP5HLListener.h"
#include "libwpd_internal.h"
#include "WPXTable.h"

WP5Parser::WP5Parser(GsfInput *input, WPXHeader *header) :
	WPXParser(input, header)
{
}

WP5Parser::~WP5Parser()
{
}

void WP5Parser::parse(GsfInput *input, WP5LLListener *llListener)
{
	llListener->startDocument();
	
	WPD_CHECK_FILE_SEEK_ERROR(gsf_input_seek(input, getHeader()->getDocumentOffset(), G_SEEK_SET));	
	
	WPD_DEBUG_MSG(("WordPerfect: Starting document body parse (position = %ld)\n",(long)gsf_input_tell(input)));
	
	parseDocument(input, llListener);
	
	llListener->endDocument();		
}

// parseDocument: parses a document body (may call itself recursively, on other streams, or itself)
void WP5Parser::parseDocument(GsfInput *input, WP5LLListener *llListener)
{
	while (!gsf_input_eof(input))
	{
		guint8 readVal;
		readVal = gsf_le_read_guint8(input);
		
		if (readVal == 0 || readVal == 0x7F || readVal == 0xFF)
		{
			// do nothing: this token is meaningless and is likely just corruption
		}
		else if (readVal >= (guint8)0x01 && readVal <= (guint8)0x1F)
		{
			// control characters
			
			switch (readVal)
			{
				case 0x0A: // hard new line
					llListener->insertEOL();
					break;
				case 0x0B: // soft new page
					break;
				case 0x0C: // hard new page
					break;
				case 0x0D: // soft new line
					llListener->insertEOL();
					break;
				default:
					// unsupported or undocumented token, ignore
					break;
			}
		}
		else if (readVal >= (guint8)0x20 && readVal <= (guint8)0x7E)
		{
			llListener->insertCharacter( readVal );
		}
		else if (readVal >= (guint8)0x80 && readVal <= (guint8)0xBF)
		{
			// single byte functions
		}			
		else 
		{
			WP5Part *part = WP5Part::constructPart(input, readVal);
			if (part != NULL)
			{
				part->parse(llListener);
				DELETEP(part);
			}
		}	
	}
}

void WP5Parser::parse(WPXHLListenerImpl *listenerImpl)
{
	GsfInput *input = getInput();
	
	try
 	{
		WP5HLListener hlListener(listenerImpl);
		parse(input, static_cast<WP5LLListener *>(&hlListener));
	}
	catch(FileException)
	{
		WPD_DEBUG_MSG(("WordPerfect: File Exception. Parse terminated prematurely."));
		throw FileException();
	}	
}
