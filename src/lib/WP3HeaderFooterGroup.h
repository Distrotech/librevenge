/* libwpd
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

#ifndef WP3HEADERFOOTERGROUP_H
#define WP3HEADERFOOTERGROUP_H

#include "WP3VariableLengthGroup.h"
#include "WP3SubDocument.h"

class WP3HeaderFooterGroup : public WP3VariableLengthGroup
{
public:
	WP3HeaderFooterGroup(WPXInputStream *input, WPXEncryption *encryption);
	~WP3HeaderFooterGroup();	
	void _readContents(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP3Listener *listener);

private:
	WP3HeaderFooterGroup(const WP3HeaderFooterGroup&);
	WP3HeaderFooterGroup& operator=(const WP3HeaderFooterGroup&);
	uint8_t m_definition;
	WP3SubDocument *m_subDocument;	
};

#endif /* WP3HEADERFOOTERGROUP_H */
