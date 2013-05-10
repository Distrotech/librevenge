/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2006 Ariya Hidayat (ariya@kde.org)
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

#ifndef __WPXSTREAMIMPLEMENTATION_H__
#define __WPXSTREAMIMPLEMENTATION_H__

#include "WPXStream.h"

class WPXFileStreamPrivate;

class WPXFileStream: public WPXInputStream
{
public:
	explicit WPXFileStream(const char *filename);
	~WPXFileStream();

	const unsigned char *read(unsigned long numBytes, unsigned long &numBytesRead);
	long tell();
	int seek(long offset, WPX_SEEK_TYPE seekType);
	bool atEOS();

	bool isOLEStream();
	WPXInputStream *getDocumentOLEStream(const char *name);

private:
	WPXFileStreamPrivate *d;
	WPXFileStream(const WPXFileStream &); // copy is not allowed
	WPXFileStream &operator=(const WPXFileStream &); // assignment is not allowed
};

class WPXStringStreamPrivate;

class WPXStringStream: public WPXInputStream
{
public:
	WPXStringStream(const unsigned char *data, const unsigned int dataSize);
	~WPXStringStream();

	const unsigned char *read(unsigned long numBytes, unsigned long &numBytesRead);
	long tell();
	int seek(long offset, WPX_SEEK_TYPE seekType);
	bool atEOS();

	bool isOLEStream();
	WPXInputStream *getDocumentOLEStream(const char *name);

private:
	WPXStringStreamPrivate *d;
	WPXStringStream(const WPXStringStream &); // copy is not allowed
	WPXStringStream &operator=(const WPXStringStream &); // assignment is not allowed
};

#endif // __WPXSTREAMIMPLEMENTATION_H__
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
