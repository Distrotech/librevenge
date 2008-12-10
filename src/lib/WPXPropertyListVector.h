/* libwpd
 * Copyright (C) 2005 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2005 Net Integration Technologies, Inc. (http://www.net-itech.com)
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

#ifndef WPXPROPERTYLISTVECTOR_H
#define WPXPROPERTYLISTVECTOR_H

#include "WPXPropertyList.h"

class WPXPropertyListVectorImpl;
class WPXPropertyListVectorIterImpl;

class WPXPropertyListVector
{
public:
	WPXPropertyListVector(const WPXPropertyListVector &);
	WPXPropertyListVector();
	virtual ~WPXPropertyListVector();
	void append(const WPXPropertyList &elem);
	void append(const WPXPropertyListVector &vec);
	unsigned long count() const;
	const WPXPropertyList& operator[](unsigned long index) const;
	WPXPropertyListVector& operator=(const WPXPropertyListVector& vect);

	class Iter
	{
	public:
		Iter(const WPXPropertyListVector &vect);
		virtual ~Iter();
		void rewind();
		bool next();
		bool last();
		const WPXPropertyList & operator()() const;

	private:
		WPXPropertyListVectorIterImpl *m_iterImpl;
		Iter(const Iter&);
		Iter& operator=(const Iter&);
	};

	friend class WPXPropertyListVector::Iter;

private:
	WPXPropertyListVectorImpl *m_impl;
};

#endif /* WPXPROPERTYLISTVECTOR_H */
