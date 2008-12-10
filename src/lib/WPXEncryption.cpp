/* libwpd
 * Copyright (C) 2007 Jaroslav Fojtik (JaFojtik@seznam.cz)
 * Copyright (C) 2007 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include "WPXEncryption.h"
#include "WPXStream.h"
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
				m_password.append(password[i] - 'a' + 'A');
			else
				m_password.append(password[i]);
		m_encryptionMaskBase = m_password.len() + 1;
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
		checkSum = (((checkSum >> 1) | (checkSum << 15)) ^ (((uint16_t)*(i())) << 8 ));
	WPD_DEBUG_MSG(("CheckSum: 0x%.4x\n", checkSum));
	return checkSum;
}

const unsigned char * WPXEncryption::readAndDecrypt(WPXInputStream *input, unsigned long numBytes, unsigned long &numBytesRead)
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
			m_buffer[i] = encryptedBuffer[i] ^ ( m_password.cstr()[passwordOffset] ^ encryptionMask);
		}
	}
	return m_buffer;
}
