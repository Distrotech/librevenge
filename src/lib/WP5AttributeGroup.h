/* libwpd
 * Copyright (C) 2003 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2003 Marc Maurer (j.m.maurer@student.utwente.nl)
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

#ifndef WP5ATTRIBUTEGROUP_H
#define WP5ATTRIBUTEGROUP_H

#include "WP5HLListener.h"
#include "WP5FixedLengthGroup.h"

class WP5AttributeGroup : public WP5FixedLengthGroup
{
 public:
	WP5AttributeGroup(WPXInputStream *input, guint8 groupID);	
	virtual void parse(WP5HLListener *llListener) = 0;
	const guint8 getAttribute() const { return m_attribute; }
	
 protected:
	virtual void _readContents(WPXInputStream *input);

 private:
	guint8 m_attribute;
};

class WP5AttributeOnGroup : public WP5AttributeGroup
{
 public:
	WP5AttributeOnGroup(WPXInputStream *input, guint8 groupID);
	virtual void parse(WP5HLListener *listener);
};

class WP5AttributeOffGroup : public WP5AttributeGroup
{
 public:
	WP5AttributeOffGroup(WPXInputStream *input, guint8 groupID);
	virtual void parse(WP5HLListener *listener);
};

#endif /* WP5ATTRIBUTEGROUP_H */
