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
 *
 * For further information visit http://librevenge.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#ifndef WP6PARSER_H
#define WP6PARSER_H

#include "RVNGParser.h"
#include "WP6Header.h"

class WP6PrefixData;
class WP6Listener;
class RVNGDocumentInterface;

class WP6Parser : public RVNGParser
{
public:
	WP6Parser(RVNGInputStream *input, RVNGHeader *header, RVNGEncryption *encryption);
	~WP6Parser();

	void parse(RVNGDocumentInterface *documentInterface);
	void parseSubDocument(RVNGDocumentInterface *documentInterface);

	static void parseDocument(RVNGInputStream *input, RVNGEncryption *encryption, WP6Listener *listener);

private:
	WP6PrefixData *getPrefixData(RVNGInputStream *input, RVNGEncryption *encryption);

	void parse(RVNGInputStream *input, RVNGEncryption *encryption, WP6Listener *listener);

	void parsePacket(WP6PrefixData *prefixData, int type, WP6Listener *listener);
	void parsePackets(WP6PrefixData *prefixData, int type, WP6Listener *listener);
};

#endif /* WP6PARSER_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
