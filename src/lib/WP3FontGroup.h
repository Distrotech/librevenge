/* libwpd
 * Copyright (C) 2005 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#ifndef WP3FONTGROUP_H
#define WP3FONTGROUP_H

#include "WP3VariableLengthGroup.h"

class WP3FontGroup : public WP3VariableLengthGroup
{
 public:
	WP3FontGroup(WPXInputStream *input);	
	virtual ~WP3FontGroup();
	virtual void _readContents(WPXInputStream *input);
	virtual void parse(WP3Listener *listener);

 private:
 	/* Set Text Color */
	RGBSColor m_fontColor;
 	/* Set Text Font */
	std::string m_fontName;
	/* Set Font Size */
	uint16_t m_fontSize;

};

#endif /* WP3FONTGROUP_H */
