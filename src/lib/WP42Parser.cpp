/* libwpd
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#include "WP42Parser.h"
#include "WP42Part.h"
#include "WPXHeader.h"
#include "libwpd_internal.h"
#include "WPXTable.h"
#include "WP42FileStructure.h"
#include "WP42StylesListener.h"

WP42Parser::WP42Parser(WPXInputStream *input) :
	WPXParser(input, NULL)
{
}

WP42Parser::~WP42Parser()
{
}

void WP42Parser::parse(WPXInputStream *input, WP42Listener *listener)
{
	listener->startDocument();

	input->seek(0, WPX_SEEK_SET);

	WPD_DEBUG_MSG(("WordPerfect: Starting document body parse (position = %ld)\n",(long)input->tell()));

	parseDocument(input, listener);

	listener->endDocument();
}

// parseDocument: parses a document body (may call itself recursively, on other streams, or itself)
void WP42Parser::parseDocument(WPXInputStream *input, WP42Listener *listener)
{
	while (!input->atEOS())
	{
		uint8_t readVal;
		readVal = readU8(input);

		if (readVal < (uint8_t)0x20)
		{
			switch (readVal)
			{
				case 0x09: // tab
					listener->insertTab(0, 0.0f);
					break;
				case 0x0A: // hard new line
					listener->insertEOL();
					break;
				case 0x0B: // soft new page
					listener->insertBreak(WPX_PAGE_BREAK);
					break;
				case 0x0C: // hard new page
					listener->insertBreak(WPX_PAGE_BREAK);
					break;
				case 0x0D: // soft new line
					listener->insertEOL();
					break;
				default:
					// unsupported or undocumented token, ignore
					break;
			}
		}
		else if (readVal >= (uint8_t)0x20 && readVal <= (uint8_t)0x7F)
		{
			// normal ASCII characters
			listener->insertCharacter( readVal );
		}
		else if (readVal >= (uint8_t)0x80 && readVal <= (uint8_t)0xBF)
		{
			// single character function codes
			switch (readVal)
			{
				case 0x92:
					listener->attributeChange(true, WP42_ATTRIBUTE_STRIKE_OUT);
					break;
				case 0x93:
					listener->attributeChange(false, WP42_ATTRIBUTE_STRIKE_OUT);
					break;
				case 0x94:
					listener->attributeChange(true, WP42_ATTRIBUTE_UNDERLINE);
					break;
				case 0x95:
					listener->attributeChange(false, WP42_ATTRIBUTE_UNDERLINE);
					break;

				case 0x90:
					listener->attributeChange(true, WP42_ATTRIBUTE_REDLINE);
					break;
				case 0x91:
					listener->attributeChange(false, WP42_ATTRIBUTE_REDLINE);
					break;

				case 0x9C:
					listener->attributeChange(false, WP42_ATTRIBUTE_BOLD);
					break;
				case 0x9D:
					listener->attributeChange(true, WP42_ATTRIBUTE_BOLD);
					break;

				case 0xB2:
					listener->attributeChange(true, WP42_ATTRIBUTE_ITALICS);
					break;
				case 0xB3:
					listener->attributeChange(false, WP42_ATTRIBUTE_ITALICS);
					break;
				case 0xB4:
					listener->attributeChange(true, WP42_ATTRIBUTE_SHADOW);
					break;
				case 0xB5:
					listener->attributeChange(false, WP42_ATTRIBUTE_SHADOW);
					break;

				default:
					// unsupported or undocumented token, ignore
					break;
			}
		}
		else
		{
			WP42Part *part = WP42Part::constructPart(input, readVal);
			if (part != NULL)
			{
				//part->parse(llListener);
				delete(part);
			}
		}
	}
}

void WP42Parser::parse(WPXHLListenerImpl *listenerImpl)
{
	WPXInputStream *input = getInput();
	std::vector<WPXPageSpan *> pageList;
	WPXTableList tableList;

	try
 	{
		// do a "first-pass" parse of the document
		// gather table border information, page properties (per-page)
		WP42StylesListener stylesListener(&pageList, tableList);
		parse(input, &stylesListener);

		// second pass: here is where we actually send the messages to the target app
		// that are necessary to emit the body of the target document
		WP42Listener listener(&pageList, listenerImpl); // FIXME: SHOULD BE CONTENT_LISTENER, AND SHOULD BE PASSED TABLE DATA!
		parse(input, &listener);

		// cleanup section: free the used resources
		for (std::vector<WPXPageSpan *>::iterator iterSpan = pageList.begin(); iterSpan != pageList.end(); iterSpan++)
		{
			delete *iterSpan;
		}
	}
	catch(FileException)
	{
		WPD_DEBUG_MSG(("WordPerfect: File Exception. Parse terminated prematurely."));

		for (std::vector<WPXPageSpan *>::iterator iterSpan = pageList.begin(); iterSpan != pageList.end(); iterSpan++)
		{
			delete *iterSpan;
		}

		throw FileException();
	}

}
