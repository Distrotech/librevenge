/* libwpd2
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

#ifndef WP6PARAGRAPHGROUP_H
#define WP6PARAGRAPHGROUP_H

#include "WP6VariableLengthGroup.h"

class WP6ParagraphGroup_OutlineDefineSubGroup : public WP6VariableLengthGroup_SubGroup
{
public:
	WP6ParagraphGroup_OutlineDefineSubGroup(GsfInput *input);
	virtual void parse(WP6LLListener *llListener, const guint8 numPrefixIDs, guint16 const *prefixIDs) const;

private:
	guint16 m_outlineHash;
	guint8 m_numberingMethods[WP6_NUM_LIST_LEVELS];
	guint8 m_outlineFlags;
	guint8 m_tabBehaviourFlag;
};

class WP6ParagraphGroup : public WP6VariableLengthGroup
{
 public:
	WP6ParagraphGroup(GsfInput *input);
	~WP6ParagraphGroup();
	virtual void _readContents(GsfInput *input);
	virtual void parse(WP6LLListener *llListener);
	const guint8 getJustification() const { return m_justification; }
	
 private:
	WP6VariableLengthGroup_SubGroup *m_subGroupData;
	guint8 m_justification; // we can't JUSTIFY creating another class to hold this piece of data
};

#endif /* WP6PARAGRAPHGROUP_H */
