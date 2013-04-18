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
 * Copyright (C) 2007 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#ifndef WP5PAGEFORMATGROUP_H
#define WP5PAGEFORMATGROUP_H

#include "WP5VariableLengthGroup.h"
#include <vector>

class WP5PageFormatGroup : public WP5VariableLengthGroup
{
public:
	WP5PageFormatGroup(WPXInputStream *input, WPXEncryption *encryption);
	~WP5PageFormatGroup();
	void _readContents(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP5Listener *listener);

private:
	// variables needed for subgroup 1 (Left/Right Margin Set)
	uint16_t m_leftMargin;
	uint16_t m_rightMargin;
	// variable needed for subgroup 2 (Spacing Set)
	double m_lineSpacing;
	// variables needed for subbroup 4 (Tab Set)
	std::vector<WPXTabStop> m_tabStops;
	uint16_t m_marginOffset;
	// variables needed for subgroup 5 (Top/Bottom Margin Set)
	uint16_t m_topMargin;
	uint16_t m_bottomMargin;
	// variables needed for subgroup 6 (Justification)
	uint8_t m_justification;
	// variable needed for subgroup 7 (Suppress Page Characteristics)
	uint8_t m_suppressCode;
	// variables needed for subgroup 11 (Form)
	uint16_t m_formLength;
	uint16_t m_formWidth;
	WPXFormOrientation m_formOrientation;

};

#endif /* WP5PAGEFORMATGROUP_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
