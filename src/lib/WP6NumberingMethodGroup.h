/* libwpd
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

#ifndef WP6NUMBERINGMETHODGROUP_H
#define WP6NUMBERINGMETHODGROUP_H

#include "WP6VariableLengthGroup.h"

class WP6NumberingMethodGroup : public WP6VariableLengthGroup
{
  public:
	WP6NumberingMethodGroup(WPXInputStream *input, WPXEncryption *encryption);	
	~WP6NumberingMethodGroup();
	void _readContents(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener);
  private:
	uint8_t m_levelNumber;
	uint8_t m_numberingMethod;
};

#endif /* WP6NUMBERINGMETHODGROUP_H */
