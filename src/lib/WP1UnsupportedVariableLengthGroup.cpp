/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2003 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2003 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 *
 * For further information visit http://librevenge.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#include "WP1UnsupportedVariableLengthGroup.h"
#include "librevenge_internal.h"

WP1UnsupportedVariableLengthGroup::WP1UnsupportedVariableLengthGroup(RVNGInputStream *input, RVNGEncryption *encryption, uint8_t group) :
	WP1VariableLengthGroup(group)
{
	_read(input, encryption);
}

void WP1UnsupportedVariableLengthGroup::_readContents(RVNGInputStream * /* input */, RVNGEncryption * /* encryption */)
{
	RVNG_DEBUG_MSG(("WordPerfect: Handling an unsupported variable length group\n"));
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
