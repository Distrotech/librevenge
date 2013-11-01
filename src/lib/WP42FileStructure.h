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
 * For further information visit http://librevenge.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#ifndef WP42FILESTRUCTURE_H
#define WP42FILESTRUCTURE_H

// size of the functiongroups 0xC0 to 0xF8
extern int WP42_FUNCTION_GROUP_SIZE[63];

#define WP42_ATTRIBUTE_BOLD 0
#define WP42_ATTRIBUTE_ITALICS 1
#define WP42_ATTRIBUTE_UNDERLINE 2
#define WP42_ATTRIBUTE_STRIKE_OUT 3
#define WP42_ATTRIBUTE_SHADOW 4
#define WP42_ATTRIBUTE_REDLINE 5

#define WP42_MARGIN_RESET_GROUP 0xC0

#define WP42_SUPPRESS_PAGE_CHARACTERISTICS_GROUP 0xCF

#define WP42_HEADER_FOOTER_GROUP 0xD1
#define WP42_HEADER_FOOTER_GROUP_ALL_BIT 1
#define WP42_HEADER_FOOTER_GROUP_ODD_BIT 2
#define WP42_HEADER_FOOTER_GROUP_EVEN_BIT 4

#define WP42_DEFINE_COLUMNS_OLD_GROUP 0xDD

#define WP42_EXTENDED_CHARACTER_GROUP 0xE1

#define WP42_DEFINE_COLUMNS_NEW_GROUP 0xF3

#endif /* WP42FILESTRUCTURE_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
