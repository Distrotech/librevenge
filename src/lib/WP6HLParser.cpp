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

#include "WP6HLParser.h"
#include "WPXHLListenerImpl.h"
#include "WP6HLStylesListener.h"
#include "WP6HLContentListener.h"
#include "WP6Header.h"
#include "WP6PrefixData.h"
#include "libwpd_internal.h"

// WP6LLParser::parse() reads AND parses a wordperfect document, passing any retrieved low-level
// information to a low-level listener
void WP6HLParser::parse(GsfInput *input, WPXHLListenerImpl *listenerImpl)
{	
	GsfInput *document = NULL;
	WP6Header * header = NULL;
	WP6PrefixData * prefixData = NULL;
	vector<WPXPage *> pageList;
	vector<WPXTable *> tableList;	
	bool isDocumentOLE = false;
 	
	try
 	{
		document =  WP6LLParser::getDocument(input);
		if (document != NULL) {
			isDocumentOLE = true;
		}
		else
			document = input;

		WPD_CHECK_FILE_SEEK_ERROR(gsf_input_seek(document, 0, G_SEEK_SET));			
		
		header = WP6LLParser::getHeader(document);
		prefixData = WP6LLParser::getPrefixData(document, header);
		
		// do a "first-pass" parse of the document
		// gather table border information, (TODO) page properties (per-page)
		WP6HLStylesListener stylesListener(&pageList, &tableList);
		stylesListener.setPrefixData(prefixData);
		WP6LLParser::parse(input, header, static_cast<WP6LLListener *>(&stylesListener));

		// second pass: here is where we actually send the messages to the target app
		// that are necessary to emit the body of the target document
		WP6HLContentListener hlListener(&pageList, &tableList, listenerImpl);
		hlListener.setPrefixData(prefixData);

		// get the relevant initial prefix packets out of storage and tell them to parse
		// themselves
		WP6LLParser::parsePacket(prefixData, WP6_INDEX_HEADER_EXTENDED_DOCUMENT_SUMMARY, static_cast<WP6LLListener *>(&hlListener));
		WP6LLParser::parsePacket(prefixData, WP6_INDEX_HEADER_INITIAL_FONT, static_cast<WP6LLListener *>(&hlListener));
		WP6LLParser::parsePackets(prefixData, WP6_INDEX_HEADER_OUTLINE_STYLE, static_cast<WP6LLListener *>(&hlListener));

		WP6LLParser::parse(input, header, static_cast<WP6LLListener *>(&hlListener));

		if (document != NULL && isDocumentOLE)
			g_object_unref(G_OBJECT(document));
		delete header;
		delete prefixData;
		for (vector<WPXPage *>::iterator iter = pageList.begin(); iter != pageList.end(); iter++) {
			delete *iter;
		}
		for (vector<WPXTable *>::iterator iter = tableList.begin(); iter != tableList.end(); iter++) {
			delete *iter;
		}
	}
	catch(FileException)
	{
		WPD_DEBUG_MSG(("WordPerfect: File Exception. Parse terminated prematurely."));
		if (document != NULL && isDocumentOLE)
			g_object_unref(G_OBJECT(document));
		if (header != NULL)
			delete header;
		if (prefixData != NULL)
			delete prefixData;
		// FIXME: iterate through page and table lists, destroy all of them

		throw FileException();
	}
	
}
