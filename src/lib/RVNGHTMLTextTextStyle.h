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

#ifndef RVNGHTMLTEXT_TEXTSTYLE_H
#define RVNGHTMLTEXT_TEXTSTYLE_H

#include <ostream>
#include <string>
#include <map>
#include <vector>

#include <librevenge-generators/librevenge-generators.h>

namespace librevenge
{
/** Small class to manage the paragraph style */
class RVNGHTMLTextParagraphStyleManager
{
public:
	//! constructor
	RVNGHTMLTextParagraphStyleManager() : m_contentNameMap(), m_idNameMap()
	{
	}
	//! destructor
	virtual ~RVNGHTMLTextParagraphStyleManager()
	{
	}
	//! define a paragraph style
	void defineParagraph(RVNGPropertyList const &pList);
	//! returns the class name corresponding to a propertylist
	std::string getClass(RVNGPropertyList const &pList);
	//! send the data to the stream
	void send(std::ostream &out);
protected:
	//! convert a property list in a html content string
	std::string getContent(RVNGPropertyList const &pList, bool isList) const;
	//! a map content -> name
	std::map<std::string, std::string> m_contentNameMap;
	//! a map id -> name
	std::map<int, std::string> m_idNameMap;
	//! add data corresponding to the border
	void parseBorders(RVNGPropertyList const &pList, std::ostream &out) const;
private:
	RVNGHTMLTextParagraphStyleManager(RVNGHTMLTextParagraphStyleManager const &orig);
	RVNGHTMLTextParagraphStyleManager operator=(RVNGHTMLTextParagraphStyleManager const &orig);
};

/** Small class to manage the list style */
class RVNGHTMLTextListStyleManager : public RVNGHTMLTextParagraphStyleManager
{
public:
	struct List
	{
		//! constructor
		List() : m_contentsList(), m_level(0)
		{
		}
		//! destructor
		~List()
		{
		}
		//! set the property correspond to a level
		void setLevel(int lvl, RVNGPropertyList const &property, bool ordered);
		//! open a new level
		void openLevel() const
		{
			m_level++;
		}
		//! open a new level
		void closeLevel() const
		{
			if (m_level <= 0)
				return;
			m_level--;
		}

		//! return the content string
		std::string str() const;
	protected:
		//! the properties
		std::vector<std::string> m_contentsList;
		//! the actual list level
		mutable int m_level;
	};
	//! constructor
	RVNGHTMLTextListStyleManager() : RVNGHTMLTextParagraphStyleManager(), m_levelNameMap(),
		m_idListMap(), m_actualIdStack()
	{
	}
	//! destructor
	~RVNGHTMLTextListStyleManager()
	{
	}
	//! add a level to the corresponding list
	void defineLevel(RVNGPropertyList const &property, bool ordered);
	//! returns the class name corresponding to a propertylist
	std::string openLevel(RVNGPropertyList const &pList, bool ordered);
	//! close a level
	void closeLevel();
	//! returns the classname corresponding to a list element
	std::string getClass(RVNGPropertyList const &pList);

	//! send the data to the stream
	void send(std::ostream &out);
protected:
	//! a map content -> list level name
	std::map<std::string, std::string> m_levelNameMap;
	//! a map listId -> list
	std::map<int, List> m_idListMap;
	//! the actual list id
	std::vector<int> m_actualIdStack;
private:
	RVNGHTMLTextListStyleManager(RVNGHTMLTextListStyleManager const &orig);
	RVNGHTMLTextListStyleManager operator=(RVNGHTMLTextListStyleManager const &orig);
};

/** Small class to manage the span style */
class RVNGHTMLTextSpanStyleManager
{
public:
	//! constructor
	RVNGHTMLTextSpanStyleManager() : m_contentNameMap(), m_idNameMap()
	{
	}
	//! destructor
	~RVNGHTMLTextSpanStyleManager()
	{
	}
	//! define a span style
	void defineSpan(RVNGPropertyList const &pList);
	//! returns the class name corresponding to a propertylist
	std::string getClass(RVNGPropertyList const &pList);
	//! send the data to the stream
	void send(std::ostream &out);
protected:
	//! convert a property list in a html content string
	std::string getContent(RVNGPropertyList const &pList) const;
	//! add data corresponding to a text position in out
	void parseTextPosition(char const *value, std::ostream &out) const;
	//! add data corresponding to the line decoration
	void parseDecorations(RVNGPropertyList const &pList, std::ostream &out) const;
	//! a map content -> name
	std::map<std::string, std::string> m_contentNameMap;
	//! a map id -> name
	std::map<int, std::string> m_idNameMap;

private:
	RVNGHTMLTextSpanStyleManager(RVNGHTMLTextSpanStyleManager const &orig);
	RVNGHTMLTextSpanStyleManager operator=(RVNGHTMLTextSpanStyleManager const &orig);
};
}

#endif
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
