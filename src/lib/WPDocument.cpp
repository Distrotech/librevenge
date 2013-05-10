/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2003 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2003-2004 Marc Maurer (uwog@uwog.net)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#include "WPXHeader.h"
#include "WPXParser.h"
#include "WP1Parser.h"
#include "WP3Parser.h"
#include "WP42Parser.h"
#include "WP1Heuristics.h"
#include "WP42Heuristics.h"
#include "WP5Parser.h"
#include "WP6Parser.h"
#include "WPXEncryption.h"
#include "libwpd_internal.h"

/**
\mainpage libwpd documentation
This document contains both the libwpd API specification and the normal libwpd
documentation.
\section api_docs libwpd API documentation
The external libwpd API is provided by the WPDocument class. This class, combined
with the WPXDocumentInterface class, are the only two classes that will be of interest
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
\return A confidence value which represents the likelyhood that the content from
the input stream can be parsed
*/
WPDConfidence WPDocument::isFileFormatSupported(WPXInputStream *input)
{
	WPD_DEBUG_MSG(("WPDocument::isFileFormatSupported()\n"));

	// by-pass the OLE stream (if it exists) and returns the (sub) stream with the
	// WordPerfect document.
	WPXInputStream *document = 0;
	bool isDocumentOLE = false;

	if (input->isOLEStream())
	{
		document = input->getDocumentOLEStream("PerfectOffice_MAIN");
		if (document)
			isDocumentOLE = true;
		else
			return WPD_CONFIDENCE_NONE;
	}
	else
		document = input;

	try
	{
		WPDConfidence confidence = WPD_CONFIDENCE_NONE;
		WPXHeader *header = WPXHeader::constructHeader(document, 0);
		if (header)
		{
			switch (header->getFileType())
			{
			case 0x0a: // WordPerfect File
				switch (header->getMajorVersion())
				{
				case 0x00: // WP5
				case 0x02: // WP6+
					confidence = WPD_CONFIDENCE_EXCELLENT;
					break;
				default:
					// unhandled file format
					confidence = WPD_CONFIDENCE_NONE;
					break;
				}
				break;
			case 0x2c: // WP Mac File
				switch (header->getMajorVersion())
				{
				case 0x02: // WP Mac 2.x
				case 0x03: // WP Mac 3.0-3.5
				case 0x04: // WP Mac 3.5e
					confidence = WPD_CONFIDENCE_EXCELLENT;
					break;
				default:
					// unhandled file format
					confidence = WPD_CONFIDENCE_NONE;
					break;
				}
				break;
			default:
				// unhandled file type
				confidence = WPD_CONFIDENCE_NONE;
				break;
			}
			if (header->getDocumentEncryption())
			{
				if (header->getMajorVersion() == 0x02)
					confidence = WPD_CONFIDENCE_UNSUPPORTED_ENCRYPTION;
				else
					confidence = WPD_CONFIDENCE_SUPPORTED_ENCRYPTION;
			}
			DELETEP(header);
		}
		else
			confidence = WP1Heuristics::isWP1FileFormat(input, 0);
		if (confidence != WPD_CONFIDENCE_EXCELLENT && confidence != WPD_CONFIDENCE_SUPPORTED_ENCRYPTION)
			confidence = LIBWPD_MAX(confidence, WP42Heuristics::isWP42FileFormat(input, 0));


		// dispose of the reference to the ole input stream, if we allocated one
		if (document && isDocumentOLE)
			DELETEP(document);

		return confidence;
	}
	catch (FileException)
	{
		WPD_DEBUG_MSG(("File Exception trapped\n"));

		// dispose of the reference to the ole input stream, if we allocated one
		if (document && isDocumentOLE)
			DELETEP(document);

		return WPD_CONFIDENCE_NONE;
	}
	catch (...)
	{
		WPD_DEBUG_MSG(("Unknown Exception trapped\n"));

		// dispose of the reference to the ole input stream, if we allocated one
		if (document && isDocumentOLE)
			DELETEP(document);

		return WPD_CONFIDENCE_NONE;
	}
}

/**
Checks whether the given password was used to encrypt the document
\param input The input stream
\param password The password used to protect the document or NULL if the document is not protected
\return A value which indicates between the given password and the password that was used to protect the document
*/
WPDPasswordMatch WPDocument::verifyPassword(WPXInputStream *input, const char *password)
{
	if (!password)
		return WPD_PASSWORD_MATCH_DONTKNOW;
	if (!input)
		return WPD_PASSWORD_MATCH_DONTKNOW;

	input->seek(0, WPX_SEEK_SET);

	WPDPasswordMatch passwordMatch = WPD_PASSWORD_MATCH_NONE;
	WPXEncryption encryption(password);

	WPXHeader *header = 0;

	WPD_DEBUG_MSG(("WPDocument::verifyPassword()\n"));

	// by-pass the OLE stream (if it exists) and returns the (sub) stream with the
	// WordPerfect document.
	WPXInputStream *document = 0;
	bool isDocumentOLE = false;

	if (input->isOLEStream())
	{
		document = input->getDocumentOLEStream("PerfectOffice_MAIN");
		if (document)
			isDocumentOLE = true;
		else
			return WPD_PASSWORD_MATCH_NONE;
	}
	else
		document = input;

	try
	{
		header = WPXHeader::constructHeader(document, 0);
		if (header)
		{
			if (header->getDocumentEncryption())
			{
				if (header->getMajorVersion() == 0x02)
					passwordMatch = WPD_PASSWORD_MATCH_DONTKNOW;
				else if (header->getDocumentEncryption() == encryption.getCheckSum())
					passwordMatch = WPD_PASSWORD_MATCH_OK;
			}
			DELETEP(header);
		}
		else
			passwordMatch = WP1Heuristics::verifyPassword(input, password);
		if (passwordMatch == WPD_PASSWORD_MATCH_NONE)
			passwordMatch = LIBWPD_MAX(passwordMatch, WP42Heuristics::verifyPassword(input, password));


		// dispose of the reference to the ole input stream, if we allocated one
		if (document && isDocumentOLE)
			DELETEP(document);

		return passwordMatch;
	}
	catch (FileException)
	{
		WPD_DEBUG_MSG(("File Exception trapped\n"));

		// dispose of the reference to the ole input stream, if we allocated one
		if (document && isDocumentOLE)
			DELETEP(document);

		return WPD_PASSWORD_MATCH_NONE;
	}
	catch (...)
	{
		WPD_DEBUG_MSG(("Unknown Exception trapped\n"));

		// dispose of the reference to the ole input stream, if we allocated one
		if (document && isDocumentOLE)
			DELETEP(document);

		return WPD_PASSWORD_MATCH_NONE;
	}
}

/**
Parses the input stream content. It will make callbacks to the functions provided by a
WPXDocumentInterface class implementation when needed. This is often commonly called the
'main parsing routine'.
\param input The input stream
\param documentInterface A WPXDocumentInterface implementation
\param password The password used to protect the document or NULL if the document
is not protected
\return A value that indicates whether the conversion was successful and in case it
was not, it indicates the reason of the error
*/
WPDResult WPDocument::parse(WPXInputStream *input, WPXDocumentInterface *documentInterface, const char *password)
{
	if (!input)
		return WPD_FILE_ACCESS_ERROR;

	if (password && verifyPassword(input, password) != WPD_PASSWORD_MATCH_OK)
		return WPD_PASSWORD_MISSMATCH_ERROR;

	input->seek(0, WPX_SEEK_SET);

	WPXParser *parser = 0;

	// by-pass the OLE stream (if it exists) and returns the (sub) stream with the
	// WordPerfect document.

	WPXInputStream *document = 0;
	bool isDocumentOLE = false;

	WPD_DEBUG_MSG(("WPDocument::parse()\n"));
	if (input->isOLEStream())
	{
		document = input->getDocumentOLEStream("PerfectOffice_MAIN");
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
		WPXEncryption *encryption = 0;
		WPXHeader *header = WPXHeader::constructHeader(document, 0);

		if (header)
		{
			switch (header->getFileType())
			{
			case 0x0a: // WordPerfect File
				switch (header->getMajorVersion())
				{
				case 0x00: // WP5
					WPD_DEBUG_MSG(("WordPerfect: Using the WP5 parser.\n"));
					if (password)
						encryption = new WPXEncryption(password, 16);
					parser = new WP5Parser(document, header, encryption);
					parser->parse(documentInterface);
					break;
				case 0x02: // WP6
					WPD_DEBUG_MSG(("WordPerfect: Using the WP6 parser.\n"));
					if (password)
					{
						delete header;
						throw UnsupportedEncryptionException();
					}
					parser = new WP6Parser(document, header, encryption);
					parser->parse(documentInterface);
					break;
				default:
					// unhandled file format
					WPD_DEBUG_MSG(("WordPerfect: Unsupported file format.\n"));
					break;
				}
				break;
			case 0x2c: // WP Mac File
				switch (header->getMajorVersion())
				{
				case 0x02: // WP Mac 2.x
				case 0x03: // WP Mac 3.0-3.5
				case 0x04: // WP Mac 3.5e
					WPD_DEBUG_MSG(("WordPerfect: Using the WP3 parser.\n"));
					if (password)
						encryption = new WPXEncryption(password, header->getDocumentOffset());
					parser = new WP3Parser(document, header, encryption);
					parser->parse(documentInterface);
					break;
				default:
					// unhandled file format
					WPD_DEBUG_MSG(("WordPerfect: Unsupported file format.\n"));
					break;
				}
				break;
			default:
				// unhandled file format
				WPD_DEBUG_MSG(("WordPerfect: Unsupported file type.\n"));
				break;
			}
			DELETEP(parser);
			DELETEP(header);
		}
		else
		{
			// WP file formats prior to version 5.x do not contain a generic
			// header which can be used to determine which parser to instanciate.
			// Use heuristics to determine with some certainty if we are dealing with
			// a file in the WP4.2 format or WP Mac 1.x format.
			if (WP1Heuristics::isWP1FileFormat(document, password) == WPD_CONFIDENCE_EXCELLENT)
			{
				WPD_DEBUG_MSG(("WordPerfect: Mostly likely the file format is WP Mac 1.x.\n\n"));
				WPD_DEBUG_MSG(("WordPerfect: Using the WP Mac 1.x parser.\n\n"));
				if (password)
					encryption = new WPXEncryption(password, 6);
				parser = new WP1Parser(document, encryption);
				parser->parse(documentInterface);
				DELETEP(parser);
			}
			else if (WP42Heuristics::isWP42FileFormat(document, password) == WPD_CONFIDENCE_EXCELLENT)
			{
				WPD_DEBUG_MSG(("WordPerfect: Mostly likely the file format is WP4.2.\n\n"));
				WPD_DEBUG_MSG(("WordPerfect: Using the WP4.2 parser.\n\n"));
				if (password)
				{
					encryption = new WPXEncryption(password, 6);
					input->seek(6, WPX_SEEK_SET);
				}
				parser = new WP42Parser(document, encryption);
				parser->parse(documentInterface);
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
	if (document && isDocumentOLE)
		DELETEP(document);

	return error;
}

WPDResult WPDocument::parseSubDocument(WPXInputStream *input, WPXDocumentInterface *documentInterface, WPDFileFormat fileFormat)
{
	WPXParser *parser = 0;

	WPDResult error = WPD_OK;

	try
	{

		switch (fileFormat)
		{
		case WPD_FILE_FORMAT_WP6:
			parser = new WP6Parser(input, 0, 0);
			break;
		case WPD_FILE_FORMAT_WP5:
			parser = new WP5Parser(input, 0, 0);
			break;
		case WPD_FILE_FORMAT_WP42:
			parser = new WP42Parser(input, 0);
			break;
		case WPD_FILE_FORMAT_WP3:
			parser = new WP3Parser(input, 0, 0);
			break;
		case WPD_FILE_FORMAT_WP1:
			parser = new WP1Parser(input, 0);
			break;
		case WPD_FILE_FORMAT_UNKNOWN:
		default:
			DELETEP(parser);
			return WPD_UNKNOWN_ERROR;
		}

		if (parser)
			parser->parseSubDocument(documentInterface);
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
	return error;
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
