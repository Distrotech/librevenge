/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
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

#ifndef WP1FOOTNOTEENDNOTEGROUP_H
#define WP1FOOTNOTEENDNOTEGROUP_H

#include "WP1VariableLengthGroup.h"
#include "librevenge_internal.h"

class WP1SubDocument;

class WP1FootnoteEndnoteGroup : public WP1VariableLengthGroup
{
public:
	WP1FootnoteEndnoteGroup(RVNGInputStream *input, RVNGEncryption *encryption, uint8_t group);
	~WP1FootnoteEndnoteGroup();
	void _readContents(RVNGInputStream *input, RVNGEncryption *encryption);
	void parse(WP1Listener *listener);

private:
	WP1FootnoteEndnoteGroup(const WP1FootnoteEndnoteGroup &);
	WP1FootnoteEndnoteGroup &operator=(const WP1FootnoteEndnoteGroup &);
	RVNGNoteType m_noteType;
	uint16_t m_noteNumber;
	WP1SubDocument *m_subDocument;
};

#endif /* WP1FOOTNOTEENDNOTEGROUP_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
