/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2002-2004 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2004 Fridrich Strba (fridrich.strba@bluewin.ch)
 * Copyright (C) 2005 Net Integration Technologies (http://www.net-itech.com)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 */

#include <cassert>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>

#include <boost/lexical_cast.hpp>

#include "RVNGHTMLTextTableStyle.h"
#include "RVNGHTMLTextTextStyle.h"

#include <librevenge-generators/librevenge-generators.h>

#include "librevenge_internal.h"

namespace librevenge
{

struct RVNGHTMLTextStream;

//! a zone to regroup footnote/endnote,... data
struct RVNGHTMLTextZone
{
	friend struct RVNGHTMLTextStream;

	//! the different zone
	enum Type { Z_Comment=0, Z_EndNote, Z_FootNote, Z_Main, Z_MetaData, Z_TextBox, Z_Unknown, Z_NumZones= Z_Unknown+1};
	//! constructor for basic stream
	RVNGHTMLTextZone(Type tp=Z_Unknown) : m_type(tp), m_actualId(0), m_stringList()
	{
	}
	//! the type
	Type type() const
	{
		return m_type;
	}
	//! the type
	void setType(Type tp)
	{
		m_type=tp;
	}
	//! returns a new stream corresponding to this zone
	RVNGHTMLTextStream *getNewStream();
	//! delete a stream corresponding to this zone
	void deleteStream(RVNGHTMLTextStream *stream);
	//! returns true if there is no data
	bool isEmpty() const
	{
		for (size_t i = 0; i < m_stringList.size(); i++)
			if (m_stringList[i].size())
				return false;
		return true;
	}
	//! send the zone data
	void send(std::ostream &out) const
	{
		if (isEmpty() || m_type==Z_Unknown || m_type==Z_Main)
			return;
		if (m_type!=Z_MetaData)
			out << "<hr>\n";
		if (m_type==Z_MetaData)
		{
			for (size_t i = 0; i < m_stringList.size(); i++)
				out << m_stringList[i];
			return;
		}
		if (m_type==Z_TextBox)
		{
			out << "<p><b>TEXT BOXES</b></p><hr>\n";
			for (size_t i = 0; i < m_stringList.size(); i++)
				out << m_stringList[i] << "<hr>\n";
			return;
		}
		for (size_t i = 0; i < m_stringList.size(); i++)
		{
			std::string const &str=m_stringList[i];
			out << str << "\n";
			// check if we need to add a return line
			size_t lastComPos=str.rfind('<');
			if (lastComPos!=std::string::npos)
			{
				if (str.compare(lastComPos,4,"</p>")==0 ||
				        str.compare(lastComPos,5,"</ul>")==0 ||
				        str.compare(lastComPos,5,"</ol>")==0 ||
				        str.compare(lastComPos,4,"<br>")==0)
					continue;
			}
			out << "<br>\n";
		}
	}

protected:
	//! return a label corresponding to the zone
	std::string label(int id) const;
	//! the zone type
	Type m_type;
	//! the actual id
	mutable int m_actualId;
	//! the list of data string
	std::vector<std::string> m_stringList;
private:
	RVNGHTMLTextZone(RVNGHTMLTextZone const &orig);
	RVNGHTMLTextZone operator=(RVNGHTMLTextZone const &orig);
};

struct RVNGHTMLTextStream
{
	//! constructor
	RVNGHTMLTextStream(RVNGHTMLTextZone *zone): m_zone(zone), m_zoneId(0), m_stream(), m_delayedLabel("")
	{
		if (m_zone)
			m_zoneId=m_zone->m_actualId++;
	}
	//! destructor
	~RVNGHTMLTextStream() { }
	//! add a label called on main and a label in this ( delayed to allow openParagraph to be called )
	void addLabel(std::ostream &output)
	{
		std::string lbl=label();
		if (!lbl.length())
			return;
		output << "<sup id=\"called" << lbl << "\"><a href=\"#data" << lbl << "\">" << lbl << "</a></sup>";
		flush();
		std::stringstream ss;
		ss << "<sup id=\"data" << lbl << "\"><a href=\"#called" << lbl << "\">" << lbl << "</a></sup>";
		m_delayedLabel=ss.str();
	}
	//! flush delayed label, ...
	void flush()
	{
		if (m_delayedLabel.length())
		{
			m_stream << m_delayedLabel;
			m_delayedLabel="";
		}
	}
	//! return the stream
	std::ostream &stream()
	{
		return m_stream;
	}
	//! send the data to the zone
	void send()
	{
		if (!m_zone || m_zone->m_type==RVNGHTMLTextZone::Z_Main)
		{
			RVNG_DEBUG_MSG(("RVNGHTMLTextStream::send: must not be called\n"));
			return;
		}
		flush();
		if (m_zone->m_stringList.size() <= size_t(m_zoneId))
			m_zone->m_stringList.resize(size_t(m_zoneId)+1);
		m_zone->m_stringList[size_t(m_zoneId)]=m_stream.str();
	}
	//! send the data to the zone
	void sendMain(std::ostream &output)
	{
		flush();
		output << m_stream.str() << "\n";
	}
protected:
	//! return the stream label
	std::string label() const
	{
		if (!m_zone || m_zone->m_type==RVNGHTMLTextZone::Z_Main)
		{
			RVNG_DEBUG_MSG(("RVNGHTMLTextStream::label: must not be called\n"));
			return "";
		}
		return m_zone->label(m_zoneId);
	}
	//! a zone
	RVNGHTMLTextZone *m_zone;
	//! the zone id
	int m_zoneId;
	//! the stream
	std::ostringstream m_stream;
	//! the label
	std::string m_delayedLabel;
private:
	RVNGHTMLTextStream(RVNGHTMLTextStream const &orig);
	RVNGHTMLTextStream operator=(RVNGHTMLTextStream const &orig);
};

RVNGHTMLTextStream *RVNGHTMLTextZone::getNewStream()
{
	return new RVNGHTMLTextStream(this);
}

void RVNGHTMLTextZone::deleteStream(RVNGHTMLTextStream *stream)
{
	if (stream) delete stream;
}

std::string RVNGHTMLTextZone::label(int id) const
{
	char c=0;
	switch (m_type)
	{
	case Z_Comment:
		c='C';
		break;
	case Z_EndNote:
		c='E';
		break;
	case Z_FootNote:
		c='F';
		break;
	case Z_TextBox:
		c='T';
		break;
	case Z_Main:
	case Z_MetaData:
	case Z_Unknown:
	case Z_NumZones:
	default:
		break;
	}
	if (c==0)
		return "";
	std::stringstream s;
	s << c << id+1;
	return s.str();
}

//! the internal state of a html document generator
struct RVNGHTMLTextGeneratorImpl
{
	//! constructor
	RVNGHTMLTextGeneratorImpl(RVNGString &document) : m_document(document), m_actualPage(0), m_ignore(false), m_listManager(), m_paragraphManager(), m_spanManager(), m_tableManager(), m_currentParaElement(), m_actualStream(), m_streamStack()
	{
		for (int i = 0; i < RVNGHTMLTextZone::Z_NumZones; ++i)
			m_zones[i].setType(RVNGHTMLTextZone::Type(i));
		m_actualStream=m_zones[RVNGHTMLTextZone::Z_Main].getNewStream();
	}
	//! destructor
	~RVNGHTMLTextGeneratorImpl()
	{
		for (size_t i=0; i<m_streamStack.size(); ++i)
		{
			if (m_streamStack[i]) delete m_streamStack[i];
		}
		if (m_actualStream) delete m_actualStream;
	}

	//! returns the actual output ( sending delayed data if needed)
	std::ostream &output(bool sendDelayed=true)
	{
		if (sendDelayed)
			m_actualStream->flush();
		return m_actualStream->stream();
	}
	//! returns the actual stream
	RVNGHTMLTextStream &stream()
	{
		return *m_actualStream;
	}
	void push(RVNGHTMLTextZone::Type type)
	{
		m_streamStack.push_back(m_actualStream);
		if (type==RVNGHTMLTextZone::Z_Main || type==RVNGHTMLTextZone::Z_NumZones)
		{
			RVNG_DEBUG_MSG(("RVNGHTMLTextGeneratorImpl::push: can not push the main zone\n"));
			type=RVNGHTMLTextZone::Z_Unknown;
		}
		m_actualStream=m_zones[type].getNewStream();
	}
	void pop()
	{
		if (m_streamStack.size() <= 0)
		{
			RVNG_DEBUG_MSG(("RVNGHTMLTextGenerator::pop: can not pop stream\n"));
			return;
		}
		if (m_actualStream)
		{
			m_actualStream->send();
			delete m_actualStream;
		}
		m_actualStream = m_streamStack.back();
		m_streamStack.pop_back();
	}
	void sendMetaData(std::ostream &out)
	{
		m_zones[RVNGHTMLTextZone::Z_MetaData].send(out);
	}
	void flushUnsent(std::ostream &out)
	{
		if (m_streamStack.size())
		{
			RVNG_DEBUG_MSG(("RVNGHTMLTextGenerator::flushUnsent: the stream stack is not empty\n"));
			while (m_streamStack.size())
				pop();
		}
		// first send the main data
		if (!m_actualStream)
		{
			RVNG_DEBUG_MSG(("RVNGHTMLTextGenerator::flushUnsent: can not find the main block\n"));
		}
		else
			m_actualStream->sendMain(out);
		m_zones[RVNGHTMLTextZone::Z_Comment].send(out);
		m_zones[RVNGHTMLTextZone::Z_FootNote].send(out);
		m_zones[RVNGHTMLTextZone::Z_EndNote].send(out);
		m_zones[RVNGHTMLTextZone::Z_TextBox].send(out);
	}

	RVNGString &m_document;

	int m_actualPage;
	bool m_ignore;

	RVNGHTMLTextListStyleManager m_listManager;
	RVNGHTMLTextParagraphStyleManager m_paragraphManager;
	RVNGHTMLTextSpanStyleManager m_spanManager;
	RVNGHTMLTextTableStyleManager m_tableManager;

	std::string m_currentParaElement;

protected:
	RVNGHTMLTextStream *m_actualStream;
	std::vector<RVNGHTMLTextStream *> m_streamStack;

	RVNGHTMLTextZone m_zones[RVNGHTMLTextZone::Z_NumZones];
private:
	RVNGHTMLTextGeneratorImpl(RVNGHTMLTextGeneratorImpl const &orig);
	RVNGHTMLTextGeneratorImpl operator=(RVNGHTMLTextGeneratorImpl const &orig);
};

RVNGHTMLTextGenerator::RVNGHTMLTextGenerator(RVNGString &document) : m_impl(new RVNGHTMLTextGeneratorImpl(document))
{
}

RVNGHTMLTextGenerator::~RVNGHTMLTextGenerator()
{
	if (m_impl) delete m_impl;
}

void RVNGHTMLTextGenerator::setDocumentMetaData(const RVNGPropertyList &propList)
{
	m_impl->push(RVNGHTMLTextZone::Z_MetaData);
	std::ostream &meta=m_impl->output();
	static char const *wpdMetaFields[9]=
	{
		"meta:initial-creator", "dc:creator", "dc:subject", "dc:publisher", "meta:keywords",
		"dc:language", "dc:description", "librevenge:descriptive-name", "librevenge:descriptive-type"
	};
	static char const *metaFields[9]=
	{
		"author", "typist", "subject", "publisher", "keywords",
		"language", "abstract", "descriptive-name", "descriptive-type"
	};
	for (int i = 0; i < 9; i++)
	{
		if (!propList[wpdMetaFields[i]])
			continue;
		meta << "<meta name=\"" << metaFields[i] << "\" content=\"" << propList[wpdMetaFields[i]]->getStr().cstr() << "\">" << std::endl;
	}
	if (propList["librevenge:descriptive-name"])
		meta << "<title>" << propList["librevenge:descriptive-name"]->getStr().cstr() << "</title>" << std::endl;
	else
		meta << "<title></title>" << std::endl;
	m_impl->pop();
}

void RVNGHTMLTextGenerator::startDocument(const RVNGPropertyList &)
{
}

void RVNGHTMLTextGenerator::endDocument()
{
	std::ostringstream out;
	out << "<!DOCTYPE HTML>" << std::endl;
	out << "<html>" << std::endl;
	out << "<head>" << std::endl;
	out << "<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\" >" << std::endl;
	m_impl->sendMetaData(out);
	out << "<style>" << std::endl;
	m_impl->m_listManager.send(out);
	m_impl->m_paragraphManager.send(out);
	m_impl->m_spanManager.send(out);
	m_impl->m_tableManager.send(out);
	out << "</style>" << std::endl;
	out << "</head>" << std::endl;
	out << "<body>" << std::endl;
	m_impl->flushUnsent(out);
	out << "</body>" << std::endl;
	out << "</html>" << std::endl;

	m_impl->m_document.append(out.str().c_str());
}

void RVNGHTMLTextGenerator::defineEmbeddedFont(const RVNGPropertyList &/*propList*/)
{
}

void RVNGHTMLTextGenerator::openPageSpan(const RVNGPropertyList & /* propList */)
{
	m_impl->m_actualPage++;
}

void RVNGHTMLTextGenerator::closePageSpan()
{
}

void RVNGHTMLTextGenerator::definePageStyle(const RVNGPropertyList &) {}

void RVNGHTMLTextGenerator::openHeader(const RVNGPropertyList & /* propList */)
{
	m_impl->m_ignore = true;
}

void RVNGHTMLTextGenerator::closeHeader()
{
	m_impl->m_ignore = false;
}


void RVNGHTMLTextGenerator::openFooter(const RVNGPropertyList & /* propList */)
{
	m_impl->m_ignore = true;
}

void RVNGHTMLTextGenerator::closeFooter()
{
	m_impl->m_ignore = false;
}

void RVNGHTMLTextGenerator::defineSectionStyle(const RVNGPropertyList &) {}
void RVNGHTMLTextGenerator::openSection(const RVNGPropertyList & /* propList */) {}
void RVNGHTMLTextGenerator::closeSection() {}

void RVNGHTMLTextGenerator::defineParagraphStyle(const RVNGPropertyList &propList)
{
	RVNGPropertyList pList(propList);
	if (pList["text:outline-level"])
		pList.remove("text:outline-level");
	m_impl->m_paragraphManager.defineParagraph(pList);
}

void RVNGHTMLTextGenerator::openParagraph(const RVNGPropertyList &propList)
{
	if (m_impl->m_ignore)
		return;

	m_impl->m_currentParaElement = "p";

	const RVNGProperty *const outlineLevel = propList["text:outline-level"];
	if (outlineLevel)
	{
		const int level = outlineLevel->getInt();
		if ((0 < level) && (7 > level))
			m_impl->m_currentParaElement = "h" + boost::lexical_cast<std::string>(level);
	}

	m_impl->output(false) << "<" << m_impl->m_currentParaElement
	                      << " class=\"" << m_impl->m_paragraphManager.getClass(propList) << "\">";
}

void RVNGHTMLTextGenerator::closeParagraph()
{
	if (m_impl->m_ignore)
		return;

	assert(!m_impl->m_currentParaElement.empty());
	m_impl->output() << "</" << m_impl->m_currentParaElement << ">" << std::endl;
}

void RVNGHTMLTextGenerator::defineCharacterStyle(const RVNGPropertyList &propList)
{
	m_impl->m_spanManager.defineSpan(propList);
}

void RVNGHTMLTextGenerator::openSpan(const RVNGPropertyList &propList)
{
	if (m_impl->m_ignore)
		return;

	m_impl->output() << "<span class=\"" << m_impl->m_spanManager.getClass(propList) << "\">";
}

void RVNGHTMLTextGenerator::closeSpan()
{
	if (m_impl->m_ignore)
		return;
	m_impl->output() << "</span>" << std::endl;
}

void RVNGHTMLTextGenerator::openLink(const RVNGPropertyList &propList)
{
	if (m_impl->m_ignore)
		return;

	if (!propList["librevenge:type"])
	{
		RVNG_DEBUG_MSG(("RVNGHTMLTextGenerator::openLink: librevenge:type: not filled, suppose link\n"));
	}
	m_impl->output() << "<a ";
	if (propList["xlink:href"])
		m_impl->output() << "href=\"" << RVNGString::escapeXML(propList["xlink:href"]->getStr()).cstr() << "\"";
	m_impl->output() << ">";
}

void RVNGHTMLTextGenerator::closeLink()
{
	if (m_impl->m_ignore)
		return;
	m_impl->output() << "</a>" << std::endl;
}

void RVNGHTMLTextGenerator::insertTab()
{
	if (m_impl->m_ignore)
		return;

	// Does not have a lot of effect since tabs in html are ignorable white-space
	m_impl->output() << "\t";
}

void RVNGHTMLTextGenerator::insertLineBreak()
{
	if (m_impl->m_ignore)
		return;
	m_impl->output() << "<br>" << std::endl;
}

void RVNGHTMLTextGenerator::insertField(const RVNGPropertyList & /* propList */)
{
	if (m_impl->m_ignore)
		return;
	m_impl->output() << "#" << std::endl;
}

void RVNGHTMLTextGenerator::insertText(const RVNGString &text)
{
	if (m_impl->m_ignore)
		return;
	m_impl->output() << RVNGString::escapeXML(text).cstr();
}

void RVNGHTMLTextGenerator::insertSpace()
{
	if (m_impl->m_ignore)
		return;
	m_impl->output() << "&nbsp;";
}

void RVNGHTMLTextGenerator::openOrderedListLevel(const RVNGPropertyList &propList)
{
	if (m_impl->m_ignore)
		return;
	m_impl->m_listManager.defineLevel(propList, true);
	// fixme: if level is > 1, we must first insert a div here
	m_impl->output(false) << "<ol class=\"" << m_impl->m_listManager.openLevel(propList, true) << "\">\n";
}

void RVNGHTMLTextGenerator::closeOrderedListLevel()
{
	if (m_impl->m_ignore)
		return;
	m_impl->m_listManager.closeLevel();
	m_impl->output() << "</ol>" << std::endl;
}

void RVNGHTMLTextGenerator::openUnorderedListLevel(const RVNGPropertyList &propList)
{
	if (m_impl->m_ignore)
		return;
	m_impl->m_listManager.defineLevel(propList, false);
	// fixme: if level is > 1, we must first insert a div here
	m_impl->output(false) << "<ul class=\"" << m_impl->m_listManager.openLevel(propList, false) << "\">\n";
}

void RVNGHTMLTextGenerator::closeUnorderedListLevel()
{
	if (m_impl->m_ignore)
		return;
	m_impl->m_listManager.closeLevel();
	m_impl->output() << "</ul>" << std::endl;
}


void RVNGHTMLTextGenerator::openListElement(const RVNGPropertyList &propList)
{
	if (m_impl->m_ignore)
		return;
	m_impl->output(false) << "<li class=\"" << m_impl->m_listManager.getClass(propList) << "\">";
}

void RVNGHTMLTextGenerator::closeListElement()
{
	if (m_impl->m_ignore)
		return;
	m_impl->output() << "</li>" << std::endl;
}

void RVNGHTMLTextGenerator::openFootnote(const RVNGPropertyList &)
{
	if (m_impl->m_ignore)
		return;
	std::ostream &out=m_impl->output();
	m_impl->push(RVNGHTMLTextZone::Z_FootNote);
	m_impl->stream().addLabel(out);
}

void RVNGHTMLTextGenerator::closeFootnote()
{
	if (m_impl->m_ignore)
		return;
	m_impl->pop();
}

void RVNGHTMLTextGenerator::openEndnote(const RVNGPropertyList &)
{
	if (m_impl->m_ignore)
		return;
	std::ostream &out=m_impl->output();
	m_impl->push(RVNGHTMLTextZone::Z_EndNote);
	m_impl->stream().addLabel(out);
}

void RVNGHTMLTextGenerator::closeEndnote()
{
	if (m_impl->m_ignore)
		return;
	m_impl->pop();
}

void RVNGHTMLTextGenerator::openComment(const RVNGPropertyList & /*propList*/)
{
	if (m_impl->m_ignore)
		return;
	std::ostream &out=m_impl->output();
	m_impl->push(RVNGHTMLTextZone::Z_Comment);
	m_impl->stream().addLabel(out);
}

void RVNGHTMLTextGenerator::closeComment()
{
	if (m_impl->m_ignore)
		return;
	m_impl->pop();
}

void RVNGHTMLTextGenerator::openTextBox(const RVNGPropertyList & /*propList*/)
{
	if (m_impl->m_ignore)
		return;
	std::ostream &out=m_impl->output();
	m_impl->push(RVNGHTMLTextZone::Z_TextBox);
	m_impl->stream().addLabel(out);
}

void RVNGHTMLTextGenerator::closeTextBox()
{
	if (m_impl->m_ignore)
		return;
	m_impl->pop();
}

void RVNGHTMLTextGenerator::openTable(const RVNGPropertyList &propList)
{
	if (m_impl->m_ignore)
		return;

	const librevenge::RVNGPropertyListVector *columns = propList.child("librevenge:table-columns");
	if (columns)
		m_impl->m_tableManager.openTable(*columns);
	m_impl->output() << "<table>" << std::endl;
	m_impl->output() << "<tbody>" << std::endl;
}

void RVNGHTMLTextGenerator::openTableRow(const RVNGPropertyList &propList)
{
	if (m_impl->m_ignore)
		return;
	m_impl->output() << "<tr class=\"" << m_impl->m_tableManager.getRowClass(propList) << "\">\n";
}

void RVNGHTMLTextGenerator::closeTableRow()
{
	if (m_impl->m_ignore)
		return;
	m_impl->output() << "</tr>" << std::endl;
}

void RVNGHTMLTextGenerator::openTableCell(const RVNGPropertyList &propList)
{
	if (m_impl->m_ignore)
		return;
	std::ostream &out=m_impl->output();
	out << "<td class=\"" << m_impl->m_tableManager.getCellClass(propList) << "\"";
	if (propList["table:number-columns-spanned"])
		out << " colspan=\"" << propList["table:number-columns-spanned"]->getInt() << "\"";
	if (propList["table:number-rows-spanned"])
		out << " rowspan=\"" << propList["table:number-rows-spanned"]->getInt() << "\"";
	out << ">" << std::endl;

}

void RVNGHTMLTextGenerator::closeTableCell()
{
	if (m_impl->m_ignore)
		return;
	m_impl->output() << "</td>" << std::endl;
}

void RVNGHTMLTextGenerator::insertCoveredTableCell(const RVNGPropertyList & /* propList */) {}

void RVNGHTMLTextGenerator::closeTable()
{
	if (m_impl->m_ignore)
		return;
	m_impl->output() << "</tbody>" << std::endl;
	m_impl->output() << "</table>" << std::endl;
	m_impl->m_tableManager.closeTable();
}

void RVNGHTMLTextGenerator::openFrame(const RVNGPropertyList & /* propList */) {}
void RVNGHTMLTextGenerator::closeFrame() {}

void RVNGHTMLTextGenerator::openGroup(const librevenge::RVNGPropertyList & /* propList */) {}
void RVNGHTMLTextGenerator::closeGroup() {}

void RVNGHTMLTextGenerator::defineGraphicStyle(const librevenge::RVNGPropertyList & /* propList */) {}
void RVNGHTMLTextGenerator::drawRectangle(const librevenge::RVNGPropertyList & /* propList */) {}
void RVNGHTMLTextGenerator::drawEllipse(const librevenge::RVNGPropertyList & /* propList */) {}
void RVNGHTMLTextGenerator::drawPolygon(const librevenge::RVNGPropertyList & /* propList */) {}
void RVNGHTMLTextGenerator::drawPolyline(const librevenge::RVNGPropertyList & /* propList */) {}
void RVNGHTMLTextGenerator::drawPath(const librevenge::RVNGPropertyList & /* propList */) {}
void RVNGHTMLTextGenerator::drawConnector(const librevenge::RVNGPropertyList & /* propList */) {}

void RVNGHTMLTextGenerator::insertBinaryObject(const RVNGPropertyList & /* propList */) {}
void RVNGHTMLTextGenerator::insertEquation(const RVNGPropertyList & /* propList */) {}

}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
