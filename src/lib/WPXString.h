/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2004 William Lachance (wrlach@gmail.com)
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

#ifndef WPXSTRING_H
#define WPXSTRING_H

class WPXStringImpl;

class WPXString
{
public:
	WPXString();
	WPXString(const WPXString &, bool escapeXML = false);
	WPXString(const char *str);
	~WPXString();

	const char *cstr() const;
	int len() const;

	void sprintf(const char *format, ...);
	void append(const WPXString &s);
	void append(const char *s);
	void append(const char c);
	void clear();
	WPXString &operator=(const WPXString &str);
	WPXString &operator=(const char *s);
	bool operator==(const char *s) const;
	bool operator==(const WPXString &str) const;
	bool operator!() const;
	inline bool operator!=(const char *s) const
	{
		return !operator==(s);
	}
	inline bool operator!=(const WPXString &str) const
	{
		return !operator==(str);
	}

	class Iter
	{
	public:
		Iter(const WPXString &str);
		virtual ~Iter();
		void rewind();
		bool next();
		bool last();
		const char *operator()() const;
	private:
		Iter(const Iter &);
		Iter &operator=(const Iter &);
		WPXStringImpl *m_stringImpl;
		int m_pos;
		mutable char *m_curChar;
	};

private:
	WPXStringImpl *m_stringImpl;
};
#endif
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
