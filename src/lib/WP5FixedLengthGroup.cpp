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
 * Copyright (C) 2004-2006 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include "WP5FixedLengthGroup.h"
#include "WP5FileStructure.h"
#include "WP5AttributeGroup.h"
#include "WP5ExtendedCharacterGroup.h"
#include "WP5TabGroup.h"
#include "WP5IndentGroup.h"
#include "WP5UnsupportedFixedLengthGroup.h"
#include "libwpd_internal.h"

WP5FixedLengthGroup::WP5FixedLengthGroup(const uint8_t groupID):
	m_group(groupID)
{
}

WP5FixedLengthGroup *WP5FixedLengthGroup::constructFixedLengthGroup(WPXInputStream *input, WPXEncryption *encryption, const uint8_t groupID)
{
	switch (groupID)
	{
	case WP5_TOP_EXTENDED_CHARACTER:
		return new WP5ExtendedCharacterGroup(input, encryption, groupID);

	case WP5_TOP_TAB_GROUP:
		return new WP5TabGroup(input, encryption, groupID);

	case WP5_TOP_INDENT_GROUP:
		return new WP5IndentGroup(input, encryption, groupID);

	case WP5_TOP_ATTRIBUTE_ON:
		return new WP5AttributeOnGroup(input, encryption, groupID);

	case WP5_TOP_ATTRIBUTE_OFF:
		return new WP5AttributeOffGroup(input, encryption, groupID);

		// Add the remaining cases here
	default:
		return new WP5UnsupportedFixedLengthGroup(input, encryption, groupID);
	}
}

bool WP5FixedLengthGroup::isGroupConsistent(WPXInputStream *input, WPXEncryption *encryption, const uint8_t groupID)
{
	long startPosition = input->tell();

	try
	{
		int size = WP5_FIXED_LENGTH_FUNCTION_GROUP_SIZE[groupID-0xC0];
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

void WP5FixedLengthGroup::_read(WPXInputStream *input, WPXEncryption *encryption)
{
	long startPosition = input->tell();
	_readContents(input, encryption);

	if (m_group >= 0xC0 && m_group <= 0xCF) // just an extra safety check
	{
		int size = WP5_FIXED_LENGTH_FUNCTION_GROUP_SIZE[m_group-0xC0];
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
