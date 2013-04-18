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
 * Copyright (C) 2003 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include "WP5Parser.h"
#include "WPXHeader.h"
#include "WP5Part.h"
#include "WP5ContentListener.h"
#include "WP5StylesListener.h"
#include "libwpd_internal.h"
#include "WPXTable.h"
#include "WP5PrefixData.h"
#include "WP5ListFontsUsedPacket.h"
#include "WP5FontNameStringPoolPacket.h"

WP5Parser::WP5Parser(WPXInputStream *input, WPXHeader *header, WPXEncryption *encryption) :
	WPXParser(input, header, encryption)
{
}

WP5Parser::~WP5Parser()
{
}

WP5PrefixData *WP5Parser::getPrefixData(WPXInputStream *input, WPXEncryption *encryption)
{
	WP5PrefixData *prefixData = 0;
	try
	{
		prefixData = new WP5PrefixData(input, encryption);
		return prefixData;
	}
	catch(FileException)
	{
		DELETEP(prefixData);
		throw FileException();
	}
}

void WP5Parser::parse(WPXInputStream *input, WPXEncryption *encryption, WP5Listener *listener)
{
	listener->startDocument();

	input->seek(getHeader()->getDocumentOffset(), WPX_SEEK_SET);

	WPD_DEBUG_MSG(("WordPerfect: Starting document body parse (position = %ld)\n", (long)input->tell()));

	parseDocument(input, encryption, listener);

	listener->endDocument();
}

// parseDocument: parses a document body (may call itself recursively, on other streams, or itself)
void WP5Parser::parseDocument(WPXInputStream *input, WPXEncryption *encryption, WP5Listener *listener)
{
	while (!input->atEOS())
	{
		uint8_t readVal;
		readVal = readU8(input, encryption);

		if (readVal == 0 || readVal == 0x7F || readVal == 0xFF)
		{
			// do nothing: this token is meaningless and is likely just corruption
		}
		else if (readVal >= (uint8_t)0x01 && readVal <= (uint8_t)0x1F)
		{
			// control characters

			switch (readVal)
			{
			case 0x0A: // hard new line
				listener->insertEOL();
				break;
			case 0x0B: // soft new page (convert like space)
				listener->insertCharacter((uint32_t) ' ');
				listener->insertBreak(WPX_SOFT_PAGE_BREAK);
				break;
			case 0x0C: // hard new page
				listener->insertBreak(WPX_PAGE_BREAK);
				break;
			case 0x0D: // soft new line (convert like space)
				listener->insertCharacter((uint32_t) ' ');
				break;
			default:
				// unsupported or undocumented token, ignore
				break;
			}
		}
		else if (readVal >= (uint8_t)0x20 && readVal <= (uint8_t)0x7E)
		{
			listener->insertCharacter( readVal );
		}
		else
		{
			WP5Part *part = WP5Part::constructPart(input, encryption, readVal);
			if (part)
			{
				part->parse(listener);
				DELETEP(part);
			}
		}
	}
}

void WP5Parser::parse(WPXDocumentInterface *documentInterface)
{
	WPXInputStream *input = getInput();
	WPXEncryption *encryption = getEncryption();
	std::list<WPXPageSpan> pageList;
	WPXTableList tableList;
	WP5PrefixData *prefixData = 0;
	std::vector<WP5SubDocument *> subDocuments;

	try
	{
		prefixData = getPrefixData(input, encryption);

		// do a "first-pass" parse of the document
		// gather table border information, page properties (per-page)
		WP5StylesListener stylesListener(pageList, tableList, subDocuments);
		parse(input, encryption, &stylesListener);

		// postprocess the pageList == remove duplicate page spans due to the page breaks
		std::list<WPXPageSpan>::iterator previousPage = pageList.begin();
		for (std::list<WPXPageSpan>::iterator Iter=pageList.begin(); Iter != pageList.end();)
		{
			if ((Iter != previousPage) && ((*previousPage)==(*Iter)))
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
		WP5ContentListener listener(pageList, subDocuments, documentInterface); // FIXME: SHOULD BE PASSED TABLE DATA!
		listener.setPrefixData(prefixData);


		// According the documentation, first font in the font list is the default font, so use it as such
		// This is the quick and dirty way and maybe one could do it differently
		// FIXME: UGLY, UGLY, UGLY!!! FIND A BETTER WAY TO ACHIEVE THE SAME
		unsigned tmpFontNameOffset = 0; // The default font in WP5 is at the position 0
		double tmpFontSize = 12.0;
		WPXString tmpFontName("Times New Roman");
		bool tmpHasFontsUsedPacket = true;

		if (listener.getGeneralPacketData(15))
		{
			tmpFontSize = static_cast<const WP5ListFontsUsedPacket *>(listener.getGeneralPacketData(15))->getFontSize(0);
			tmpFontNameOffset = static_cast<const WP5ListFontsUsedPacket *>(listener.getGeneralPacketData(15))->getFontNameOffset(0);
		}
		else if (listener.getGeneralPacketData(2))
		{
			tmpFontSize = static_cast<const WP5ListFontsUsedPacket *>(listener.getGeneralPacketData(2))->getFontSize(0);
			tmpFontNameOffset = static_cast<const WP5ListFontsUsedPacket *>(listener.getGeneralPacketData(2))->getFontNameOffset(0);
		}
		else
			tmpHasFontsUsedPacket = false;

		if (tmpHasFontsUsedPacket && (listener.getGeneralPacketData(7)))
			tmpFontName = static_cast<const WP5FontNameStringPoolPacket *>(listener.getGeneralPacketData(7))->getFontName(tmpFontNameOffset);

		listener.setFont(tmpFontName, tmpFontSize);
		listener.setDefaultFont(tmpFontName, tmpFontSize);
		// FIXME: UGLY, UGLY, UGLY!!! FIND A BETTER WAY TO ACHIEVE THE SAME


		parse(input, encryption, &listener);

		// cleanup section: free the used resources
		delete prefixData;
		for (std::vector<WP5SubDocument *>::iterator iterSubDoc = subDocuments.begin(); iterSubDoc != subDocuments.end(); ++iterSubDoc)
		{
			if (*iterSubDoc)
				delete (*iterSubDoc);
		}
	}
	catch(FileException)
	{
		WPD_DEBUG_MSG(("WordPerfect: File Exception. Parse terminated prematurely."));

		delete prefixData;
		for (std::vector<WP5SubDocument *>::iterator iterSubDoc = subDocuments.begin(); iterSubDoc != subDocuments.end(); ++iterSubDoc)
		{
			if (*iterSubDoc)
				delete (*iterSubDoc);
		}

		throw FileException();
	}
}

void WP5Parser::parseSubDocument(WPXDocumentInterface *documentInterface)
{
	std::list<WPXPageSpan> pageList;
	WPXTableList tableList;
	std::vector<WP5SubDocument *> subDocuments;

	WPXInputStream *input = getInput();

	try
	{
		WP5StylesListener stylesListener(pageList, tableList, subDocuments);
		stylesListener.startSubDocument();
		parseDocument(input, 0, &stylesListener);
		stylesListener.endSubDocument();

		input->seek(0, WPX_SEEK_SET);

		WP5ContentListener listener(pageList, subDocuments, documentInterface);
		listener.startSubDocument();
		parseDocument(input, 0, &listener);
		listener.endSubDocument();

		for (std::vector<WP5SubDocument *>::iterator iterSubDoc = subDocuments.begin(); iterSubDoc != subDocuments.end(); ++iterSubDoc)
			if (*iterSubDoc)
				delete (*iterSubDoc);
	}
	catch(FileException)
	{
		WPD_DEBUG_MSG(("WordPerfect: File Exception. Parse terminated prematurely."));
		for (std::vector<WP5SubDocument *>::iterator iterSubDoc = subDocuments.begin(); iterSubDoc != subDocuments.end(); ++iterSubDoc)
			if (*iterSubDoc)
				delete (*iterSubDoc);
		throw FileException();
	}
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
