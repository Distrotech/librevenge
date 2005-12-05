/* libwpd
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002 Marc Maurer (j.m.maurer@student.utwente.nl)
 *  
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
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

#ifndef WPXPARSER_H
#define WPXPARSER_H

#include "WPXHeader.h"
//#include "WPXListener.h"

class WPXHLListenerImpl;

class WPXParser
{
public:
	WPXParser(WPXInputStream * input, WPXHeader *header);
	virtual ~WPXParser();

	virtual void parse(WPXHLListenerImpl *listenerImpl) = 0;

protected:
	WPXHeader * getHeader() { return m_header; }
	WPXInputStream * getInput() { return m_input; }
	
private:
	WPXInputStream * m_input;
//	WPXListener * m_hlListener;

	WPXHeader * m_header;
};

#endif /* WPXPARSER_H */
