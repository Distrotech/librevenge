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
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 */

#ifndef INCLUDED_LIBREVENGE_LIBREVENGE_API_H
#define INCLUDED_LIBREVENGE_LIBREVENGE_API_H

#ifdef DLL_EXPORT
#ifdef LIBREVENGE_BUILD
#define REVENGE_API __declspec(dllexport)
#else
#define REVENGE_API __declspec(dllimport)
#endif
#else // !DLL_EXPORT
#ifdef LIBREVENGE_VISIBILITY
#define REVENGE_API __attribute__((visibility("default")))
#else
#define REVENGE_API
#endif
#endif

#ifdef __GNUC__
#define REVENGE_ATTRIBUTE_PRINTF(fmt, arg) __attribute__((format(printf, fmt, arg)))
#else
#define REVENGE_ATTRIBUTE_PRINTF(fmt, arg)
#endif

#endif /* INCLUDED_LIBREVENGE_LIBREVENGE_API_H */

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
