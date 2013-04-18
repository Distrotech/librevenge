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

#ifndef WPXPAGE_H
#define WPXPAGE_H
#include "WPXFileStructure.h"
#include <vector>
#include "WPXTable.h"
#include "libwpd_internal.h"
#include "WPXSubDocument.h"

// intermediate page representation class: for internal use only (by the high-level content/styles listeners). should not be exported.

class WPXHeaderFooter
{
public:
	WPXHeaderFooter(const WPXHeaderFooterType headerFooterType, const WPXHeaderFooterOccurence occurence,
	                const uint8_t internalType, const WPXSubDocument *subDocument, WPXTableList tableList);
	WPXHeaderFooter(const WPXHeaderFooterType headerFooterType, const WPXHeaderFooterOccurence occurence,
	                const uint8_t internalType, const WPXSubDocument *subDocument);
	WPXHeaderFooter(const WPXHeaderFooter &headerFooter);
	~WPXHeaderFooter();
	WPXHeaderFooter &operator=(const WPXHeaderFooter &headerFooter);
	WPXHeaderFooterType getType() const
	{
		return m_type;
	}
	WPXHeaderFooterOccurence getOccurence() const
	{
		return m_occurence;
	}
	uint8_t getInternalType() const
	{
		return m_internalType;
	}
	const WPXSubDocument *getSubDocument() const
	{
		return m_subDocument;
	}
	WPXTableList getTableList() const
	{
		return m_tableList;
	}

private:
	WPXHeaderFooterType m_type;
	WPXHeaderFooterOccurence m_occurence;
	uint8_t m_internalType; // for suppression
	const WPXSubDocument *m_subDocument;  // for the actual text
	WPXTableList m_tableList;
};

class WPXPageSpan
{
public:
	WPXPageSpan();
	WPXPageSpan(const WPXPageSpan &page, double paragraphMarginLeft, double paragraphMarginRight);
	virtual ~WPXPageSpan();

	bool getPageNumberSuppression() const
	{
		return m_isPageNumberSuppressed;
	}
	bool getHeaderFooterSuppression(const uint8_t headerFooterType) const
	{
		if (headerFooterType <= WPX_FOOTER_B) return m_isHeaderFooterSuppressed[headerFooterType];
		return false;
	}
	double getFormLength() const
	{
		return m_formLength;
	}
	double getFormWidth() const
	{
		return m_formWidth;
	}
	WPXFormOrientation getFormOrientation() const
	{
		return m_formOrientation;
	}
	double getMarginLeft() const
	{
		return m_marginLeft;
	}
	double getMarginRight() const
	{
		return m_marginRight;
	}
	double getMarginTop() const
	{
		return m_marginTop;
	}
	double getMarginBottom() const
	{
		return m_marginBottom;
	}
	WPXPageNumberPosition getPageNumberPosition() const
	{
		return m_pageNumberPosition;
	}
	bool getPageNumberOverriden() const
	{
		return m_isPageNumberOverridden;
	}
	int getPageNumberOverride() const
	{
		return m_pageNumberOverride;
	}
	WPXNumberingType getPageNumberingType() const
	{
		return m_pageNumberingType;
	}
	double getPageNumberingFontSize() const
	{
		return m_pageNumberingFontSize;
	}
	WPXString getPageNumberingFontName() const
	{
		return m_pageNumberingFontName;
	}
	int getPageSpan() const
	{
		return m_pageSpan;
	}
	const std::vector<WPXHeaderFooter> &getHeaderFooterList() const
	{
		return m_headerFooterList;
	}

	void setHeaderFooter(const WPXHeaderFooterType type, const uint8_t headerFooterType, const WPXHeaderFooterOccurence occurence,
	                     const WPXSubDocument *subDocument, WPXTableList tableList);
	void setPageNumberSuppression(const bool suppress)
	{
		m_isPageNumberSuppressed = suppress;
	}
	void setHeadFooterSuppression(const uint8_t headerFooterType, const bool suppress)
	{
		m_isHeaderFooterSuppressed[headerFooterType] = suppress;
	}
	void setFormLength(const double formLength)
	{
		m_formLength = formLength;
	}
	void setFormWidth(const double formWidth)
	{
		m_formWidth = formWidth;
	}
	void setFormOrientation(const WPXFormOrientation formOrientation)
	{
		m_formOrientation = formOrientation;
	}
	void setMarginLeft(const double marginLeft)
	{
		m_marginLeft = marginLeft;
	}
	void setMarginRight(const double marginRight)
	{
		m_marginRight = marginRight;
	}
	void setMarginTop(const double marginTop)
	{
		m_marginTop = marginTop;
	}
	void setMarginBottom(const double marginBottom)
	{
		m_marginBottom = marginBottom;
	}
	void setPageNumberPosition(const WPXPageNumberPosition pageNumberPosition)
	{
		m_pageNumberPosition = pageNumberPosition;
	}
	void setPageNumber(const int pageNumberOverride)
	{
		m_pageNumberOverride = pageNumberOverride;
		m_isPageNumberOverridden = true;
	}
	void setPageNumberingType(const WPXNumberingType pageNumberingType)
	{
		m_pageNumberingType = pageNumberingType;
	}
	void setPageNumberingFontSize(const double pageNumberingFontSize)
	{
		m_pageNumberingFontSize = pageNumberingFontSize;
	}
	void setPageNumberingFontName(const WPXString &pageNumberingFontName)
	{
		m_pageNumberingFontName = pageNumberingFontName;
	}
	void setPageSpan(const int pageSpan)
	{
		m_pageSpan = pageSpan;
	}

protected:
	void _removeHeaderFooter(WPXHeaderFooterType type, WPXHeaderFooterOccurence occurence);
	bool _containsHeaderFooter(WPXHeaderFooterType type, WPXHeaderFooterOccurence occurence);

private:
	bool m_isHeaderFooterSuppressed[WPX_NUM_HEADER_FOOTER_TYPES];
	bool m_isPageNumberSuppressed;
	double m_formLength, m_formWidth;
	WPXFormOrientation m_formOrientation;
	double m_marginLeft, m_marginRight;
	double m_marginTop, m_marginBottom;
	WPXPageNumberPosition m_pageNumberPosition;
	bool m_isPageNumberOverridden;
	int m_pageNumberOverride;
	WPXNumberingType m_pageNumberingType;
	WPXString m_pageNumberingFontName;
	double m_pageNumberingFontSize;
	std::vector<WPXHeaderFooter> m_headerFooterList;

	int m_pageSpan;
};

bool operator==(const WPXPageSpan &, const WPXPageSpan &);
#endif /* WPXPAGE_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
