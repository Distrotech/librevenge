/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
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
 * For further information visit http://librevenge.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#include "RVNGHeader.h"
#include "RVNGParser.h"
#include "WP1Parser.h"
#include "WP3Parser.h"
#include "WP42Parser.h"
#include "WP1Heuristics.h"
#include "WP42Heuristics.h"
#include "WP5Parser.h"
#include "WP6Parser.h"
#include "RVNGEncryption.h"
#include "librevenge_internal.h"

/**
\mainpage librevenge documentation
This document contains both the librevenge API specification and the normal librevenge
documentation.
\section api_docs librevenge API documentation
The external librevenge API is provided by the RVNGocument class. This class, combined
with the RVNGDocumentInterface class, are the only two classes that will be of interest
for the application programmer using librevenge.
\section lib_docs librevenge documentation
If you are interrested in the structure of librevenge itself, this whole document
would be a good starting point for exploring the interals of librevenge. Mind that
this document is a work-in-progress, and will most likely not cover librevenge for
the full 100%.
*/

/**
Analyzes the content of an input stream to see if it can be parsed
\param input The input stream
\return A confidence value which represents the likelyhood that the content from
the input stream can be parsed
*/
RVNGConfidence RVNGocument::isFileFormatSupported(RVNGInputStream *input)
{
	RVNG_DEBUG_MSG(("RVNGocument::isFileFormatSupported()\n"));

	// by-pass the OLE stream (if it exists) and returns the (sub) stream with the
	// WordPerfect document.
	RVNGInputStream *document = 0;
	bool isDocumentOLE = false;

	if (input->isOLEStream())
	{
		document = input->getDocumentOLEStream("PerfectOffice_MAIN");
		if (document)
			isDocumentOLE = true;
		else
			return RVNG_CONFIDENCE_NONE;
	}
	else
		document = input;

	try
	{
		RVNGConfidence confidence = RVNG_CONFIDENCE_NONE;
		RVNGHeader *header = RVNGHeader::constructHeader(document, 0);
		if (header)
		{
			switch (header->getFileType())
			{
			case 0x0a: // WordPerfect File
				switch (header->getMajorVersion())
				{
				case 0x00: // WP5
				case 0x02: // WP6+
					confidence = RVNG_CONFIDENCE_EXCELLENT;
					break;
				default:
					// unhandled file format
					confidence = RVNG_CONFIDENCE_NONE;
					break;
				}
				break;
			case 0x2c: // WP Mac File
				switch (header->getMajorVersion())
				{
				case 0x02: // WP Mac 2.x
				case 0x03: // WP Mac 3.0-3.5
				case 0x04: // WP Mac 3.5e
					confidence = RVNG_CONFIDENCE_EXCELLENT;
					break;
				default:
					// unhandled file format
					confidence = RVNG_CONFIDENCE_NONE;
					break;
				}
				break;
			default:
				// unhandled file type
				confidence = RVNG_CONFIDENCE_NONE;
				break;
			}
			if (header->getDocumentEncryption())
			{
				if (header->getMajorVersion() == 0x02)
					confidence = RVNG_CONFIDENCE_UNSUPPORTED_ENCRYPTION;
				else
					confidence = RVNG_CONFIDENCE_SUPPORTED_ENCRYPTION;
			}
			DELETEP(header);
		}
		else
			confidence = WP1Heuristics::isWP1FileFormat(input, 0);
		if (confidence != RVNG_CONFIDENCE_EXCELLENT && confidence != RVNG_CONFIDENCE_SUPPORTED_ENCRYPTION)
			confidence = LIBREVENGE_MAX(confidence, WP42Heuristics::isWP42FileFormat(input, 0));


		// dispose of the reference to the ole input stream, if we allocated one
		if (document && isDocumentOLE)
			DELETEP(document);

		return confidence;
	}
	catch (FileException)
	{
		RVNG_DEBUG_MSG(("File Exception trapped\n"));

		// dispose of the reference to the ole input stream, if we allocated one
		if (document && isDocumentOLE)
			DELETEP(document);

		return RVNG_CONFIDENCE_NONE;
	}
	catch (...)
	{
		RVNG_DEBUG_MSG(("Unknown Exception trapped\n"));

		// dispose of the reference to the ole input stream, if we allocated one
		if (document && isDocumentOLE)
			DELETEP(document);

		return RVNG_CONFIDENCE_NONE;
	}
}

/**
Checks whether the given password was used to encrypt the document
\param input The input stream
\param password The password used to protect the document or NULL if the document is not protected
\return A value which indicates between the given password and the password that was used to protect the document
*/
RVNGPasswordMatch RVNGocument::verifyPassword(RVNGInputStream *input, const char *password)
{
	if (!password)
		return RVNG_PASSWORD_MATCH_DONTKNOW;
	if (!input)
		return RVNG_PASSWORD_MATCH_DONTKNOW;

	input->seek(0, RVNG_SEEK_SET);

	RVNGPasswordMatch passwordMatch = RVNG_PASSWORD_MATCH_NONE;
	RVNGEncryption encryption(password);

	RVNGHeader *header = 0;

	RVNG_DEBUG_MSG(("RVNGocument::verifyPassword()\n"));

	// by-pass the OLE stream (if it exists) and returns the (sub) stream with the
	// WordPerfect document.
	RVNGInputStream *document = 0;
	bool isDocumentOLE = false;

	if (input->isOLEStream())
	{
		document = input->getDocumentOLEStream("PerfectOffice_MAIN");
		if (document)
			isDocumentOLE = true;
		else
			return RVNG_PASSWORD_MATCH_NONE;
	}
	else
		document = input;

	try
	{
		header = RVNGHeader::constructHeader(document, 0);
		if (header)
		{
			if (header->getDocumentEncryption())
			{
				if (header->getMajorVersion() == 0x02)
					passwordMatch = RVNG_PASSWORD_MATCH_DONTKNOW;
				else if (header->getDocumentEncryption() == encryption.getCheckSum())
					passwordMatch = RVNG_PASSWORD_MATCH_OK;
			}
			DELETEP(header);
		}
		else
			passwordMatch = WP1Heuristics::verifyPassword(input, password);
		if (passwordMatch == RVNG_PASSWORD_MATCH_NONE)
			passwordMatch = LIBREVENGE_MAX(passwordMatch, WP42Heuristics::verifyPassword(input, password));


		// dispose of the reference to the ole input stream, if we allocated one
		if (document && isDocumentOLE)
			DELETEP(document);

		return passwordMatch;
	}
	catch (FileException)
	{
		RVNG_DEBUG_MSG(("File Exception trapped\n"));

		// dispose of the reference to the ole input stream, if we allocated one
		if (document && isDocumentOLE)
			DELETEP(document);

		return RVNG_PASSWORD_MATCH_NONE;
	}
	catch (...)
	{
		RVNG_DEBUG_MSG(("Unknown Exception trapped\n"));

		// dispose of the reference to the ole input stream, if we allocated one
		if (document && isDocumentOLE)
			DELETEP(document);

		return RVNG_PASSWORD_MATCH_NONE;
	}
}

/**
Parses the input stream content. It will make callbacks to the functions provided by a
RVNGDocumentInterface class implementation when needed. This is often commonly called the
'main parsing routine'.
\param input The input stream
\param documentInterface A RVNGDocumentInterface implementation
\param password The password used to protect the document or NULL if the document
is not protected
\return A value that indicates whether the conversion was successful and in case it
was not, it indicates the reason of the error
*/
RVNGResult RVNGocument::parse(RVNGInputStream *input, RVNGDocumentInterface *documentInterface, const char *password)
{
	if (!input)
		return RVNG_FILE_ACCESS_ERROR;

	if (password && verifyPassword(input, password) != RVNG_PASSWORD_MATCH_OK)
		return RVNG_PASSWORD_MISSMATCH_ERROR;

	input->seek(0, RVNG_SEEK_SET);

	RVNGParser *parser = 0;

	// by-pass the OLE stream (if it exists) and returns the (sub) stream with the
	// WordPerfect document.

	RVNGInputStream *document = 0;
	bool isDocumentOLE = false;

	RVNG_DEBUG_MSG(("RVNGocument::parse()\n"));
	if (input->isOLEStream())
	{
		document = input->getDocumentOLEStream("PerfectOffice_MAIN");
		if (document)
			isDocumentOLE = true;
		else
			return RVNG_OLE_ERROR;
	}
	else
		document = input;

	RVNGResult error = RVNG_OK;

	try
	{
		RVNGEncryption *encryption = 0;
		RVNGHeader *header = RVNGHeader::constructHeader(document, 0);

		if (header)
		{
			switch (header->getFileType())
			{
			case 0x0a: // WordPerfect File
				switch (header->getMajorVersion())
				{
				case 0x00: // WP5
					RVNG_DEBUG_MSG(("WordPerfect: Using the WP5 parser.\n"));
					if (password)
						encryption = new RVNGEncryption(password, 16);
					parser = new WP5Parser(document, header, encryption);
					parser->parse(documentInterface);
					break;
				case 0x02: // WP6
					RVNG_DEBUG_MSG(("WordPerfect: Using the WP6 parser.\n"));
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
					RVNG_DEBUG_MSG(("WordPerfect: Unsupported file format.\n"));
					break;
				}
				break;
			case 0x2c: // WP Mac File
				switch (header->getMajorVersion())
				{
				case 0x02: // WP Mac 2.x
				case 0x03: // WP Mac 3.0-3.5
				case 0x04: // WP Mac 3.5e
					RVNG_DEBUG_MSG(("WordPerfect: Using the WP3 parser.\n"));
					if (password)
						encryption = new RVNGEncryption(password, header->getDocumentOffset());
					parser = new WP3Parser(document, header, encryption);
					parser->parse(documentInterface);
					break;
				default:
					// unhandled file format
					RVNG_DEBUG_MSG(("WordPerfect: Unsupported file format.\n"));
					break;
				}
				break;
			default:
				// unhandled file format
				RVNG_DEBUG_MSG(("WordPerfect: Unsupported file type.\n"));
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
			if (WP1Heuristics::isWP1FileFormat(document, password) == RVNG_CONFIDENCE_EXCELLENT)
			{
				RVNG_DEBUG_MSG(("WordPerfect: Mostly likely the file format is WP Mac 1.x.\n\n"));
				RVNG_DEBUG_MSG(("WordPerfect: Using the WP Mac 1.x parser.\n\n"));
				if (password)
					encryption = new RVNGEncryption(password, 6);
				parser = new WP1Parser(document, encryption);
				parser->parse(documentInterface);
				DELETEP(parser);
			}
			else if (WP42Heuristics::isWP42FileFormat(document, password) == RVNG_CONFIDENCE_EXCELLENT)
			{
				RVNG_DEBUG_MSG(("WordPerfect: Mostly likely the file format is WP4.2.\n\n"));
				RVNG_DEBUG_MSG(("WordPerfect: Using the WP4.2 parser.\n\n"));
				if (password)
				{
					encryption = new RVNGEncryption(password, 6);
					input->seek(6, RVNG_SEEK_SET);
				}
				parser = new WP42Parser(document, encryption);
				parser->parse(documentInterface);
				DELETEP(parser);
			}
			else

				error = RVNG_FILE_ACCESS_ERROR;
		}
	}

	catch (FileException)
	{
		RVNG_DEBUG_MSG(("File Exception trapped\n"));
		error = RVNG_FILE_ACCESS_ERROR;
	}
	catch (ParseException)
	{
		RVNG_DEBUG_MSG(("Parse Exception trapped\n"));
		error = RVNG_PARSE_ERROR;
	}
	catch (UnsupportedEncryptionException)
	{
		RVNG_DEBUG_MSG(("Encrypted document exception trapped\n"));
		error = RVNG_UNSUPPORTED_ENCRYPTION_ERROR;
	}
	catch (...)
	{
		RVNG_DEBUG_MSG(("Unknown Exception trapped\n"));
		error = RVNG_UNKNOWN_ERROR;
	}

	DELETEP(parser);
	if (document && isDocumentOLE)
		DELETEP(document);

	return error;
}

RVNGResult RVNGocument::parseSubDocument(RVNGInputStream *input, RVNGDocumentInterface *documentInterface, RVNGFileFormat fileFormat)
{
	RVNGParser *parser = 0;

	RVNGResult error = RVNG_OK;

	try
	{

		switch (fileFormat)
		{
		case RVNG_FILE_FORMAT_WP6:
			parser = new WP6Parser(input, 0, 0);
			break;
		case RVNG_FILE_FORMAT_WP5:
			parser = new WP5Parser(input, 0, 0);
			break;
		case RVNG_FILE_FORMAT_WP42:
			parser = new WP42Parser(input, 0);
			break;
		case RVNG_FILE_FORMAT_WP3:
			parser = new WP3Parser(input, 0, 0);
			break;
		case RVNG_FILE_FORMAT_WP1:
			parser = new WP1Parser(input, 0);
			break;
		case RVNG_FILE_FORMAT_UNKNOWN:
		default:
			DELETEP(parser);
			return RVNG_UNKNOWN_ERROR;
		}

		if (parser)
			parser->parseSubDocument(documentInterface);
	}
	catch (FileException)
	{
		RVNG_DEBUG_MSG(("File Exception trapped\n"));
		error = RVNG_FILE_ACCESS_ERROR;
	}
	catch (ParseException)
	{
		RVNG_DEBUG_MSG(("Parse Exception trapped\n"));
		error = RVNG_PARSE_ERROR;
	}
	catch (UnsupportedEncryptionException)
	{
		RVNG_DEBUG_MSG(("Encrypted document exception trapped\n"));
		error = RVNG_UNSUPPORTED_ENCRYPTION_ERROR;
	}
	catch (...)
	{
		RVNG_DEBUG_MSG(("Unknown Exception trapped\n"));
		error = RVNG_UNKNOWN_ERROR;
	}
	DELETEP(parser);
	return error;
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
