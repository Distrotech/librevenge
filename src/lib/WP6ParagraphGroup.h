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

#ifndef WP6PARAGRAPHGROUP_H
#define WP6PARAGRAPHGROUP_H

#include "WP6VariableLengthGroup.h"
#include "WP6FileStructure.h"
#include <vector>

class WP6ParagraphGroup_LineSpacingSubGroup : public WP6VariableLengthGroup_SubGroup
{
public:
	WP6ParagraphGroup_LineSpacingSubGroup(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener, const uint8_t numPrefixIDs, uint16_t const *prefixIDs) const;

private:
	double m_lineSpacing;
};

class WP6ParagraphGroup_JustificationModeSubGroup : public WP6VariableLengthGroup_SubGroup
{
public:
	WP6ParagraphGroup_JustificationModeSubGroup(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener, const uint8_t numPrefixIDs, uint16_t const *prefixIDs) const;

private:
	uint8_t m_justification;
};

class WP6ParagraphGroup_SpacingAfterParagraphSubGroup : public WP6VariableLengthGroup_SubGroup
{
public:
	WP6ParagraphGroup_SpacingAfterParagraphSubGroup(WPXInputStream *input, WPXEncryption *encryption, const uint16_t sizeNonDeletable);
	void parse(WP6Listener *listener, const uint8_t numPrefixIDs, uint16_t const *prefixIDs) const;

private:
	double m_spacingAfterParagraphAbsolute;
	double m_spacingAfterParagraphRelative;
	uint16_t m_sizeNonDeletable;
};

class WP6ParagraphGroup_TabSetSubGroup : public WP6VariableLengthGroup_SubGroup
{
public:
	WP6ParagraphGroup_TabSetSubGroup(WPXInputStream *input, WPXEncryption *encryption);
	~WP6ParagraphGroup_TabSetSubGroup();
	void parse(WP6Listener *listener, const uint8_t numPrefixIDs, uint16_t const *prefixIDs) const;

private:
	bool m_isRelative;
	double m_tabAdjustValue;
	std::vector<bool> m_usePreWP9LeaderMethods;
	std::vector<WPXTabStop> m_tabStops;
};

class WP6ParagraphGroup_IndentFirstLineSubGroup : public WP6VariableLengthGroup_SubGroup
{
public:
	WP6ParagraphGroup_IndentFirstLineSubGroup(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener, const uint8_t numPrefixIDs, uint16_t const *prefixIDs) const;

private:
	int16_t m_firstLineOffset;
};

class WP6ParagraphGroup_LeftMarginAdjustmentSubGroup : public WP6VariableLengthGroup_SubGroup
{
public:
	WP6ParagraphGroup_LeftMarginAdjustmentSubGroup(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener, const uint8_t numPrefixIDs, uint16_t const *prefixIDs) const;

private:
	int16_t m_leftMargin;
};

class WP6ParagraphGroup_RightMarginAdjustmentSubGroup : public WP6VariableLengthGroup_SubGroup
{
public:
	WP6ParagraphGroup_RightMarginAdjustmentSubGroup(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener, const uint8_t numPrefixIDs, uint16_t const *prefixIDs) const;

private:
	int16_t m_rightMargin;
};

class WP6ParagraphGroup_OutlineDefineSubGroup : public WP6VariableLengthGroup_SubGroup
{
public:
	WP6ParagraphGroup_OutlineDefineSubGroup(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener, const uint8_t numPrefixIDs, uint16_t const *prefixIDs) const;

private:
	uint16_t m_outlineHash;
	uint8_t m_numberingMethods[WP6_NUM_LIST_LEVELS];
	uint8_t m_tabBehaviourFlag;
};

class WP6ParagraphGroup : public WP6VariableLengthGroup
{
public:
	WP6ParagraphGroup(WPXInputStream *input, WPXEncryption *encryption);
	~WP6ParagraphGroup();
	void _readContents(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener);
private:
	WP6ParagraphGroup(const WP6ParagraphGroup &);
	WP6ParagraphGroup &operator=(const WP6ParagraphGroup &);
	WP6VariableLengthGroup_SubGroup *m_subGroupData;
};

#endif /* WP6PARAGRAPHGROUP_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
