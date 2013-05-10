/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2005 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include "WP3WindowGroup.h"
#include "WP3FileStructure.h"
#include "WPXFileStructure.h"
#include "libwpd_internal.h"
#include "WP3Listener.h"
#include "WP3ResourceFork.h"

WP3WindowGroup::WP3WindowGroup(WPXInputStream *input, WPXEncryption *encryption) :
	WP3VariableLengthGroup(),
	m_figureFlags(0),
	m_leftColumn(0),
	m_rightColumn(0),
	m_boxType(0xFF),
	m_width(0.0),
	m_height(0.0),
	m_horizontalOffset(0.0),
	m_verticalOffset(0.0),
	m_resourceID(0),
	m_subDocument(NULL),
	m_caption(NULL)
{
	_read(input, encryption);
}

WP3WindowGroup::~WP3WindowGroup()
{
	if (m_subDocument)
		delete m_subDocument;
	if (m_caption)
		delete m_caption;
}

void WP3WindowGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	switch (getSubGroup())
	{
	case WP3_WINDOW_GROUP_FIGURE_BOX_FUNCTION:
	case WP3_WINDOW_GROUP_TABLE_BOX_FUNCTION:
	case WP3_WINDOW_GROUP_TEXT_BOX_FUNCTION:
	case WP3_WINDOW_GROUP_USER_BOX_FUNCTION:
	case WP3_WINDOW_GROUP_EQUATION_BOX_FUNCTION:
	case WP3_WINDOW_GROUP_HTML_IMAGE_BOX_FUNCTION:
	{
		input->seek(14, WPX_SEEK_CUR);
		m_figureFlags = readU16(input, encryption, true); // picture flags
		input->seek(2, WPX_SEEK_CUR);
		m_leftColumn = readU8(input, encryption); // left align column
		m_rightColumn = readU8(input, encryption); // right align column
		input->seek(28, WPX_SEEK_CUR);
		m_boxType = readU8(input, encryption);
		input->seek(1, WPX_SEEK_CUR);
		m_resourceID = readU16(input, encryption, true);
		m_verticalOffset = fixedPointToDouble(readU32(input, encryption, true));
		m_horizontalOffset = fixedPointToDouble(readU32(input, encryption, true));
		m_width = fixedPointToDouble(readU32(input, encryption, true));
		m_height = fixedPointToDouble(readU32(input, encryption, true));
		input->seek(9, WPX_SEEK_CUR);
		uint8_t tmpNumSubRect = readU8(input, encryption);
		input->seek(tmpNumSubRect * 8, WPX_SEEK_CUR);
		uint16_t tmpBoxCaptionSize = readU16(input, encryption, true);
		if (tmpBoxCaptionSize)
			m_caption = new WP3SubDocument(input, encryption, tmpBoxCaptionSize);
		uint16_t tmpTextBoxLength = readU16(input, encryption, true);
		if (tmpTextBoxLength)
			m_subDocument = new WP3SubDocument(input, encryption, tmpTextBoxLength);
	}
	break;

	case WP3_WINDOW_GROUP_HORIZONTAL_LINE:
		break;

	default: /* something else we don't support, since it isn't in the docs */
		break;
	}
}

void WP3WindowGroup::parse(WP3Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a Window group\n"));

	switch (getSubGroup())
	{
	case WP3_WINDOW_GROUP_FIGURE_BOX_FUNCTION:
	case WP3_WINDOW_GROUP_TABLE_BOX_FUNCTION:
	case WP3_WINDOW_GROUP_TEXT_BOX_FUNCTION:
	case WP3_WINDOW_GROUP_USER_BOX_FUNCTION:
	case WP3_WINDOW_GROUP_EQUATION_BOX_FUNCTION:
	case WP3_WINDOW_GROUP_HTML_IMAGE_BOX_FUNCTION:
		if (m_boxType == 0x02)
		{
			WPXBinaryData tmpWBOXData;
			for (int i=0; i < 512; i++)
				tmpWBOXData.append((unsigned char)0);
			if (listener->getResourceFork() && listener->getResourceFork()->getResource(0x57424f58, m_resourceID)) // WBOX resource
			{
				tmpWBOXData.append(listener->getResourceFork()->getResource(0x57424f58, m_resourceID)->getResourceData());
				listener->insertPicture(m_height, m_width, m_verticalOffset, m_horizontalOffset, m_leftColumn, m_rightColumn, m_figureFlags, tmpWBOXData);
			}
		}
		else if (m_boxType == 0x01 || m_boxType == 0x03)
		{
			WPXBinaryData tmpPICTData;
			for (int i=0; i < 512; i++)
				tmpPICTData.append((unsigned char)0);
			if (listener->getResourceFork() && listener->getResourceFork()->getResource(0x50494354, m_resourceID)) // replacement picture in PICT format
			{
				tmpPICTData.append(listener->getResourceFork()->getResource(0x50494354, m_resourceID)->getResourceData());
				listener->insertPicture(m_height, m_width, m_verticalOffset, m_horizontalOffset, m_leftColumn, m_rightColumn, m_figureFlags, tmpPICTData);
			}
		}
		else if (m_boxType == 0x00)
		{
			if (m_subDocument || m_caption)
				listener->insertTextBox(m_height, m_width, m_verticalOffset, m_horizontalOffset, m_leftColumn, m_rightColumn, m_figureFlags, m_subDocument, m_caption);
		}
		else if (m_boxType == 0x04 || m_boxType == 0x05)
		{
			if (m_subDocument || m_caption)
				listener->insertWP51Table(m_height, m_width, m_verticalOffset, m_horizontalOffset, m_leftColumn, m_rightColumn, m_figureFlags, m_subDocument, m_caption);
		}
		break;

	case WP3_WINDOW_GROUP_HORIZONTAL_LINE:
		break;

	default:
		break;
	}
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
