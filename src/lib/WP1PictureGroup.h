/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
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

#ifndef WP1PICTUREGROUP_H
#define WP1PICTUREGROUP_H

#include "WP1VariableLengthGroup.h"
#include "libwpd_internal.h"

class WP1SubDocument;

class WP1PictureGroup : public WP1VariableLengthGroup
{
public:
	WP1PictureGroup(WPXInputStream *input, WPXEncryption *encryption, uint8_t group);
	~WP1PictureGroup();
	void _readContents(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP1Listener *listener);

private:
	WP1PictureGroup(const WP1PictureGroup &);
	WP1PictureGroup &operator=(const WP1PictureGroup &);
	WPXBinaryData m_binaryData;
	uint16_t m_width, m_height;
};

#endif /* WP1PICTUREGROUP_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
