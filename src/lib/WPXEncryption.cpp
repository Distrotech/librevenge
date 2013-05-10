/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2007 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include "WPXEncryption.h"
#include "libwpd_internal.h"
#include <string.h>

WPXEncryption::WPXEncryption(const char *password, const unsigned long encryptionStartOffset) :
	m_buffer(NULL),
	m_password(),
	m_encryptionStartOffset(encryptionStartOffset),
	m_encryptionMaskBase(0)
{
	if (password)
	{
		for (unsigned long i = 0; i < strlen(password); i++)
			if (password[i] >= 'a' && password[i] <= 'z')
				m_password.append((char)(password[i] - 'a' + 'A'));
			else
				m_password.append(password[i]);
		m_encryptionMaskBase = (uint8_t)(m_password.len() + 1);
	}
}

WPXEncryption::~WPXEncryption()
{
	if (m_buffer)
		delete [] m_buffer;
}


uint16_t WPXEncryption::getCheckSum() const
{
	if (m_password.len() <= 0)
		return 0;
	WPXString::Iter i(m_password);
	uint16_t checkSum = 0;
	for (i.rewind(); i.next();)
		checkSum = (uint16_t)(((checkSum >> 1) | (checkSum << 15)) ^ (((uint16_t)*(i())) << 8 ));
	WPD_DEBUG_MSG(("CheckSum: 0x%.4x\n", checkSum));
	return checkSum;
}

const unsigned char *WPXEncryption::readAndDecrypt(WPXInputStream *input, unsigned long numBytes, unsigned long &numBytesRead)
{
	if ((m_password.len() <= 0) || (m_encryptionStartOffset > input->tell() + numBytes))
		return input->read(numBytes, numBytesRead);

	unsigned long readStartPosition = input->tell();
	if (readStartPosition == (unsigned long)-1)
		return 0;
	const unsigned char *encryptedBuffer = input->read(numBytes, numBytesRead);
	if (m_buffer)
		delete [] m_buffer;
	m_buffer = new unsigned char[numBytesRead];
	for (unsigned long i=0; i<numBytesRead; i++)
	{
		if (readStartPosition + i < m_encryptionStartOffset)
			m_buffer[i] = encryptedBuffer[i];
		else
		{
			unsigned long passwordOffset = (readStartPosition + i - m_encryptionStartOffset) % m_password.len();
			unsigned char encryptionMask = (unsigned char)(m_encryptionMaskBase + readStartPosition + i - m_encryptionStartOffset);
			m_buffer[i] = (uint8_t)(encryptedBuffer[i] ^ ( m_password.cstr()[passwordOffset] ^ encryptionMask));
		}
	}
	return m_buffer;
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
