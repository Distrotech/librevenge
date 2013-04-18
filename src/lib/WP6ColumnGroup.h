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

#ifndef WP6COLUMNGROUP_H
#define WP6COLUMNGROUP_H

#include "WP6VariableLengthGroup.h"
#include <vector>
#include "libwpd_internal.h"

class WPXInputStream;
class WP6Listener;

class WP6ColumnGroup : public WP6VariableLengthGroup
{
public:
	WP6ColumnGroup(WPXInputStream *input, WPXEncryption *encryption);
	void _readContents(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener);

private:
	// variables needed for subgroup 0 and 1 (Left/Right Margin Set)
	uint16_t m_margin;

	// variables used for subgroup 2 (Columns)
	uint8_t m_colType;
	uint8_t m_numColumns;
	double m_rowSpacing;
	std::vector<bool> m_isFixedWidth;
	std::vector<double> m_columnWidth;
};

#endif /* WP6COLUMNGROUP_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
