/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
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
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#include "WP5PrefixData.h"
#include "WP5GeneralPacketIndex.h"
#include "WP5SpecialHeaderIndex.h"
#include "libwpd_internal.h"

WP5PrefixData::WP5PrefixData(WPXInputStream *input, WPXEncryption *encryption) :
	m_generalPacketData()
{
	std::vector<WP5GeneralPacketIndex> prefixIndexVector;
	int id = 0;
	while (true)
	{
		WP5SpecialHeaderIndex shi = WP5SpecialHeaderIndex(input, encryption);

		if ((shi.getType() != 0xfffb) || (shi.getNumOfIndexes() != 5) || (shi.getIndexBlockSize() != 50))
		{
			WPD_DEBUG_MSG(("WordPerfect: detected possible prefix data corruption, ignoring this and all following packets.\n"));
			break;
		}

		bool tmpFoundPossibleCorruption = false;
		for (int i=0; i<(shi.getNumOfIndexes()-1); i++)
		{
			WP5GeneralPacketIndex gpi = WP5GeneralPacketIndex(input, encryption, id);
			if ((gpi.getType() > 0x02ff) && (gpi.getType() < 0xfffb))
			{
				WPD_DEBUG_MSG(("WordPerfect: detected possible prefix data corruption, ignoring this and all following packets.\n"));
				tmpFoundPossibleCorruption = true;
				break;
			}

			if ((gpi.getType() != 0) && (gpi.getType() != 0xffff))
			{
				WPD_DEBUG_MSG(("WordPerfect: constructing general packet index 0x%x\n", id));
				prefixIndexVector.push_back(gpi);
				id++;
			}
		}
		if (tmpFoundPossibleCorruption)
			break;

		if (shi.getNextBlockOffset() != 0)
			input->seek(shi.getNextBlockOffset(), WPX_SEEK_SET);
		else
			break;
	}

	std::vector<WP5GeneralPacketIndex>::iterator gpiIter;
	for (gpiIter = prefixIndexVector.begin(); gpiIter != prefixIndexVector.end(); ++gpiIter)
	{
		WPD_DEBUG_MSG(("WordPerfect: constructing general packet data %i\n", (*gpiIter).getID()));
		WP5GeneralPacketData *generalPacketData = WP5GeneralPacketData::constructGeneralPacketData(input, encryption, &(*gpiIter));
		if (generalPacketData)
		{
			m_generalPacketData[gpiIter->getType()] = generalPacketData;
		}
	}
}

WP5PrefixData::~WP5PrefixData()
{
	std::map<int, WP5GeneralPacketData *>::const_iterator Iter;
	for (Iter = m_generalPacketData.begin(); Iter != m_generalPacketData.end(); ++Iter)
		delete (Iter->second);
}

const WP5GeneralPacketData *WP5PrefixData::getGeneralPacketData(const int type) const
{
	std::map<int, WP5GeneralPacketData *>::const_iterator Iter = m_generalPacketData.find(type);
	if (Iter != m_generalPacketData.end())
		return static_cast<const WP5GeneralPacketData *>(Iter->second);
	else
		return 0;
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
