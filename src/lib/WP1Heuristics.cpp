/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
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
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#include "WP1Heuristics.h"
#include "WP1FileStructure.h"
#include "libwpd_internal.h"
#include <limits>

WPDPasswordMatch WP1Heuristics::verifyPassword(WPXInputStream *input, const char *password)
{
	if (!password)
		return WPD_PASSWORD_MATCH_DONTKNOW;

	input->seek(0, WPX_SEEK_SET);
	WPXEncryption *encryption = 0;
	try
	{
		if (readU8(input, 0) == 0xFE && readU8(input, 0) == 0xFF &&
		        readU8(input, 0) == 0x61 && readU8(input, 0) == 0x61)
		{
			encryption = new WPXEncryption(password, 6);
			if (readU16(input, 0, true) == encryption->getCheckSum())
			{
				delete encryption;
				return WPD_PASSWORD_MATCH_OK;
			}
			else
			{
				delete encryption;
				return WPD_PASSWORD_MATCH_NONE;
			}
		}
		else
			return WPD_PASSWORD_MATCH_DONTKNOW;
	}
	catch (...)
	{
		if (encryption)
			delete encryption;
		return WPD_PASSWORD_MATCH_DONTKNOW;
	}
}

WPDConfidence WP1Heuristics::isWP1FileFormat(WPXInputStream *input, const char *password)
{
	input->seek(0, WPX_SEEK_SET);
	WPXEncryption *encryption = 0;

	try
	{
		if (readU8(input, 0) == 0xFE && readU8(input, 0) == 0xFF &&
		        readU8(input, 0) == 0x61 && readU8(input, 0) == 0x61)
		{
			if (password)
			{
				encryption = new WPXEncryption(password, 6);
				if (readU16(input, 0, true) != encryption->getCheckSum())
				{
					delete encryption;
					return WPD_CONFIDENCE_SUPPORTED_ENCRYPTION;
				}
			}
			else
			{
				if (readU16(input,0) != 0x0000)
					return WPD_CONFIDENCE_SUPPORTED_ENCRYPTION;
			}
		}

		input->seek(0, WPX_SEEK_SET);
		if (password && encryption)
			input->seek(6, WPX_SEEK_SET);

		int functionGroupCount = 0;

		WPD_DEBUG_MSG(("WP1Heuristics::isWP1FileFormat()\n"));

		while (!input->atEOS())
		{
			uint8_t readVal = readU8(input, encryption);

			WPD_DEBUG_MSG(("WP1Heuristics, Offset 0x%.8x, value 0x%.2x (%c)\n", (unsigned int)input->tell() - 1, readVal, readVal));

			if (readVal < (uint8_t)0x20)
			{
				// line breaks et al, skip
			}
			else if (readVal >= (uint8_t)0x20 && readVal <= (uint8_t)0x7F)
			{
				// normal ASCII characters, skip
			}
			else if (readVal >= (uint8_t)0x80 && readVal <= (uint8_t)0xBF)
			{
				// single character function codes, skip
				functionGroupCount++;
			}
			else if (readVal >= (uint8_t)0xFF)
			{
				if (encryption)
					delete encryption;
				// special codes that should not be found as separate functions
				return WPD_CONFIDENCE_NONE;
			}
			else
			{
				// multi character function group
				// check that the size constrains are valid, and that every group_member
				// is properly closed at the right place

				if (WP1_FUNCTION_GROUP_SIZE[readVal-0xC0] == -1)
				{
					// variable length function group

					// We are checking following structure:
					//   <function code>{function length}...{function length}<function code>
					//   that we observed in variable length WP1 functions

					unsigned functionLength = readU32(input, encryption, true);
					if (functionLength > ((std::numeric_limits<uint32_t>::max)() / 2))
					{
						if (encryption)
							delete encryption;
						return WPD_CONFIDENCE_NONE;
					}
					if (functionLength == 0)
					{
						if (encryption)
							delete encryption;
						return WPD_CONFIDENCE_NONE;
					}
					WPD_DEBUG_MSG(("WP1Heuristics functionLength = 0x%.8x\n", (unsigned int)functionLength));

					input->seek(functionLength, WPX_SEEK_CUR);
					unsigned long closingFunctionLength = readU32(input, encryption, true);
					WPD_DEBUG_MSG(("WP1Heuristics closingFunctionLength = 0x%.8x\n", (unsigned int)closingFunctionLength));
					if (functionLength != closingFunctionLength)
					{
						if (encryption)
							delete encryption;
						return WPD_CONFIDENCE_NONE;
					}

					uint8_t closingGate = 0;
					if (!input->atEOS())
					{
						closingGate = readU8(input, encryption);
						WPD_DEBUG_MSG(("WP1Heuristics closingGate = 0x%.2x\n", closingGate));
						if (closingGate != readVal)
						{
							if (encryption)
								delete encryption;
							return WPD_CONFIDENCE_NONE;
						}
					}

					// when passed the complete file, we don't allow for open groups when we've reached EOF
					if (input->atEOS() && (closingGate != readVal))
					{
						if (encryption)
							delete encryption;
						return WPD_CONFIDENCE_NONE;
					}

					functionGroupCount++;
				}
				else
				{
					// fixed length function group

					// seek to the position where the closing gate should be
					int res = input->seek(WP1_FUNCTION_GROUP_SIZE[readVal-0xC0]-2, WPX_SEEK_CUR);
					// when passed the complete file, we should be able to do that
					if (res)
					{
						if (encryption)
							delete encryption;
						return WPD_CONFIDENCE_NONE;
					}

					// read the closing gate
					uint8_t readNextVal = readU8(input, encryption);
					if (readNextVal != readVal)
					{
						if (encryption)
							delete encryption;
						return WPD_CONFIDENCE_NONE;
					}

					functionGroupCount++;
				}
			}
		}

		/* When we get here, the document is in a format that we *could* import properly.
		However, if we didn't encounter a single WP1 function group) we need to be more carefull:
		this would be the case when passed a plaintext file for example, which libwpd is not
		supposed to handle. */
		if (!functionGroupCount)
		{
			if (encryption)
			{
				delete encryption;
				return WPD_CONFIDENCE_EXCELLENT;
			}
			return WPD_CONFIDENCE_NONE;
		}
		if (encryption)
			delete encryption;
		return WPD_CONFIDENCE_EXCELLENT;
	}
	catch (...)
	{
		if (encryption)
			delete encryption;
		return WPD_CONFIDENCE_NONE;
	}
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
