/* libwpd
 * Copyright (C) 2004 Marc Maurer (j.m.maurer@student.utwente.nl)
 * Copyright (C) 2004 Fridrich Strba (fridrich.strba@bluewin.ch)
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
 
#include "WP3Parser.h"
#include "WPXHeader.h"
#include "WP3Part.h"
#include "WP3HLListener.h"
#include "WP3HLStylesListener.h"
#include "libwpd_internal.h"
#include "WPXTable.h"

WP3Parser::WP3Parser(WPXInputStream *input, WPXHeader *header) :
	WPXParser(input, header)
{
}

WP3Parser::~WP3Parser()
{
}

void WP3Parser::parse(WPXInputStream *input, WP3HLListener *listener)
{
	listener->startDocument();
	
	input->seek(getHeader()->getDocumentOffset(), WPX_SEEK_SET);	
	
	WPD_DEBUG_MSG(("WordPerfect: Starting document body parse (position = %ld)\n",(long)input->tell()));
	
	parseDocument(input, listener);
	
	listener->endDocument();		
}

// parseDocument: parses a document body (may call itself recursively, on other streams, or itself)
void WP3Parser::parseDocument(WPXInputStream *input, WP3HLListener *listener)
{
	while (!input->atEOS())
	{
		uint8_t readVal;
		readVal = readU8(input);
		
		if (readVal == 0 || readVal == 0x7F || readVal == 0xFF)
		{
			// FIXME: VERIFY: is this IF clause correct? (0xFF seems to be OK at least)
			// do nothing: this token is meaningless and is likely just corruption
		}
		else if (readVal >= (uint8_t)0x01 && readVal <= (uint8_t)0x1F)
		{
			// control characters ?	
		}
		else if (readVal >= (uint8_t)0x20 && readVal <= (uint8_t)0x7E)
		{
			listener->insertCharacter( readVal );
		}
		else 
		{
			WP3Part *part = WP3Part::constructPart(input, readVal);
			if (part != NULL)
			{
				part->parse(listener);
				DELETEP(part);
			}
		}
	}
}

void WP3Parser::parse(WPXHLListenerImpl *listenerImpl)
{
	WPXInputStream *input = getInput();
	vector<WPXPageSpan *> pageList;
	WPXTableList tableList;	
	
	try
 	{
		// do a "first-pass" parse of the document
		// gather table border information, page properties (per-page)
		WP3HLStylesListener stylesListener(&pageList, tableList);
		parse(input, &stylesListener);

		// second pass: here is where we actually send the messages to the target app
		// that are necessary to emit the body of the target document
		WP3HLListener listener(&pageList, listenerImpl); // FIXME: SHOULD BE CONTENT_LISTENER, AND SHOULD BE PASSED TABLE DATA!
		parse(input, &listener);
		
		// cleanup section: free the used resources
		for (vector<WPXPageSpan *>::iterator iterSpan = pageList.begin(); iterSpan != pageList.end(); iterSpan++)
		{
			delete *iterSpan;
		}
	}
	catch(FileException)
	{
		WPD_DEBUG_MSG(("WordPerfect: File Exception. Parse terminated prematurely."));

		for (vector<WPXPageSpan *>::iterator iterSpan = pageList.begin(); iterSpan != pageList.end(); iterSpan++)
		{
			delete *iterSpan;
		}

		throw FileException();
	}	
}
