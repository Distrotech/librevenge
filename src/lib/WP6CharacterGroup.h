/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
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
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#ifndef WP6CHARACTERGROUP_H
#define WP6CHARACTERGROUP_H

#include "WP6VariableLengthGroup.h"
#include "WP6FontDescriptorPacket.h"

class WP6CharacterGroup_SetAlignmentCharacterSubGroup: public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_SetAlignmentCharacterSubGroup(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener, const uint8_t numPrefixIDs, uint16_t const *prefixIDs) const;

private:
	uint8_t m_character;
	uint8_t m_characterSet;
};

class WP6CharacterGroup_ColorSubGroup: public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_ColorSubGroup(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener, const uint8_t numPrefixIDs, uint16_t const *prefixIDs) const;

private:
	uint8_t m_red;
	uint8_t m_green;
	uint8_t m_blue;
};

class WP6CharacterGroup_CharacterShadingChangeSubGroup: public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_CharacterShadingChangeSubGroup(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener, const uint8_t numPrefixIDs, uint16_t const *prefixIDs) const;

private:
	uint8_t m_shading;
};

class WP6CharacterGroup_FontFaceChangeSubGroup : public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_FontFaceChangeSubGroup(WPXInputStream *input, WPXEncryption *encryption, uint16_t sizeDeletable);
	~WP6CharacterGroup_FontFaceChangeSubGroup();
	void parse(WP6Listener *listener, const uint8_t numPrefixIDs, uint16_t const *prefixIDs) const;

private:
	uint16_t m_oldMatchedPointSize;
	uint16_t m_hash;
	uint16_t m_matchedFontIndex;
	uint16_t m_matchedFontPointSize;
	WP6FontDescriptorPacket *m_packet;
	// Unimplemented to prevent compiler from creating crasher ones
	WP6CharacterGroup_FontFaceChangeSubGroup(const WP6CharacterGroup_FontFaceChangeSubGroup &);
	WP6CharacterGroup_FontFaceChangeSubGroup &operator=(const WP6CharacterGroup_FontFaceChangeSubGroup &);

};

class WP6CharacterGroup_FontSizeChangeSubGroup : public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_FontSizeChangeSubGroup(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener, const uint8_t numPrefixIDs, uint16_t const *prefixIDs) const;

private:
	uint16_t m_desiredFontPointSize;
};

class WP6CharacterGroup_SetDotLeaderCharactersSubGroup: public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_SetDotLeaderCharactersSubGroup(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener, const uint8_t numPrefixIDs, uint16_t const *prefixIDs) const;

private:
	uint8_t m_character;
	uint8_t m_characterSet;
	uint8_t m_numberOfSpaces;
};

class WP6CharacterGroup_ParagraphNumberOnSubGroup : public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_ParagraphNumberOnSubGroup(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener, const uint8_t numPrefixIDs, uint16_t const *prefixIDs) const;

private:
	uint16_t m_outlineHash;
	uint8_t m_level;
	uint8_t m_flag;
};

class WP6CharacterGroup_TableDefinitionOnSubGroup : public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_TableDefinitionOnSubGroup(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener, const uint8_t numPrefixIDs, uint16_t const *prefixIDs) const;

private:
	uint8_t m_flags;
	uint8_t m_position;
	uint16_t m_leftOffset;
};

class WP6CharacterGroup_TableDefinitionOffSubGroup : public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_TableDefinitionOffSubGroup(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener, const uint8_t numPrefixIDs, uint16_t const *prefixIDs) const;

private:

};

class WP6CharacterGroup_TableColumnSubGroup : public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_TableColumnSubGroup(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener, const uint8_t numPrefixIDs, uint16_t const *prefixIDs) const;

private:
	uint8_t m_flags;
	uint16_t m_width;
	uint16_t m_leftGutter;
	uint16_t m_rightGutter;
	uint32_t m_attributes;
	uint8_t m_alignment;
	uint16_t m_absPosFromRight;
	uint16_t m_numberType;
	uint8_t m_currencyIndex;
};

class WP6CharacterGroup_CommentSubGroup : public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_CommentSubGroup(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener, const uint8_t numPrefixIDs, uint16_t const *prefixIDs) const;

private:

};

class WP6CharacterGroup : public WP6VariableLengthGroup
{
public:
	WP6CharacterGroup(WPXInputStream *input, WPXEncryption *encryption);
	~WP6CharacterGroup();
	void _readContents(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener);

private:
	WP6CharacterGroup(const WP6CharacterGroup &);
	WP6CharacterGroup &operator=(const WP6CharacterGroup &);
	WP6VariableLengthGroup_SubGroup *m_subGroupData;

};

#endif /* WP6CHARACTERGROUP_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
