/* libwpd
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2005-2006 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#ifndef WPXLISTENER_H
#define WPXLISTENER_H

#include "libwpd_internal.h"
#include "WPXString.h"
#include <vector>

typedef struct _WPXDocumentMetaData WPXDocumentMetaData;
struct _WPXDocumentMetaData
{
	WPXString m_author;
	WPXString m_subject;
	WPXString m_publisher;
	WPXString m_category;
	WPXString m_keywords;
	WPXString m_language;
	WPXString m_abstract;
	WPXString m_descriptiveName;
	WPXString m_descriptiveType;
};

class WPXListener
{
public:
	WPXListener();
	virtual ~WPXListener();

protected:
	bool isUndoOn() { return m_isUndoOn; }
	void setUndoOn(bool isUndoOn) { m_isUndoOn = isUndoOn; }
	
private:
	bool m_isUndoOn;
};

#endif /* WPXLISTENER_H */
