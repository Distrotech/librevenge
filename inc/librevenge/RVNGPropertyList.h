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

#ifndef RVNGPROPERTYLIST_H
#define RVNGPROPERTYLIST_H

#include "librevenge-api.h"

#include "RVNGProperty.h"

namespace librevenge
{

// we use the pimpl pattern so we don't expose any STL symbols to the rest of
// the world.. yes, this is quite annoying.

class RVNGPropertyListImpl;
class RVNGPropertyListIterImpl;
class RVNGPropertyListVector;

class REVENGE_API RVNGPropertyList
{
public:
	RVNGPropertyList();
	RVNGPropertyList(const RVNGPropertyList &);
	virtual ~RVNGPropertyList();
	void insert(const char *name, RVNGProperty *prop);
	void insert(const char *name, const char *val);
	void insert(const char *name, const int val);
	void insert(const char *name, const bool val);
	void insert(const char *name, const RVNGString &val);
	void insert(const char *name, const double val, const RVNGUnit units = RVNG_INCH);
	void insert(const char *name, const unsigned char *buffer, const unsigned long bufferSize);
	void insert(const char *name, const RVNGBinaryData &data);
	void insert(const char *name, const RVNGPropertyListVector &vec);
	void remove(const char *name);
	void clear();
	bool empty() const;

	const RVNGProperty *operator[](const char *name) const;
	const RVNGPropertyListVector *child(const char *name) const;
	const RVNGPropertyList &operator=(const RVNGPropertyList &propList);

	RVNGString getPropString() const;

	class REVENGE_API Iter
	{
	public:
		Iter(const RVNGPropertyList &propList);
		virtual ~Iter();
		void rewind();
		bool next();
		bool last();
		const RVNGProperty *operator()() const;
		const char *key() const;
		const RVNGPropertyListVector *child() const;
	private:
		RVNGPropertyListIterImpl *m_iterImpl;
		Iter(const Iter &);
		Iter &operator=(const Iter &);
	};
	friend class RVNGPropertyList::Iter;

private:
	mutable RVNGPropertyListImpl *m_impl;
};

}

#endif /* RVNGPROPERTYLIST_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
