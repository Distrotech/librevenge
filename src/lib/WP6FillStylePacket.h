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

#ifndef WP6FILLSTYLEPACKET_H
#define WP6FILLSTYLEPACKET_H
#include "WP6PrefixDataPacket.h"
#include "WP6FileStructure.h"
#include "libwpd_internal.h"

struct _RGBSColor;
typedef struct _RGBSColor RGBSColor;

class WP6FillStylePacket : public WP6PrefixDataPacket
{
public:
	WP6FillStylePacket(WPXInputStream *input, WPXEncryption *encryption, int id, uint32_t dataOffset, uint32_t dataSize);
	~WP6FillStylePacket();
	void _readContents(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener * /* listener */) const {}
	const RGBSColor *getFgColor() const
	{
		return &m_fgColor;
	}
	const RGBSColor *getBgColor() const
	{
		return &m_bgColor;
	}
private:
	RGBSColor m_fgColor;
	RGBSColor m_bgColor;
};
#endif /* WP6FILLSTYLEPACKET_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
