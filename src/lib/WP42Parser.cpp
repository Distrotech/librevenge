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
#include "WP42HLListener.h"
#include "WP42Parser.h"
#include "WP42Part.h"
#include "WPXHeader.h"
#include "libwpd_internal.h"
#include "WPXTable.h"
#include "WP42FileStructure.h"

WP42Parser::WP42Parser(GsfInput *input) :
	WPXParser(input, NULL)
{
}

WP42Parser::~WP42Parser()
{
}

void WP42Parser::parse(WPXHLListenerImpl *listenerImpl)
{
	WP42HLListener hlListener(listenerImpl);
	hlListener.startDocument();

	GsfInput *input = getInput();
	
	gsf_input_seek(input, 0, G_SEEK_SET);
	
	while (!gsf_input_eof(input))
	{
		guint8 readVal;
		readVal = gsf_le_read_guint8(input);
		
		if (readVal < (guint8)0x20)
		{
			switch (readVal)
			{
				case 0x09: // tab
					hlListener.insertTab(0);
					break;
				case 0x0A: // hard new line
					hlListener.insertEOL();
					break;
				case 0x0B: // soft new page
					break;
				case 0x0C: // hard new page
					break;
				case 0x0D: // soft new line
					hlListener.insertEOL();
					break;
				default:
					// unsupported or undocumented token, ignore
					break;
			}
		}
		else if (readVal >= (guint8)0x20 && readVal <= (guint8)0x7F)
		{
			// normal ASCII characters
			hlListener.insertCharacter( readVal );
		}
		else if (readVal >= (guint8)0x80 && readVal <= (guint8)0xBF)
		{
			// single character function codes
			switch (readVal)
			{				
				case 0x92:
					hlListener.attributeChange(true, WP42_ATTRIBUTE_STRIKE_OUT);
					break;
				case 0x93:
					hlListener.attributeChange(false, WP42_ATTRIBUTE_STRIKE_OUT);
					break;				
				case 0x94:
					hlListener.attributeChange(true, WP42_ATTRIBUTE_UNDERLINE);
					break;
				case 0x95:
					hlListener.attributeChange(false, WP42_ATTRIBUTE_UNDERLINE);
					break;	

				case 0x90:
					hlListener.attributeChange(true, WP42_ATTRIBUTE_REDLINE);
					break;
				case 0x91:
					hlListener.attributeChange(false, WP42_ATTRIBUTE_REDLINE);
					break;

				case 0x9C:
					hlListener.attributeChange(false, WP42_ATTRIBUTE_BOLD);
					break;
				case 0x9D:
					hlListener.attributeChange(true, WP42_ATTRIBUTE_BOLD);
					break;
				
				case 0xB2:
					hlListener.attributeChange(true, WP42_ATTRIBUTE_ITALICS);
					break;
				case 0xB3:
					hlListener.attributeChange(false, WP42_ATTRIBUTE_ITALICS);
					break;
				case 0xB4:
					hlListener.attributeChange(true, WP42_ATTRIBUTE_SHADOW);
					break;
				case 0xB5:
					hlListener.attributeChange(false, WP42_ATTRIBUTE_SHADOW);
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
	
	hlListener.endDocument();
}
