/* libwpd2
 * Copyright (C) 2002 William Lachance (wlach@interlog.com)
 * Copyright (C) 2002 Marc Maurer (j.m.maurer@student.utwente.nl)
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 * 
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by 
 * Corel Corporation or Corel Corporation Limited."
 */
 
#ifndef WP6HLLISTENER_H
#define WP6HLLISTENER_H
 
#include "WP6LLListener.h"

class WPXHLListenerImpl;

class WP6HLListener : public WP6LLListener
{
 public:
	WP6HLListener(WPXHLListenerImpl *listenerImpl);
	~WP6HLListener();

	// for getting low-level messages from the parser
	virtual void startDocument();
	virtual void insertCharacter(guint16 character);
	virtual void insertEOL();
	virtual void endDocument();

 protected:
	void _flushText();

 private:
	WPXHLListenerImpl * m_listenerImpl;
	GArray *m_textArray;
	gboolean m_isParagraphOpened;
	gboolean m_isParagraphClosed;
};

#endif /* WP6HLLISTENER_H */
