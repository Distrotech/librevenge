/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#include "WP1PictureGroup.h"
#include "libwpd_internal.h"

#ifndef DUMP_PICTURE
#define DUMP_PICTURE 0
#endif

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
	uint8_t tmpWhatNot = readU8(input, encryption);
	if (tmpWhatNot)
		input->seek(1, WPX_SEEK_CUR);
	m_width = readU16(input, encryption, true);
	m_height = readU16(input, encryption, true);
	input->seek(6, WPX_SEEK_CUR);
	uint32_t dataSize = readU16(input, encryption, true);
	WPD_DEBUG_MSG(("WP1PictureGroup: Offset = 0x%.4x, Width = %i, Height = %i, Data Size = 0x%.4x\n", (unsigned)input->tell(), m_width, m_height, dataSize));
	if (dataSize + 13 > getSize())
		return;
	for (int i = 0; i < 512; i++)
		m_binaryData.append((unsigned char)0);
	m_binaryData.append((uint8_t)((dataSize + 512)>>8));
	m_binaryData.append((uint8_t)(dataSize + 512));
	for (unsigned long j = 2; j < dataSize && !input->atEOS(); j++ )
		m_binaryData.append(readU8(input, encryption));
#if DUMP_PICTURE
	std::ostringstream filename;
	filename << "binarydump" << pictureId++ << ".pct";
	FILE *f = fopen(filename.str().c_str(), "wb");
	if (f)
	{
		WPXInputStream *tmpStream = const_cast<WPXInputStream *>(m_binaryData.getDataStream());
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
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
