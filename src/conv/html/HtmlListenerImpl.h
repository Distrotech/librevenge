 /* libwpd2
 * Copyright (C) 2002 William Lachance (wlach@interlog.com)
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

#ifndef HTMLLISTENERIMPL_H
#define HTMLLISTENERIMPL_H

#include <glib.h>
#include "WPXHLListenerImpl.h"
#include "WPXHLListener.h"

class HtmlListenerImpl : public WPXHLListenerImpl
{
 public:
    HtmlListenerImpl();
    virtual ~HtmlListenerImpl();
    
    virtual void startDocument();
    virtual void endDocument();
    virtual void openParagraph(guint32 textAttributeBits);
    virtual void openSection(guint numColumns, gfloat marginLeft, gfloat marginRight);
    virtual void openSpan(guint32 textAttributeBits);
    virtual void insertText(const guint16 *textArray, const guint len);

 protected:
    void _appendTextAttributes(guint32 textAttributeBits);
    
 private:
    gboolean m_isSectionOpened;
    gboolean m_isParagraphOpened;
    gboolean m_isSpanOpened;
};

#endif /* HTMLLISTENERIMPL_H */
