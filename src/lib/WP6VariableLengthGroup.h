/* libwpd2
 * Copyright (C) 2002 William Lachance (wlach@interlog.com)
 * Copyright (C) 2002 Marc Maurer (j.m.maurer@student.utwente.nl)
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by 
 * Corel Corporation or Corel Corporation Limited."
 */

#ifndef WP6VARIABLELENGTHGROUP_H
#define WP6VARIABLELENGTHGROUP_H

#include "WP6Part.h"

class WP6VariableLengthGroup : public WP6Part
{
 public:
	WP6VariableLengthGroup(); // WP6VariableLengthGroup should _never_ be constructed, only its inherited classes
	virtual ~WP6VariableLengthGroup();
	
	static WP6VariableLengthGroup * WP6VariableLengthGroup::constructVariableLengthGroup(FILE *stream, guint8 groupID);

 protected:
	void _read(FILE *stream);
	virtual void _readContents(FILE *stream) {} // we don't always need more information than that provided generically

	const guint8 getGroup() const { return m_group; }
	const guint8 getSubGroup() const { return m_subGroup; }
	const guint8 getFlags() const { return m_flags; }
	const guint8 getNumPrefixIDs() const { return m_numPrefixIDs; }
	guint16 * const getPrefixIDs() const { return m_prefixIDs; }
	const guint16 getSizeNonDeletable() const { return m_sizeNonDeletable; }

 private:
	guint8 m_group;
	guint8 m_subGroup;
	guint16 m_size;
	guint8 m_flags;
	guint8 m_numPrefixIDs;
	guint16 *m_prefixIDs;
	guint16 m_sizeNonDeletable;
};

#endif /* WP6VARIABLELENGTHGROUP_H */
