/* libwpd2
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
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
 
#include <gsf/gsf-input.h>
#include <gsf/gsf-infile.h>
#include <gsf/gsf-infile-msole.h>
#include "WP6LLListener.h"
#include "WP6HLStylesListener.h"
#include "WP6LLParser.h"
#include "WPXHeader.h"
#include "WP6Header.h"
#include "WP60Header.h"
#include "WP61Header.h"
#include "WP6PrefixData.h"
#include "WP6Part.h"
#include "libwpd_internal.h"
#include "WP6DefaultInitialFontPacket.h"
#include "WPXTable.h"

// getDocument: by-passes the OLE stream (if it exists) and returns the (sub) stream with the
// WordPerfect document. Returns NULL if the document is not OLE.
// NB: It is the responsibility of the application to release this input stream
GsfInput * WP6LLParser::getDocument(GsfInput *input)
{
	GsfInput *document = NULL;

	GsfInfile * ole = GSF_INFILE(gsf_infile_msole_new (input, NULL));
	if (ole != NULL) 
	{
		document = gsf_infile_child_by_name(ole, "PerfectOffice_MAIN");
		g_object_unref(G_OBJECT (ole));
		if (document == NULL) // is an OLE document, but does not contain a WP document
			throw FileException();
	}

	return document;
}

WP6Header * WP6LLParser::getHeader(GsfInput *input)
{
	WP6Header * header = NULL;
	try
	{
		WPXHeader fileHeader(input);
		
		switch (fileHeader.getMinorVersion())
		{
		case 0x00:
			header = new WP60Header(input, fileHeader);
		default: // assume this header can be parsed by a WP61 header parser
			header = new WP61Header(input, fileHeader);
		}
	}
	catch(FileException)
	{
		if (header)
			delete header;
		throw FileException(); 
	}

	return header;
}

WP6PrefixData * WP6LLParser::getPrefixData(GsfInput *input, WP6Header *header)
{
	WP6PrefixData *prefixData = NULL;
	try
	{
		prefixData = new WP6PrefixData(input, header->getNumPrefixIndices());
		return prefixData;
	}
	catch(FileException)
	{
		if (prefixData)
			delete prefixData;
		throw FileException();
	}
}

// WP6LLParser::parse() reads AND parses the main body of a wordperfect document, passing any retrieved low-level
// information to a low-level listener
void WP6LLParser::parse(GsfInput *input, WP6Header *header, WP6LLListener *llListener)
{	
	llListener->startDocument();
	
	WPD_CHECK_FILE_SEEK_ERROR(gsf_input_seek(input, header->getDocumentOffset(), G_SEEK_SET));
	
	WPD_DEBUG_MSG(("WordPerfect: Starting document body parse (position = %ld)\n",(long)gsf_input_tell(input)));
	WP6LLParser::parseDocument(input, llListener);
	
	llListener->endDocument();
}

// parseDocument: parses a document body (may call itself recursively, on other streams, or itself)
void WP6LLParser::parseDocument(GsfInput *input, WP6LLListener *llListener)
{
	while (!gsf_input_eof(input))
	{
		guint8 readVal;
		readVal = gsf_le_read_guint8(input);
		
		if (readVal <= (guint8)0x20)
		{
			llListener->insertCharacter( extendedInternationalCharacterMap[(readVal-1)] );
		}
		else if (readVal >= (guint8)0x21 && readVal <= (guint8)0x7F)
		{
			// normal ASCII characters
			llListener->insertCharacter( (guint16)readVal );
		}
		else 
		{
			WP6Part *part = WP6Part::constructPart(input, readVal);
			if (part != NULL)
			{

				ParseResult result = part->parse(llListener);
				delete(part);
			}
		}
		
	}
}

void WP6LLParser::parsePacket(WP6PrefixData *prefixData, int type, WP6LLListener *llListener)
{
	pair< MPDP_CIter, MPDP_CIter > * typeIterPair;
	typeIterPair = prefixData->getPrefixDataPacketsOfType(type); 
	if (typeIterPair->first != typeIterPair->second) 
		typeIterPair->first->second->parse(llListener);

	delete typeIterPair;
}

void WP6LLParser::parsePackets(WP6PrefixData *prefixData, int type, WP6LLListener *llListener)
{
	pair< MPDP_CIter, MPDP_CIter > * typeIterPair;

	typeIterPair = prefixData->getPrefixDataPacketsOfType(type);
	for (MPDP_CIter iter=typeIterPair->first; 
	     iter != typeIterPair->second; 
	     iter++) 
	{
		iter->second->parse(llListener);
	}

	delete typeIterPair;

}
