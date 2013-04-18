/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
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

#include "WP6ColumnGroup.h"
#include "WP6Listener.h"
#include "libwpd_internal.h"
#include "WP6FileStructure.h"
#include "WPXFileStructure.h"

WP6ColumnGroup::WP6ColumnGroup(WPXInputStream *input, WPXEncryption *encryption) :
	WP6VariableLengthGroup(),
	m_margin(0),
	m_colType(0),
	m_numColumns(0),
	m_rowSpacing(0.0),
	m_isFixedWidth(),
	m_columnWidth()
{
	_read(input, encryption);
}

void WP6ColumnGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	// this group can contain different kinds of data, thus we need to read
	// the contents accordingly
	switch (getSubGroup())
	{
	case 0: // Left Margin Set
	case 1: // Right Margin Set
	{
		m_margin = readU16(input, encryption);
		WPD_DEBUG_MSG(("WordPerfect: Read column group margin size (margin: %i)\n", m_margin));
	}
	break;
	case 2:
	{
		m_colType = readU8(input, encryption);
		uint32_t tmpRowSpacing = readU32(input, encryption);
		int16_t tmpRowSpacingIntegerPart = (int16_t)((tmpRowSpacing & 0xffff0000) >> 16);
		double tmpRowSpacingFractionalPart = (double)((double)(tmpRowSpacing & 0xffff)/(double)0x10000);
		m_rowSpacing = (double)tmpRowSpacingIntegerPart + tmpRowSpacingFractionalPart;
		m_numColumns = readU8(input, encryption);
		if (m_numColumns > 1)
		{
			uint16_t tmpWidth;
			uint8_t tmpDefinition;
			for (int i=0; i<((2*m_numColumns)-1); i++)
			{
				tmpDefinition = readU8(input, encryption);
				tmpWidth = readU16(input, encryption);
				if ((tmpDefinition & 0x01) == 0x01)
				{
					m_isFixedWidth.push_back(true);
					m_columnWidth.push_back((double)((double)tmpWidth/(double)WPX_NUM_WPUS_PER_INCH));
				}
				else
				{
					m_isFixedWidth.push_back(false);
					m_columnWidth.push_back((double)((double)tmpWidth/(double)0x10000));
				}

			}
		}
		WPD_DEBUG_MSG(("WordPerfect: Column type: %d\n", m_colType & 0x03));
		WPD_DEBUG_MSG(("WordPerfect: Numer of columns: %d\n", m_numColumns));
	}
	break;
	case 3: /* TODO: Column Border */
	{
	}
	break;
	default: /* something else we don't support, since it isn't in the docs */
		break;
	}
}

void WP6ColumnGroup::parse(WP6Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a Column group\n"));

	if (getFlags() & 0x40)  // Ignore function flag
		return;

	switch (getSubGroup())
	{
	case 0: // Left Margin Set
	{
		listener->marginChange(WPX_LEFT, m_margin);
	}
	break;
	case 1: // Right Margin Set
	{
		listener->marginChange(WPX_RIGHT, m_margin);
	}
	break;
	case 2: // Define Text Columns
	{
		// number of columns = {0,1} means columns off
		if ((m_numColumns == 0) || (m_numColumns == 1))
		{
			listener->columnChange(NEWSPAPER, 1, m_columnWidth, m_isFixedWidth); // the value "1" is bugus, the false bool gives you all the information you need here
		}
		else
		{
			switch (m_colType & 0x03)
			{
			case WP6_COLUMN_TYPE_NEWSPAPER:
				listener->columnChange(NEWSPAPER, m_numColumns, m_columnWidth, m_isFixedWidth);
				break;
			case WP6_COLUMN_TYPE_NEWSPAPER_VERTICAL_BALANCE:
				listener->columnChange(NEWSPAPER_VERTICAL_BALANCE, m_numColumns, m_columnWidth, m_isFixedWidth);
				break;
			case WP6_COLUMN_TYPE_PARALLEL:
				listener->columnChange(PARALLEL, m_numColumns, m_columnWidth, m_isFixedWidth);
				break;
			case WP6_COLUMN_TYPE_PARALLEL_PROTECT:
				listener->columnChange(PARALLEL_PROTECT, m_numColumns, m_columnWidth, m_isFixedWidth);
				break;
			default: // something else we don't support, since it isn't in the docs
				break;
			}
		}
	}
	break;
	case 3: // TODO: Column Border
	{
	}
	break;
	default: // something else we don't support, since it isn't in the docs
		break;
	}
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
