/* libwpd
 * Copyright (C) 2005 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include "WP3ResourceFork.h"
#include "libwpd_internal.h"
#include <stdio.h>
#include <vector>

#define DEBUG_DATA

WP3ResourceFork::WP3ResourceFork(WPXInputStream *input, WPXEncryption *encryption) :
	m_resourcesMultimap()
{
	input->seek(16, WPX_SEEK_SET);
	uint32_t dataOffset = readU32(input, encryption, true);
	uint32_t mapOffset = readU32(input, encryption, true);
	/* uint32_t dataLength = */ readU32(input, encryption, true);
	/* uint32_t mapLength = */ readU32(input, encryption, true);
	
	input->seek(40+mapOffset, WPX_SEEK_SET);
	
	uint32_t typeOffset = readU16(input, encryption, true);
	uint32_t nameListOffset = readU16(input, encryption, true);

	input->seek(16+typeOffset+mapOffset, WPX_SEEK_SET);
	uint16_t resourceTypesNumber = 1 + readU16(input, encryption, true);

	for (unsigned i=0; i < resourceTypesNumber; i++)
	{
		uint32_t resourceType = readU32(input,encryption, true);
		uint32_t resourcesNumber = 1+readU16(input, encryption, true);
		uint32_t referenceListOffset = 16+typeOffset+mapOffset+readU16(input, encryption, true);
		unsigned position = input->tell();
		input->seek(referenceListOffset, WPX_SEEK_SET);
		for (uint32_t j=0; j < resourcesNumber; j++)
		{
			uint32_t resourceReferenceID = readU16(input, encryption, true);
			uint16_t resourceNameOffset = readU16(input, encryption, true);
			WPXString resourceName;
			if (resourceNameOffset != 0xffff)
			{
				unsigned position2 = input->tell();
				input->seek(16+mapOffset+nameListOffset+resourceNameOffset, WPX_SEEK_SET);
				resourceName = readPascalString(input, encryption);
				input->seek(position2, WPX_SEEK_SET);
			}
			uint8_t resourceAttributes = readU8(input, encryption);
			uint32_t offsetToData = (readU8(input, encryption) << 16);
			offsetToData |= readU16(input, encryption, true);
			offsetToData += 16+dataOffset;
			unsigned position3 = input->tell();
			input->seek(offsetToData, WPX_SEEK_SET);
			uint32_t resourceDataSize = readU32(input, encryption, true);
			WPXBinaryData resourceData(input, encryption, (size_t)resourceDataSize);
			input->seek(position3, WPX_SEEK_SET);
			WP3Resource *resource = new WP3Resource(resourceType, resourceReferenceID, resourceName, resourceAttributes, resourceData);
			m_resourcesMultimap.insert(std::multimap<uint32_t, WP3Resource *>::value_type( resourceType, resource ) );
			WPD_DEBUG_MSG(("WP3Resource: Type 0x%.8x (%s), ID %i, name %s, attributes 0x%.2x\n", resourceType, resource->getResourceTypeString().cstr(),
				resourceReferenceID, resourceName.cstr(), resourceAttributes));
			input->seek(4, WPX_SEEK_CUR);
		}
		input->seek(position, WPX_SEEK_SET);
	}
}

WP3ResourceFork::~WP3ResourceFork()
{
}
