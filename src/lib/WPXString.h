/* libwpd
 * Copyright (C) 2004 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2005 Net Integration Technologies (http://www.net-itech.com)
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
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

	const char * cstr() const;
	int len() const;

	void sprintf(const char *format, ...);
	void append(const WPXString &s);
	void append(const char *s);
	void append(const char c);
	void clear();
	WPXString& operator=(const WPXString &str);
	WPXString& operator=(const char *s);
	bool operator==(const char *s) const;
	bool operator==(const WPXString &str) const;
        bool operator!() const;
	inline bool operator!=(const char *s) const { return !operator==(s); }
	inline bool operator!=(const WPXString &str) const { return !operator==(str); }

	class Iter
	{
	public:
		Iter(const WPXString &str);
		virtual ~Iter();
		void rewind();
		bool next();
		bool last();
		const char * operator()() const;
	private:
		Iter(const Iter&);
		Iter& operator=(const Iter&);
		WPXStringImpl *m_stringImpl;
		int m_pos;
		mutable char *m_curChar;
	};
		
private:
	WPXStringImpl *m_stringImpl;
};
#endif
