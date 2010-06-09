/* libwpd
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by 
 * Corel Corporation or Corel Corporation Limited."
 */
 
#include "WP6StylesListener.h"
#include "WP6ContentListener.h"
#include "WP6Parser.h"
#include "WPXHeader.h"
#include "WP6Header.h"
#include "WP60Header.h"
#include "WP61Header.h"
#include "WP6PrefixData.h"
#include "WP6Part.h"
#include "libwpd_internal.h"
#include "WP6DefaultInitialFontPacket.h"
#include "WPXTable.h"

WP6Parser::WP6Parser(WPXInputStream *input, WPXHeader *header, WPXEncryption *encryption) :
	WPXParser(input, header, encryption)
{
}

WP6Parser::~WP6Parser()
{
}

WP6PrefixData * WP6Parser::getPrefixData(WPXInputStream *input, WPXEncryption *encryption)
{
	WP6PrefixData *prefixData = 0;
	try
	{
		prefixData = new WP6PrefixData(input, encryption, ((WP6Header*)getHeader())->getNumPrefixIndices());
		return prefixData;
	}
	catch(FileException)
	{
		WPD_DEBUG_MSG(("WordPerfect: Prefix Data most likely corrupted.\n"));
		// TODO: Try to check packet after packet so that we try to recover at least the begining if the corruption is not at
		//       the begining.
		DELETEP(prefixData);
		throw FileException();
	}
	catch(...)
	{
		WPD_DEBUG_MSG(("WordPerfect: Prefix Data most likely corrupted. Trying to ignore.\n"));
		// TODO: Try to check packet after packet so that we try to recover at least the begining if the corruption is not at
		//       the begining.
		DELETEP(prefixData);
		return 0;
	}
}

void WP6Parser::parse(WPXInputStream *input, WPXEncryption *encryption, WP6Listener *listener)
{
	listener->startDocument();
	
	input->seek(getHeader()->getDocumentOffset(), WPX_SEEK_SET);	
	
	WPD_DEBUG_MSG(("WordPerfect: Starting document body parse (position = %ld)\n",(long)input->tell()));
	
	parseDocument(input, encryption, listener);
	
	listener->endDocument();		
}

static const uint16_t extendedInternationalCharacterMap[] =
{
  229, // lower case 'a' with a small circle
  197, // upper case 'a' with a small circle
  230, // lower case 'ae'
  198, // upper case 'ae'
  228, // lower case 'a' with diathesis
  196, // upper case 'a' with diathesis
  225, // lower case 'a' with acute
  224, // lower case 'a' with grave
  226, // lower case 'a' with circonflex
  227, // lower case 'a' with tilde
  195, // upper case 'a' with tilde
  231, // lower case 'c' with hook
  199, // upper case 'c' with hook
  235, // lower case 'e' with diathesis
  233, // lower case 'e' with acute
  201, // upper case 'e' with acute
  232, // lower case 'e' with grave
  234, // lower case 'e' with circonflex
  237, // lower case 'i' with acute
  241, // lower case 'n' with tilde
  209, // upper case 'n' with tilde
  248, // lower case 'o' with stroke
  216, // upper case 'o' with stroke
  245, // lower case 'o' with tilde
  213, // upper case 'o' with tilde
  246, // lower case 'o' with diathesis
  214, // upper case 'o' with diathesis
  252, // lower case 'u' with diathesis
  220, // upper case 'u' with diathesis
  250, // lower case 'u' with acute
  249, // lower case 'u' with grave
  223 // double s
};

// parseDocument: parses a document body (may call itself recursively, on other streams, or itself)
void WP6Parser::parseDocument(WPXInputStream *input, WPXEncryption *encryption, WP6Listener *listener)
{
	while (!input->atEOS())
	{
		uint8_t readVal;
		readVal = readU8(input, encryption);
		
		if (readVal == (uint8_t)0x00)
		{
			// do nothing: this token is meaningless and is likely just corruption
		}
		else if (readVal <= (uint8_t)0x20)
		{
			listener->insertCharacter( extendedInternationalCharacterMap[(readVal-1)] );
		}
		else if (readVal >= (uint8_t)0x21 && readVal <= (uint8_t)0x7F)
		{
			// normal ASCII characters
			listener->insertCharacter( (uint16_t)readVal );
		}
		else 
		{
			WP6Part *part = WP6Part::constructPart(input, encryption, readVal);
			if (part)
			{
				part->parse(listener);
				DELETEP(part);
			}
		}	
	}
}

void WP6Parser::parsePacket(WP6PrefixData *prefixData, int type, WP6Listener *listener)
{
	if (!prefixData)
		return;

	std::pair< MPDP_CIter, MPDP_CIter > typeIterPair = prefixData->getPrefixDataPacketsOfType(type); 
	if (typeIterPair.first != typeIterPair.second) 
	{
		typeIterPair.first->second->parse(listener);
	}
}

void WP6Parser::parsePackets(WP6PrefixData *prefixData, int type, WP6Listener *listener)
{
	if (!prefixData)
		return;

	std::pair< MPDP_CIter, MPDP_CIter > typeIterPair = prefixData->getPrefixDataPacketsOfType(type);
	for (MPDP_CIter iter=typeIterPair.first; 
	     iter != typeIterPair.second; 
	     iter++) 
	{
		iter->second->parse(listener);
	}
}

// WP6Parser::parse() reads AND parses a wordperfect document, passing any retrieved low-level
// information to a low-level listener
void WP6Parser::parse(WPXDocumentInterface *documentInterface)
{
	WP6PrefixData * prefixData = 0;
	std::list<WPXPageSpan> pageList;
	WPXTableList tableList;	

	WPXInputStream *input = getInput();
	WPXEncryption *encryption = getEncryption();

	try
 	{
		prefixData = getPrefixData(input, encryption);

		// do a "first-pass" parse of the document
		// gather table border information, page properties (per-page)
		WP6StylesListener stylesListener(pageList, tableList);
		stylesListener.setPrefixData(prefixData);
		parse(input, encryption, &stylesListener);

		// postprocess the pageList == remove duplicate page spans due to the page breaks
		std::list<WPXPageSpan>::iterator previousPage = pageList.begin();
		for (std::list<WPXPageSpan>::iterator Iter=pageList.begin(); Iter != pageList.end(); /* Iter++ */)
		{
			if ((Iter != previousPage) && ((*previousPage)==(*Iter)))
			{
				(*previousPage).setPageSpan((*previousPage).getPageSpan() + (*Iter).getPageSpan());
				Iter = pageList.erase(Iter);
			}
			else
			{
				previousPage = Iter;
				Iter++;
			}
		}

		// second pass: here is where we actually send the messages to the target app
		// that are necessary to emit the body of the target document
		WP6ContentListener listener(pageList, tableList, documentInterface);
		listener.setPrefixData(prefixData);

		// get the relevant initial prefix packets out of storage and tell them to parse
		// themselves
		parsePacket(prefixData, WP6_INDEX_HEADER_EXTENDED_DOCUMENT_SUMMARY, &listener);
		parsePacket(prefixData, WP6_INDEX_HEADER_INITIAL_FONT, &listener);
		parsePackets(prefixData, WP6_INDEX_HEADER_OUTLINE_STYLE, &listener);

		parse(input, encryption, &listener);

		// cleanup section: free the used resources
		delete prefixData;
	}
	catch(FileException)
	{
		WPD_DEBUG_MSG(("WordPerfect: File Exception. Parse terminated prematurely."));

		delete prefixData;

		throw FileException();
	}
}

void WP6Parser::parseSubDocument(WPXDocumentInterface *documentInterface)
{	
	std::list<WPXPageSpan> pageList;
	WPXTableList tableList;	

	WPXInputStream *input = getInput();

	try
 	{
		WP6StylesListener stylesListener(pageList, tableList);
		stylesListener.startSubDocument();
		parseDocument(input, 0, &stylesListener);
		stylesListener.endSubDocument();
		
		input->seek(0, WPX_SEEK_SET);
		
		WP6ContentListener listener(pageList, tableList, documentInterface);
		listener.startSubDocument();
		parseDocument(input, 0, &listener);
		listener.endSubDocument();
	}
	catch(FileException)
	{
		WPD_DEBUG_MSG(("WordPerfect: File Exception. Parse terminated prematurely."));
		throw FileException();
	}
}
