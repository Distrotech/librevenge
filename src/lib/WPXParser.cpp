/* libwpd2
 * Copyright (C) 2002 William Lachance (wlach@interlog.com)
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by 
 * Corel Corporation or Corel Corporation Limited."
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "UT_libwpd2.h"
#include "WPXParser.h"
#include "WP6Parser.h"
#include "WP6FileStructure.h"

WPXParser::WPXParser(FILE * stream)
{
	m_stream = stream;
}

WPXParser * WPXParser::constructParser(FILE * stream)
{
	if (!stream)
	{
		WPD_DEBUG_MSG(("WordPerfect: Stream is NULL!\n"));
		return NULL;
	}

	// we will add this functionality back in.. eventually
	//WPXHeader *header = WPXHeader::constructHeader(stream);

	return new WP6Parser(stream);
}
