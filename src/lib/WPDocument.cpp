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
#include <gsf/gsf-infile.h>
#include <gsf/gsf-infile-msole.h>
#include <stdlib.h>
#include <string.h>
#include "WPDocument.h"
#include "WPXParser.h"
#include "WP42Parser.h"
#include "WP42Heuristics.h"
#include "WP5Parser.h"
#include "WP6Parser.h"
#include "libwpd_internal.h"

WPDConfidence WPDocument::isFileFormatSupported(GsfInput *input, bool partialContent)
{
	WPDConfidence confidence = WPD_CONFIDENCE_NONE;
	
	WPXHeader *header = NULL;	
	
	// by-pass the OLE stream (if it exists) and returns the (sub) stream with the
	// WordPerfect document. 
	GsfInput *document = NULL;
	bool isDocumentOLE = false;
	
	// BIG BIG NOTE: very unsave on partial content!!!
	GsfInfile * ole = GSF_INFILE(gsf_infile_msole_new (input, NULL)); 
	if (ole != NULL) 
	{
		if (!partialContent)
		{
			document = gsf_infile_child_by_name(ole, "PerfectOffice_MAIN");
			g_object_unref(G_OBJECT (ole));
			if (document == NULL) // is an OLE document, but does not contain a WP document
			{
				g_object_unref(G_OBJECT(document));
				return WPD_CONFIDENCE_NONE; // TODO: throw an exception instead of return?
			}
		}
		else
		{
			// HELP! you can't get a child from partial content :-/
			// here we go again: 
			// TODO: dig trough the ole stream manually
			
			g_object_unref(G_OBJECT (ole));
			return WPD_CONFIDENCE_LIKELY; // HACK: too high, but for now, we'll live with it
		}
	}

	if (document == NULL)
		document = input;
	else
		isDocumentOLE = true;	
	
	try
	{
		// NOTE: even when passed partial content, we for now just assume that
		// we can extract the header from it. We could also read the major version
		// and the preceding -1 WPC stuff manually.
		header = WPXHeader::constructHeader(document);
		if (header)
		{
			switch (header->getMajorVersion())
			{
				case 0x00: // WP5 
					confidence = WPD_CONFIDENCE_EXCELLENT;
					break;
				case 0x01: // ???
					confidence = WPD_CONFIDENCE_NONE;
					break;
				case 0x02: // WP6
					confidence = WPD_CONFIDENCE_EXCELLENT;
					break;
				default:
					// unhandled file format
					confidence = WPD_CONFIDENCE_NONE;
					break;
			}
			DELETEP(header);
		}
		else
			confidence = WP42Heuristics::isWP42FileFormat(input, partialContent);
		
		if (document != NULL && isDocumentOLE)
			g_object_unref(G_OBJECT(document));
		
		return confidence;
	}	
	catch (FileException)
	{
		if (document != NULL && isDocumentOLE)
			g_object_unref(G_OBJECT(document));
		
		return WPD_CONFIDENCE_NONE;
	}
		
	return WPD_CONFIDENCE_NONE;
}

void WPDocument::parse(GsfInput *input, WPXHLListenerImpl *listenerImpl)
{
	WPXParser *parser = NULL;
	
	// by-pass the OLE stream (if it exists) and returns the (sub) stream with the
	// WordPerfect document. 
	GsfInput *document = NULL;
	bool isDocumentOLE = false;
	
	GsfInfile * ole = GSF_INFILE(gsf_infile_msole_new (input, NULL));
	if (ole != NULL) 
	{
		document = gsf_infile_child_by_name(ole, "PerfectOffice_MAIN");
		g_object_unref(G_OBJECT (ole));
		if (document == NULL) // is an OLE document, but does not contain a WP document
		{
			g_object_unref(G_OBJECT(document));
			return; // FIXME: throw an exception instead of return
		}
	}

	if (document == NULL)
		document = input;
	else
		isDocumentOLE = true;
	
	try
	{
		WPXHeader *header = WPXHeader::constructHeader(document);
		
		if (header)
		{
			switch (header->getMajorVersion())
			{
				case 0x00: // WP5 
					WPD_DEBUG_MSG(("WordPerfect: Using the WP5 parser.\n"));
					parser = new WP5Parser(document, header);
					parser->parse(listenerImpl);
					break;
				case 0x01: // ???
					WPD_DEBUG_MSG(("WordPerfect: Unsupported file format.\n"));
					// NOTE: WHEN WE KNOW WHICH PARSER TO INSTANIATE, UPDATE ::isFileFormatSupported AS WELL!
					break;
				case 0x02: // WP6
					WPD_DEBUG_MSG(("WordPerfect: Using the WP6 parser.\n"));
					parser = new WP6Parser(document, header);
					parser->parse(listenerImpl);
					break;
				default:
					// unhandled file format
					WPD_DEBUG_MSG(("WordPerfect: Unsupported file format.\n"));
					break;
			}
	
			//DELETEP(parser); // deletes the header as well
		}
		else
		{
			// WP file formats prior to version 5.x do not contain a generic 
			// header which can be used to determine which parser to instanciate. 
			// Use heuristics to determine with some certainty if we are dealing with
			// a file in the WP4.2 format.		
			int confidence = WP42Heuristics::isWP42FileFormat(document, false /* FIXME: allow for partial content */);
	
			if (confidence == WPD_CONFIDENCE_GOOD || confidence == WPD_CONFIDENCE_EXCELLENT)
			{
				WPD_DEBUG_MSG(("WordPerfect: Mostly likely the file format is WP4.2.\n\n"));
				WPD_DEBUG_MSG(("WordPerfect: Using the WP4.2 parser.\n\n"));
				WP42Parser *parser = new WP42Parser(document);
				parser->parse(listenerImpl);
				DELETEP(parser);
			}
		}
		
		if (document != NULL && isDocumentOLE)
			g_object_unref(G_OBJECT(document));
	}
	catch (FileException)
	{
		DELETEP(parser);
		if (document != NULL && isDocumentOLE)
			g_object_unref(G_OBJECT(document));
		throw FileException(); 
	}
}

/*

TOTALLY BROKEN, DO NOT USE!!!

void WPDocument::parse(GsfInput *input, WPXHLListenerImpl *listenerImpl, WPXFileType fileType)
{
	WPXParser *parser = NULL;
	WPXHeader *header = NULL;
	
	try
	{
		switch (fileType)
		{
			case WP42_DOCUMENT:
				WPD_DEBUG_MSG(("WordPerfect: Using the WP42 parser.\n"));
				parser = new WP42Parser(input);
				parser->parse(listenerImpl);
				DELETEP(parser);
				break;
			case WP5_DOCUMENT:
				WPD_DEBUG_MSG(("WordPerfect: Using the WP5 parser.\n"));
				header = new WPXHeader(input);
				parser = new WP5Parser(input, header);
				parser->parse(listenerImpl);
				DELETEP(parser);
				DELETEP(header);
				break;
			case WP6_DOCUMENT:
				WPD_DEBUG_MSG(("WordPerfect: Using the WP6 parser.\n"));
				header = new WPXHeader(input);
				parser = new WP6Parser(input, header);
				parser->parse(listenerImpl);
				DELETEP(parser);			
				DELETEP(header);
				break;
			default:
				// unhandled file format
				WPD_DEBUG_MSG(("WordPerfect: Unsupported file format.\n"));
				break;
		}
	}
	catch (NoFileHeaderException)
	{
		// should not happen
		DELETEP(parser);
		DELETEP(header);		
		throw NoFileHeaderException();
	}
	catch (FileException)
	{
		DELETEP(parser);
		DELETEP(header);
		throw FileException(); 
	}
}

*/


/*

TOTALLY BROKEN - MIGHT BE NICE TO HAVE

WPXFileType WPDocument::WPXParser::getFileType(GsfInput *input)
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
}*/

/*void WPDocument::parse(GsfInput *input, WPXLLListener *llistener)
{
	// TODO: implement me
}*/
