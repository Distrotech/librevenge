/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2007 Fridrich Strba (fridrich.strba@bluewin.ch)
 * Copyright (C) 2007 Novell Inc. (http://www.novell.com)
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

#include "WP6BoxGroup.h"
#include "WP6Listener.h"
#include "libwpd_internal.h"
#include "WP6FileStructure.h"
#include "WPXFileStructure.h"
#include "WP6GraphicsFilenamePacket.h"
#include "WP6GeneralTextPacket.h"
#include "WP6GraphicsBoxStylePacket.h"

WP6BoxGroup::WP6BoxGroup(WPXInputStream *input, WPXEncryption *encryption) :
	WP6VariableLengthGroup(),
	m_generalPositioningFlagsMask(0x00),
	m_generalPositioningFlagsData(0x00),
	m_hasHorizontalPositioning(false),
	m_horizontalPositioningFlags(0x00),
	m_horizontalOffset(0),
	m_leftColumn(0),
	m_rightColumn(0),
	m_hasVerticalPositioning(false),
	m_verticalPositioningFlags(0x00),
	m_verticalOffset(0),
	m_hasWidthInformation(false),
	m_widthFlags(0x00),
	m_width(0),
	m_hasHeightInformation(false),
	m_heightFlags(0x00),
	m_height(0),
	m_hasZOrderInformation(false),
	m_zOrderFlags(0x00),
	m_hasBoxContentType(false),
	m_boxContentType(0x00),
	m_nativeWidth(0),
	m_nativeHeight(0)
{
	_read(input, encryption);
}

void WP6BoxGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
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
		uint16_t tmpOverrideFlags = readU16(input, encryption);
		if (tmpOverrideFlags & WP6_BOX_GROUP_BOX_COUNTER_DATA_BIT)
		{
			long tmpEndOfData = readU16(input, encryption) + input->tell();
#ifdef DEBUG
			tmpOverrideFlags = readU16(input, encryption);
#else
			readU16(input, encryption);
#endif
			WPD_DEBUG_MSG(("WP6BoxGroup: parsing Box counter data -- override flags: 0x%x\n", tmpOverrideFlags));
			input->seek(tmpEndOfData, WPX_SEEK_SET);
		}
		if (tmpOverrideFlags & WP6_BOX_GROUP_BOX_POSITIONING_DATA_BIT)
		{
			long tmpEndOfData = readU16(input, encryption) + input->tell();

			tmpOverrideFlags = readU16(input, encryption);

			if (tmpOverrideFlags & 0x8000)
				input->seek(2, WPX_SEEK_CUR);
			if (tmpOverrideFlags & 0x4000)
			{
				m_generalPositioningFlagsMask = readU8(input, encryption);
				m_generalPositioningFlagsData = readU8(input, encryption);
				WPD_DEBUG_MSG(("Box general positioning flags (mask: 0x%.2x) (data: 0x%.2x)\n",
				               m_generalPositioningFlagsMask, m_generalPositioningFlagsData));
			}
			if (tmpOverrideFlags & 0x2000)
			{
				m_hasHorizontalPositioning = true;
				m_horizontalPositioningFlags = readU8(input, encryption);
				m_horizontalOffset = (int16_t)readU16(input, encryption);
				m_leftColumn = readU8(input, encryption);
				m_rightColumn = readU8(input, encryption);
				WPD_DEBUG_MSG(("Box horizontal positioning flags: 0x%.2x\n", m_horizontalPositioningFlags));
				WPD_DEBUG_MSG(("Box horizontal offset: %i\n", m_horizontalOffset));
				WPD_DEBUG_MSG(("Box left column: %i, right column: %i\n", m_leftColumn, m_rightColumn));
			}
			if (tmpOverrideFlags & 0x1000)
			{
				m_hasVerticalPositioning = true;
				m_verticalPositioningFlags = readU8(input, encryption);
				m_verticalOffset = (int16_t)readU16(input, encryption);
				WPD_DEBUG_MSG(("Box vertical positioning flags: 0x%.2x\n", m_verticalPositioningFlags));
				WPD_DEBUG_MSG(("Box vertical offset: %i\n", m_verticalOffset));
			}
			if (tmpOverrideFlags & 0x0800)
			{
				m_hasWidthInformation = true;
				m_widthFlags = readU8(input, encryption);
				m_width = readU16(input, encryption);
				WPD_DEBUG_MSG(("Box width flags: 0x%.2x\n", m_widthFlags));
				WPD_DEBUG_MSG(("Box width value: %i\n", m_width));
			}
			if (tmpOverrideFlags & 0x0400)
			{
				m_hasHeightInformation = true;
				m_heightFlags = readU8(input, encryption);
				m_height = readU16(input, encryption);
				WPD_DEBUG_MSG(("Box height flags: 0x%.2x\n", m_heightFlags));
				WPD_DEBUG_MSG(("Box height value: %i\n", m_height));
			}
			if (tmpOverrideFlags & 0x0200)
			{
				m_hasZOrderInformation = true;
				m_zOrderFlags = readU8(input, encryption);
				WPD_DEBUG_MSG(("Box z-order flags: 0x%.2x\n", m_zOrderFlags));
			}
			WPD_DEBUG_MSG(("WP6BoxGroup: parsing Box positioning data -- override flags: 0x%x\n", tmpOverrideFlags));
			input->seek(tmpEndOfData, WPX_SEEK_SET);
		}
		if (tmpOverrideFlags & WP6_BOX_GROUP_BOX_CONTENT_DATA_BIT)
		{
			long tmpEndOfData = readU16(input, encryption) + input->tell();

			tmpOverrideFlags = readU16(input, encryption);

			if (tmpOverrideFlags & 0x8000)
				input->seek(2, WPX_SEEK_CUR);

			if (tmpOverrideFlags & 0x4000)
			{
				m_hasBoxContentType = true;
				m_boxContentType = readU8(input, encryption);
			}

			if (tmpOverrideFlags & 0x2000) // content rendering information
			{
				if (m_hasBoxContentType && (m_boxContentType == 0x03)) // Image
				{
					uint16_t tmpImageContentOverrideSize = readU16(input, encryption);
					long tmpImageContentOverrideStart = input->tell();
					uint16_t tmpImageContentOverrideFlags = readU16(input, encryption);

					if (tmpImageContentOverrideFlags & 0x8000)
						input->seek(2, WPX_SEEK_CUR);
					if (tmpImageContentOverrideFlags & 0x4000)
					{
						m_nativeWidth = readU16(input, encryption);
						m_nativeHeight = readU16(input, encryption);
					}
					input->seek(tmpImageContentOverrideStart + tmpImageContentOverrideSize, WPX_SEEK_SET);
				}
			}
#ifdef DEBUG
			WPD_DEBUG_MSG(("WP6BoxGroup: parsing Box content data -- override flags: 0x%x\n", tmpOverrideFlags));

			if (m_hasBoxContentType)
				WPD_DEBUG_MSG(("WP6BoxGroup: parsing Box content data -- content type: 0x%.2x\n", m_boxContentType));
#endif
			input->seek(tmpEndOfData, WPX_SEEK_SET);
		}
		if (tmpOverrideFlags & WP6_BOX_GROUP_BOX_CAPTION_DATA_BIT)
		{
			long tmpEndOfData = readU16(input, encryption) + input->tell();
#ifdef DEBUG
			tmpOverrideFlags = readU16(input, encryption);
#else
			readU16(input, encryption);
#endif
			WPD_DEBUG_MSG(("WP6BoxGroup: parsing Box caption data -- override flags: 0x%x\n", tmpOverrideFlags));
			input->seek(tmpEndOfData, WPX_SEEK_SET);
		}
		if (tmpOverrideFlags & WP6_BOX_GROUP_BOX_BORDER_DATA_BIT)
		{
			long tmpEndOfData = readU16(input, encryption) + input->tell();
#ifdef DEBUG
			tmpOverrideFlags = readU16(input, encryption);
#else
			readU16(input, encryption);
#endif
			WPD_DEBUG_MSG(("WP6BoxGroup: parsing Box border data -- override flags: 0x%x\n", tmpOverrideFlags));
			input->seek(tmpEndOfData, WPX_SEEK_SET);
		}
		if (tmpOverrideFlags & WP6_BOX_GROUP_BOX_FILL_DATA_BIT)
		{
			long tmpEndOfData = readU16(input, encryption) + input->tell();
#ifdef DEBUG
			tmpOverrideFlags = readU16(input, encryption);
#else
			readU16(input, encryption);
#endif
			WPD_DEBUG_MSG(("WP6BoxGroup: parsing Box fill data -- override flags: 0x%x\n", tmpOverrideFlags));
			input->seek(tmpEndOfData, WPX_SEEK_SET);
		}
		if (tmpOverrideFlags & WP6_BOX_GROUP_BOX_BOX_WRAPPING_DATA_BIT)
		{
			long tmpEndOfData = readU16(input, encryption) + input->tell();
#ifdef DEBUG
			tmpOverrideFlags = readU16(input, encryption);
#else
			readU16(input, encryption);
#endif
			WPD_DEBUG_MSG(("WP6BoxGroup: parsing Box wrapping data -- override flags: 0x%x\n", tmpOverrideFlags));
			input->seek(tmpEndOfData, WPX_SEEK_SET);
		}
		if (tmpOverrideFlags & WP6_BOX_GROUP_BOX_BOX_HYPERTEXT_WRAPPING_DATA_BIT)
		{
			long tmpEndOfData = readU16(input, encryption) + input->tell();
#ifdef DEBUG
			tmpOverrideFlags = readU16(input, encryption);
#else
			readU16(input, encryption);
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
			long tmpEndOfData = readU16(input, encryption) + input->tell();
#ifdef DEBUG
			tmpOverrideFlags = readU16(input, encryption);
#else
			readU16(input, encryption);
#endif
			WPD_DEBUG_MSG(("WP6BoxGroup: parsing Box grouping data -- override flags: 0x%x\n", tmpOverrideFlags));
			input->seek(tmpEndOfData, WPX_SEEK_SET);
		}
		if (tmpOverrideFlags & WP6_BOX_GROUP_BOX_DRAW_OBJECT_DATA_BIT)
		{
			long tmpEndOfData = readU16(input, encryption) + input->tell();
#ifdef DEBUG
			tmpOverrideFlags = readU16(input, encryption);
#else
			readU16(input, encryption);
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

	if ((getSubGroup() != WP6_BOX_GROUP_CHARACTER_ANCHORED_BOX ) && (getSubGroup() != WP6_BOX_GROUP_PARAGRAPH_ANCHORED_BOX) &&
	        (getSubGroup() != WP6_BOX_GROUP_PAGE_ANCHORED_BOX))  // Don't handle Graphics Rule for the while
		return;

	const WP6GraphicsBoxStylePacket *gbsPacket = 0;
	for (int j=0; j<getNumPrefixIDs(); j++)
		if ((gbsPacket = dynamic_cast<const WP6GraphicsBoxStylePacket *>(listener->getPrefixDataPacket(getPrefixIDs()[j]))))
			break;

	uint8_t tmpContentType = 0;
	if (gbsPacket)
		tmpContentType = gbsPacket->getContentType();
	if (m_hasBoxContentType)
		tmpContentType = m_boxContentType;

	if (tmpContentType != 0x01 && tmpContentType != 0x03)
		return;

	if (!m_nativeWidth && gbsPacket)
		m_nativeWidth = gbsPacket->getNativeWidth();
	if (!m_nativeHeight && gbsPacket)
		m_nativeHeight = gbsPacket->getNativeHeight();

	std::vector<unsigned> graphicsDataIds;
	std::vector<unsigned>::iterator gdiIter;
	WP6SubDocument *subDocument = 0;

	// Get the box content
	for (int i=0; i<getNumPrefixIDs(); i++)
	{
		if (tmpContentType == 0x03)
			if (const WP6GraphicsFilenamePacket *gfPacket = dynamic_cast<const WP6GraphicsFilenamePacket *>(listener->getPrefixDataPacket(getPrefixIDs()[i])))
			{
				graphicsDataIds = gfPacket->getChildIds();
				break;
			}
		if (tmpContentType == 0x01)
			if (const WP6GeneralTextPacket *gtPacket = dynamic_cast<const WP6GeneralTextPacket *>(listener->getPrefixDataPacket(getPrefixIDs()[i])))
			{
				subDocument = gtPacket->getSubDocument();
				break;
			}
	}

	// Get the box anchoring
	uint8_t tmpAnchoringType = 0;
	switch (getSubGroup())
	{
	case WP6_BOX_GROUP_CHARACTER_ANCHORED_BOX:
		tmpAnchoringType = WPX_CHARACTER;
		break;
	case WP6_BOX_GROUP_PARAGRAPH_ANCHORED_BOX:
		tmpAnchoringType = WPX_PARAGRAPH;
		break;
	case WP6_BOX_GROUP_PAGE_ANCHORED_BOX:
		tmpAnchoringType = WPX_PAGE;
		break;
	case WP6_BOX_GROUP_GRAPHICS_RULE:
	default: /* something else we don't support, since it isn't in the docs */
		break;
	}

	// Get the box general positioning
	uint8_t tmpGeneralPositioningFlags = 0;
	if (gbsPacket)
		tmpGeneralPositioningFlags = (uint8_t)((gbsPacket->getGeneralPositioningFlags() & (~ m_generalPositioningFlagsMask)) |
		                                       (m_generalPositioningFlagsData & m_generalPositioningFlagsMask));
	else  // here we did not manage to get the packet. Let's try to go with the override information
		tmpGeneralPositioningFlags = (m_generalPositioningFlagsData & m_generalPositioningFlagsMask);

	// Get the box horizontal position
	if (gbsPacket && !m_hasHorizontalPositioning)
	{
		m_horizontalPositioningFlags = gbsPacket->getHorizontalPositioningFlags();
		m_horizontalOffset = gbsPacket->getHorizontalOffset();
		m_leftColumn = gbsPacket->getLeftColumn();
		m_rightColumn = gbsPacket->getRightColumn();
	}

	// Get the box vertical position
	if (gbsPacket && !m_hasVerticalPositioning)
	{
		m_verticalPositioningFlags = gbsPacket->getVerticalPositioningFlags();
		m_verticalOffset = gbsPacket->getVerticalOffset();
	}

	// Get the box width
	if (gbsPacket && !m_hasWidthInformation)
	{
		m_widthFlags = gbsPacket->getWidthFlags();
		m_width = gbsPacket->getWidth();
	}

	// Get the box height
	if (gbsPacket && !m_hasHeightInformation)
	{
		m_heightFlags = gbsPacket->getHeightFlags();
		m_height = gbsPacket->getHeight();
	}

	// Send the box information to the listener and start box
	listener->boxOn(tmpAnchoringType, tmpGeneralPositioningFlags, m_horizontalPositioningFlags, m_horizontalOffset, m_leftColumn, m_rightColumn,
	                m_verticalPositioningFlags, m_verticalOffset, m_widthFlags, m_width, m_heightFlags, m_height, tmpContentType, m_nativeWidth, m_nativeHeight);

	// Send the content according to its kind
	if (tmpContentType == 0x03)
	{
		for (gdiIter = graphicsDataIds.begin(); gdiIter != graphicsDataIds.end(); ++gdiIter)
			listener->insertGraphicsData(((uint16_t)*gdiIter));
	}
	if ((tmpContentType == 0x01) && (subDocument))
	{
		listener->insertTextBox(subDocument);
	}

	// End the box
	listener->boxOff();
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
