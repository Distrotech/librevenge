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
 * Copyright (C) 2004 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#ifndef WP3PAGEFORMATGROUP_H
#define WP3PAGEFORMATGROUP_H

#include "WP3VariableLengthGroup.h"
#include "libwpd_internal.h"
#include <vector>

class WP3PageFormatGroup : public WP3VariableLengthGroup
{
public:
	WP3PageFormatGroup(WPXInputStream *input, WPXEncryption *encryption);
	~WP3PageFormatGroup();
	void _readContents(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP3Listener *listener);

private:
	// variables needed for subgroup 0x01 (Horizontal Margins)
	uint32_t m_leftMargin;
	uint32_t m_rightMargin;
	// variable needed for subgroup 0x02 (Line Spacing)
	double m_lineSpacing;
	// variables needed for subgroup 0x04 (Set Tabs)
	bool m_isRelative;
	std::vector<WPXTabStop> m_tabStops;
	// variables needed for subgroup 0x05 (Vertical Margins)
	uint32_t m_topMargin;
	uint32_t m_bottomMargin;
	// variable needed for subgroup 0x06 (Justification Mode)
	uint8_t m_justification;
	// variable needed for subgroup 0x07 (Suppress Page)
	uint16_t m_suppressCode;
	// variable needed for subgroup 0x0C (Indent At Beginning of Line)
	uint32_t m_indent;

};

#endif /* WP3PAGEFORMATGROUP_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
