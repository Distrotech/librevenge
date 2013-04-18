/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2004-2005 William Lachance (wrlach@gmail.com)
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

#ifndef WPXSTREAM_H
#define WPXSTREAM_H

enum WPX_SEEK_TYPE
{
    WPX_SEEK_CUR,
    WPX_SEEK_SET,
    WPX_SEEK_END
};

class WPXInputStream
{
public:
	WPXInputStream() {}
	virtual ~WPXInputStream() {}

	/**
	Analyses the content of the input stream to see whether it is an OLE2 storage.
	\return A boolean value that should be true if the input stream is an OLE2 storage
	and false if it is not the case
	*/
	virtual bool isOLEStream() = 0;
	/**
	Extracts a \c PerfectOffice_MAIN stream from an OLE2 storage.
	\return Should be a pointer to WPXInputStream constructed from the \c PerfectOffice_MAIN stream if it exists.
	\return Should be 0, if the \c PerfectOffice_MAIN stream does not exist inside the OLE2 storage
	or if the input stream is not an OLE2 storage.
	*/
	virtual WPXInputStream *getDocumentOLEStream(const char *name) = 0;

	/**
	Tries to read a given number of bytes starting from the current position inside the input stream.
	\param numBytes Number of bytes desired to be read.
	\param numBytesRead Number of bytes that were possible to be read.
	\return Should be a pointer to an array of numBytesRead bytes (unsigned char[numBytesRead]).
	\return Optionally it could be 0 if the desired number of bytes could not be read.
	*/
	virtual const unsigned char *read(unsigned long numBytes, unsigned long &numBytesRead) = 0;
	/**
	Moves to the next location inside the input stream.
	\param offset The offset of the location inside the input stream to move to.
	It is relative either to the current position or to the beginning of the input stream
	depending on the value of the \c seekType parameter.
	\param seekType Determines whether the \c offset is relative to the
	beginning of the input stream (\c WPX_SEEK_SET) or to the current position (\c WPX_SEEK_CUR).
	\return An integer value that should be 0 (zero) if the seek was successful and any other value
	if it failed (i.e. the requested \c offset is beyond the end of the input stream or before its beginning).
	*/
	virtual int seek(long offset, WPX_SEEK_TYPE seekType) = 0;
	/**
	Returns the actual position inside the input stream.
	\return A long integer value that should correspond to the position of the next location to be read in the input stream.
	*/
	virtual long tell() = 0;
	/**
	Determines whether the current position is at the end of the stream.
	\return A boolean value that should be true if the next location to be read in the input stream
	is beyond its end. In all other cases, it should be false.
	*/
	virtual bool atEOS() = 0;
};
#endif
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
