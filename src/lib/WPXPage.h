/* libwpd2
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
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

#ifndef WPXPAGE_H
#define WPXPAGE_H
class WPXPage
{
public:
	WPXPage();
	WPXPage(WPXPage &page, float paragraphMarginLeft=0.0f, float paragraphMarginRight=0.0f);
	
	const int getHeaderAPID() const { return m_headerAPID; }
	const int getHeaderBPID() const { return m_headerBPID; }
	const int getFooterAPID() const { return m_footerAPID; }
	const int getFooterBPID() const { return m_footerBPID; }
	const float getMarginLeft() const { return m_marginLeft; }
 	const float getMarginRight() const { return m_marginRight; }
 	const float getMarginTop() const { return m_marginTop; }
 	const float getMarginBottom() const { return m_marginBottom; }
	const int getPageSpan() const { return m_pageSpan; }

	void setHeaderAPID(const int headerAPID) { m_headerAPID = headerAPID; }
	void setHeaderBPID(const int headerBPID) { m_headerBPID = headerBPID; }
	void setFooterAPID(const int footerAPID) { m_footerAPID = footerAPID; }
	void setFooterBPID(const int footerBPID) { m_footerBPID = footerBPID; }
	void setMarginLeft(const float marginLeft) { m_marginLeft = marginLeft; }
 	void setMarginRight(const float marginRight) { m_marginRight = marginRight; }
 	void setMarginTop(const float marginTop) { m_marginTop = marginTop; }
 	void setMarginBottom(const float marginBottom) { m_marginBottom = marginBottom; }
	void setPageSpan(const int pageSpan) { m_pageSpan = pageSpan; }
	
private:
	int m_headerAPID, m_headerBPID;
	int m_footerAPID, m_footerBPID; 
	float m_marginLeft, m_marginRight;
	float m_marginTop, m_marginBottom;

	int m_pageSpan;
};

bool operator==(const WPXPage &, const WPXPage &);
#endif /* WPXPAGE_H */
