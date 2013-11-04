/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2012 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 */

#include <vector>
#include <librevenge/librevenge.h>

namespace librevenge
{

class RVNGStringVectorImpl
{
public:
	RVNGStringVectorImpl() : m_strings() {}
	~RVNGStringVectorImpl() {}
	std::vector<RVNGString> m_strings;
};

RVNGStringVector::RVNGStringVector()
	: m_pImpl(new RVNGStringVectorImpl())
{
}

RVNGStringVector::RVNGStringVector(const RVNGStringVector &vec)
	: m_pImpl(new RVNGStringVectorImpl(*(vec.m_pImpl)))
{
}

RVNGStringVector::~RVNGStringVector()
{
	delete m_pImpl;
}

RVNGStringVector &RVNGStringVector::operator=(const RVNGStringVector &vec)
{
	// Check for self-assignment
	if (this == &vec)
		return *this;
	if (m_pImpl)
		delete m_pImpl;
	m_pImpl = new RVNGStringVectorImpl(*(vec.m_pImpl));
	return *this;
}

unsigned RVNGStringVector::size() const
{
	return (unsigned)(m_pImpl->m_strings.size());
}

bool RVNGStringVector::empty() const
{
	return m_pImpl->m_strings.empty();
}

const RVNGString &RVNGStringVector::operator[](unsigned idx) const
{
	return m_pImpl->m_strings[idx];
}

void RVNGStringVector::append(const RVNGString &str)
{
	m_pImpl->m_strings.push_back(str);
}

void RVNGStringVector::clear()
{
	m_pImpl->m_strings.clear();
}

}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
