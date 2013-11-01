/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2005 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#ifndef RVNGSUBDOCUMENT_H
#define RVNGSUBDOCUMENT_H

#include "RVNGMemoryStream.h"
#include "librevenge_types.h"

class RVNGListener;
class RVNGEncryption;

class RVNGSubDocument
{
public:
	RVNGSubDocument(RVNGInputStream *input, RVNGEncryption *encryption, const unsigned dataSize);
	RVNGSubDocument(uint8_t *streamData, const unsigned dataSize);
	virtual ~RVNGSubDocument();
	RVNGMemoryInputStream *getStream() const
	{
		return m_stream;
	}

private:
	RVNGMemoryInputStream *m_stream;
	uint8_t *m_streamData;
	RVNGSubDocument();
	RVNGSubDocument(const RVNGSubDocument &);
	RVNGSubDocument &operator=(const RVNGSubDocument &);

};
#endif /* RVNGSUBDOCUMENT_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
