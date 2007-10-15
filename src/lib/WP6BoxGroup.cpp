/* libwpd
 * Copyright (C) 2007 Fridrich Strba (fridrich.strba@bluewin.ch)
 * Copyright (C) 2007 Novell Inc. (http://www.novell.com)
s *  
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
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

#include "WP6BoxGroup.h"
#include "WP6Listener.h"
#include "libwpd_internal.h"
#include "WP6FileStructure.h"
#include "WPXFileStructure.h"
#include "WP6GraphicsFilenamePacket.h"
#include "WP6GeneralTextPacket.h"
#include "WP6GraphicsBoxStylePacket.h"

WP6BoxGroup::WP6BoxGroup(WPXInputStream *input) :
	WP6VariableLengthGroup(),
	m_isBoxContentType(false),
	m_boxContentType(0x00)
{
	_read(input);
}

void WP6BoxGroup::_readContents(WPXInputStream *input)
{
	switch (getSubGroup())	
	{
		case WP6_BOX_GROUP_CHARACTER_ANCHORED_BOX:
		case WP6_BOX_GROUP_PARAGRAPH_ANCHORED_BOX:
		case WP6_BOX_GROUP_PAGE_ANCHORED_BOX:
		{
			input->seek(14, WPX_SEEK_CUR); // reserved for future use
			input->seek(2, WPX_SEEK_CUR); // total size of override and wrap rectangle data for box
			input->seek(2, WPX_SEEK_CUR); // total size of override data
			uint16_t tmpOverrideFlags = readU16(input);
			if (tmpOverrideFlags & WP6_BOX_GROUP_BOX_COUNTER_DATA_BIT)
			{
				long tmpEndOfData = readU16(input) + input->tell();
#ifdef DEBUG
				uint16_t tmpOverrideFlags = readU16(input);
#else
				readU16(input);
#endif
				WPD_DEBUG_MSG(("WP6BoxGroup: parsing Box counter data -- override flags: 0x%x\n", tmpOverrideFlags));
				input->seek(tmpEndOfData, WPX_SEEK_SET); 
			}
			if (tmpOverrideFlags & WP6_BOX_GROUP_BOX_POSITIONING_DATA_BIT)
			{
				long tmpEndOfData = readU16(input) + input->tell();

				uint16_t tmpOverrideFlags = readU16(input);

				if (tmpOverrideFlags & 0x8000)
					input->seek(2, WPX_SEEK_CUR);
				if (tmpOverrideFlags & 0x4000)
				{
					WPD_DEBUG_MSG(("Box general positioning flags (mask: 0x%.2x) (data: 0x%.2x)\n", readU8(input), readU8(input)));
				}
				if (tmpOverrideFlags & 0x2000)
				{
					WPD_DEBUG_MSG(("Box horizontal positioning flags: 0x%.2x\n", readU8(input)));
					WPD_DEBUG_MSG(("Box horizontal offset: %i\n", (int16_t)readU16(input)));
					WPD_DEBUG_MSG(("Box left column: %i, right column: %i\n", readU8(input), readU8(input)));
				}
				if (tmpOverrideFlags & 0x1000)
				{
					WPD_DEBUG_MSG(("Box vertical positioning flags: 0x%.2x\n", readU8(input)));
					WPD_DEBUG_MSG(("Box vertical offset: %i\n", (int16_t)readU16(input)));
				}
				if (tmpOverrideFlags & 0x0800)
				{
					WPD_DEBUG_MSG(("Box width flags: 0x%.2x\n", readU8(input)));
					WPD_DEBUG_MSG(("Box width value: %i\n", readU16(input)));
				}
				if (tmpOverrideFlags & 0x0400)
				{
					WPD_DEBUG_MSG(("Box height flags: 0x%.2x\n", readU8(input)));
					WPD_DEBUG_MSG(("Box height value: %i\n", readU16(input)));
				}
				if (tmpOverrideFlags & 0x0200)
				{
					WPD_DEBUG_MSG(("Box z-order flags: 0x%.2x\n", readU8(input)));
				}
				WPD_DEBUG_MSG(("WP6BoxGroup: parsing Box positioning data -- override flags: 0x%x\n", tmpOverrideFlags));
				input->seek(tmpEndOfData, WPX_SEEK_SET); 
			}
			if (tmpOverrideFlags & WP6_BOX_GROUP_BOX_CONTENT_DATA_BIT)
			{
				long tmpEndOfData = readU16(input) + input->tell();

				uint16_t tmpOverrideFlags = readU16(input);

				if (tmpOverrideFlags & 0x8000)
					input->seek(2, WPX_SEEK_CUR);

				if (tmpOverrideFlags & 0x4000)
				{
					m_isBoxContentType = true;
					m_boxContentType = readU8(input);
				}
				
				WPD_DEBUG_MSG(("WP6BoxGroup: parsing Box content data -- override flags: 0x%x\n", tmpOverrideFlags));

				if (m_isBoxContentType)
					WPD_DEBUG_MSG(("WP6BoxGroup: parsing Box content data -- content type: 0x%.2x\n", m_boxContentType));

				input->seek(tmpEndOfData, WPX_SEEK_SET); 
			}
			if (tmpOverrideFlags & WP6_BOX_GROUP_BOX_CAPTION_DATA_BIT)
			{
				long tmpEndOfData = readU16(input) + input->tell();
#ifdef DEBUG
				uint16_t tmpOverrideFlags = readU16(input);
#else
				readU16(input);
#endif
				WPD_DEBUG_MSG(("WP6BoxGroup: parsing Box caption data -- override flags: 0x%x\n", tmpOverrideFlags));
				input->seek(tmpEndOfData, WPX_SEEK_SET); 
			}
			if (tmpOverrideFlags & WP6_BOX_GROUP_BOX_BORDER_DATA_BIT)
			{
				long tmpEndOfData = readU16(input) + input->tell();
#ifdef DEBUG
				uint16_t tmpOverrideFlags = readU16(input);
#else
				readU16(input);
#endif
				WPD_DEBUG_MSG(("WP6BoxGroup: parsing Box border data -- override flags: 0x%x\n", tmpOverrideFlags));
				input->seek(tmpEndOfData, WPX_SEEK_SET); 
			}
			if (tmpOverrideFlags & WP6_BOX_GROUP_BOX_FILL_DATA_BIT)
			{
				long tmpEndOfData = readU16(input) + input->tell();
#ifdef DEBUG
				uint16_t tmpOverrideFlags = readU16(input);
#else
				readU16(input);
#endif
				WPD_DEBUG_MSG(("WP6BoxGroup: parsing Box fill data -- override flags: 0x%x\n", tmpOverrideFlags));
				input->seek(tmpEndOfData, WPX_SEEK_SET); 
			}
			if (tmpOverrideFlags & WP6_BOX_GROUP_BOX_BOX_WRAPPING_DATA_BIT)
			{
				long tmpEndOfData = readU16(input) + input->tell();
#ifdef DEBUG
				uint16_t tmpOverrideFlags = readU16(input);
#else
				readU16(input);
#endif
				WPD_DEBUG_MSG(("WP6BoxGroup: parsing Box wrapping data -- override flags: 0x%x\n", tmpOverrideFlags));
				input->seek(tmpEndOfData, WPX_SEEK_SET); 
			}
			if (tmpOverrideFlags & WP6_BOX_GROUP_BOX_BOX_HYPERTEXT_WRAPPING_DATA_BIT)
			{
				long tmpEndOfData = readU16(input) + input->tell();
#ifdef DEBUG
				uint16_t tmpOverrideFlags = readU16(input);
#else
				readU16(input);
#endif
				WPD_DEBUG_MSG(("WP6BoxGroup: parsing Box hypertext wrapping data -- override flags: 0x%x\n", tmpOverrideFlags));
				input->seek(tmpEndOfData, WPX_SEEK_SET); 
			}
			if (tmpOverrideFlags & WP6_BOX_GROUP_BOX_HTML_BIT)
			{
				WPD_DEBUG_MSG(("WP6BoxGroup: parsing HTML\n"));
			}
			if (tmpOverrideFlags & WP6_BOX_GROUP_BOX_GROUPING_DATA_BIT)
			{
				long tmpEndOfData = readU16(input) + input->tell();
#ifdef DEBUG
				uint16_t tmpOverrideFlags = readU16(input);
#else
				readU16(input);
#endif
				WPD_DEBUG_MSG(("WP6BoxGroup: parsing Box grouping data -- override flags: 0x%x\n", tmpOverrideFlags));
				input->seek(tmpEndOfData, WPX_SEEK_SET); 
			}
			if (tmpOverrideFlags & WP6_BOX_GROUP_BOX_DRAW_OBJECT_DATA_BIT)
			{
				long tmpEndOfData = readU16(input) + input->tell();
#ifdef DEBUG
				uint16_t tmpOverrideFlags = readU16(input);
#else
				readU16(input);
#endif
				WPD_DEBUG_MSG(("WP6BoxGroup: parsing Box draw object data -- override flags: 0x%x\n", tmpOverrideFlags));
				input->seek(tmpEndOfData, WPX_SEEK_SET); 
			}
		}
			break;
		case WP6_BOX_GROUP_GRAPHICS_RULE:
			break;
		default: /* something else we don't support, since it isn't in the docs */
			break;
	}
}

void WP6BoxGroup::parse(WP6Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a Column group\n"));

	if (getFlags() & 0x40)  // Ignore function flag
		return;
		
	std::vector<uint16_t> graphicsDataIds;
	std::vector<uint16_t>::iterator gdiIter;
	WP6SubDocument *subDocument = 0;
	for (int i=0; i<getNumPrefixIDs(); i++)
	{
		if (const WP6GraphicsFilenamePacket *gfPacket = dynamic_cast<const WP6GraphicsFilenamePacket *>(listener->getPrefixDataPacket(getPrefixIDs()[i]))) 
		{
			graphicsDataIds = gfPacket->getChildIds();
			break;
		}
		if (const WP6GeneralTextPacket *gtPacket = dynamic_cast<const WP6GeneralTextPacket *>(listener->getPrefixDataPacket(getPrefixIDs()[i])))
		{
			subDocument = gtPacket->getSubDocument();
			break;
		}
			
	}
	
	switch (getSubGroup())
	{
		case WP6_BOX_GROUP_CHARACTER_ANCHORED_BOX:
			for (gdiIter = graphicsDataIds.begin(); gdiIter != graphicsDataIds.end(); gdiIter++)
				listener->insertGraphicsData((*gdiIter), WPX_CHARACTER);
			if (subDocument)
				listener->insertTextBox(subDocument, WPX_CHARACTER);
			break;
		case WP6_BOX_GROUP_PARAGRAPH_ANCHORED_BOX:
			for (gdiIter = graphicsDataIds.begin(); gdiIter != graphicsDataIds.end(); gdiIter++)
				listener->insertGraphicsData((*gdiIter), WPX_PARAGRAPH);
			if (subDocument)
				listener->insertTextBox(subDocument, WPX_PARAGRAPH);
			break;
		case WP6_BOX_GROUP_PAGE_ANCHORED_BOX:
			for (gdiIter = graphicsDataIds.begin(); gdiIter != graphicsDataIds.end(); gdiIter++)
				listener->insertGraphicsData((*gdiIter), WPX_PAGE);
			if (subDocument)
				listener->insertTextBox(subDocument, WPX_PAGE);
			break;
		case WP6_BOX_GROUP_GRAPHICS_RULE:
			break;
		default: /* something else we don't support, since it isn't in the docs */
			break;
	}
}
