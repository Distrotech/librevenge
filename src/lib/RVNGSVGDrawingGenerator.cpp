/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2006 Ariya Hidayat (ariya@kde.org)
 * Copyright (C) 2005 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 */

#include <map>
#include <sstream>
#include <string>
#include <stdio.h>
#include <iostream>

#include <librevenge-generators/librevenge-generators.h>

#include "librevenge_internal.h"

namespace librevenge
{

namespace
{

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

} // anomymous namespace

struct RVNGSVGDrawingGeneratorPrivate
{
	RVNGSVGDrawingGeneratorPrivate(RVNGStringVector &vec, const RVNGString &nmSpace);

	void setStyle(const RVNGPropertyList &propList);
	void writeStyle(bool isClosed=true);
	void drawPolySomething(const RVNGPropertyListVector &vertices, bool isClosed);

	//! return the namespace and the delimiter
	std::string const &getNamespaceAndDelim() const
	{
		return m_nmSpaceAndDelim;
	}

	std::map<int, RVNGPropertyList> m_idSpanMap;

	RVNGPropertyListVector m_gradient;
	RVNGPropertyList m_style;
	int m_gradientIndex, m_shadowIndex;
	//! index uses when fill=bitmap
	int m_patternIndex;
	int m_arrowStartIndex /** start arrow index*/, m_arrowEndIndex /** end arrow index */;
	//! layerId used if svg:id is not defined when calling startLayer
	int m_layerId;
	//! a prefix used to define the svg namespace
	std::string m_nmSpace;
	//! a prefix used to define the svg namespace with delimiter
	std::string m_nmSpaceAndDelim;
	std::ostringstream m_outputSink;
	RVNGStringVector &m_vec;
};

RVNGSVGDrawingGeneratorPrivate::RVNGSVGDrawingGeneratorPrivate(RVNGStringVector &vec, const RVNGString &nmSpace) :
	m_idSpanMap(),
	m_gradient(),
	m_style(),
	m_gradientIndex(1),
	m_shadowIndex(1),
	m_patternIndex(1),
	m_arrowStartIndex(1),
	m_arrowEndIndex(1),
	m_layerId(1000),
	m_nmSpace(nmSpace.cstr()),
	m_nmSpaceAndDelim(""),
	m_outputSink(),
	m_vec(vec)
{
	if (!m_nmSpace.empty())
		m_nmSpaceAndDelim = m_nmSpace+":";
}

void RVNGSVGDrawingGeneratorPrivate::drawPolySomething(const RVNGPropertyListVector &vertices, bool isClosed)
{
	if (vertices.count() < 2)
		return;

	if (vertices.count() == 2)
	{
		if (!vertices[0]["svg:x"]||!vertices[0]["svg:y"]||!vertices[1]["svg:x"]||!vertices[1]["svg:y"])
			return;
		m_outputSink << "<" << getNamespaceAndDelim() << "line ";
		m_outputSink << "x1=\"" << doubleToString(72*(vertices[0]["svg:x"]->getDouble())) << "\"  y1=\"" << doubleToString(72*(vertices[0]["svg:y"]->getDouble())) << "\" ";
		m_outputSink << "x2=\"" << doubleToString(72*(vertices[1]["svg:x"]->getDouble())) << "\"  y2=\"" << doubleToString(72*(vertices[1]["svg:y"]->getDouble())) << "\"\n";
		writeStyle();
		m_outputSink << "/>\n";
	}
	else
	{
		if (isClosed)
			m_outputSink << "<" << getNamespaceAndDelim() << "polygon ";
		else
			m_outputSink << "<" << getNamespaceAndDelim() << "polyline ";

		m_outputSink << "points=\"";
		for (unsigned i = 0; i < vertices.count(); i++)
		{
			if (!vertices[i]["svg:x"]||!vertices[i]["svg:y"])
				continue;
			m_outputSink << doubleToString(72*(vertices[i]["svg:x"]->getDouble())) << " " << doubleToString(72*(vertices[i]["svg:y"]->getDouble()));
			if (i < vertices.count()-1)
				m_outputSink << ", ";
		}
		m_outputSink << "\"\n";
		writeStyle(isClosed);
		m_outputSink << "/>\n";
	}
}

void RVNGSVGDrawingGeneratorPrivate::setStyle(const RVNGPropertyList &propList)
{
	m_style.clear();
	m_style = propList;

	const librevenge::RVNGPropertyListVector *gradient = propList.child("svg:linearGradient");
	if (!gradient)
		gradient = propList.child("svg:radialGradient");
	m_gradient = gradient ? *gradient : librevenge::RVNGPropertyListVector();
	if (m_style["draw:shadow"] && m_style["draw:shadow"]->getStr() == "visible" && m_style["draw:shadow-opacity"])
	{
		double shadowRed = 0.0;
		double shadowGreen = 0.0;
		double shadowBlue = 0.0;
		if (m_style["draw:shadow-color"])
		{
			unsigned shadowColour = stringToColour(m_style["draw:shadow-color"]->getStr());
			shadowRed = (double)((shadowColour & 0x00ff0000) >> 16)/255.0;
			shadowGreen = (double)((shadowColour & 0x0000ff00) >> 8)/255.0;
			shadowBlue = (double)(shadowColour & 0x000000ff)/255.0;
		}
		m_outputSink << "<" << getNamespaceAndDelim() << "defs>\n";
		m_outputSink << "<" << getNamespaceAndDelim() << "filter filterUnits=\"userSpaceOnUse\" id=\"shadow" << m_shadowIndex++ << "\">";
		m_outputSink << "<" << getNamespaceAndDelim() << "feOffset in=\"SourceGraphic\" result=\"offset\" ";
		if (m_style["draw:shadow-offset-x"])
			m_outputSink << "dx=\"" << doubleToString(72*m_style["draw:shadow-offset-x"]->getDouble()) << "\" ";
		if (m_style["draw:shadow-offset-y"])
			m_outputSink << "dy=\"" << doubleToString(72*m_style["draw:shadow-offset-y"]->getDouble()) << "\" ";
		m_outputSink << "/>";
		m_outputSink << "<" << getNamespaceAndDelim() << "feColorMatrix in=\"offset\" result=\"offset-color\" type=\"matrix\" values=\"";
		m_outputSink << "0 0 0 0 " << doubleToString(shadowRed) ;
		m_outputSink << " 0 0 0 0 " << doubleToString(shadowGreen);
		m_outputSink << " 0 0 0 0 " << doubleToString(shadowBlue);
		if (m_style["draw:opacity"] && m_style["draw:opacity"]->getDouble() < 1)
			m_outputSink << " 0 0 0 "   << doubleToString(m_style["draw:shadow-opacity"]->getDouble()/m_style["draw:opacity"]->getDouble()) << " 0\"/>";
		else
			m_outputSink << " 0 0 0 "   << doubleToString(m_style["draw:shadow-opacity"]->getDouble()) << " 0\"/>";

		m_outputSink << "<" << getNamespaceAndDelim() << "feMerge>";
		m_outputSink << "<" << getNamespaceAndDelim() << "feMergeNode in=\"offset-color\" />";
		m_outputSink << "<" << getNamespaceAndDelim() << "feMergeNode in=\"SourceGraphic\" />";
		m_outputSink << "</" << getNamespaceAndDelim() << "feMerge>";
		m_outputSink << "</" << getNamespaceAndDelim() << "filter>";
		m_outputSink << "</" << getNamespaceAndDelim() << "defs>";
	}

	if (m_style["draw:fill"] && m_style["draw:fill"]->getStr() == "gradient")
	{
		double angle = (m_style["draw:angle"] ? m_style["draw:angle"]->getDouble() : 0.0);
		angle *= -1.0;
		while (angle < 0)
			angle += 360;
		while (angle > 360)
			angle -= 360;

		if (m_style["draw:style"] && (m_style["draw:style"]->getStr() == "radial" || m_style["draw:style"]->getStr() == "rectangular" ||
		                              m_style["draw:style"]->getStr() == "square" || m_style["draw:style"]->getStr() == "ellipsoid"))
		{
			m_outputSink << "<" << getNamespaceAndDelim() << "defs>\n";
			m_outputSink << "  <" << getNamespaceAndDelim() << "radialGradient id=\"grad" << m_gradientIndex++ << "\"";

			if (m_style["svg:cx"])
				m_outputSink << " cx=\"" << m_style["svg:cx"]->getStr().cstr() << "\"";
			else if (m_style["draw:cx"])
				m_outputSink << " cx=\"" << m_style["draw:cx"]->getStr().cstr() << "\"";

			if (m_style["svg:cy"])
				m_outputSink << " cy=\"" << m_style["svg:cy"]->getStr().cstr() << "\"";
			else if (m_style["draw:cy"])
				m_outputSink << " cy=\"" << m_style["draw:cy"]->getStr().cstr() << "\"";
			if (m_style["svg:r"])
				m_outputSink << " r=\"" << m_style["svg:r"]->getStr().cstr() << "\"";
			else if (m_style["draw:border"])
				m_outputSink << " r=\"" << doubleToString((1 - m_style["draw:border"]->getDouble())*100.0) << "%\"";
			else
				m_outputSink << " r=\"100%\"";
			m_outputSink << " >\n";
			if (m_gradient.count())
			{
				for (unsigned c = 0; c < m_gradient.count(); c++)
				{
					RVNGPropertyList const &grad=m_gradient[c];
					m_outputSink << "    <" << getNamespaceAndDelim() << "stop";
					if (grad["svg:offset"])
						m_outputSink << " offset=\"" << grad["svg:offset"]->getStr().cstr() << "\"";
					if (grad["svg:stop-color"])
						m_outputSink << " stop-color=\"" << grad["svg:stop-color"]->getStr().cstr() << "\"";
					if (grad["svg:stop-opacity"])
						m_outputSink << " stop-opacity=\"" << doubleToString(grad["svg:stop-opacity"]->getDouble()) << "\"";
					m_outputSink << "/>" << std::endl;
				}
			}
			else if (m_style["draw:start-color"] && m_style["draw:end-color"])
			{
				m_outputSink << "    <" << getNamespaceAndDelim() << "stop offset=\"0%\"";
				m_outputSink << " stop-color=\"" << m_style["draw:end-color"]->getStr().cstr() << "\"";
				m_outputSink << " stop-opacity=\"" << doubleToString(m_style["librevenge:end-opacity"] ? m_style["librevenge:end-opacity"]->getDouble() : 1) << "\" />" << std::endl;

				m_outputSink << "    <" << getNamespaceAndDelim() << "stop offset=\"100%\"";
				m_outputSink << " stop-color=\"" << m_style["draw:start-color"]->getStr().cstr() << "\"";
				m_outputSink << " stop-opacity=\"" << doubleToString(m_style["librevenge:start-opacity"] ? m_style["librevenge:start-opacity"]->getDouble() : 1) << "\" />" << std::endl;
			}
			m_outputSink << "  </" << getNamespaceAndDelim() << "radialGradient>\n";
			m_outputSink << "</" << getNamespaceAndDelim() << "defs>\n";
		}
		else if (!m_style["draw:style"] || m_style["draw:style"]->getStr() == "linear" || m_style["draw:style"]->getStr() == "axial")
		{
			m_outputSink << "<" << getNamespaceAndDelim() << "defs>\n";
			m_outputSink << "  <" << getNamespaceAndDelim() << "linearGradient id=\"grad" << m_gradientIndex++ << "\" >\n";

			if (m_gradient.count())
			{
				bool canBuildAxial = false;
				if (m_style["draw:style"] && m_style["draw:style"]->getStr() == "axial")
				{
					// check if we can reconstruct the linear offset, ie. if each offset is a valid percent%
					canBuildAxial = true;
					for (unsigned c = 0; c < m_gradient.count(); ++c)
					{
						RVNGPropertyList const &grad=m_gradient[c];
						if (!grad["svg:offset"] || grad["svg:offset"]->getDouble()<0 || grad["svg:offset"]->getDouble() > 1)
						{
							canBuildAxial=false;
							break;
						}
						RVNGString str=grad["svg:offset"]->getStr();
						int len=str.len();
						if (len<1 || str.cstr()[len-1]!='%')
						{
							canBuildAxial=false;
							break;
						}
					}
				}
				if (canBuildAxial)
				{
					for (unsigned long c = m_gradient.count(); c>0 ;)
					{
						RVNGPropertyList const &grad=m_gradient[--c];
						m_outputSink << "    <" << getNamespaceAndDelim() << "stop ";
						if (grad["svg:offset"])
							m_outputSink << "offset=\"" << doubleToString(50.-50.*grad["svg:offset"]->getDouble()) << "%\"";
						if (grad["svg:stop-color"])
							m_outputSink << " stop-color=\"" << grad["svg:stop-color"]->getStr().cstr() << "\"";
						if (grad["svg:stop-opacity"])
							m_outputSink << " stop-opacity=\"" << doubleToString(grad["svg:stop-opacity"]->getDouble()) << "\"";
						m_outputSink << "/>" << std::endl;
					}
					for (unsigned long c = 0; c < m_gradient.count(); ++c)
					{
						RVNGPropertyList const &grad=m_gradient[c];
						if (c==0 && grad["svg:offset"] && grad["svg:offset"]->getDouble() <= 0)
							continue;
						m_outputSink << "    <" << getNamespaceAndDelim() << "stop ";
						if (grad["svg:offset"])
							m_outputSink << "offset=\"" << doubleToString(50.+50.*grad["svg:offset"]->getDouble()) << "%\"";
						if (grad["svg:stop-color"])
							m_outputSink << " stop-color=\"" << grad["svg:stop-color"]->getStr().cstr() << "\"";
						if (grad["svg:stop-opacity"])
							m_outputSink << " stop-opacity=\"" << doubleToString(grad["svg:stop-opacity"]->getDouble()) << "\"";
						m_outputSink << "/>" << std::endl;
					}
				}
				else
				{
					for (unsigned c = 0; c < m_gradient.count(); c++)
					{
						RVNGPropertyList const &grad=m_gradient[c];
						m_outputSink << "    <" << getNamespaceAndDelim() << "stop";
						if (grad["svg:offset"])
							m_outputSink << " offset=\"" << grad["svg:offset"]->getStr().cstr() << "\"";
						if (grad["svg:stop-color"])
							m_outputSink << " stop-color=\"" << grad["svg:stop-color"]->getStr().cstr() << "\"";
						if (grad["svg:stop-opacity"])
							m_outputSink << " stop-opacity=\"" << doubleToString(grad["svg:stop-opacity"]->getDouble()) << "\"";
						m_outputSink << "/>" << std::endl;
					}
				}
			}
			else if (m_style["draw:start-color"] && m_style["draw:end-color"])
			{
				if (!m_style["draw:style"] || m_style["draw:style"]->getStr() == "linear")
				{
					m_outputSink << "    <" << getNamespaceAndDelim() << "stop offset=\"0%\"";
					m_outputSink << " stop-color=\"" << m_style["draw:start-color"]->getStr().cstr() << "\"";
					m_outputSink << " stop-opacity=\"" << doubleToString(m_style["librevenge:start-opacity"] ? m_style["librevenge:start-opacity"]->getDouble() : 1) << "\" />" << std::endl;

					m_outputSink << "    <" << getNamespaceAndDelim() << "stop offset=\"100%\"";
					m_outputSink << " stop-color=\"" << m_style["draw:end-color"]->getStr().cstr() << "\"";
					m_outputSink << " stop-opacity=\"" << doubleToString(m_style["librevenge:end-opacity"] ? m_style["librevenge:end-opacity"]->getDouble() : 1) << "\" />" << std::endl;
				}
				else
				{
					m_outputSink << "    <" << getNamespaceAndDelim() << "stop offset=\"0%\"";
					m_outputSink << " stop-color=\"" << m_style["draw:end-color"]->getStr().cstr() << "\"";
					m_outputSink << " stop-opacity=\"" << doubleToString(m_style["librevenge:end-opacity"] ? m_style["librevenge:end-opacity"]->getDouble() : 1) << "\" />" << std::endl;

					m_outputSink << "    <" << getNamespaceAndDelim() << "stop offset=\"50%\"";
					m_outputSink << " stop-color=\"" << m_style["draw:start-color"]->getStr().cstr() << "\"";
					m_outputSink << " stop-opacity=\"" << doubleToString(m_style["librevenge:start-opacity"] ? m_style["librevenge:start-opacity"]->getDouble() : 1) << "\" />" << std::endl;

					m_outputSink << "    <" << getNamespaceAndDelim() << "stop offset=\"100%\"";
					m_outputSink << " stop-color=\"" << m_style["draw:end-color"]->getStr().cstr() << "\"";
					m_outputSink << " stop-opacity=\"" << doubleToString(m_style["librevenge:end-opacity"] ? m_style["librevenge:end-opacity"]->getDouble() : 1) << "\" />" << std::endl;
				}
			}
			m_outputSink << "  </" << getNamespaceAndDelim() << "linearGradient>\n";

			// not a simple horizontal gradient
			if (angle<270 || angle>270)
			{
				m_outputSink << "  <" << getNamespaceAndDelim() << "linearGradient xlink:href=\"#grad" << m_gradientIndex-1 << "\"";
				m_outputSink << " id=\"grad" << m_gradientIndex++ << "\" ";
				m_outputSink << "x1=\"0\" y1=\"0\" x2=\"0\" y2=\"1\" ";
				m_outputSink << "gradientTransform=\"rotate(" << angle << " .5 .5)\" ";
				m_outputSink << "gradientUnits=\"objectBoundingBox\" >\n";
				m_outputSink << "  </" << getNamespaceAndDelim() << "linearGradient>\n";
			}

			m_outputSink << "</" << getNamespaceAndDelim() << "defs>\n";
		}
	}
	else if (m_style["draw:fill"] && m_style["draw:fill"]->getStr() == "bitmap" && m_style["draw:fill-image"] && m_style["librevenge:mime-type"])
	{
		m_outputSink << "<" << getNamespaceAndDelim() << "defs>\n";
		m_outputSink << "  <" << getNamespaceAndDelim() << "pattern id=\"img" << m_patternIndex++ << "\" patternUnits=\"userSpaceOnUse\" ";
		if (m_style["svg:width"])
			m_outputSink << "width=\"" << doubleToString(72*(m_style["svg:width"]->getDouble())) << "\" ";
		else
			m_outputSink << "width=\"100\" ";

		if (m_style["svg:height"])
			m_outputSink << "height=\"" << doubleToString(72*(m_style["svg:height"]->getDouble())) << "\">" << std::endl;
		else
			m_outputSink << "height=\"100\">" << std::endl;
		m_outputSink << "<" << getNamespaceAndDelim() << "image ";

		if (m_style["svg:x"])
			m_outputSink << "x=\"" << doubleToString(72*(m_style["svg:x"]->getDouble())) << "\" ";
		else
			m_outputSink << "x=\"0\" ";

		if (m_style["svg:y"])
			m_outputSink << "y=\"" << doubleToString(72*(m_style["svg:y"]->getDouble())) << "\" ";
		else
			m_outputSink << "y=\"0\" ";

		if (m_style["svg:width"])
			m_outputSink << "width=\"" << doubleToString(72*(m_style["svg:width"]->getDouble())) << "\" ";
		else
			m_outputSink << "width=\"100\" ";

		if (m_style["svg:height"])
			m_outputSink << "height=\"" << doubleToString(72*(m_style["svg:height"]->getDouble())) << "\" ";
		else
			m_outputSink << "height=\"100\" ";

		m_outputSink << "xlink:href=\"data:" << m_style["librevenge:mime-type"]->getStr().cstr() << ";base64,";
		m_outputSink << m_style["draw:fill-image"]->getStr().cstr();
		m_outputSink << "\" />\n";
		m_outputSink << "  </" << getNamespaceAndDelim() << "pattern>\n";
		m_outputSink << "</" << getNamespaceAndDelim() << "defs>\n";
	}

	// check for arrow and if find some, define a basic arrow
	if (m_style["draw:marker-start-path"])
	{
		m_outputSink << "<" << getNamespaceAndDelim() << "defs>\n";
		m_outputSink << "<" << getNamespaceAndDelim() << "marker id=\"startMarker" << m_arrowStartIndex++ << "\" ";
		m_outputSink << " markerUnits=\"strokeWidth\" orient=\"auto\" markerWidth=\"8\" markerHeight=\"6\"\n";
		m_outputSink << " viewBox=\"0 0 10 10\" refX=\"9\" refY=\"5\">\n";
		m_outputSink << "<" << getNamespaceAndDelim() << "polyline points=\"10,0 0,5 10,10 9,5\" fill=\"solid\" />\n";
		m_outputSink << "</" << getNamespaceAndDelim() << "marker>\n";
		m_outputSink << "</" << getNamespaceAndDelim() << "defs>\n";
	}
	if (m_style["draw:marker-end-path"])
	{
		m_outputSink << "<" << getNamespaceAndDelim() << "defs>\n";
		m_outputSink << "<" << getNamespaceAndDelim() << "marker id=\"endMarker" << m_arrowEndIndex++ << "\" ";
		m_outputSink << " markerUnits=\"strokeWidth\" orient=\"auto\" markerWidth=\"8\" markerHeight=\"6\"\n";
		m_outputSink << " viewBox=\"0 0 10 10\" refX=\"1\" refY=\"5\">\n";
		m_outputSink << "<" << getNamespaceAndDelim() << "polyline points=\"0,0 10,5 0,10 1,5\" fill=\"solid\" />\n";
		m_outputSink << "</" << getNamespaceAndDelim() << "marker>\n";
		m_outputSink << "</" << getNamespaceAndDelim() << "defs>\n";
	}
}

// create "style" attribute based on current pen and brush
void RVNGSVGDrawingGeneratorPrivate::writeStyle(bool /* isClosed */)
{
	m_outputSink << "style=\"";

	double width = 1.0 / 72.0;
	if (m_style["svg:stroke-width"])
	{
		width = m_style["svg:stroke-width"]->getDouble();
#if 0
		// add me in libmspub and libcdr
		if (width <= 0.0 && m_style["draw:stroke"] && m_style["draw:stroke"]->getStr() != "none")
			width = 0.2 / 72.0; // reasonable hairline
#endif
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
			dots1len = 72.*m_style["draw:dots1-length"]->getDouble();
			std::string str = m_style["draw:dots1-length"]->getStr().cstr();
			if (str.size() > 1 && str[str.size()-1]=='%')
				dots1len *=width;
		}
		if (m_style["draw:dots2-length"])
		{
			dots2len = 72.*m_style["draw:dots2-length"]->getDouble();
			std::string str = m_style["draw:dots2-length"]->getStr().cstr();
			if (str.size() > 1 && str[str.size()-1]=='%')
				dots2len *=width;
		}
		if (m_style["draw:distance"])
		{
			gap = 72.*m_style["draw:distance"]->getDouble();
			std::string str = m_style["draw:distance"]->getStr().cstr();
			if (str.size() > 1 && str[str.size()-1]=='%')
				gap *=width;
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


RVNGSVGDrawingGenerator::RVNGSVGDrawingGenerator(RVNGStringVector &vec, const RVNGString &nmSpace) :
	m_pImpl(new RVNGSVGDrawingGeneratorPrivate(vec, nmSpace))
{
}

RVNGSVGDrawingGenerator::~RVNGSVGDrawingGenerator()
{
	delete m_pImpl;
}

void RVNGSVGDrawingGenerator::startDocument(const RVNGPropertyList & /*propList*/) {}
void RVNGSVGDrawingGenerator::endDocument() {}
void RVNGSVGDrawingGenerator::setDocumentMetaData(const RVNGPropertyList & /*propList*/) {}

void RVNGSVGDrawingGenerator::startPage(const RVNGPropertyList &propList)
{
#if 0
	m_pImpl->m_outputSink << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
	m_pImpl->m_outputSink << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"";
	m_pImpl->m_outputSink << " \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";
#endif
	m_pImpl->m_outputSink << "<" << m_pImpl->getNamespaceAndDelim() << "svg version=\"1.1\" xmlns";
	m_pImpl->m_outputSink << (m_pImpl->m_nmSpace.empty() ? "" : ":") << m_pImpl->m_nmSpace << "=\"http://www.w3.org/2000/svg\" ";
	m_pImpl->m_outputSink << "xmlns:xlink=\"http://www.w3.org/1999/xlink\" ";
	if (propList["svg:width"])
		m_pImpl->m_outputSink << "width=\"" << doubleToString(72*(propList["svg:width"]->getDouble())) << "\" ";
	if (propList["svg:height"])
		m_pImpl->m_outputSink << "height=\"" << doubleToString(72*(propList["svg:height"]->getDouble())) << "\"";
	m_pImpl->m_outputSink << " >\n";
}

void RVNGSVGDrawingGenerator::endPage()
{
	m_pImpl->m_outputSink << "</" << m_pImpl->getNamespaceAndDelim() << "svg>\n";
	m_pImpl->m_vec.append(m_pImpl->m_outputSink.str().c_str());
	m_pImpl->m_outputSink.str("");
}


void RVNGSVGDrawingGenerator::startLayer(const RVNGPropertyList &propList)
{
	m_pImpl->m_outputSink << "<" << m_pImpl->getNamespaceAndDelim() << "g";
	if (propList["svg:id"])
		m_pImpl->m_outputSink << " id=\"Layer" << propList["svg:id"]->getStr().cstr() << "\"";
	else
		m_pImpl->m_outputSink << " id=\"Layer" << m_pImpl->m_layerId++ << "\"";
	if (propList["svg:fill-rule"])
		m_pImpl->m_outputSink << " fill-rule=\"" << propList["svg:fill-rule"]->getStr().cstr() << "\"";
	m_pImpl->m_outputSink << " >\n";
}

void RVNGSVGDrawingGenerator::endLayer()
{
	m_pImpl->m_outputSink << "</" << m_pImpl->getNamespaceAndDelim() << "g>\n";
}

void RVNGSVGDrawingGenerator::startEmbeddedGraphics(const RVNGPropertyList & /*propList*/) {}
void RVNGSVGDrawingGenerator::endEmbeddedGraphics() {}

void RVNGSVGDrawingGenerator::openGroup(const RVNGPropertyList & /*propList*/) {}
void RVNGSVGDrawingGenerator::closeGroup() {}

void RVNGSVGDrawingGenerator::setStyle(const RVNGPropertyList &propList)
{
	m_pImpl->setStyle(propList);
}

void RVNGSVGDrawingGenerator::drawRectangle(const RVNGPropertyList &propList)
{
	if (!propList["svg:x"] || !propList["svg:y"] || !propList["svg:width"] || !propList["svg:height"])
		return;
	m_pImpl->m_outputSink << "<" << m_pImpl->getNamespaceAndDelim() << "rect ";
	m_pImpl->m_outputSink << "x=\"" << doubleToString(72*propList["svg:x"]->getDouble()) << "\" y=\"" << doubleToString(72*propList["svg:y"]->getDouble()) << "\" ";
	m_pImpl->m_outputSink << "width=\"" << doubleToString(72*propList["svg:width"]->getDouble()) << "\" height=\"" << doubleToString(72*propList["svg:height"]->getDouble()) << "\" ";
	if (propList["svg:rx"] && propList["svg:rx"]->getDouble() > 0 && propList["svg:ry"] && propList["svg:ry"]->getDouble()>0)
		m_pImpl->m_outputSink << "rx=\"" << doubleToString(72*propList["svg:rx"]->getDouble()) << "\" ry=\"" << doubleToString(72*propList["svg:ry"]->getDouble()) << "\" ";
	m_pImpl->writeStyle();
	m_pImpl->m_outputSink << "/>\n";
}

void RVNGSVGDrawingGenerator::drawEllipse(const RVNGPropertyList &propList)
{
	if (!propList["svg:cx"] || !propList["svg:cy"] || !propList["svg:rx"] || !propList["svg:ry"])
		return;
	m_pImpl->m_outputSink << "<" << m_pImpl->getNamespaceAndDelim() << "ellipse ";
	m_pImpl->m_outputSink << "cx=\"" << doubleToString(72*propList["svg:cx"]->getDouble()) << "\" cy=\"" << doubleToString(72*propList["svg:cy"]->getDouble()) << "\" ";
	m_pImpl->m_outputSink << "rx=\"" << doubleToString(72*propList["svg:rx"]->getDouble()) << "\" ry=\"" << doubleToString(72*propList["svg:ry"]->getDouble()) << "\" ";
	m_pImpl->writeStyle();
	if (propList["librevenge:rotate"] && (propList["librevenge:rotate"]->getDouble()<0||propList["librevenge:rotate"]->getDouble()>0))
		m_pImpl->m_outputSink << " transform=\" rotate(" << doubleToString(-propList["librevenge:rotate"]->getDouble())
		                      << ", " << doubleToString(72*propList["svg:cy"]->getDouble())
		                      << ", " << doubleToString(72*propList["svg:cy"]->getDouble())
		                      << ")\" ";
	m_pImpl->m_outputSink << "/>\n";
}

void RVNGSVGDrawingGenerator::drawPolyline(const RVNGPropertyList &propList)
{
	const RVNGPropertyListVector *vertices = propList.child("svg:points");
	if (vertices && vertices->count())
		m_pImpl->drawPolySomething(*vertices, false);
}

void RVNGSVGDrawingGenerator::drawPolygon(const RVNGPropertyList &propList)
{
	const RVNGPropertyListVector *vertices = propList.child("svg:points");
	if (vertices && vertices->count())
		m_pImpl->drawPolySomething(*vertices, true);
}

void RVNGSVGDrawingGenerator::drawPath(const RVNGPropertyList &propList)
{
	const RVNGPropertyListVector *path = propList.child("svg:d");
	if (!path)
		return;
	m_pImpl->m_outputSink << "<" << m_pImpl->getNamespaceAndDelim() << "path d=\" ";
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
			m_pImpl->m_outputSink << "\nH" << doubleToString(72*(pList["svg:x"]->getDouble()));
		else if (pList["svg:y"] && action[0] == 'V')
			m_pImpl->m_outputSink << "\nV" << doubleToString(72*(pList["svg:y"]->getDouble()));
		else if (coordOk && (action[0] == 'M' || action[0] == 'L' || action[0] == 'T'))
		{
			m_pImpl->m_outputSink << "\n" << action;
			m_pImpl->m_outputSink << doubleToString(72*(pList["svg:x"]->getDouble())) << "," << doubleToString(72*(pList["svg:y"]->getDouble()));
		}
		else if (coord1Ok && (action[0] == 'Q' || action[0] == 'S'))
		{
			m_pImpl->m_outputSink << "\n" << action;
			m_pImpl->m_outputSink << doubleToString(72*(pList["svg:x1"]->getDouble())) << "," << doubleToString(72*(pList["svg:y1"]->getDouble())) << " ";
			m_pImpl->m_outputSink << doubleToString(72*(pList["svg:x"]->getDouble())) << "," << doubleToString(72*(pList["svg:y"]->getDouble()));
		}
		else if (coord2Ok && action[0] == 'C')
		{
			m_pImpl->m_outputSink << "\nC";
			m_pImpl->m_outputSink << doubleToString(72*(pList["svg:x1"]->getDouble())) << "," << doubleToString(72*(pList["svg:y1"]->getDouble())) << " ";
			m_pImpl->m_outputSink << doubleToString(72*(pList["svg:x2"]->getDouble())) << "," << doubleToString(72*(pList["svg:y2"]->getDouble())) << " ";
			m_pImpl->m_outputSink << doubleToString(72*(pList["svg:x"]->getDouble())) << "," << doubleToString(72*(pList["svg:y"]->getDouble()));
		}
		else if (coordOk && pList["svg:rx"] && pList["svg:ry"] && action[0] == 'A')
		{
			m_pImpl->m_outputSink << "\nA";
			m_pImpl->m_outputSink << doubleToString(72*(pList["svg:rx"]->getDouble())) << "," << doubleToString(72*(pList["svg:ry"]->getDouble())) << " ";
			m_pImpl->m_outputSink << doubleToString(pList["librevenge:rotate"] ? pList["librevenge:rotate"]->getDouble() : 0) << " ";
			m_pImpl->m_outputSink << (pList["librevenge:large-arc"] ? pList["librevenge:large-arc"]->getInt() : 1) << ",";
			m_pImpl->m_outputSink << (pList["librevenge:sweep"] ? pList["librevenge:sweep"]->getInt() : 1) << " ";
			m_pImpl->m_outputSink << doubleToString(72*(pList["svg:x"]->getDouble())) << "," << doubleToString(72*(pList["svg:y"]->getDouble()));
		}
		else if (action[0] == 'Z')
		{
			isClosed = true;
			m_pImpl->m_outputSink << "\nZ";
		}
	}

	m_pImpl->m_outputSink << "\" \n";
	m_pImpl->writeStyle(isClosed);
	m_pImpl->m_outputSink << "/>\n";
}

void RVNGSVGDrawingGenerator::drawGraphicObject(const RVNGPropertyList &propList)
{
	if (!propList["librevenge:mime-type"] || propList["librevenge:mime-type"]->getStr().len() <= 0)
		return;
	if (!propList["office:binary-data"])
		return;
	m_pImpl->m_outputSink << "<" << m_pImpl->getNamespaceAndDelim() << "image ";
	if (propList["svg:x"] && propList["svg:y"] && propList["svg:width"] && propList["svg:height"])
	{
		double x(propList["svg:x"]->getDouble());
		double y(propList["svg:y"]->getDouble());
		double width(propList["svg:width"]->getDouble());
		double height(propList["svg:height"]->getDouble());
		bool flipX(propList["draw:mirror-horizontal"] && propList["draw:mirror-horizontal"]->getInt());
		bool flipY(propList["draw:mirror-vertical"] && propList["draw:mirror-vertical"]->getInt());

		m_pImpl->m_outputSink << "x=\"" << doubleToString(72*x) << "\" y=\"" << doubleToString(72*y) << "\" ";
		m_pImpl->m_outputSink << "width=\"" << doubleToString(72*width) << "\" height=\"" << doubleToString(72*height) << "\" ";
		if (flipX || flipY || propList["librevenge:rotate"])
		{
			double xmiddle = x + width / 2.0;
			double ymiddle = y + height / 2.0;
			m_pImpl->m_outputSink << "transform=\"";
			m_pImpl->m_outputSink << " translate(" << doubleToString(72*xmiddle) << ", " << doubleToString(72*ymiddle) << ") ";
			m_pImpl->m_outputSink << " scale(" << (flipX ? "-1" : "1") << ", " << (flipY ? "-1" : "1") << ") ";
			// rotation is around the center of the object's bounding box
			if (propList["librevenge:rotate"])
			{
				double angle(propList["librevenge:rotate"]->getDouble());
				while (angle > 180.0)
					angle -= 360.0;
				while (angle < -180.0)
					angle += 360.0;
				m_pImpl->m_outputSink << " rotate(" << doubleToString(angle) << ") ";
			}
			m_pImpl->m_outputSink << " translate(" << doubleToString(-72*xmiddle) << ", " << doubleToString(-72*ymiddle) << ") ";
			m_pImpl->m_outputSink << "\" ";
		}
	}
	m_pImpl->m_outputSink << "xlink:href=\"data:" << propList["librevenge:mime-type"]->getStr().cstr() << ";base64,";
	m_pImpl->m_outputSink << propList["office:binary-data"]->getStr().cstr();
	m_pImpl->m_outputSink << "\" />\n";
}

void RVNGSVGDrawingGenerator::drawConnector(const RVNGPropertyList &/*propList*/)
{
	// TODO: implement me
}

void RVNGSVGDrawingGenerator::startTextObject(const RVNGPropertyList &propList)
{
	double x = 0.0;
	double y = 0.0;
	double height = 0.0;
	m_pImpl->m_outputSink << "<" << m_pImpl->getNamespaceAndDelim() << "text ";
	if (propList["svg:x"] && propList["svg:y"])
	{
		x = propList["svg:x"]->getDouble();
		y = propList["svg:y"]->getDouble();
	}

	double xmiddle = x;
	double ymiddle = y;

	if (propList["svg:width"])
	{
		double width = propList["svg:width"]->getDouble();
		xmiddle += width / 2.0;
	}

	if (propList["svg:height"])
	{
		height = propList["svg:height"]->getDouble();
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

	m_pImpl->m_outputSink << "x=\"" << doubleToString(72*x) << "\" y=\"" << doubleToString(72*y) << "\"";

	// rotation is around the center of the object's bounding box
	if (propList["librevenge:rotate"] && (propList["librevenge:rotate"]->getDouble()<0||propList["librevenge:rotate"]->getDouble()>0))
	{
		double angle(propList["librevenge:rotate"]->getDouble());
		while (angle > 180.0)
			angle -= 360.0;
		while (angle < -180.0)
			angle += 360.0;
		m_pImpl->m_outputSink << " transform=\"rotate(" << doubleToString(angle) << ", " << doubleToString(72*xmiddle) << ", " << doubleToString(72*ymiddle) << ")\" ";
	}
	m_pImpl->m_outputSink << ">\n";

}

void RVNGSVGDrawingGenerator::endTextObject()
{
	m_pImpl->m_outputSink << "</" << m_pImpl->getNamespaceAndDelim() << "text>\n";
}

void RVNGSVGDrawingGenerator::openOrderedListLevel(const RVNGPropertyList & /*propList*/) {}
void RVNGSVGDrawingGenerator::closeOrderedListLevel() {}

void RVNGSVGDrawingGenerator::openUnorderedListLevel(const RVNGPropertyList & /*propList*/) {}
void RVNGSVGDrawingGenerator::closeUnorderedListLevel() {}

void RVNGSVGDrawingGenerator::openListElement(const RVNGPropertyList & /*propList*/) {}
void RVNGSVGDrawingGenerator::closeListElement() {}

void RVNGSVGDrawingGenerator::defineParagraphStyle(const RVNGPropertyList & /*propList*/) {}
void RVNGSVGDrawingGenerator::openParagraph(const RVNGPropertyList & /*propList*/) {}
void RVNGSVGDrawingGenerator::closeParagraph() {}

void RVNGSVGDrawingGenerator::defineCharacterStyle(const RVNGPropertyList &propList)
{
	if (!propList["librevenge:span-id"])
	{
		RVNG_DEBUG_MSG(("RVNGSVGDrawingGenerator::defineCharacterStyle: can not find the span-id\n"));
		return;
	}
	m_pImpl->m_idSpanMap[propList["librevenge:span-id"]->getInt()]=propList;
}

void RVNGSVGDrawingGenerator::openSpan(const RVNGPropertyList &propList)
{
	RVNGPropertyList pList(propList);
	if (propList["librevenge:span-id"] &&
	        m_pImpl->m_idSpanMap.find(propList["librevenge:span-id"]->getInt())!=m_pImpl->m_idSpanMap.end())
		pList=m_pImpl->m_idSpanMap.find(propList["librevenge:span-id"]->getInt())->second;

	m_pImpl->m_outputSink << "<" << m_pImpl->getNamespaceAndDelim() << "tspan ";
	if (pList["style:font-name"])
		m_pImpl->m_outputSink << "font-family=\"" << pList["style:font-name"]->getStr().cstr() << "\" ";
	if (pList["fo:font-style"])
		m_pImpl->m_outputSink << "font-style=\"" << pList["fo:font-style"]->getStr().cstr() << "\" ";
	if (pList["fo:font-weight"])
		m_pImpl->m_outputSink << "font-weight=\"" << pList["fo:font-weight"]->getStr().cstr() << "\" ";
	if (pList["fo:font-variant"])
		m_pImpl->m_outputSink << "font-variant=\"" << pList["fo:font-variant"]->getStr().cstr() << "\" ";
	if (pList["fo:font-size"])
		m_pImpl->m_outputSink << "font-size=\"" << doubleToString(pList["fo:font-size"]->getDouble()) << "\" ";
	if (pList["fo:color"])
		m_pImpl->m_outputSink << "fill=\"" << pList["fo:color"]->getStr().cstr() << "\" ";
	if (pList["fo:text-transform"])
		m_pImpl->m_outputSink << "text-transform=\"" << pList["fo:text-transform"]->getStr().cstr() << "\" ";
	if (pList["svg:fill-opacity"])
		m_pImpl->m_outputSink << "fill-opacity=\"" << doubleToString(pList["svg:fill-opacity"]->getDouble()) << "\" ";
	if (pList["svg:stroke-opacity"])
		m_pImpl->m_outputSink << "stroke-opacity=\"" << doubleToString(pList["svg:stroke-opacity"]->getDouble()) << "\" ";
	m_pImpl->m_outputSink << ">\n";
}

void RVNGSVGDrawingGenerator::closeSpan()
{
	m_pImpl->m_outputSink << "</" << m_pImpl->getNamespaceAndDelim() << "tspan>\n";
}

void RVNGSVGDrawingGenerator::openLink(const RVNGPropertyList & /*propList*/) {}
void RVNGSVGDrawingGenerator::closeLink() {}

void RVNGSVGDrawingGenerator::insertText(const RVNGString &str)
{
	m_pImpl->m_outputSink << RVNGString::escapeXML(str).cstr();
}

void RVNGSVGDrawingGenerator::insertTab()
{
	m_pImpl->m_outputSink << "\t";
}

void RVNGSVGDrawingGenerator::insertSpace()
{
	m_pImpl->m_outputSink << " ";
}

void RVNGSVGDrawingGenerator::insertLineBreak()
{
	m_pImpl->m_outputSink << "\n";
}

void RVNGSVGDrawingGenerator::insertField(const RVNGPropertyList & /*propList*/) {}

void RVNGSVGDrawingGenerator::startTableObject(const RVNGPropertyList &/*propList*/)
{
	// TODO: implement me
}

void RVNGSVGDrawingGenerator::openTableRow(const RVNGPropertyList &/*propList*/)
{
	// TODO: implement me
}

void RVNGSVGDrawingGenerator::closeTableRow()
{
	// TODO: implement me
}

void RVNGSVGDrawingGenerator::openTableCell(const RVNGPropertyList &/*propList*/)
{
	// TODO: implement me
}

void RVNGSVGDrawingGenerator::closeTableCell()
{
	// TODO: implement me
}

void RVNGSVGDrawingGenerator::insertCoveredTableCell(const RVNGPropertyList &/*propList*/)
{
	// TODO: implement me
}

void RVNGSVGDrawingGenerator::endTableObject()
{
	// TODO: implement me
}

}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
