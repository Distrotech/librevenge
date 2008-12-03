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

#ifndef WPXPROPERTYLIST_H
#define WPXPROPERTYLIST_H
#include "WPXProperty.h"

// we use the pimpl pattern so we don't expose any STL symbols to the rest of 
// the world.. yes, this is quite annoying.

class WPXMapImpl;
class WPXMapIterImpl;

class WPXPropertyList
{
public:
	WPXPropertyList();
	WPXPropertyList(const WPXPropertyList &);
	virtual ~WPXPropertyList();
	void insert(const char * name, WPXProperty *prop);
	void insert(const char * name, const char *val);
	void insert(const char * name, const int val);
	void insert(const char * name, const bool val);
	void insert(const char * name, const WPXString &val);
	void insert(const char * name, const double val, const WPXUnit units = WPX_INCH); 

	void remove(const char * name);
	const WPXProperty * operator[](const char *name) const;
	const WPXPropertyList& operator=(const WPXPropertyList& propList);
	void clear();

	class Iter
	{
	public:
		Iter(const WPXPropertyList &propList);
		virtual ~Iter();
		void rewind();
		bool next();
		bool last();
		const WPXProperty * operator()() const;
		const char * key();
	private:
		WPXMapIterImpl *m_iterImpl;
		Iter(const Iter&);
		Iter& operator=(const Iter&);
	};
	friend class WPXPropertyList::Iter;

private:
	mutable WPXMapImpl *m_mapImpl;
};
#endif /* WPXPROPERTYLIST_H */
