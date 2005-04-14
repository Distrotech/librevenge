/* libwpd
 * Copyright (C) 2004, 2005 William Lachance (william.lachance@sympatico.ca)
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
#ifndef GSFSTREAM_H
#define GSFSTREAM_H

#include "WPXStream.h"

#include <gsf/gsf-input.h>

class GSFInputStream : public WPXInputStream
{
public:
	GSFInputStream(GsfInput *input);
	virtual ~GSFInputStream();

	virtual bool isOLEStream();
	virtual WPXInputStream * getDocumentOLEStream();

	virtual const uint8_t *read(size_t numBytes, size_t &numBytesRead);
	virtual int seek(long offset, WPX_SEEK_TYPE seekType);
	virtual long tell() { return gsf_input_tell(m_input); }
	virtual bool atEOS() { return gsf_input_eof(m_input); }

private:
	GsfInput *m_input;
	GsfInfile *m_ole;
};

#endif
