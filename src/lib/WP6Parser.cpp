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
#include "WP6Parser.h"
#include "WP6Part.h"
#include "UT_libwpd2.h"

WP6Parser::WP6Parser(FILE * stream, WP6Header * header)
	: WPXParser(stream, header)
{
}

gboolean WP6Parser::parse()
{
	if (!getHeader()->parse() )
		return FALSE;
	
	fseek(getStream(), getHeader()->m_iDocumentOffset - ftell(getStream()), SEEK_CUR);
	
	WP6Part * part;
	while (part = WP6Part::constructPart(this))
	{
		part->parse();
		delete part;
	}

	WPD_DEBUG_MSG(("WordPerfect: Finished with document parse (position = %ld)\n",(long)ftell(m_pStream)));
	
	return TRUE;
}
