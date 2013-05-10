/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2007 Jaroslav Fojtik (JaFojtik@seznam.cz)
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

#ifndef WPXENCRYPTION_H
#define WPXENCRYPTION_H

#include <libwpd/libwpd.h>
#include "libwpd_types.h"

class WPXInputStream;

class WPXEncryption
{
public:
	WPXEncryption(const char *password, const unsigned long encryptionStartOffset = 0);
	~WPXEncryption();

	const unsigned char *readAndDecrypt(WPXInputStream *input, unsigned long numBytes, unsigned long &numBytesRead);
	uint16_t getCheckSum() const;

	void setEncryptionStartOffset(unsigned long encryptionStartOffset)
	{
		m_encryptionStartOffset = encryptionStartOffset;
	}
	unsigned long getEncryptionStartOffset() const
	{
		return m_encryptionStartOffset;
	}

	void setEncryptionMaskBase(unsigned char encryptionMaskBase)
	{
		m_encryptionMaskBase = encryptionMaskBase;
	}
	unsigned char getEncryptionMaskBase() const
	{
		return m_encryptionMaskBase;
	}

	const WPXString &getEncryptionPassword() const
	{
		return m_password;
	}

private:
	unsigned char *m_buffer;
	WPXString m_password;
	unsigned long m_encryptionStartOffset;
	unsigned char m_encryptionMaskBase;
	// Unimplemented to prevent compiler from creating crasher ones
	WPXEncryption(const WPXEncryption &);
	WPXEncryption &operator=(const WPXEncryption &);
};

#endif /* WPXENCRYPTION_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
