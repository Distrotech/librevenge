/* libwpd
 * Copyright (C) 2003 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2003-2004 Marc Maurer (j.m.maurer@student.utwente.nl)
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

#include <stdlib.h>
#include <string.h>
#include "WPDocument.h"
#include "WPXHeader.h"
#include "WPXParser.h"
#include "WP3Parser.h"
#include "WP42Parser.h"
#include "WP42Heuristics.h"
#include "WP5Parser.h"
#include "WP6Parser.h"
#include "WPXStream.h"
#include "libwpd_internal.h"

/**
\mainpage libwpd documentation
This document contains both the libwpd API specification and the normal libwpd
documentation.
\section api_docs libwpd API documentation
The external libwpd API is provided by the WPDocument class. This class, combined
with the WPXHLListenerImpl class, are the only two classes that will be of interest
for the application programmer using libwpd.
\section lib_docs libwpd documentation
If you are interrested in the structure of libwpd itself, this whole document
would be a good starting point for exploring the interals of libwpd. Mind that
this document is a work-in-progress, and will most likely not cover libwpd for
the full 100%.
*/

/**
Analyzes the content of an input stream to see if it can be parsed
\param input The input stream
\param partialContent A boolean which states if the content from the input stream
represents the full contents of a WordPerfect file, or just the first X bytes
\return A confidence value which represents the likelyhood that the content from
the input stream can be parsed
*/
WPDConfidence WPDocument::isFileFormatSupported(WPXInputStream *input, bool partialContent)
{
	WPDConfidence confidence = WPD_CONFIDENCE_NONE;

	WPXHeader *header = NULL;

	WPD_DEBUG_MSG(("WPDocument::isFileFormatSupported()\n"));

	// by-pass the OLE stream (if it exists) and returns the (sub) stream with the
	// WordPerfect document.
	WPXInputStream *document = NULL;
	bool isDocumentOLE = false;

	// BIG BIG NOTE: very unsafe on partial content!!!
	if (input->isOLEStream())
	{
		document = input->getDocumentOLEStream();
		if (document)
			isDocumentOLE = true;
		else
		{
			if (partialContent)
				return WPD_CONFIDENCE_LIKELY; // HACK: too high, but for now, we'll live with it
			else
				return WPD_CONFIDENCE_NONE;
		}
	}
	else
		document = input;

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
				case 0x02: // WP6
				case 0x03: // WP Mac 3.0-3.5
				case 0x04: // WP Mac 3.5e
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

		// dispose of the reference to the ole input stream, if we allocated one
		if (document != NULL && isDocumentOLE)
			DELETEP(document);

		return confidence;
	}
	catch (FileException)
	{
		WPD_DEBUG_MSG(("File Exception trapped\n"));

		// dispose of the reference to the ole input stream, if we allocated one
		if (document != NULL && isDocumentOLE)
			DELETEP(document);

		return WPD_CONFIDENCE_NONE;
	}
	catch (...)
	{
		WPD_DEBUG_MSG(("Unknown Exception trapped\n"));

		// dispose of the reference to the ole input stream, if we allocated one
		if (document != NULL && isDocumentOLE)
			DELETEP(document);

		return WPD_CONFIDENCE_NONE;
	}

	return WPD_CONFIDENCE_NONE;
}

/**
Parses the input stream content. It will make callbacks to the functions provided by a
WPXHLListenerImpl class implementation when needed. This is often commonly called the
'main parsing routine'.
\param input The input stream
\param listenerImpl A WPXListener implementation
*/
WPDResult WPDocument::parse(WPXInputStream *input, WPXHLListenerImpl *listenerImpl)
{
	WPXParser *parser = NULL;

	// by-pass the OLE stream (if it exists) and returns the (sub) stream with the
	// WordPerfect document.

	WPXInputStream *document = NULL;
	bool isDocumentOLE = false;

	WPD_DEBUG_MSG(("WPDocument::parse()\n"));
	if (input->isOLEStream())
	{
		document = input->getDocumentOLEStream();
		if (document)
			isDocumentOLE = true;
		else
			return WPD_OLE_ERROR;
	}
	else
		document = input;

	WPDResult error = WPD_OK;

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
				case 0x02: // WP6
					WPD_DEBUG_MSG(("WordPerfect: Using the WP6 parser.\n"));
					parser = new WP6Parser(document, header);
					parser->parse(listenerImpl);
					break;
				case 0x03: // WP Mac 3.0-3.5
				case 0x04: // WP Mac 3.5e
					WPD_DEBUG_MSG(("WordPerfect: Using the WP3 parser.\n"));
					parser = new WP3Parser(document, header);
					parser->parse(listenerImpl);
					break;
				default:
					// unhandled file format
					WPD_DEBUG_MSG(("WordPerfect: Unsupported file format.\n"));
					break;
			}

			DELETEP(parser); // deletes the header as well
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
			else
				error = WPD_FILE_ACCESS_ERROR;
		}
	}

	catch (FileException)
	{
		WPD_DEBUG_MSG(("File Exception trapped\n"));
		error = WPD_FILE_ACCESS_ERROR;
	}
	catch (ParseException)
	{
		WPD_DEBUG_MSG(("Parse Exception trapped\n"));
		error = WPD_PARSE_ERROR;
	}
	catch (UnsupportedEncryptionException)
	{
		WPD_DEBUG_MSG(("Encrypted document exception trapped\n"));
		error = WPD_UNSUPPORTED_ENCRYPTION_ERROR;
	}
	catch (...)
	{
		WPD_DEBUG_MSG(("Unknown Exception trapped\n"));
		error = WPD_UNKNOWN_ERROR; 
	}

	DELETEP(parser);
	if (document != NULL && isDocumentOLE)
		DELETEP(document);

	return error;
}

/*

TOTALLY BROKEN, DO NOT USE!!!
I MEAN IT!

void WPDocument::parse(WPXInputStream *input, WPXHLListenerImpl *listenerImpl, WPXFileType fileType)
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

WPXFileType WPDocument::WPXParser::getFileType(WPXInputStream *input)
{

}*/
