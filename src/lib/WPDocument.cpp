/* libwpd2
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

#include <gsf/gsf-input.h>
#include <stdlib.h>
#include <string.h>
#include "WPDocument.h"
#include "WPXParser.h"
#include "WP42Parser.h"
#include "WP5Parser.h"
#include "WP6Parser.h"
#include "libwpd_internal.h"

void WPDocument::parse(GsfInput *input, WPXHLListenerImpl *listenerImpl)
{
	WPXParser *parser = NULL;
	WPXHeader *header = NULL;
	
	try
	{
		header = new WPXHeader(input);
		
		switch (header->getMajorVersion())
		{
			case 0x00: // WP5 
				WPD_DEBUG_MSG(("WordPerfect: Using the WP5 parser.\n"));
				parser = new WP5Parser(input, header);
				parser->parse(listenerImpl);
				break;
			case 0x01: // ???
				WPD_DEBUG_MSG(("WordPerfect: Unsupported file format.\n"));
				break;
			case 0x02: // WP6
				WPD_DEBUG_MSG(("WordPerfect: Using the WP6 parser.\n"));
				parser = new WP6Parser(input, header);
				parser->parse(listenerImpl);
				break;
			default:
				// unhandled file format
				WPD_DEBUG_MSG(("WordPerfect: Unsupported file format.\n"));
				break;
		}
		
		DELETEP(parser);
		DELETEP(header);	
	}
	catch (NoFileHeaderException)
	{
		// WP file formats prior to version 5.x do not contain a generic 
		// header which can be used to determine which parser to instanciate. 
		// Use heuristics to determine with some certainty if we are dealing with
		// a file in the WP4.2 format.		
		
		WPD_DEBUG_MSG(("WordPerfect: Mostly likely the file format is WP4.2.\n\n"));
		WPD_DEBUG_MSG(("WordPerfect: Using the WP4.2 parser.\n\n"));
		WP42Parser *parser = new WP42Parser(input);
		parser->parse(listenerImpl);
		DELETEP(parser);
	}
	catch (FileException)
	{
		DELETEP(header);
		throw FileException(); 
	}
}

/*void WPXParser::parse(GsfInput *input, WPXLLListener *llistener)
{
	// TODO: implement me
}*/
