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

#include "WP6DisplayNumberReferenceGroup.h"
#include "WP6Listener.h"
#include "libwpd_internal.h"

WP6DisplayNumberReferenceGroup::WP6DisplayNumberReferenceGroup(WPXInputStream *input, WPXEncryption *encryption) :
	WP6VariableLengthGroup(),
	m_levelNumberToDisplay(0)
{
	_read(input, encryption);
}

void WP6DisplayNumberReferenceGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	if (!(getSubGroup() % 2) || getSubGroup() == 0)
		m_levelNumberToDisplay = readU8(input, encryption);
}

void WP6DisplayNumberReferenceGroup::parse(WP6Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a DisplayNumberReference group\n"));

	if (!(getSubGroup() % 2) || getSubGroup() == 0)
		listener->displayNumberReferenceGroupOn(getSubGroup(), m_levelNumberToDisplay);
	else
		listener->displayNumberReferenceGroupOff(getSubGroup());
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
