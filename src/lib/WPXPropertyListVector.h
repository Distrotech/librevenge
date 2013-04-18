/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2005 William Lachance (wrlach@gmail.com)
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
	const WPXPropertyList &operator[](unsigned long index) const;
	WPXPropertyListVector &operator=(const WPXPropertyListVector &vect);

	class Iter
	{
	public:
		Iter(const WPXPropertyListVector &vect);
		virtual ~Iter();
		void rewind();
		bool next();
		bool last();
		const WPXPropertyList &operator()() const;

	private:
		WPXPropertyListVectorIterImpl *m_iterImpl;
		Iter(const Iter &);
		Iter &operator=(const Iter &);
	};

	friend class WPXPropertyListVector::Iter;

private:
	WPXPropertyListVectorImpl *m_impl;
};

#endif /* WPXPROPERTYLISTVECTOR_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
