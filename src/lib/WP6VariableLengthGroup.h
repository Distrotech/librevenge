/* libwpd
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#ifndef WP6VARIABLELENGTHGROUP_H
#define WP6VARIABLELENGTHGROUP_H

#include "WP6Part.h"

class WP6VariableLengthGroup_SubGroup
{
public:
	virtual ~WP6VariableLengthGroup_SubGroup() {}
	virtual void parse(WP6Listener *listener, const uint8_t numPrefixIDs, uint16_t const *prefixIDs) const = 0;
};

class WP6VariableLengthGroup : public WP6Part
{
public:
	WP6VariableLengthGroup(); // WP6VariableLengthGroup should _never_ be constructed, only its inherited classes
	virtual ~WP6VariableLengthGroup();

	static WP6VariableLengthGroup * constructVariableLengthGroup(WPXInputStream *input, WPXEncryption *encryption, const uint8_t groupID);

	static bool isGroupConsistent(WPXInputStream *input, WPXEncryption *encryption, const uint8_t groupID);

protected:
	void _read(WPXInputStream *input, WPXEncryption *encryption);
	virtual void _readContents(WPXInputStream * /* input */, WPXEncryption * /* encryption */) {} // we don't always need more information than that provided generically

	uint8_t getSubGroup() const { return m_subGroup; }
	uint16_t getSize() const { return m_size; }
	uint8_t getFlags() const { return m_flags; }
	uint8_t getNumPrefixIDs() const { return m_numPrefixIDs; }
	const uint16_t * getPrefixIDs() const { return m_prefixIDs; }
	uint16_t getSizeNonDeletable() const { return m_sizeNonDeletable; }
	uint16_t getSizeDeletable() const { return m_sizeDeletable; }

private:
	WP6VariableLengthGroup(const WP6VariableLengthGroup&);
	WP6VariableLengthGroup& operator=(const WP6VariableLengthGroup&);
	uint8_t m_subGroup;
	uint16_t m_size;
	uint8_t m_flags;
	uint8_t m_numPrefixIDs;
	uint16_t *m_prefixIDs;
	uint16_t m_sizeNonDeletable;
	uint16_t m_sizeDeletable;
};

#endif /* WP6VARIABLELENGTHGROUP_H */
