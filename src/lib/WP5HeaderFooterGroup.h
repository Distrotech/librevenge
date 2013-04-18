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

#ifndef WP5HEADERFOOTERGROUP_H
#define WP5HEADERFOOTERGROUP_H

#include "WP5VariableLengthGroup.h"
#include "WP5SubDocument.h"

class WP5HeaderFooterGroup : public WP5VariableLengthGroup
{
public:
	WP5HeaderFooterGroup(WPXInputStream *input, WPXEncryption *encryption);
	~WP5HeaderFooterGroup();
	void _readContents(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP5Listener *listener);

private:
	WP5HeaderFooterGroup(const WP5HeaderFooterGroup &);
	WP5HeaderFooterGroup &operator=(const WP5HeaderFooterGroup &);
	uint8_t m_occurenceBits;
	WP5SubDocument *m_subDocument;
};

#endif /* WP5HEADERFOOTERGROUP_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
