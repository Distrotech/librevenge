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
#include "WP6HLContentListener.h"
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

WP6Parser::WP6Parser(GsfInput *input, WPXHeader *header) :
	WPXParser(input, header)
{
}

WP6Parser::~WP6Parser()
{
}

WPXFileType WP6Parser::getFileType(GsfInput *input)
{
	WPXFileType fileType = OTHER;
	GsfInput *document = NULL;
	bool isDocumentOLE = false;
	
	try
	{
		document = getDocument(input);
		if (document != NULL) {
			isDocumentOLE = true;
		}
		else
			document = input;
		
		gsf_input_seek(document, 0, G_SEEK_SET);			
		WPXHeader fileHeader(document);

		if (fileHeader.getFileType() ==  WP6_DOCUMENT_FILE_TYPE &&
		    fileHeader.getMajorVersion() == WP6_EXPECTED_MAJOR_VERSION)
			fileType = WP6_DOCUMENT;
	}
	catch (FileException)
	{
		// no action required: no memory allocated
	}

	if (document != NULL && isDocumentOLE)
		g_object_unref(G_OBJECT(document));

	return fileType;
}

// getDocument: by-passes the OLE stream (if it exists) and returns the (sub) stream with the
// WordPerfect document. Returns NULL if the document is not OLE or it does not contain the
// the expected WordPerfect type.
// NB: It is the responsibility of the application to release this input stream
GsfInput * WP6Parser::getDocument(GsfInput *input)
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

WP6Header * WP6Parser::getHeader(GsfInput *input)
{
	WP6Header * header = NULL;
	try
	{
		gsf_input_seek(input, 0, G_SEEK_SET);
		switch (WPXParser::getHeader()->getMinorVersion())
		{
		case 0x00:
			header = new WP60Header(input);
		default: // assume this header can be parsed by a WP61 header parser
			header = new WP61Header(input);
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

WP6PrefixData * WP6Parser::getPrefixData(GsfInput *input, WP6Header *header)
{
	WP6PrefixData *prefixData = NULL;
	try
	{
		prefixData = new WP6PrefixData(input, header->getNumPrefixIndices());
		return prefixData;
	}
	catch(FileException)
	{
		delete prefixData;
		throw FileException();
	}
}

// WP6LLParser::parse() reads AND parses the main body of a wordperfect document, passing any retrieved low-level
// information to a low-level listener
void WP6Parser::parse(GsfInput *input, WP6Header *header, WP6LLListener *llListener)
{	
	llListener->startDocument();
	
	WPD_CHECK_FILE_SEEK_ERROR(gsf_input_seek(input, header->getDocumentOffset(), G_SEEK_SET));
	
	WPD_DEBUG_MSG(("WordPerfect: Starting document body parse (position = %ld)\n",(long)gsf_input_tell(input)));
	parseDocument(input, llListener);
	
	llListener->endDocument();
}

// parseDocument: parses a document body (may call itself recursively, on other streams, or itself)
void WP6Parser::parseDocument(GsfInput *input, WP6LLListener *llListener)
{
	while (!gsf_input_eof(input))
	{
		guint8 readVal;
		readVal = gsf_le_read_guint8(input);
		
		if (readVal == (guint8)0x00)
		{
			// do nothing: this token is meaningless and is likely just corruption
		}
		else if (readVal <= (guint8)0x20)
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
				part->parse(llListener);
				delete(part);
			}
		}
		
	}
}

void WP6Parser::parsePacket(WP6PrefixData *prefixData, int type, WP6LLListener *llListener)
{
	pair< MPDP_CIter, MPDP_CIter > * typeIterPair;
	typeIterPair = prefixData->getPrefixDataPacketsOfType(type); 
	if (typeIterPair->first != typeIterPair->second) 
		typeIterPair->first->second->parse(llListener);

	delete typeIterPair;
}

void WP6Parser::parsePackets(WP6PrefixData *prefixData, int type, WP6LLListener *llListener)
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

// WP6Parser::parse() reads AND parses a wordperfect document, passing any retrieved low-level
// information to a low-level listener
void WP6Parser::parse(WPXHLListenerImpl *listenerImpl)
{	
	GsfInput *document = NULL;
	WP6Header * header = NULL;
	WP6PrefixData * prefixData = NULL;
	vector<WPXPageSpan *> pageList;
	vector<WPXTable *> tableList;	
	bool isDocumentOLE = false;
 	
	GsfInput *input = getInput();
	
	try
 	{
		document = getDocument(input);
		if (document != NULL) {
			isDocumentOLE = true;
		}
		else
			document = input;

		WPD_CHECK_FILE_SEEK_ERROR(gsf_input_seek(document, 0, G_SEEK_SET));			
		
		header = getHeader(document);
		prefixData = getPrefixData(document, header);
		
		// do a "first-pass" parse of the document
		// gather table border information, page properties (per-page)
		WP6HLStylesListener stylesListener(&pageList, &tableList);
		stylesListener.setPrefixData(prefixData);
		parse(document, header, static_cast<WP6LLListener *>(&stylesListener));

		// second pass: here is where we actually send the messages to the target app
		// that are necessary to emit the body of the target document
		WP6HLContentListener hlListener(&pageList, &tableList, listenerImpl);
		hlListener.setPrefixData(prefixData);

		// get the relevant initial prefix packets out of storage and tell them to parse
		// themselves
		parsePacket(prefixData, WP6_INDEX_HEADER_EXTENDED_DOCUMENT_SUMMARY, static_cast<WP6LLListener *>(&hlListener));
		parsePacket(prefixData, WP6_INDEX_HEADER_INITIAL_FONT, static_cast<WP6LLListener *>(&hlListener));
		parsePackets(prefixData, WP6_INDEX_HEADER_OUTLINE_STYLE, static_cast<WP6LLListener *>(&hlListener));

		parse(document, header, static_cast<WP6LLListener *>(&hlListener));

		// cleanup section: free the used resources
		if (document != NULL && isDocumentOLE)
			g_object_unref(G_OBJECT(document));
		delete header;
		delete prefixData;
		for (vector<WPXPageSpan *>::iterator iter = pageList.begin(); iter != pageList.end(); iter++) {
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
		delete header;
		delete prefixData;
		
		for (vector<WPXPageSpan *>::iterator iter = pageList.begin(); iter != pageList.end(); iter++) {
			delete *iter;
		}
		for (vector<WPXTable *>::iterator iter = tableList.begin(); iter != tableList.end(); iter++) {
			delete *iter;
		}

		throw FileException();
	}
}
