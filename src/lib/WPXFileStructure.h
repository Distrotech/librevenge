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

#ifndef WPXFILESTRUCTURE_H
#define WPXFILESTRUCTURE_H

#define WPX_NUM_WPUS_PER_INCH 1200

// header defines
#define WPX_HEADER_MAGIC_OFFSET 1
#define WPX_HEADER_DOCUMENT_POINTER_OFFSET 4
#define WPX_HEADER_PRODUCT_TYPE_OFFSET 8
#define WPX_HEADER_FILE_TYPE_OFFSET 9
#define WPX_HEADER_MAJOR_VERSION_OFFSET 10
#define WPX_HEADER_MINOR_VERSION_OFFSET 11
#define WPX_HEADER_ENCRYPTION_OFFSET 12

#define WPX_NUM_HEADER_FOOTER_TYPES 6
#define WPX_HEADER_A 0x00
#define WPX_HEADER_B 0x01
#define WPX_FOOTER_A 0x02
#define WPX_FOOTER_B 0x03
#define WPX_WATERMARK_A 0x04
#define WPX_WATERMARK_B 0x05

#define WPX_CHARACTER 0x00
#define WPX_PARAGRAPH 0x01
#define WPX_PAGE 0x01

#endif /* WPXFILESTRUCTURE_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
