/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2005 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#ifndef WP3DISPLAYGROUP_H
#define WP3DISPLAYGROUP_H

#include "WP3VariableLengthGroup.h"
#include <libwpd/libwpd.h>

class WP3DisplayGroup : public WP3VariableLengthGroup
{
public:
	WP3DisplayGroup(WPXInputStream *input, WPXEncryption *encryption);
	~WP3DisplayGroup();
	void _readContents(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP3Listener *listener);

private:
	WPXString m_noteReference;
	WPXString m_pageNumber;
};

#endif /* WP3DISPLAYGROUP_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
