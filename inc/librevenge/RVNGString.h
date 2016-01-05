/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
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
 */

#ifndef RVNGSTRING_H
#define RVNGSTRING_H

#include "librevenge-api.h"

namespace librevenge
{

class RVNGStringImpl;

/** UTF-8 string.
  */
class REVENGE_API RVNGString
{
public:
	RVNGString();
	RVNGString(const RVNGString &other);
	RVNGString(const char *str);
	~RVNGString();

	/** Create a new string from \a s as escaped XML.
	  *
	  * This function can be used instead of #appendEscapedXML(const RVNGString&) in contexts
	  * where only a temporary string is needed.
	  *
	  * @sa appendEscapedXML(const RVNGString&)
	  */
	static RVNGString escapeXML(const RVNGString &s);

	/** Create a new string from \a s as escaped XML.
	  *
	  * This function can be used instead of #appendEscapedXML(const char*) in contexts
	  * where only a temporary string is needed.
	  *
	  * @sa appendEscapedXML(const char*)
	  */
	static RVNGString escapeXML(const char *s);

	const char *cstr() const;

	/** Return the number of UTF-8 characters.
	  *
	  * @sa size()
	  */
	int len() const;
	/** Return the size in bytes of the data buffer.
	  *
	  * This is equivalent to <code>strlen(str.cstr())</code>
	  *
	  * @sa len()
	  */
	unsigned long size() const;

	bool empty() const;

	void sprintf(const char *format, ...) REVENGE_ATTRIBUTE_PRINTF(2, 3);
	void append(const RVNGString &s);
	void append(const char *s);
	void append(const char c);

	/** Append the content of @a s as escaped XML.

	@sa escapeXML(const RVNGString&)
	*/
	void appendEscapedXML(const RVNGString &s);

	/** Append the content of @a s as escaped XML.

	@sa escapeXML(const char*)
	*/
	void appendEscapedXML(const char *s);

	void clear();
	RVNGString &operator=(const RVNGString &str);
	RVNGString &operator=(const char *s);

	// Comparison
	bool operator==(const char *s) const;
	bool operator==(const RVNGString &str) const;
	inline bool operator!=(const char *s) const
	{
		return !operator==(s);
	}
	inline bool operator!=(const RVNGString &str) const
	{
		return !operator==(str);
	}
	bool operator<(const char *s) const;
	bool operator<(const RVNGString &str) const;
	inline bool operator<=(const char *s) const
	{
		return operator==(s) || operator<(s);
	}
	inline bool operator<=(const RVNGString &str) const
	{
		return operator==(str) || operator<(str);
	}
	inline bool operator>=(const char *s) const
	{
		return !operator<(s);
	}
	inline bool operator>=(const RVNGString &str) const
	{
		return !operator<(str);
	}
	inline bool operator>(const char *s) const
	{
		return !operator<=(s);
	}
	inline bool operator>(const RVNGString &str) const
	{
		return !operator<=(str);
	}

	class REVENGE_API Iter
	{
	public:
		Iter(const RVNGString &str);
		virtual ~Iter();
		void rewind();
		bool next();
		bool last();
		const char *operator()() const;
	private:
		Iter(const Iter &);
		Iter &operator=(const Iter &);
		RVNGStringImpl *m_stringImpl;
		int m_pos;
		mutable char *m_curChar;
	};

private:
	RVNGStringImpl *m_stringImpl;
};

}

#endif
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
