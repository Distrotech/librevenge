/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2003 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2003 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 *
 * For further information visit http://librevenge.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#ifndef WP1PARSER_H
#define WP1PARSER_H

#include "RVNGParser.h"

class RVNGDocumentInterface;
class WP1Listener;

class WP1Parser : public RVNGParser
{
public:
	WP1Parser(RVNGInputStream *input, RVNGEncryption *encryption);
	~WP1Parser();

	void parse(RVNGDocumentInterface *documentInterface);
	void parseSubDocument(RVNGDocumentInterface *documentInterface);

	static void parseDocument(RVNGInputStream *input, RVNGEncryption *encryption, WP1Listener *listener);

private:
	void parse(RVNGInputStream *input, RVNGEncryption *encryption, WP1Listener *listener);
};

#endif /* WP1PARSER_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
