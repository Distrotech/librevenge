/* libwpd
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002 Marc Maurer (j.m.maurer@student.utwente.nl)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#ifndef WP6CHARACTERGROUP_H
#define WP6CHARACTERGROUP_H

#include "WP6VariableLengthGroup.h"

class WP6CharacterGroup_SetAlignmentCharacterSubGroup: public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_SetAlignmentCharacterSubGroup(WPXInputStream *input);
	virtual void parse(WP6HLListener *listener, const guint8 numPrefixIDs, guint16 const *prefixIDs) const;

private:
	guint8 m_character;
	guint8 m_characterSet;
};

class WP6CharacterGroup_ColorSubGroup: public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_ColorSubGroup(WPXInputStream *input);
	virtual void parse(WP6HLListener *listener, const guint8 numPrefixIDs, guint16 const *prefixIDs) const;

private:
	guint8 m_red;
	guint8 m_green;
	guint8 m_blue;
};

class WP6CharacterGroup_CharacterShadingChangeSubGroup: public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_CharacterShadingChangeSubGroup(WPXInputStream *input);
	virtual void parse(WP6HLListener *listener, const guint8 numPrefixIDs, guint16 const *prefixIDs) const;

private:
	guint8 m_shading;
};

class WP6CharacterGroup_FontFaceChangeSubGroup : public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_FontFaceChangeSubGroup(WPXInputStream *input);
	virtual void parse(WP6HLListener *listener, const guint8 numPrefixIDs, guint16 const *prefixIDs) const;

private:
	guint16 m_oldMatchedPointSize;
	guint16 m_hash;
	guint16 m_matchedFontIndex;
	guint16 m_matchedFontPointSize;
};

class WP6CharacterGroup_SetDotLeaderCharactersSubGroup: public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_SetDotLeaderCharactersSubGroup(WPXInputStream *input);
	virtual void parse(WP6HLListener *listener, const guint8 numPrefixIDs, guint16 const *prefixIDs) const;

private:
	guint8 m_character;
	guint8 m_characterSet;
	guint8 m_numberOfSpaces;
};

class WP6CharacterGroup_ParagraphNumberOnSubGroup : public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_ParagraphNumberOnSubGroup(WPXInputStream *input);
	virtual void parse(WP6HLListener *listener, const guint8 numPrefixIDs, guint16 const *prefixIDs) const;

private:
	guint16 m_outlineHash;
	guint8 m_level;
	guint8 m_flag;
};

class WP6CharacterGroup_TableDefinitionOnSubGroup : public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_TableDefinitionOnSubGroup(WPXInputStream *input);
	virtual void parse(WP6HLListener *listener, const guint8 numPrefixIDs, guint16 const *prefixIDs) const;

private:
	guint8 m_flags;
	guint8 m_position;
	guint16 m_leftOffset;
};

class WP6CharacterGroup_TableDefinitionOffSubGroup : public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_TableDefinitionOffSubGroup(WPXInputStream *input);
	virtual void parse(WP6HLListener *listener, const guint8 numPrefixIDs, guint16 const *prefixIDs) const;

private:

};

class WP6CharacterGroup_TableColumnSubGroup : public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_TableColumnSubGroup(WPXInputStream *input);
	virtual void parse(WP6HLListener *listener, const guint8 numPrefixIDs, guint16 const *prefixIDs) const;

private:
	guint8 m_flags;
	guint16 m_width;
	guint16 m_leftGutter;
	guint16 m_rigthGutter;
	guint16 m_attribWord1;
	guint16 m_attribWord2;
	guint8 m_alignment;
	guint16 m_absPosFromRight;
	guint16 m_numberType;
	guint8 m_currencyIndex;
};

class WP6CharacterGroup : public WP6VariableLengthGroup
{
 public:
	WP6CharacterGroup(WPXInputStream *input);	
	virtual ~WP6CharacterGroup();
	virtual void _readContents(WPXInputStream *input);
	virtual void parse(WP6HLListener *listener);

 private:
	WP6VariableLengthGroup_SubGroup *m_subGroupData;

};

#endif /* WP6CHARACTERGROUP_H */
