/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 */

#include <locale.h>

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

#include <librevenge-generators/librevenge-generators.h>

#include "librevenge_internal.h"

namespace librevenge
{

namespace PresentationSVG
{

static double getInchValue(librevenge::RVNGProperty const &prop)
{
	double value=prop.getDouble();
	switch (prop.getUnit())
	{
	case librevenge::RVNG_GENERIC: // assume inch
	case librevenge::RVNG_INCH:
		return value;
	case librevenge::RVNG_POINT:
		value /= 72.;
		return value;
	case librevenge::RVNG_TWIP:
		value /= 1440.;
		return value;
	case librevenge::RVNG_PERCENT:
	case librevenge::RVNG_UNIT_ERROR:
	default:
	{
		static bool first=true;
		if (first)
		{
			RVNG_DEBUG_MSG(("librevenge::getInchValue: call with no double value\n"));
			first=false;
		}
		break;
	}
	}
	return value;
}

static std::string doubleToString(const double value)
{
	RVNGProperty *prop = RVNGPropertyFactory::newDoubleProp(value);
	std::string retVal = prop->getStr().cstr();
	delete prop;
	return retVal;
}

static unsigned stringToColour(const RVNGString &s)
{
	std::string str(s.cstr());
	if (str[0] == '#')
	{
		if (str.length() != 7)
			return 0;
		else
			str.erase(str.begin());
	}
	else
		return 0;

	std::istringstream istr(str);
	unsigned val = 0;
	istr >> std::hex >> val;
	return val;
}

//! basic class used to store table information
struct Table
{
	//! constructor
	Table(const RVNGPropertyList &propList) : m_column(0), m_row(0), m_x(0), m_y(0), m_columnsDistanceFromOrigin(), m_rowsDistanceFromOrigin()
	{
		if (propList["svg:x"])
			m_x=getInchValue(*propList["svg:x"]);
		if (propList["svg:y"])
			m_y=getInchValue(*propList["svg:y"]);
		// we do not actually use height/width, so...

		m_columnsDistanceFromOrigin.push_back(0);
		m_rowsDistanceFromOrigin.push_back(0);

		const librevenge::RVNGPropertyListVector *columns = propList.child("librevenge:table-columns");
		if (columns)
		{
			double actDist=0;
			for (unsigned long i=0; i<columns->count(); ++i)
			{
				if ((*columns)[i]["style:column-width"])
					actDist+=getInchValue(*(*columns)[i]["style:column-width"]);
				m_columnsDistanceFromOrigin.push_back(actDist);
			}
		}
		else
		{
			RVNG_DEBUG_MSG(("librevenge::DrawingSVG::Table::Table: can not find any columns\n"));
		}
	}
	//! calls to open a row
	void openRow(const RVNGPropertyList &propList)
	{
		double height=0;
		if (propList["style:row-height"])
			height=getInchValue(*propList["style:row-height"]);
		// changeme
		else if (propList["style:min-row-height"])
			height=getInchValue(*propList["style:min-row-height"]);
		else
		{
			RVNG_DEBUG_MSG(("librevenge::DrawingSVG::Table::openRow: can not determine row height\n"));
		}
		m_rowsDistanceFromOrigin.push_back(m_rowsDistanceFromOrigin.back()+height);
	}
	//! call to close a row
	void closeRow()
	{
		++m_row;
	}
	//! returns the position of a cellule
	bool getPosition(int column, int row, double &x, double &y) const
	{
		bool ok=true;
		if (column>=0 && column <int(m_columnsDistanceFromOrigin.size()))
			x=m_x+m_columnsDistanceFromOrigin[size_t(column)];
		else
		{
			ok=false;
			RVNG_DEBUG_MSG(("librevenge::DrawingSVG::Table::getPosition: the column %d seems bad\n", column));
			x=(column<0 || m_columnsDistanceFromOrigin.empty()) ? m_x : m_x + m_columnsDistanceFromOrigin.back();
		}
		if (row>=0 && row <int(m_rowsDistanceFromOrigin.size()))
			y=m_y+m_rowsDistanceFromOrigin[size_t(row)];
		else
		{
			ok=false;
			RVNG_DEBUG_MSG(("librevenge::DrawingSVG::Table::getPosition: the row %d seems bad\n", row));
			y=(row<0 || m_rowsDistanceFromOrigin.empty()) ? m_y : m_y + m_rowsDistanceFromOrigin.back();
		}
		return ok;
	}
	//! the actual column
	int m_column;
	//! the actual row
	int m_row;
	//! the origin table position in inches
	double m_x, m_y;
	//! the distance of each begin column in inches from origin
	std::vector<double> m_columnsDistanceFromOrigin;
	//! the distance of each begin row in inches from origin
	std::vector<double> m_rowsDistanceFromOrigin;
};

} // PresentationSVG namespace

using namespace PresentationSVG;

struct RVNGSVGPresentationGeneratorImpl
{
	RVNGSVGPresentationGeneratorImpl(RVNGStringVector &m_vec);

	std::map<int, RVNGPropertyList> m_idSpanMap;

	RVNGPropertyListVector m_gradient;
	RVNGPropertyList m_style;
	int m_gradientIndex;
	int m_patternIndex;
	int m_arrowStartIndex /** start arrow index*/, m_arrowEndIndex /** end arrow index */;
	//! layerId used if svg:id is not defined when calling startLayer
	int m_layerId;
	int m_shadowIndex;

	void writeStyle(bool isClosed=true);
	void drawPolySomething(const RVNGPropertyListVector &vertices, bool isClosed);

	std::ostringstream m_outputSink;
	RVNGStringVector &m_vec;
	//! the actual master name
	RVNGString m_masterName;
	//! a map master name to master content
	std::map<RVNGString, std::string> m_masterNameToContentMap;
	//! the actual opened table
	boost::shared_ptr<Table> m_table;
};

RVNGSVGPresentationGeneratorImpl::RVNGSVGPresentationGeneratorImpl(RVNGStringVector &vec)
	: m_idSpanMap()
	, m_gradient()
	, m_style()
	, m_gradientIndex(1)
	, m_patternIndex(1)
	, m_arrowStartIndex(1)
	, m_arrowEndIndex(1)
	, m_layerId(1000)
	, m_shadowIndex(1)
	, m_outputSink()
	, m_vec(vec)
	, m_masterName(), m_masterNameToContentMap()
	, m_table()
{
}

RVNGSVGPresentationGenerator::RVNGSVGPresentationGenerator(RVNGStringVector &vec)
	: m_impl(new RVNGSVGPresentationGeneratorImpl(vec))
{
}

RVNGSVGPresentationGenerator::~RVNGSVGPresentationGenerator()
{
	delete m_impl;
}

void RVNGSVGPresentationGenerator::startDocument(const RVNGPropertyList &)
{
}

void RVNGSVGPresentationGenerator::endDocument()
{
}

void RVNGSVGPresentationGenerator::setDocumentMetaData(const RVNGPropertyList &)
{
}

void RVNGSVGPresentationGenerator::defineEmbeddedFont(const RVNGPropertyList &)
{
}

void RVNGSVGPresentationGenerator::startSlide(const RVNGPropertyList &propList)
{
	if (propList["librevenge:master-page-name"])
	{
		if (m_impl->m_masterNameToContentMap.find(propList["librevenge:master-page-name"]->getStr())!=
		        m_impl->m_masterNameToContentMap.end())
		{
			m_impl->m_outputSink << m_impl->m_masterNameToContentMap.find(propList["librevenge:master-page-name"]->getStr())->second;
			return;
		}
		RVNG_DEBUG_MSG(("RVNGSVGPresentationGenerator::startSlide: can not find master slide with given master name\n"));
	}

	m_impl->m_outputSink << "<svg:svg version=\"1.1\" xmlns:svg=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" ";
	if (propList["svg:width"])
		m_impl->m_outputSink << "width=\"" << doubleToString(72*getInchValue(*propList["svg:width"])) << "\" ";
	if (propList["svg:height"])
		m_impl->m_outputSink << "height=\"" << doubleToString(72*getInchValue(*propList["svg:height"])) << "\"";
	m_impl->m_outputSink << " >\n";
}

void RVNGSVGPresentationGenerator::endSlide()
{
	m_impl->m_outputSink << "</svg:svg>\n";
	m_impl->m_vec.append(m_impl->m_outputSink.str().c_str());
	m_impl->m_outputSink.str("");
}

void RVNGSVGPresentationGenerator::startMasterSlide(const RVNGPropertyList &propList)
{
	if (!m_impl->m_masterName.empty())
	{
		RVNG_DEBUG_MSG(("RVNGSVGPresentationGenerator::startMasterSlide: a master slide is already opened\n"));
	}
	else if (propList["librevenge:master-page-name"])
	{
		m_impl->m_masterName=propList["librevenge:master-page-name"]->getStr();
		RVNGPropertyList pList(propList);
		pList.remove("librevenge:master-page-name");
		startSlide(pList);
	}
	else
	{
		RVNG_DEBUG_MSG(("RVNGSVGPresentationGenerator::startMasterSlide: can not find the master name\n"));
	}
}

void RVNGSVGPresentationGenerator::endMasterSlide()
{
	if (m_impl->m_masterName.empty())
	{
		RVNG_DEBUG_MSG(("RVNGSVGPresentationGenerator::endMasterSlide: no master slides are opened\n"));
	}
	else
	{
		if (m_impl->m_masterNameToContentMap.find(m_impl->m_masterName)!=m_impl->m_masterNameToContentMap.end())
		{
			RVNG_DEBUG_MSG(("RVNGSVGPresentationGenerator::endMasterSlide: a master slide with name %s already exists\n",
			                m_impl->m_masterName.cstr()));
		}
		// no need to close the page, this will be done when the master page will be used
		m_impl->m_masterNameToContentMap[m_impl->m_masterName]=m_impl->m_outputSink.str();
		m_impl->m_masterName.clear();
	}
	// reset the content
	m_impl->m_outputSink.str("");
}

void RVNGSVGPresentationGenerator::setSlideTransition(const RVNGPropertyList &)
{
}

void RVNGSVGPresentationGenerator::setStyle(const RVNGPropertyList &propList)
{
	m_impl->m_style.clear();
	m_impl->m_style = propList;

	const librevenge::RVNGPropertyListVector *gradient = propList.child("svg:linearGradient");
	if (!gradient)
		gradient = propList.child("svg:radialGradient");
	m_impl->m_gradient = gradient ? *gradient : librevenge::RVNGPropertyListVector();
	if (m_impl->m_style["draw:shadow"] && m_impl->m_style["draw:shadow"]->getStr() == "visible")
	{
		double shadowRed = 0.0;
		double shadowGreen = 0.0;
		double shadowBlue = 0.0;
		if (m_impl->m_style["draw:shadow-color"])
		{
			const unsigned shadowColour = stringToColour(m_impl->m_style["draw:shadow-color"]->getStr());
			shadowRed = (double)((shadowColour & 0x00ff0000) >> 16)/255.0;
			shadowGreen = (double)((shadowColour & 0x0000ff00) >> 8)/255.0;
			shadowBlue = (double)(shadowColour & 0x000000ff)/255.0;
		}
		m_impl->m_outputSink << "<svg:defs>\n";
		m_impl->m_outputSink << "<svg:filter filterUnits=\"userSpaceOnUse\" id=\"shadow" << m_impl->m_shadowIndex++ << "\">";
		m_impl->m_outputSink << "<svg:feOffset in=\"SourceGraphic\" result=\"offset\" ";
		if (m_impl->m_style["draw:shadow-offset-x"])
			m_impl->m_outputSink << "dx=\"" << doubleToString(72*getInchValue(*m_impl->m_style["draw:shadow-offset-x"])) << "\" ";
		if (m_impl->m_style["draw:shadow-offset-y"])
			m_impl->m_outputSink << "dy=\"" << doubleToString(72*getInchValue(*m_impl->m_style["draw:shadow-offset-y"])) << "\" ";
		m_impl->m_outputSink << "/>";
		m_impl->m_outputSink << "<svg:feColorMatrix in=\"offset\" result=\"offset-color\" type=\"matrix\" values=\"";
		m_impl->m_outputSink << "0 0 0 0 " << doubleToString(shadowRed) ;
		m_impl->m_outputSink << " 0 0 0 0 " << doubleToString(shadowGreen);
		m_impl->m_outputSink << " 0 0 0 0 " << doubleToString(shadowBlue);
		if (m_impl->m_style["draw:opacity"] && m_impl->m_style["draw:opacity"]->getDouble() < 1)
			m_impl->m_outputSink << " 0 0 0 "   << doubleToString(m_impl->m_style["draw:shadow-opacity"]->getDouble()/m_impl->m_style["draw:opacity"]->getDouble()) << " 0\"/>";
		else
			m_impl->m_outputSink << " 0 0 0 "   << doubleToString(m_impl->m_style["draw:shadow-opacity"]->getDouble()) << " 0\"/>";
		m_impl->m_outputSink << "<svg:feMerge><svg:feMergeNode in=\"offset-color\" /><svg:feMergeNode in=\"SourceGraphic\" /></svg:feMerge></svg:filter></svg:defs>";
	}

	if (m_impl->m_style["draw:fill"] && m_impl->m_style["draw:fill"]->getStr() == "gradient")
	{
		double angle = (m_impl->m_style["draw:angle"] ? m_impl->m_style["draw:angle"]->getDouble() : 0.0);
		angle *= -1.0;
		while (angle < 0)
			angle += 360;
		while (angle > 360)
			angle -= 360;

		if (!m_impl->m_gradient.count())
		{
			if (m_impl->m_style["draw:style"] &&
			        (m_impl->m_style["draw:style"]->getStr() == "radial" ||
			         m_impl->m_style["draw:style"]->getStr() == "rectangular" ||
			         m_impl->m_style["draw:style"]->getStr() == "square" ||
			         m_impl->m_style["draw:style"]->getStr() == "ellipsoid"))
			{
				m_impl->m_outputSink << "<svg:defs>\n";
				m_impl->m_outputSink << "  <svg:radialGradient id=\"grad" << m_impl->m_gradientIndex++ << "\"";

				if (m_impl->m_style["svg:cx"])
					m_impl->m_outputSink << " cx=\"" << m_impl->m_style["svg:cx"]->getStr().cstr() << "\"";
				else if (m_impl->m_style["draw:cx"])
					m_impl->m_outputSink << " cx=\"" << m_impl->m_style["draw:cx"]->getStr().cstr() << "\"";

				if (m_impl->m_style["svg:cy"])
					m_impl->m_outputSink << " cy=\"" << m_impl->m_style["svg:cy"]->getStr().cstr() << "\"";
				else if (m_impl->m_style["draw:cy"])
					m_impl->m_outputSink << " cy=\"" << m_impl->m_style["draw:cy"]->getStr().cstr() << "\"";
				m_impl->m_outputSink << " r=\"" << (1 - (m_impl->m_style["draw:border"] ? m_impl->m_style["draw:border"]->getDouble() : 0))*100.0 << "%\" >\n";
				m_impl->m_outputSink << " >\n";

				if (m_impl->m_style["draw:start-color"] && m_impl->m_style["draw:end-color"])
				{
					m_impl->m_outputSink << "    <svg:stop offset=\"0%\"";
					m_impl->m_outputSink << " stop-color=\"" << m_impl->m_style["draw:end-color"]->getStr().cstr() << "\"";
					m_impl->m_outputSink << " stop-opacity=\"" << (m_impl->m_style["librevenge:end-opacity"] ? m_impl->m_style["librevenge:end-opacity"]->getDouble() : 1) << "\" />" << std::endl;

					m_impl->m_outputSink << "    <svg:stop offset=\"100%\"";
					m_impl->m_outputSink << " stop-color=\"" << m_impl->m_style["draw:start-color"]->getStr().cstr() << "\"";
					m_impl->m_outputSink << " stop-opacity=\"" << (m_impl->m_style["librevenge:start-opacity"] ? m_impl->m_style["librevenge:start-opacity"]->getDouble() : 1) << "\" />" << std::endl;
				}
				m_impl->m_outputSink << "  </svg:radialGradient>\n";
				m_impl->m_outputSink << "</svg:defs>\n";
			}
			else if (m_impl->m_style["draw:style"] && m_impl->m_style["draw:style"]->getStr() == "linear")
			{
				m_impl->m_outputSink << "<svg:defs>\n";
				m_impl->m_outputSink << "  <svg:linearGradient id=\"grad" << m_impl->m_gradientIndex++ << "\" >\n";

				if (m_impl->m_style["draw:start-color"] && m_impl->m_style["draw:end-color"])
				{
					m_impl->m_outputSink << "    <svg:stop offset=\"0%\"";
					m_impl->m_outputSink << " stop-color=\"" << m_impl->m_style["draw:start-color"]->getStr().cstr() << "\"";
					m_impl->m_outputSink << " stop-opacity=\"" << (m_impl->m_style["librevenge:start-opacity"] ? m_impl->m_style["librevenge:start-opacity"]->getDouble() : 1) << "\" />" << std::endl;

					m_impl->m_outputSink << "    <svg:stop offset=\"100%\"";
					m_impl->m_outputSink << " stop-color=\"" << m_impl->m_style["draw:end-color"]->getStr().cstr() << "\"";
					m_impl->m_outputSink << " stop-opacity=\"" << (m_impl->m_style["librevenge:end-opacity"] ? m_impl->m_style["librevenge:end-opacity"]->getDouble() : 1) << "\" />" << std::endl;
				}
				m_impl->m_outputSink << "  </svg:linearGradient>\n";

				// not a simple horizontal gradient
				if (angle < 270 || angle > 270)
				{
					m_impl->m_outputSink << "  <svg:linearGradient xlink:href=\"#grad" << m_impl->m_gradientIndex-1 << "\"";
					m_impl->m_outputSink << " id=\"grad" << m_impl->m_gradientIndex++ << "\" ";
					m_impl->m_outputSink << "x1=\"0\" y1=\"0\" x2=\"0\" y2=\"1\" ";
					m_impl->m_outputSink << "gradientTransform=\"rotate(" << angle << " .5 .5)\" ";
					m_impl->m_outputSink << "gradientUnits=\"objectBoundingBox\" >\n";
					m_impl->m_outputSink << "  </svg:linearGradient>\n";
				}

				m_impl->m_outputSink << "</svg:defs>\n";
			}
			else if (m_impl->m_style["draw:style"] && m_impl->m_style["draw:style"]->getStr() == "axial")
			{
				m_impl->m_outputSink << "<svg:defs>\n";
				m_impl->m_outputSink << "  <svg:linearGradient id=\"grad" << m_impl->m_gradientIndex++ << "\" >\n";

				if (m_impl->m_style["draw:start-color"] && m_impl->m_style["draw:end-color"])
				{
					m_impl->m_outputSink << "    <svg:stop offset=\"0%\"";
					m_impl->m_outputSink << " stop-color=\"" << m_impl->m_style["draw:end-color"]->getStr().cstr() << "\"";
					m_impl->m_outputSink << " stop-opacity=\"" << (m_impl->m_style["librevenge:end-opacity"] ? m_impl->m_style["librevenge:end-opacity"]->getDouble() : 1) << "\" />" << std::endl;

					m_impl->m_outputSink << "    <svg:stop offset=\"50%\"";
					m_impl->m_outputSink << " stop-color=\"" << m_impl->m_style["draw:start-color"]->getStr().cstr() << "\"";
					m_impl->m_outputSink << " stop-opacity=\"" << (m_impl->m_style["librevenge:start-opacity"] ? m_impl->m_style["librevenge:start-opacity"]->getDouble() : 1) << "\" />" << std::endl;

					m_impl->m_outputSink << "    <svg:stop offset=\"100%\"";
					m_impl->m_outputSink << " stop-color=\"" << m_impl->m_style["draw:end-color"]->getStr().cstr() << "\"";
					m_impl->m_outputSink << " stop-opacity=\"" << (m_impl->m_style["librevenge:end-opacity"] ? m_impl->m_style["librevenge:end-opacity"]->getDouble() : 1) << "\" />" << std::endl;
				}
				m_impl->m_outputSink << "  </svg:linearGradient>\n";

				// not a simple horizontal gradient
				if (angle < 270 || angle > 270)
				{
					m_impl->m_outputSink << "  <svg:linearGradient xlink:href=\"#grad" << m_impl->m_gradientIndex-1 << "\"";
					m_impl->m_outputSink << " id=\"grad" << m_impl->m_gradientIndex++ << "\" ";
					m_impl->m_outputSink << "x1=\"0\" y1=\"0\" x2=\"0\" y2=\"1\" ";
					m_impl->m_outputSink << "gradientTransform=\"rotate(" << angle << " .5 .5)\" ";
					m_impl->m_outputSink << "gradientUnits=\"objectBoundingBox\" >\n";
					m_impl->m_outputSink << "  </svg:linearGradient>\n";
				}

				m_impl->m_outputSink << "</svg:defs>\n";
			}
		}
		else
		{
			if (m_impl->m_style["draw:style"] && m_impl->m_style["draw:style"]->getStr() == "radial")
			{
				m_impl->m_outputSink << "<svg:defs>\n";
				m_impl->m_outputSink << "  <svg:radialGradient id=\"grad" << m_impl->m_gradientIndex++ << "\" cx=\"" << m_impl->m_style["svg:cx"]->getStr().cstr() << "\" cy=\"" << m_impl->m_style["svg:cy"]->getStr().cstr() << "\" r=\"" << m_impl->m_style["svg:r"]->getStr().cstr() << "\" >\n";
				for (unsigned c = 0; c < m_impl->m_gradient.count(); c++)
				{
					m_impl->m_outputSink << "    <svg:stop offset=\"" << m_impl->m_gradient[c]["svg:offset"]->getStr().cstr() << "\"";

					m_impl->m_outputSink << " stop-color=\"" << m_impl->m_gradient[c]["svg:stop-color"]->getStr().cstr() << "\"";
					m_impl->m_outputSink << " stop-opacity=\"" << m_impl->m_gradient[c]["svg:stop-opacity"]->getDouble() << "\" />" << std::endl;

				}
				m_impl->m_outputSink << "  </svg:radialGradient>\n";
				m_impl->m_outputSink << "</svg:defs>\n";
			}
			else
			{
				m_impl->m_outputSink << "<svg:defs>\n";
				m_impl->m_outputSink << "  <svg:linearGradient id=\"grad" << m_impl->m_gradientIndex++ << "\" >\n";
				for (unsigned c = 0; c < m_impl->m_gradient.count(); c++)
				{
					m_impl->m_outputSink << "    <svg:stop offset=\"" << m_impl->m_gradient[c]["svg:offset"]->getStr().cstr() << "\"";

					m_impl->m_outputSink << " stop-color=\"" << m_impl->m_gradient[c]["svg:stop-color"]->getStr().cstr() << "\"";
					m_impl->m_outputSink << " stop-opacity=\"" << m_impl->m_gradient[c]["svg:stop-opacity"]->getDouble() << "\" />" << std::endl;

				}
				m_impl->m_outputSink << "  </svg:linearGradient>\n";

				// not a simple horizontal gradient
				if (angle < 270 || angle > 270)
				{
					m_impl->m_outputSink << "  <svg:linearGradient xlink:href=\"#grad" << m_impl->m_gradientIndex-1 << "\"";
					m_impl->m_outputSink << " id=\"grad" << m_impl->m_gradientIndex++ << "\" ";
					m_impl->m_outputSink << "x1=\"0\" y1=\"0\" x2=\"0\" y2=\"1\" ";
					m_impl->m_outputSink << "gradientTransform=\"rotate(" << angle << " .5 .5)\" ";
					m_impl->m_outputSink << "gradientUnits=\"objectBoundingBox\" >\n";
					m_impl->m_outputSink << "  </svg:linearGradient>\n";
				}

				m_impl->m_outputSink << "</svg:defs>\n";
			}
		}
	}
	else if (m_impl->m_style["draw:fill"] && m_impl->m_style["draw:fill"]->getStr() == "bitmap")
	{
		if (m_impl->m_style["draw:fill-image"] && m_impl->m_style["librevenge:mime-type"])
		{
			m_impl->m_outputSink << "<svg:defs>\n";
			m_impl->m_outputSink << "  <svg:pattern id=\"img" << m_impl->m_patternIndex++ << "\" patternUnits=\"userSpaceOnUse\" ";
			if (m_impl->m_style["svg:width"])
				m_impl->m_outputSink << "width=\"" << doubleToString(72*getInchValue(*m_impl->m_style["svg:width"])) << "\" ";
			else
				m_impl->m_outputSink << "width=\"100\" ";

			if (m_impl->m_style["svg:height"])
				m_impl->m_outputSink << "height=\"" << doubleToString(72*getInchValue(*m_impl->m_style["svg:height"])) << "\">" << std::endl;
			else
				m_impl->m_outputSink << "height=\"100\">" << std::endl;
			m_impl->m_outputSink << "<svg:image ";

			if (m_impl->m_style["svg:x"])
				m_impl->m_outputSink << "x=\"" << doubleToString(72*getInchValue(*m_impl->m_style["svg:x"])) << "\" ";
			else
				m_impl->m_outputSink << "x=\"0\" ";

			if (m_impl->m_style["svg:y"])
				m_impl->m_outputSink << "y=\"" << doubleToString(72*getInchValue(*m_impl->m_style["svg:y"])) << "\" ";
			else
				m_impl->m_outputSink << "y=\"0\" ";

			if (m_impl->m_style["svg:width"])
				m_impl->m_outputSink << "width=\"" << doubleToString(72*getInchValue(*m_impl->m_style["svg:width"])) << "\" ";
			else
				m_impl->m_outputSink << "width=\"100\" ";

			if (m_impl->m_style["svg:height"])
				m_impl->m_outputSink << "height=\"" << doubleToString(72*getInchValue(*m_impl->m_style["svg:height"])) << "\" ";
			else
				m_impl->m_outputSink << "height=\"100\" ";

			m_impl->m_outputSink << "xlink:href=\"data:" << m_impl->m_style["librevenge:mime-type"]->getStr().cstr() << ";base64,";
			m_impl->m_outputSink << m_impl->m_style["draw:fill-image"]->getStr().cstr();
			m_impl->m_outputSink << "\" />\n";
			m_impl->m_outputSink << "  </svg:pattern>\n";
			m_impl->m_outputSink << "</svg:defs>\n";
		}
	}
}

void RVNGSVGPresentationGenerator::startLayer(const RVNGPropertyList &propList)
{
	m_impl->m_outputSink << "<svg:g";
	librevenge::RVNGString layer("Layer");
	if (propList["draw:layer"])
		layer.append(propList["draw:layer"]->getStr());
	else if (propList["svg:id"])
		layer.append(propList["svg:id"]->getStr());
	else
		layer.sprintf("Layer%d", m_impl->m_layerId++);
	librevenge::RVNGString finalName("");
	finalName.appendEscapedXML(layer);
	m_impl->m_outputSink << " id=\"" << finalName.cstr() << "\"";
	if (propList["svg:fill-rule"])
		m_impl->m_outputSink << " fill-rule=\"" << propList["svg:fill-rule"]->getStr().cstr() << "\"";
	m_impl->m_outputSink << " >\n";
}

void RVNGSVGPresentationGenerator::endLayer()
{
	m_impl->m_outputSink << "</svg:g>\n";
}

void RVNGSVGPresentationGenerator::startEmbeddedGraphics(const RVNGPropertyList & /*propList*/)
{
}

void RVNGSVGPresentationGenerator::endEmbeddedGraphics()
{
}

void RVNGSVGPresentationGenerator::openGroup(const RVNGPropertyList &/*propList*/)
{
	// TODO: handle svg:id
	m_impl->m_outputSink << "<svg:g>\n";
}

void RVNGSVGPresentationGenerator::closeGroup()
{
	m_impl->m_outputSink << "</svg:g>\n";
}

void RVNGSVGPresentationGenerator::drawRectangle(const RVNGPropertyList &propList)
{
	if (!propList["svg:x"] || !propList["svg:y"] || !propList["svg:width"] || !propList["svg:height"])
		return;
	m_impl->m_outputSink << "<svg:rect ";
	m_impl->m_outputSink << "x=\"" << doubleToString(72*getInchValue(*propList["svg:x"])) << "\" y=\"" << doubleToString(72*getInchValue(*propList["svg:y"])) << "\" ";
	m_impl->m_outputSink << "width=\"" << doubleToString(72*getInchValue(*propList["svg:width"])) << "\" height=\"" << doubleToString(72*getInchValue(*propList["svg:height"])) << "\" ";
	if ((propList["svg:rx"] && propList["svg:rx"]->getInt() !=0) || (propList["svg:ry"] && propList["svg:ry"]->getInt() !=0))
		m_impl->m_outputSink << "rx=\"" << doubleToString(72*getInchValue(*propList["svg:rx"])) << "\" ry=\"" << doubleToString(72*getInchValue(*propList["svg:ry"])) << "\" ";
	m_impl->writeStyle();
	m_impl->m_outputSink << "/>\n";
}

void RVNGSVGPresentationGenerator::drawEllipse(const RVNGPropertyList &propList)
{
	if (!propList["svg:cx"] || !propList["svg:cy"] || !propList["svg:rx"] || !propList["svg:ry"])
		return;
	m_impl->m_outputSink << "<svg:ellipse ";
	m_impl->m_outputSink << "cx=\"" << doubleToString(72*getInchValue(*propList["svg:cx"])) << "\" cy=\"" << doubleToString(72*getInchValue(*propList["svg:cy"])) << "\" ";
	m_impl->m_outputSink << "rx=\"" << doubleToString(72*getInchValue(*propList["svg:rx"])) << "\" ry=\"" << doubleToString(72*getInchValue(*propList["svg:ry"])) << "\" ";
	m_impl->writeStyle();
	if (propList["librevenge:rotate"] && (propList["librevenge:rotate"]->getDouble()<0||propList["librevenge:rotate"]->getDouble()>0))
		m_impl->m_outputSink << " transform=\" rotate(" << doubleToString(-propList["librevenge:rotate"]->getDouble())
		                     << ", " << doubleToString(72*getInchValue(*propList["svg:cy"]))
		                     << ", " << doubleToString(72*getInchValue(*propList["svg:cy"]))
		                     << ")\" ";
	m_impl->m_outputSink << "/>\n";
}

void RVNGSVGPresentationGenerator::drawPolyline(const RVNGPropertyList &propList)
{
	const RVNGPropertyListVector *vertices = propList.child("svg:points");
	if (vertices && vertices->count())
		m_impl->drawPolySomething(*vertices, false);
}

void RVNGSVGPresentationGenerator::drawPolygon(const RVNGPropertyList &propList)
{
	const RVNGPropertyListVector *vertices = propList.child("svg:points");
	if (vertices && vertices->count())
		m_impl->drawPolySomething(*vertices, true);
}

void RVNGSVGPresentationGeneratorImpl::drawPolySomething(const RVNGPropertyListVector &vertices, bool isClosed)
{
	if (vertices.count() < 2)
		return;

	if (vertices.count() == 2)
	{
		if (!vertices[0]["svg:x"]||!vertices[0]["svg:y"]||!vertices[1]["svg:x"]||!vertices[1]["svg:y"])
			return;
		m_outputSink << "<svg:line ";
		m_outputSink << "x1=\"" << doubleToString(72*getInchValue(*vertices[0]["svg:x"])) << "\"  y1=\"" << doubleToString(72*getInchValue(*vertices[0]["svg:y"])) << "\" ";
		m_outputSink << "x2=\"" << doubleToString(72*getInchValue(*vertices[1]["svg:x"])) << "\"  y2=\"" << doubleToString(72*getInchValue(*vertices[1]["svg:y"])) << "\"\n";
		writeStyle();
		m_outputSink << "/>\n";
	}
	else
	{
		if (isClosed)
			m_outputSink << "<svg:polygon ";
		else
			m_outputSink << "<svg:polyline ";

		m_outputSink << "points=\"";
		for (unsigned i = 0; i < vertices.count(); i++)
		{
			if (!vertices[i]["svg:x"]||!vertices[i]["svg:y"])
				continue;
			m_outputSink << doubleToString(72*getInchValue(*vertices[i]["svg:x"])) << " " << doubleToString(72*getInchValue(*vertices[i]["svg:y"]));
			if (i < vertices.count()-1)
				m_outputSink << ", ";
		}
		m_outputSink << "\"\n";
		writeStyle(isClosed);
		m_outputSink << "/>\n";
	}
}

void RVNGSVGPresentationGenerator::drawPath(const RVNGPropertyList &propList)
{
	const RVNGPropertyListVector *path = propList.child("svg:d");
	if (!path)
		return;
	m_impl->m_outputSink << "<svg:path d=\" ";
	bool isClosed = false;
	unsigned i=0;
	for (i=0; i < path->count(); i++)
	{
		RVNGPropertyList pList((*path)[i]);
		if (!pList["librevenge:path-action"]) continue;
		std::string action=pList["librevenge:path-action"]->getStr().cstr();
		if (action.length()!=1) continue;
		bool coordOk=pList["svg:x"]&&pList["svg:y"];
		bool coord1Ok=coordOk && pList["svg:x1"]&&pList["svg:y1"];
		bool coord2Ok=coord1Ok && pList["svg:x2"]&&pList["svg:y2"];
		if (pList["svg:x"] && action[0] == 'H')
			m_impl->m_outputSink << "\nH" << doubleToString(72*getInchValue(*pList["svg:x"]));
		else if (pList["svg:y"] && action[0] == 'V')
			m_impl->m_outputSink << "\nV" << doubleToString(72*getInchValue(*pList["svg:y"]));
		else if (coordOk && (action[0] == 'M' || action[0] == 'L' || action[0] == 'T'))
		{
			m_impl->m_outputSink << "\n" << action;
			m_impl->m_outputSink << doubleToString(72*getInchValue(*pList["svg:x"])) << "," << doubleToString(72*getInchValue(*pList["svg:y"]));
		}
		else if (coord1Ok && (action[0] == 'Q' || action[0] == 'S'))
		{
			m_impl->m_outputSink << "\n" << action;
			m_impl->m_outputSink << doubleToString(72*getInchValue(*pList["svg:x1"])) << "," << doubleToString(72*getInchValue(*pList["svg:y1"])) << " ";
			m_impl->m_outputSink << doubleToString(72*getInchValue(*pList["svg:x"])) << "," << doubleToString(72*getInchValue(*pList["svg:y"]));
		}
		else if (coord2Ok && action[0] == 'C')
		{
			m_impl->m_outputSink << "\nC";
			m_impl->m_outputSink << doubleToString(72*getInchValue(*pList["svg:x1"])) << "," << doubleToString(72*getInchValue(*pList["svg:y1"])) << " ";
			m_impl->m_outputSink << doubleToString(72*getInchValue(*pList["svg:x2"])) << "," << doubleToString(72*getInchValue(*pList["svg:y2"])) << " ";
			m_impl->m_outputSink << doubleToString(72*getInchValue(*pList["svg:x"])) << "," << doubleToString(72*getInchValue(*pList["svg:y"]));
		}
		else if (coordOk && pList["svg:rx"] && pList["svg:ry"] && action[0] == 'A')
		{
			m_impl->m_outputSink << "\nA";
			m_impl->m_outputSink << doubleToString(72*getInchValue(*pList["svg:rx"])) << "," << doubleToString(72*getInchValue(*pList["svg:ry"])) << " ";
			m_impl->m_outputSink << doubleToString(pList["librevenge:rotate"] ? pList["librevenge:rotate"]->getDouble() : 0) << " ";
			m_impl->m_outputSink << (pList["librevenge:large-arc"] ? pList["librevenge:large-arc"]->getInt() : 1) << ",";
			m_impl->m_outputSink << (pList["librevenge:sweep"] ? pList["librevenge:sweep"]->getInt() : 1) << " ";
			m_impl->m_outputSink << doubleToString(72*getInchValue(*pList["svg:x"])) << "," << doubleToString(72*getInchValue(*pList["svg:y"]));
		}
		else if (action[0] == 'Z')
		{
			isClosed = true;
			m_impl->m_outputSink << "\nZ";
		}
	}

	m_impl->m_outputSink << "\" \n";
	m_impl->writeStyle(isClosed);
	m_impl->m_outputSink << "/>\n";
}

void RVNGSVGPresentationGenerator::drawGraphicObject(const RVNGPropertyList &propList)
{
	if (!propList["librevenge:mime-type"] || propList["librevenge:mime-type"]->getStr().len() <= 0)
		return;
	if (!propList["office:binary-data"])
		return;
	m_impl->m_outputSink << "<svg:image ";
	if (propList["svg:x"] && propList["svg:y"] && propList["svg:width"] && propList["svg:height"])
	{
		double x=getInchValue(*propList["svg:x"]);
		double y=getInchValue(*propList["svg:y"]);
		double width=getInchValue(*propList["svg:width"]);
		double height=getInchValue(*propList["svg:height"]);
		bool flipX(propList["draw:mirror-horizontal"] && propList["draw:mirror-horizontal"]->getInt());
		bool flipY(propList["draw:mirror-vertical"] && propList["draw:mirror-vertical"]->getInt());

		m_impl->m_outputSink << "x=\"" << doubleToString(72*x) << "\" y=\"" << doubleToString(72*y) << "\" ";
		m_impl->m_outputSink << "width=\"" << doubleToString(72*width) << "\" height=\"" << doubleToString(72*height) << "\" ";
		if (flipX || flipY || propList["librevenge:rotate"])
		{
			double xmiddle = x + width / 2.0;
			double ymiddle = y + height / 2.0;
			m_impl->m_outputSink << "transform=\"";
			m_impl->m_outputSink << " translate(" << doubleToString(72*xmiddle) << ", " << doubleToString(72*ymiddle) << ") ";
			m_impl->m_outputSink << " scale(" << (flipX ? "-1" : "1") << ", " << (flipY ? "-1" : "1") << ") ";
			// rotation is around the center of the object's bounding box
			if (propList["librevenge:rotate"])
			{
				double angle(propList["librevenge:rotate"]->getDouble());
				while (angle > 180.0)
					angle -= 360.0;
				while (angle < -180.0)
					angle += 360.0;
				m_impl->m_outputSink << " rotate(" << doubleToString(angle) << ") ";
			}
			m_impl->m_outputSink << " translate(" << doubleToString(-72*xmiddle) << ", " << doubleToString(-72*ymiddle) << ") ";
			m_impl->m_outputSink << "\" ";
		}
	}
	m_impl->m_outputSink << "xlink:href=\"data:" << propList["librevenge:mime-type"]->getStr().cstr() << ";base64,";
	m_impl->m_outputSink << propList["office:binary-data"]->getStr().cstr();
	m_impl->m_outputSink << "\" />\n";
}

void RVNGSVGPresentationGenerator::drawConnector(const RVNGPropertyList &/*propList*/)
{
	// TODO: implement me
}

void RVNGSVGPresentationGenerator::startTextObject(const RVNGPropertyList &propList)
{
	double x = 0.0;
	double y = 0.0;
	double height = 0.0;
	m_impl->m_outputSink << "<svg:text ";
	if (propList["svg:x"] && propList["svg:y"])
	{
		x = getInchValue(*propList["svg:x"]);
		y = getInchValue(*propList["svg:y"]);
	}

	double xmiddle = x;
	double ymiddle = y;

	if (propList["svg:width"])
	{
		double width = getInchValue(*propList["svg:width"]);
		xmiddle += width / 2.0;
	}

	if (propList["svg:height"])
	{
		height = getInchValue(*propList["svg:height"]);
		ymiddle += height / 2.0;
	}

	if (propList["draw:textarea-vertical-align"])
	{
		if (propList["draw:textarea-vertical-align"]->getStr() == "middle")
			y = ymiddle;
		if (propList["draw:textarea-vertical-align"]->getStr() == "bottom")
		{
			y += height;
			if (propList["fo:padding-bottom"])
				y -= propList["fo:padding-bottom"]->getDouble();
		}
	}
	else
		y += height;

	if (propList["fo:padding-left"])
		x += propList["fo:padding-left"]->getDouble();

	m_impl->m_outputSink << "x=\"" << doubleToString(72*x) << "\" y=\"" << doubleToString(72*y) << "\"";

	// rotation is around the center of the object's bounding box
	if (propList["librevenge:rotate"] && (propList["librevenge:rotate"]->getDouble()<0||propList["librevenge:rotate"]->getDouble()>0))
	{
		double angle(propList["librevenge:rotate"]->getDouble());
		while (angle > 180.0)
			angle -= 360.0;
		while (angle < -180.0)
			angle += 360.0;
		m_impl->m_outputSink << " transform=\"rotate(" << doubleToString(angle) << ", " << doubleToString(72*xmiddle) << ", " << doubleToString(72*ymiddle) << ")\" ";
	}
	m_impl->m_outputSink << ">\n";
}

void RVNGSVGPresentationGenerator::endTextObject()
{
	m_impl->m_outputSink << "</svg:text>\n";
}

void RVNGSVGPresentationGenerator::defineCharacterStyle(const RVNGPropertyList &propList)
{
	if (!propList["librevenge:span-id"])
	{
		RVNG_DEBUG_MSG(("RVNGSVGPresentationGenerator::defineCharacterStyle: can not find the span-id\n"));
		return;
	}
	m_impl->m_idSpanMap[propList["librevenge:span-id"]->getInt()]=propList;
}

void RVNGSVGPresentationGenerator::openSpan(const RVNGPropertyList &propList)
{
	RVNGPropertyList pList(propList);
	if (propList["librevenge:span-id"] &&
	        m_impl->m_idSpanMap.find(propList["librevenge:span-id"]->getInt())!=m_impl->m_idSpanMap.end())
		pList=m_impl->m_idSpanMap.find(propList["librevenge:span-id"]->getInt())->second;

	m_impl->m_outputSink << "<svg:tspan ";
	if (pList["style:font-name"])
		m_impl->m_outputSink << "font-family=\"" << pList["style:font-name"]->getStr().cstr() << "\" ";
	if (pList["fo:font-style"])
		m_impl->m_outputSink << "font-style=\"" << pList["fo:font-style"]->getStr().cstr() << "\" ";
	if (pList["fo:font-weight"])
		m_impl->m_outputSink << "font-weight=\"" << pList["fo:font-weight"]->getStr().cstr() << "\" ";
	if (pList["fo:font-variant"])
		m_impl->m_outputSink << "font-variant=\"" << pList["fo:font-variant"]->getStr().cstr() << "\" ";
	if (pList["fo:font-size"])
		m_impl->m_outputSink << "font-size=\"" << doubleToString(pList["fo:font-size"]->getDouble()) << "\" ";
	if (pList["fo:color"])
		m_impl->m_outputSink << "fill=\"" << pList["fo:color"]->getStr().cstr() << "\" ";
	if (pList["fo:text-transform"])
		m_impl->m_outputSink << "text-transform=\"" << pList["fo:text-transform"]->getStr().cstr() << "\" ";
	if (pList["svg:fill-opacity"])
		m_impl->m_outputSink << "fill-opacity=\"" << doubleToString(pList["svg:fill-opacity"]->getDouble()) << "\" ";
	if (pList["svg:stroke-opacity"])
		m_impl->m_outputSink << "stroke-opacity=\"" << doubleToString(pList["svg:stroke-opacity"]->getDouble()) << "\" ";
	m_impl->m_outputSink << ">\n";
}

void RVNGSVGPresentationGenerator::closeSpan()
{
	m_impl->m_outputSink << "</svg:tspan>\n";
}

void RVNGSVGPresentationGenerator::openLink(const RVNGPropertyList & /* propList */)
{
}

void RVNGSVGPresentationGenerator::closeLink()
{
}

void RVNGSVGPresentationGenerator::insertTab()
{
	m_impl->m_outputSink << '\t';
}

void RVNGSVGPresentationGenerator::insertSpace()
{
	m_impl->m_outputSink << ' ';
}

void RVNGSVGPresentationGenerator::insertText(const RVNGString &str)
{
	m_impl->m_outputSink << RVNGString::escapeXML(str).cstr() << "\n";
}

void RVNGSVGPresentationGenerator::insertLineBreak()
{
	m_impl->m_outputSink << '\n';
}

void RVNGSVGPresentationGenerator::insertField(const RVNGPropertyList &/*propList*/)
{
	// TODO: implement me
}

void RVNGSVGPresentationGenerator::openOrderedListLevel(const RVNGPropertyList &)
{
}

void RVNGSVGPresentationGenerator::openUnorderedListLevel(const RVNGPropertyList &)
{
}

void RVNGSVGPresentationGenerator::closeOrderedListLevel()
{
}

void RVNGSVGPresentationGenerator::closeUnorderedListLevel()
{
}

void RVNGSVGPresentationGenerator::openListElement(const RVNGPropertyList &propList)
{
	openParagraph(propList);
}

void RVNGSVGPresentationGenerator::closeListElement()
{
	closeParagraph();
}

void RVNGSVGPresentationGenerator::defineParagraphStyle(const RVNGPropertyList &)
{
}

void RVNGSVGPresentationGenerator::openParagraph(const RVNGPropertyList &)
{
}

void RVNGSVGPresentationGenerator::closeParagraph()
{
	m_impl->m_outputSink << '\n';
}

void RVNGSVGPresentationGenerator::startTableObject(const RVNGPropertyList &propList)
{
	if (m_impl->m_table)
	{
		RVNG_DEBUG_MSG(("RVNGSVGPresentationGenerator::startTableObject: a table is already opened\n"));
		return;
	}
	m_impl->m_table.reset(new Table(propList));
}

void RVNGSVGPresentationGenerator::openTableRow(const RVNGPropertyList &propList)
{
	if (!m_impl->m_table) return;
	m_impl->m_table->openRow(propList);
}

void RVNGSVGPresentationGenerator::closeTableRow()
{
	if (!m_impl->m_table) return;
	m_impl->m_table->closeRow();
}

void RVNGSVGPresentationGenerator::openTableCell(const RVNGPropertyList &propList)
{
	if (!m_impl->m_table) return;

	if (propList["librevenge:column"])
		m_impl->m_table->m_column=propList["librevenge:column"]->getInt();
	if (propList["librevenge:row"])
		m_impl->m_table->m_row=propList["librevenge:row"]->getInt();

	double x = 0, y=0;
	m_impl->m_table->getPosition(m_impl->m_table->m_column, m_impl->m_table->m_row, x, y);
	m_impl->m_outputSink << "<svg:text ";
	m_impl->m_outputSink << "x=\"" << doubleToString(72*x) << "\" y=\"" << doubleToString(72*y) << "\" ";
	m_impl->m_outputSink << ">\n";

	// time to update the next cell's column
	if (propList["table:number-columns-spanned"])
		m_impl->m_table->m_column += propList["librevenge:column"]->getInt();
	else
		++m_impl->m_table->m_column;
}

void RVNGSVGPresentationGenerator::closeTableCell()
{
	if (!m_impl->m_table) return;
	m_impl->m_outputSink << "</svg:text>\n";
}

void RVNGSVGPresentationGenerator::insertCoveredTableCell(const RVNGPropertyList &/*propList*/)
{
	if (!m_impl->m_table) return;
	// TODO: implement me
}

void RVNGSVGPresentationGenerator::endTableObject()
{
	if (!m_impl->m_table)
	{
		RVNG_DEBUG_MSG(("RVNGSVGDrawingGenerator::endTableObject: no table is already opened\n"));
		return;
	}
	m_impl->m_table.reset();
}

void RVNGSVGPresentationGenerator::startComment(const RVNGPropertyList &/*propList*/)
{
	// TODO: implement me
}

void RVNGSVGPresentationGenerator::endComment()
{
	// TODO: implement me
}


void RVNGSVGPresentationGenerator::startNotes(const RVNGPropertyList &/*propList*/)
{
	// TODO: implement me
}

void RVNGSVGPresentationGenerator::endNotes()
{
	// TODO: implement me
}

void RVNGSVGPresentationGenerator::defineChartStyle(const RVNGPropertyList &)
{
}

void RVNGSVGPresentationGenerator::openChart(const RVNGPropertyList &)
{
}

void RVNGSVGPresentationGenerator::closeChart()
{
}

void RVNGSVGPresentationGenerator::openChartTextObject(const RVNGPropertyList &)
{
}

void RVNGSVGPresentationGenerator::closeChartTextObject()
{
}

void RVNGSVGPresentationGenerator::openChartPlotArea(const RVNGPropertyList &)
{
}

void RVNGSVGPresentationGenerator::closeChartPlotArea()
{
}

void RVNGSVGPresentationGenerator::insertChartAxis(const RVNGPropertyList &)
{
}

void RVNGSVGPresentationGenerator::openChartSeries(const librevenge::RVNGPropertyList &)
{
}

void RVNGSVGPresentationGenerator::closeChartSeries()
{
}

// create "style" attribute based on current pen and brush
void RVNGSVGPresentationGeneratorImpl::writeStyle(bool /* isClosed */)
{
	m_outputSink << "style=\"";

	double width = 1.0 / 72.0;
	if (m_style["svg:stroke-width"])
	{
		width = getInchValue(*m_style["svg:stroke-width"]);
		if (width <= 0.0 && m_style["draw:stroke"] && m_style["draw:stroke"]->getStr() != "none")
			width = 0.2 / 72.0; // reasonable hairline
		m_outputSink << "stroke-width: " << doubleToString(72*width) << "; ";
	}

	if (m_style["draw:stroke"] && m_style["draw:stroke"]->getStr() != "none")
	{
		if (m_style["svg:stroke-color"])
			m_outputSink << "stroke: " << m_style["svg:stroke-color"]->getStr().cstr()  << "; ";
		if (m_style["svg:stroke-opacity"] &&  m_style["svg:stroke-opacity"]->getInt()!= 1)
			m_outputSink << "stroke-opacity: " << doubleToString(m_style["svg:stroke-opacity"]->getDouble()) << "; ";
	}

	if (m_style["draw:stroke"] && m_style["draw:stroke"]->getStr() == "solid")
		m_outputSink << "stroke-dasharray: none; ";
	else if (m_style["draw:stroke"] && m_style["draw:stroke"]->getStr() == "dash")
	{
		int dots1 = m_style["draw:dots1"] ? m_style["draw:dots1"]->getInt() : 0;
		int dots2 = m_style["draw:dots2"] ? m_style["draw:dots2"]->getInt() : 0;
		double dots1len = 72.*width, dots2len = 72.*width, gap = 72.*width;
		if (m_style["draw:dots1-length"])
		{
			if (m_style["draw:dots1-length"]->getUnit()==librevenge::RVNG_PERCENT)
				dots1len=72*m_style["draw:dots1-length"]->getDouble()*width;
			else
				dots1len=72*getInchValue(*m_style["draw:dots1-length"]);
		}
		if (m_style["draw:dots2-length"])
		{
			if (m_style["draw:dots2-length"]->getUnit()==librevenge::RVNG_PERCENT)
				dots2len=72*m_style["draw:dots2-length"]->getDouble()*width;
			else
				dots2len=72*getInchValue(*m_style["draw:dots2-length"]);
		}
		if (m_style["draw:distance"])
		{
			if (m_style["draw:distance"]->getUnit()==librevenge::RVNG_PERCENT)
				gap=72*m_style["draw:distance"]->getDouble()*width;
			else
				gap=72*getInchValue(*m_style["draw:distance"]);
		}
		m_outputSink << "stroke-dasharray: ";
		for (int i = 0; i < dots1; i++)
		{
			if (i)
				m_outputSink << ", ";
			m_outputSink << doubleToString(dots1len);
			m_outputSink << ", ";
			m_outputSink << doubleToString(gap);
		}
		for (int j = 0; j < dots2; j++)
		{
			m_outputSink << ", ";
			m_outputSink << doubleToString(dots2len);
			m_outputSink << ", ";
			m_outputSink << doubleToString(gap);
		}
		m_outputSink << "; ";
	}

	if (m_style["svg:stroke-linecap"])
		m_outputSink << "stroke-linecap: " << m_style["svg:stroke-linecap"]->getStr().cstr() << "; ";

	if (m_style["svg:stroke-linejoin"])
		m_outputSink << "stroke-linejoin: " << m_style["svg:stroke-linejoin"]->getStr().cstr() << "; ";

	if (m_style["draw:fill"] && m_style["draw:fill"]->getStr() == "none")
		m_outputSink << "fill: none; ";
	else if (m_style["svg:fill-rule"])
		m_outputSink << "fill-rule: " << m_style["svg:fill-rule"]->getStr().cstr() << "; ";

	if (m_style["draw:fill"] && m_style["draw:fill"]->getStr() == "gradient")
		m_outputSink << "fill: url(#grad" << m_gradientIndex-1 << "); ";
	else if (m_style["draw:fill"] && m_style["draw:fill"]->getStr() == "bitmap")
		m_outputSink << "fill: url(#img" << m_patternIndex-1 << "); ";

	if (m_style["draw:shadow"] && m_style["draw:shadow"]->getStr() == "visible")
		m_outputSink << "filter:url(#shadow" << m_shadowIndex-1 << "); ";

	if (m_style["draw:fill"] && m_style["draw:fill"]->getStr() == "solid")
		if (m_style["draw:fill-color"])
			m_outputSink << "fill: " << m_style["draw:fill-color"]->getStr().cstr() << "; ";
	if (m_style["draw:opacity"] && m_style["draw:opacity"]->getDouble() < 1)
		m_outputSink << "fill-opacity: " << doubleToString(m_style["draw:opacity"]->getDouble()) << "; ";

	if (m_style["draw:marker-start-path"])
		m_outputSink << "marker-start: url(#startMarker" << m_arrowStartIndex-1 << "); ";
	if (m_style["draw:marker-end-path"])
		m_outputSink << "marker-end: url(#endMarker" << m_arrowEndIndex-1 << "); ";

	m_outputSink << "\""; // style
}

void RVNGSVGPresentationGenerator::openAnimationSequence(const RVNGPropertyList &)
{
}

void RVNGSVGPresentationGenerator::closeAnimationSequence()
{
}

void RVNGSVGPresentationGenerator::openAnimationGroup(const RVNGPropertyList &)
{
}

void RVNGSVGPresentationGenerator::closeAnimationGroup()
{
}

void RVNGSVGPresentationGenerator::openAnimationIteration(const RVNGPropertyList &)
{
}

void RVNGSVGPresentationGenerator::closeAnimationIteration()
{
}

void RVNGSVGPresentationGenerator::insertMotionAnimation(const RVNGPropertyList &)
{
}

void RVNGSVGPresentationGenerator::insertColorAnimation(const RVNGPropertyList &)
{
}

void RVNGSVGPresentationGenerator::insertAnimation(const RVNGPropertyList &)
{
}

void RVNGSVGPresentationGenerator::insertEffect(const RVNGPropertyList &)
{
}

}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
