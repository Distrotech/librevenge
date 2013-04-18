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

#include "WP6FileStructure.h"

// size of the fixed length functiongroups 0xF0 to 0xFF
uint8_t WP6_FIXED_LENGTH_FUNCTION_GROUP_SIZE[16] =
{
	4,	// 0xF0
	5,	// 0xF1
	3,	// 0xF2
	3,	// 0xF3
	3,	// 0xF4
	3,	// 0xF5
	4,	// 0xF6
	4,	// 0xF7
	4,	// 0xF8
	5,	// 0xF9
	5,	// 0xFA
	6,	// 0xFB
	6,	// 0xFC
	8,	// 0xFD
	8,	// 0xFE
	(uint8_t)-1	// 0xFF - Cannot be used. -1 is reserved so no size is assigned to this value.
};
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
