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

#include "WP3FixedLengthGroup.h"
#include "WP3FileStructure.h"
#include "WP3AttributeGroup.h"
#include "WP3UnsupportedFixedLengthGroup.h"
#include "WP3ExtendedCharacterGroup.h"
#include "WP3DoubleByteScriptCharacterGroup.h"
#include "WP3TabGroup.h"
#include "WP3IndentGroup.h"
#include "WP3UndoGroup.h"
#include "libwpd_internal.h"

WP3FixedLengthGroup::WP3FixedLengthGroup(const uint8_t groupID)
	: m_group(groupID)
{
}

WP3FixedLengthGroup *WP3FixedLengthGroup::constructFixedLengthGroup(WPXInputStream *input, WPXEncryption *encryption, const uint8_t groupID)
{
	switch (groupID)
	{
	case WP3_EXTENDED_CHARACTER_GROUP:
		return new WP3ExtendedCharacterGroup(input, encryption, groupID);

	case WP3_TAB_GROUP:
		return new WP3TabGroup(input, encryption, groupID);

	case WP3_INDENT_GROUP:
		return new WP3IndentGroup(input, encryption, groupID);

	case WP3_UNDO_GROUP:
		return new WP3UndoGroup(input, encryption, groupID);

	case WP3_ATTRIBUTE_GROUP:
		return new WP3AttributeGroup(input, encryption, groupID);

	case WP3_DOUBLE_BYTE_SCRIPT_CHARACTER_GROUP:
		return new WP3DoubleByteScriptCharacterGroup(input, encryption, groupID);

		// Add the remaining cases here
	default:
		return new WP3UnsupportedFixedLengthGroup(input, encryption, groupID);
	}
}

bool WP3FixedLengthGroup::isGroupConsistent(WPXInputStream *input, WPXEncryption *encryption, const uint8_t groupID)
{
	long startPosition = input->tell();

	try
	{
		int size = WP3_FIXED_LENGTH_FUNCTION_GROUP_SIZE[groupID-0xC0];
		if (input->seek((startPosition + size - 2), WPX_SEEK_SET) || input->atEOS())
		{
			input->seek(startPosition, WPX_SEEK_SET);
			return false;
		}
		if (groupID != readU8(input, encryption))
		{
			input->seek(startPosition, WPX_SEEK_SET);
			return false;
		}

		input->seek(startPosition, WPX_SEEK_SET);
		return true;
	}
	catch(...)
	{
		input->seek(startPosition, WPX_SEEK_SET);
		return false;
	}
}

void WP3FixedLengthGroup::_read(WPXInputStream *input, WPXEncryption *encryption)
{
	long startPosition = input->tell();
	_readContents(input, encryption);

	if (m_group >= 0xC0 && m_group <= 0xCF) // just an extra safety check
	{
		int size = WP3_FIXED_LENGTH_FUNCTION_GROUP_SIZE[m_group-0xC0];
		input->seek((startPosition + size - 2), WPX_SEEK_SET);
		if (m_group != readU8(input, encryption))
		{
			WPD_DEBUG_MSG(("WordPerfect: Possible corruption detected. Bailing out!\n"));
			throw FileException();
		}
	}
	else
		throw FileException();
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
