/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002-2004 Marc Maurer (uwog@uwog.net)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 */

#ifndef RVNGRAWGENERATORBASE_H
#define RVNGRAWGENERATORBASE_H

#include <stack>
#include <librevenge/librevenge.h>

#ifdef RVNG_CALLGRAPH_ENTER
#warning who defined RVNG_CALLGRAPH_ENTER?
#undef RVNG_CALLGRAPH_ENTER
#endif

#define RVNG_CALLGRAPH_ENTER(M, L) \
	m_impl->m_atLeastOneCallback = true; \
	if (!m_impl->m_printCallgraphScore) \
			m_impl->iuprintf M; \
	else \
		m_impl->m_callStack.push(L);

#ifdef RVNG_CALLGRAPH_LEAVE
#warning who defined RVNG_CALLGRAPH_LEAVE?
#undef RVNG_CALLGRAPH_LEAVE
#endif

#define RVNG_CALLGRAPH_LEAVE(M, L) \
	m_impl->m_atLeastOneCallback = true; \
	if (!m_impl->m_printCallgraphScore) \
			m_impl->idprintf M; \
	else \
	{ \
		const int lc = m_impl->m_callStack.top(); \
		if (lc != L) \
			m_impl->m_callbackMisses++; \
		m_impl->m_callStack.pop(); \
	}

namespace librevenge
{

struct RVNGRawGeneratorBase
{
	explicit RVNGRawGeneratorBase(bool printCallgraphScore);
	virtual ~RVNGRawGeneratorBase();

	int m_indent;
	int m_callbackMisses;
	bool m_atLeastOneCallback;
	bool m_printCallgraphScore;
	std::stack<int> m_callStack;

	void indentUp()
	{
		m_indent++;
	}
	void indentDown()
	{
		if (m_indent > 0) m_indent--;
	}

	void iprintf(const char *format, ...);
	void iuprintf(const char *format, ...);
	void idprintf(const char *format, ...);
};

} // namespace librevenge

#endif // RVNGRAWGENERATORBASE_H

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
