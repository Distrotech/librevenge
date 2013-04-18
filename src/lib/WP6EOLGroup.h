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

#ifndef WP6EOLGROUP_H
#define WP6EOLGROUP_H

#include "WP6VariableLengthGroup.h"

class WP6EOLGroup : public WP6VariableLengthGroup
{
public:
	WP6EOLGroup(WPXInputStream *input, WPXEncryption *encryption);
	~WP6EOLGroup();
	void _readContents(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener);

private:
	WP6EOLGroup(const WP6EOLGroup &);
	WP6EOLGroup &operator=(const WP6EOLGroup &);
	uint8_t m_colSpan, m_rowSpan;
	bool m_boundFromAbove;

	bool m_useCellAttributes, m_useCellJustification;
	bool m_ignoreInCalculations, m_cellIsLocked;
	uint32_t m_cellAttributes;
	uint8_t m_cellJustification;
	WPXVerticalAlignment m_cellVerticalAlign;

	// cell forground and background color
	RGBSColor *m_cellFgColor;
	RGBSColor *m_cellBgColor;
	RGBSColor *m_cellBorderColor;

	uint8_t m_cellBorders;
	bool m_isHeaderRow;
	bool m_isMinimumHeight;
	uint16_t m_rowHeight;

	bool m_isDontEndAParagraphStyleForThisHardReturn;
};

#endif /* WP6EOLGROUP_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
