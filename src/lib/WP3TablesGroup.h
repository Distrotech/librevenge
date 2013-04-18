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

#ifndef WP3TABLESGROUP_H
#define WP3TABLESGROUP_H

#include <vector>
#include "WP3VariableLengthGroup.h"
#include "libwpd_internal.h"

class WP3TablesGroup : public WP3VariableLengthGroup
{
public:
	WP3TablesGroup(WPXInputStream *input, WPXEncryption *encryption);
	~WP3TablesGroup();
	void _readContents(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP3Listener *listener);

private:
	// variables needed for subgroup 1 (Table Function)
	uint8_t m_tableMode;
	uint32_t m_offsetFromLeftEdge;
	uint32_t m_topGutterSpacing;
	uint32_t m_leftGutterSpacing;
	uint32_t m_bottomGutterSpacing;
	uint32_t m_rightGutterSpacing;
	uint8_t m_numColumns;
	std::vector<uint8_t> m_columnMode;
	std::vector<uint8_t> m_numberFormat;
	std::vector<uint32_t> m_columnWidth;
	std::vector<uint32_t> m_rightOffsetForDecimalAlign;

	// variables needed for subgroup 2 (Set Table Cell Span)
	uint16_t m_colSpan;
	uint16_t m_rowSpan;

	// variables needed for subgroup 0x0B (Set Table Cell Fill Color/Patern)
	RGBSColor m_cellFillColor;
};

#endif /* WP3TABLESGROUP_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
