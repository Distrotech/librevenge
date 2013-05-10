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
	void insert(const char *name, WPXProperty *prop);
	void insert(const char *name, const char *val);
	void insert(const char *name, const int val);
	void insert(const char *name, const bool val);
	void insert(const char *name, const WPXString &val);
	void insert(const char *name, const double val, const WPXUnit units = WPX_INCH);

	void remove(const char *name);
	const WPXProperty *operator[](const char *name) const;
	const WPXPropertyList &operator=(const WPXPropertyList &propList);
	void clear();

	class Iter
	{
	public:
		Iter(const WPXPropertyList &propList);
		virtual ~Iter();
		void rewind();
		bool next();
		bool last();
		const WPXProperty *operator()() const;
		const char *key();
	private:
		WPXMapIterImpl *m_iterImpl;
		Iter(const Iter &);
		Iter &operator=(const Iter &);
	};
	friend class WPXPropertyList::Iter;

private:
	mutable WPXMapImpl *m_mapImpl;
};
#endif /* WPXPROPERTYLIST_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
