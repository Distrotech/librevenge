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

#include <stdarg.h>
#include <stdio.h>
#include <map>
#include <string>

#include "RVNGRawGeneratorBase.h"

namespace librevenge
{

RVNGRawGeneratorBase::RVNGRawGeneratorBase(bool printCallgraphScore)
	: m_indent(0)
	, m_callbackMisses(0)
	, m_atLeastOneCallback(false)
	, m_printCallgraphScore(printCallgraphScore)
	, m_callStack()
{
}

RVNGRawGeneratorBase::~RVNGRawGeneratorBase()
{
}

void RVNGRawGeneratorBase::iprintf(const char *format, ...)
{
	m_atLeastOneCallback = true;
	if (m_printCallgraphScore) return;

	va_list args;
	va_start(args, format);
	for (int i=0; i<m_indent; i++)
		printf("  ");
	vprintf(format, args);
	va_end(args);
}

void RVNGRawGeneratorBase::iuprintf(const char *format, ...)
{
	m_atLeastOneCallback = true;
	va_list args;
	va_start(args, format);
	for (int i=0; i<m_indent; i++)
		printf("  ");
	vprintf(format, args);
	indentUp();
	va_end(args);
}

void RVNGRawGeneratorBase::idprintf(const char *format, ...)
{
	m_atLeastOneCallback = true;
	va_list args;
	va_start(args, format);
	indentDown();
	for (int i=0; i<m_indent; i++)
		printf("  ");
	vprintf(format, args);
	va_end(args);
}

} // namespace librevenge

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
