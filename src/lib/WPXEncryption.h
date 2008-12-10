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

#ifndef WPXENCRYPTION_H
#define WPXENCRYPTION_H

#include "libwpd_types.h"
#include "WPXString.h"

class WPXInputStream;

class WPXEncryption
{
public:
	WPXEncryption(const char *password, const unsigned long encryptionStartOffset = 0);
	~WPXEncryption();
	
	const unsigned char *readAndDecrypt(WPXInputStream *input, unsigned long numBytes, unsigned long &numBytesRead);
	uint16_t getCheckSum() const;
	
	void setEncryptionStartOffset(unsigned long encryptionStartOffset) { m_encryptionStartOffset = encryptionStartOffset; }
	unsigned long getEncryptionStartOffset() const { return m_encryptionStartOffset; }
	
	void setEncryptionMaskBase(unsigned char encryptionMaskBase) { m_encryptionMaskBase = encryptionMaskBase; }
	unsigned char getEncryptionMaskBase() const { return m_encryptionMaskBase; }
	
	const WPXString& getEncryptionPassword() const { return m_password; }

private:
	unsigned char *m_buffer;
	WPXString m_password;
	unsigned long m_encryptionStartOffset;
	unsigned char m_encryptionMaskBase;
};

#endif /* WPXENCRYPTION_H */
