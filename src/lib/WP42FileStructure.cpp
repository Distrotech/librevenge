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
 * Copyright (C) 2003-2004 Marc Maurer (uwog@uwog.net)
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

#include "WP42FileStructure.h"

// size of the function groups 0xC0 to 0xFE
// -1 means the size is variable
int WP42_FUNCTION_GROUP_SIZE[63] =
{
	6,	// 0xC0
	4,
	3,
	5,
	5,
	6,
	4,
	6,
	8,
	42,
	3,
	6,
	4,
	3,
	4,
	3,
	6,	// 0xD0
	-1,
	-1,
	4,
	4,
	4,
	6,
	-1,
	4,
	4,
	4,
	4,
	-1,
	24,
	4,
	-1,
	4,	// 0XE0
	3,
	-1,
	150,
	6,
	23,
	11,
	3,
	3,
	-1,
	-1,
	-1,	// 0XEB Documentation lies that the size is 32, but it is not true.
	4,
	-1,
	44,
	18,
	6,	// 0XF0
	106,
	-1,
	100,
	4,
	-1,
	5,
	-1,
	-1,
	-1,	// 0xF9
	-1,	// 0xFA
	-1,	// 0xFB
	-1,	// 0xFC
	-1,	// 0xFD
	-1	// 0xFE
};
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
