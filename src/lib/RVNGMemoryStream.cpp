/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2004-2005 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 */

#include "RVNGMemoryStream.h"

namespace librevenge
{

RVNGMemoryInputStream::RVNGMemoryInputStream(unsigned char *data, unsigned long size) :
	RVNGInputStream(),
	m_offset(0),
	m_size(size),
	m_data(data)
{
}

RVNGMemoryInputStream::~RVNGMemoryInputStream()
{
}

const unsigned char *RVNGMemoryInputStream::read(unsigned long numBytes, unsigned long &numBytesRead)
{
	numBytesRead = 0;

	if (numBytes == 0)
		return 0;

	long numBytesToRead;

	if (m_offset+long(numBytes) < long(m_size))
		numBytesToRead = long(numBytes);
	else
		numBytesToRead = long(m_size) - long(m_offset);

	numBytesRead = (unsigned long) numBytesToRead; // about as paranoid as we can be..

	if (numBytesToRead == 0)
		return 0;

	long oldOffset = m_offset;
	m_offset += numBytesToRead;

	return &m_data[oldOffset];
}

int RVNGMemoryInputStream::seek(long offset, RVNG_SEEK_TYPE seekType)
{
	if (seekType == RVNG_SEEK_CUR)
		m_offset += offset;
	else if (seekType == RVNG_SEEK_SET)
		m_offset = offset;
	else if (seekType == RVNG_SEEK_END)
		m_offset = (long)m_size+offset;

	if (m_offset < 0)
	{
		m_offset = 0;
		return -1;
	}
	if ((long)m_offset > (long)m_size)
	{
		m_offset = (long) m_size;
		return -1;
	}

	return 0;
}

long RVNGMemoryInputStream::tell()
{
	return m_offset;
}

bool RVNGMemoryInputStream::isEnd()
{
	if ((long)m_offset == (long)m_size)
		return true;

	return false;
}

}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
