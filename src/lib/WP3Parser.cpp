/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2004 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2004 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include "WP3Parser.h"
#include "WPXHeader.h"
#include "WP3Part.h"
#include "WP3ContentListener.h"
#include "WP3StylesListener.h"
#include "WP3ResourceFork.h"
#include "libwpd_internal.h"
#include "WPXTable.h"

WP3Parser::WP3Parser(WPXInputStream *input, WPXHeader *header, WPXEncryption *encryption) :
	WPXParser(input, header, encryption)
{
}

WP3Parser::~WP3Parser()
{
}

WP3ResourceFork *WP3Parser::getResourceFork(WPXInputStream *input, WPXEncryption *encryption)
{
	WP3ResourceFork *resourceFork = 0;

	// Certain WP2 documents actually don't contain resource fork, so check for its existence
	if (!getHeader() || getHeader()->getDocumentOffset() <= 0x10)
	{
		WPD_DEBUG_MSG(("WP3Parser: Document does not contain resource fork\n"));
		return 0;
	}

	try
	{
		resourceFork = new WP3ResourceFork(input, encryption);
		return resourceFork;
	}
	catch(FileException)
	{
		DELETEP(resourceFork);
		throw FileException();
	}
}

void WP3Parser::parse(WPXInputStream *input, WPXEncryption *encryption, WP3Listener *listener)
{
	listener->startDocument();

	input->seek(getHeader()->getDocumentOffset(), WPX_SEEK_SET);

	WPD_DEBUG_MSG(("WordPerfect: Starting document body parse (position = %ld)\n",(long)input->tell()));

	parseDocument(input, encryption, listener);

	listener->endDocument();
}

// parseDocument: parses a document body (may call itself recursively, on other streams, or itself)
void WP3Parser::parseDocument(WPXInputStream *input, WPXEncryption *encryption, WP3Listener *listener)
{
	while (!input->atEOS())
	{
		uint8_t readVal;
		readVal = readU8(input, encryption);

		if (readVal == 0 || readVal == 0x7F || readVal == 0xFF)
		{
			// FIXME: VERIFY: is this IF clause correct? (0xFF seems to be OK at least)
			// do nothing: this token is meaningless and is likely just corruption
		}
		else if (readVal >= (uint8_t)0x01 && readVal <= (uint8_t)0x1F)
		{
			// control characters ?
		}
		else if (readVal >= (uint8_t)0x20 && readVal <= (uint8_t)0x7E)
		{
			listener->insertCharacter( readVal );
		}
		else
		{
			WP3Part *part = WP3Part::constructPart(input, encryption, readVal);
			if (part)
			{
				part->parse(listener);
				DELETEP(part);
			}
		}
	}
}

void WP3Parser::parse(WPXDocumentInterface *documentInterface)
{
	WPXInputStream *input = getInput();
	WPXEncryption *encryption = getEncryption();
	std::list<WPXPageSpan> pageList;
	WPXTableList tableList;
	WP3ResourceFork *resourceFork = 0;
	std::vector<WP3SubDocument *> subDocuments;

	try
	{
		resourceFork = getResourceFork(input, encryption);

		// do a "first-pass" parse of the document
		// gather table border information, page properties (per-page)
		WP3StylesListener stylesListener(pageList, tableList, subDocuments);
		stylesListener.setResourceFork(resourceFork);
		parse(input, encryption, &stylesListener);

		// postprocess the pageList == remove duplicate page spans due to the page breaks
		std::list<WPXPageSpan>::iterator previousPage = pageList.begin();
		for (std::list<WPXPageSpan>::iterator Iter=pageList.begin(); Iter != pageList.end(); /* Iter++ */)
		{
			if ((Iter != previousPage) && (*previousPage==*Iter))
			{
				(*previousPage).setPageSpan((*previousPage).getPageSpan() + (*Iter).getPageSpan());
				Iter = pageList.erase(Iter);
			}
			else
			{
				previousPage = Iter;
				++Iter;
			}
		}

		// second pass: here is where we actually send the messages to the target app
		// that are necessary to emit the body of the target document
		WP3ContentListener listener(pageList, subDocuments, documentInterface); // FIXME: SHOULD BE CONTENT_LISTENER, AND SHOULD BE PASSED TABLE DATA!
		listener.setResourceFork(resourceFork);
		parse(input, encryption, &listener);

		// cleanup section: free the used resources
		for (std::vector<WP3SubDocument *>::iterator iterSubDoc = subDocuments.begin(); iterSubDoc != subDocuments.end(); ++iterSubDoc)
		{
			if (*iterSubDoc)
				delete *iterSubDoc;
		}

		delete resourceFork;
	}
	catch(FileException)
	{
		WPD_DEBUG_MSG(("WordPerfect: File Exception. Parse terminated prematurely."));

		for (std::vector<WP3SubDocument *>::iterator iterSubDoc = subDocuments.begin(); iterSubDoc != subDocuments.end(); ++iterSubDoc)
		{
			if (*iterSubDoc)
				delete *iterSubDoc;
		}

		delete resourceFork;

		throw FileException();
	}
}

void WP3Parser::parseSubDocument(WPXDocumentInterface *documentInterface)
{
	std::list<WPXPageSpan> pageList;
	WPXTableList tableList;
	std::vector<WP3SubDocument *> subDocuments;

	WPXInputStream *input = getInput();

	try
	{
		WP3StylesListener stylesListener(pageList, tableList, subDocuments);
		stylesListener.startSubDocument();
		parseDocument(input, 0, &stylesListener);
		stylesListener.endSubDocument();

		input->seek(0, WPX_SEEK_SET);

		WP3ContentListener listener(pageList, subDocuments, documentInterface);
		listener.startSubDocument();
		parseDocument(input, 0, &listener);
		listener.endSubDocument();

		for (std::vector<WP3SubDocument *>::iterator iterSubDoc = subDocuments.begin(); iterSubDoc != subDocuments.end(); ++iterSubDoc)
			if (*iterSubDoc)
				delete *iterSubDoc;
	}
	catch(FileException)
	{
		WPD_DEBUG_MSG(("WordPerfect: File Exception. Parse terminated prematurely."));
		for (std::vector<WP3SubDocument *>::iterator iterSubDoc = subDocuments.begin(); iterSubDoc != subDocuments.end(); ++iterSubDoc)
			if (*iterSubDoc)
				delete *iterSubDoc;
		throw FileException();
	}
}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
