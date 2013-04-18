/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
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

#ifndef WP6NUMBERINGMETHODGROUP_H
#define WP6NUMBERINGMETHODGROUP_H

#include "WP6VariableLengthGroup.h"

class WP6NumberingMethodGroup : public WP6VariableLengthGroup
{
public:
	WP6NumberingMethodGroup(WPXInputStream *input, WPXEncryption *encryption);
	~WP6NumberingMethodGroup();
	void _readContents(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener);
private:
	uint8_t m_levelNumber;
	uint8_t m_numberingMethod;
};

#endif /* WP6NUMBERINGMETHODGROUP_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
