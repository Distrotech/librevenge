/* libwpd
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#ifndef WPXSTYLESLISTENER_H
#define WPXSTYLESLISTENER_H

#include "libwpd_internal.h"
#include "WPXPageSpan.h"
#include "WPXListener.h"
#include "WPXPageSpan.h"
#include <vector>
#include <list>

class WPXStylesListener : public WPXListener
{
public:
	WPXStylesListener(std::list<WPXPageSpan> &pageList, WPXHLListenerImpl *listenerImpl);
	virtual ~WPXStylesListener();

	WPXHLListenerImpl * m_listenerImpl;
	std::list <WPXPageSpan> &m_pageList;

protected:
	bool isUndoOn() { return m_isUndoOn; }
	void setUndoOn(bool isUndoOn) { m_isUndoOn = isUndoOn; }
		

};

#endif /* WPXSTYLESLISTENER_H */
