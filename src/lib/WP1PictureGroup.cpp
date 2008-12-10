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

#include "WP1PictureGroup.h"
#include "libwpd_internal.h"

#define DUMP_PICTURE 0

#if DUMP_PICTURE
static unsigned pictureId = 0;
#include <sstream>
#endif

WP1PictureGroup::WP1PictureGroup(WPXInputStream *input, WPXEncryption *encryption, uint8_t group) :
	WP1VariableLengthGroup(group),
	m_binaryData(),
	m_width(0),
	m_height(0)
{
	_read(input, encryption);
}

WP1PictureGroup::~WP1PictureGroup()
{
}

void WP1PictureGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	m_binaryData.clear();
	input->seek(1, WPX_SEEK_CUR);
	m_width = readU16(input, encryption, true);
	m_height = readU16(input, encryption, true);
	input->seek(6, WPX_SEEK_CUR);
	uint32_t dataSize = readU16(input, encryption, true);
	if (dataSize + 11 > getSize())
		return;
	input->seek(-2, WPX_SEEK_CUR);
	for (int i = 0; i < 512; i++)
		m_binaryData.append((unsigned char)0);
	for (unsigned long j = 0; j < dataSize && !input->atEOS(); j++ );
		m_binaryData.append(readU8(input, encryption));
#if DUMP_PICTURE
	std::ostringstream filename;
	filename << "binarydump" << pictureId++ << ".pct";
	FILE *f = fopen(filename.str().c_str(), "wb");
	if (f)
	{
		WPXInputStream* tmpStream = const_cast<WPXInputStream *>(m_binaryData.getDataStream());
		while (!tmpStream->atEOS())
			fprintf(f, "%c", readU8(tmpStream, 0));
		fclose(f);
	}
#endif
}

void WP1PictureGroup::parse(WP1Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a Picture group\n"));
	listener->insertPicture(m_width, m_height, m_binaryData);
}
