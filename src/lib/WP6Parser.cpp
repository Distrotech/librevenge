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
#include "WP6Parser.h"
#include "WPXHeader.h"
#include "WP6Header.h"
#include "WP60Header.h"
#include "WP61Header.h"
#include "WP6PrefixData.h"
#include "WP6Part.h"
#include "libwpd_internal.h"
#include "WP6DefaultInitialFontPacket.h"

WP6Parser::WP6Parser(GsfInput * input, WPXLLListener *llListener/*,  WP6Header *header */)
	: WPXParser(input, llListener)
{
}

// WP6Parser::parse() reads AND parses a wordperfect document, passing any retrieved low-level
// information to a low-level listener
void WP6Parser::parse()
{	
	try
	{
		GsfInfile * ole = GSF_INFILE(gsf_infile_msole_new (getInput(), NULL));
		if (ole != NULL) 
			{
				setInput(gsf_infile_child_by_name (ole, "PerfectOffice_MAIN"));
				g_object_unref(G_OBJECT (ole));
				if (getInput() == NULL) 
					{
						throw FileException();
					} 
			}
		else
			WPD_CHECK_FILE_SEEK_ERROR(gsf_input_seek(getInput(), 0, G_SEEK_SET));
		
		WPXHeader * fileHeader = new WPXHeader(getInput());
		WP6Header * header = NULL;
		switch (fileHeader->getMinorVersion())
		{
			case 0x00:
				header = new WP60Header(getInput());
				break;
			default: // assume this header can be parsed by a WP61 header parser
				header = new WP61Header(getInput());
				break;
		}
		WP6PrefixData *prefixData = new WP6PrefixData(getInput(), header->getNumPrefixIndices());
		static_cast<WP6LLListener *>(getLLListener())->setPrefixData(prefixData);

		// get the relevant initial prefix packets out of storage and tell them to parse
		// themselves
		_parsePacket(prefixData, WP6_INDEX_HEADER_EXTENDED_DOCUMENT_SUMMARY);
		_parsePacket(prefixData, WP6_INDEX_HEADER_INITIAL_FONT);
		_parsePackets(prefixData, WP6_INDEX_HEADER_OUTLINE_STYLE);

		getLLListener()->startDocument();
		
		WPD_CHECK_FILE_SEEK_ERROR(gsf_input_seek(getInput(), fileHeader->getDocumentOffset(), G_SEEK_SET));
	
		WPD_DEBUG_MSG(("WordPerfect: Starting document body parse (position = %ld)\n",(long)gsf_input_tell(getInput())));
		WP6Parser::parseDocument(getInput(), static_cast<WP6LLListener *>(getLLListener()));

		getLLListener()->endDocument();
		WPD_DEBUG_MSG(("WordPerfect: Finished with document parse (position = %ld)\n",(long)gsf_input_tell(getInput())));
		delete(header);
		delete(fileHeader);
		delete(prefixData);
	}
	catch(FileException)
	{
		WPD_DEBUG_MSG(("WordPerfect: File Exception. Parse terminated prematurely."));
		throw FileException();
	}
	
}

// parseDocument: parses a document body (may call itself recursively, on other streams)
void WP6Parser::parseDocument(GsfInput *stream, WP6LLListener *llListener)
{
	while (!gsf_input_eof(stream))
	{
		guint8 readVal;
		readVal = *(const guint8 *)gsf_input_read(stream, sizeof(guint8), NULL);
		
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
			WP6Part *part = WP6Part::constructPart(stream, readVal);
			if (part != NULL)
			{
				part->parse(llListener);
				delete(part);
			}
		}
		
	}
}

void WP6Parser::_parsePacket(WP6PrefixData *prefixData, int type)
{
	pair< MPDP_CIter, MPDP_CIter > * typeIterPair;
	typeIterPair = prefixData->getPrefixDataPacketsOfType(type); 
	if (typeIterPair->first != typeIterPair->second) 
		typeIterPair->first->second->parse(static_cast<WP6LLListener *>(getLLListener()));

	delete typeIterPair;
}

void WP6Parser::_parsePackets(WP6PrefixData *prefixData, int type)
{
	pair< MPDP_CIter, MPDP_CIter > * typeIterPair;

	typeIterPair = prefixData->getPrefixDataPacketsOfType(type);
	for (MPDP_CIter iter=typeIterPair->first; 
	     iter != typeIterPair->second; 
	     iter++) 
	{
		iter->second->parse(static_cast<WP6LLListener *>(getLLListener()));
	}

	delete typeIterPair;

}
