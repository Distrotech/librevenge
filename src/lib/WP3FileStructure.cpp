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

#include "WP3FileStructure.h"

// size of the fixed length functiongroups 0xC0 to 0xCF
int WP3_FIXED_LENGTH_FUNCTION_GROUP_SIZE[16] =
{
	5,	// 0xC0
	8,
	7,
	4,
	4,
	7,
	10,
	7,
	4,
	5,
	6,
	6,
	7,
	9,
	7,
	4	// 0xCF
};
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
