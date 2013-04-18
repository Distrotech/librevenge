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

#include "WP5BoxGroup.h"
#include "WPXListener.h"
#include "libwpd_internal.h"
#include "WP5FileStructure.h"
#include "WP5PrefixData.h"
#include "WP5Listener.h"
#include "WP5GraphicsInformationPacket.h"

WP5BoxGroup::WP5BoxGroup(WPXInputStream *input, WPXEncryption *encryption) :
	WP5VariableLengthGroup(),
	m_boxNumber(0),
	m_positionAndType(0),
	m_alignment(0),
	m_width(0),
	m_height(0),
	m_x(0),
	m_y(0),
	m_boxType(0),
	m_graphicsOffset(0),
	m_data(NULL)
{
	_read(input, encryption);
}

void WP5BoxGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	switch(getSubGroup())
	{
	case WP5_TOP_BOX_GROUP_FIGURE:
		m_boxNumber = readU16(input, encryption);
		m_positionAndType = readU8(input, encryption);
		m_alignment = readU8(input, encryption);
		m_width = readU16(input, encryption);
		m_height = readU16(input, encryption);
		m_x = readU16(input, encryption);
		m_y = readU16(input, encryption);
		input->seek(36, WPX_SEEK_CUR);
		m_boxType = readU8(input, encryption);
		if (m_boxType == 0x80)
		{
			input->seek(60, WPX_SEEK_CUR);
			m_graphicsOffset = readU16(input, encryption);
		}
		break;
	case WP5_TOP_BOX_GROUP_TABLE:
	case WP5_TOP_BOX_GROUP_TEXT_BOX:
	case WP5_TOP_BOX_GROUP_USER_DEFINED_BOX:
	case WP5_TOP_BOX_GROUP_EQUATION:
		break;
	case WP5_TOP_BOX_GROUP_HORIZONTAL_LINE:
		break;
	case WP5_TOP_BOX_GROUP_VERTICAL_LINE:
		break;
	default:
		break;
	}
}

void WP5BoxGroup::parse(WP5Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a Box group\n"));

	switch(getSubGroup())
	{
	case WP5_TOP_BOX_GROUP_FIGURE:
		if (m_boxType != 0x80)
			break;
		if (listener->getGeneralPacketData(8))
			m_data = static_cast<const WP5GraphicsInformationPacket *>(listener->getGeneralPacketData(8))->getImage(m_graphicsOffset);
		if (m_data)
		{
			listener->boxOn(m_positionAndType, m_alignment, m_width, m_height, m_x, m_y);
			listener->insertGraphicsData(m_data);
			listener->boxOff();
		}
		break;

	case WP5_TOP_BOX_GROUP_TABLE:
	case WP5_TOP_BOX_GROUP_TEXT_BOX:
	case WP5_TOP_BOX_GROUP_USER_DEFINED_BOX:
	case WP5_TOP_BOX_GROUP_EQUATION:
		break;
	case WP5_TOP_BOX_GROUP_HORIZONTAL_LINE:
		break;
	case WP5_TOP_BOX_GROUP_VERTICAL_LINE:
		break;
	default:
		break;
	}
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
