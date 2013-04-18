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

#ifndef WP5DEFINITIONGROUP_H
#define WP5DEFINITIONGROUP_H

#include "WP5VariableLengthGroup.h"
#include "WP5FileStructure.h"

class WP5DefinitionGroup_DefineTablesSubGroup : public WP5VariableLengthGroup_SubGroup
{
public:
	WP5DefinitionGroup_DefineTablesSubGroup(WPXInputStream *input, WPXEncryption *encryption, uint16_t subGroupSize);
	void parse(WP5Listener *listener);

private:
	uint8_t m_position;
	uint16_t m_numColumns;
	uint16_t m_leftOffset;
	uint16_t m_leftGutter;
	uint16_t m_rightGutter;
	uint16_t m_columnWidth[32];
	uint16_t m_attributeBits[32];
	uint8_t m_columnAlignment[32];

};

class WP5DefinitionGroup : public WP5VariableLengthGroup
{
public:
	WP5DefinitionGroup(WPXInputStream *input, WPXEncryption *encryption);
	~WP5DefinitionGroup();
	void parse(WP5Listener *listener);

protected:
	virtual void _readContents(WPXInputStream *input, WPXEncryption *encryption);

private:
	WP5DefinitionGroup(const WP5DefinitionGroup &);
	WP5DefinitionGroup &operator=(const WP5DefinitionGroup &);
	WP5VariableLengthGroup_SubGroup *m_subGroupData;
};

#endif /* WP5DEFINITIONGROUP_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
