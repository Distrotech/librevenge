/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2004 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2005 Net Integration Technologies (http://www.net-itech.com)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 *
 * For further information visit http://librevenge.sourceforge.net
 */

#include <ctype.h>
#include <locale.h>

#include <sstream>
#include <string>

#include <librevenge/librevenge.h>

namespace
{

static RVNGString doubleToString(const double value)
{
	RVNGString tempString;
	if (value < 0.0001 && value > -0.0001)
		tempString.sprintf("0.0000");
	else
		tempString.sprintf("%.4f", value);
#ifndef __ANDROID__
	std::string decimalPoint(localeconv()->decimal_point);
#else
	std::string decimalPoint(".");
#endif
	if ((decimalPoint.size() == 0) || (decimalPoint == "."))
		return tempString;
	std::string stringValue(tempString.cstr());
	if (!stringValue.empty())
	{
		std::string::size_type pos;
		while ((pos = stringValue.find(decimalPoint)) != std::string::npos)
			stringValue.replace(pos,decimalPoint.size(),".");
	}
	return RVNGString(stringValue.c_str());
}

} // anonymous namespace

class RVNGStringProperty : public RVNGProperty
{
public:
	RVNGStringProperty(const RVNGString &str);
	RVNGStringProperty(const char *str);
	~RVNGStringProperty() {}
	virtual int getInt() const
	{
		double val;
		RVNGUnit unit;
		if (!findDouble(val, unit)) return 0;
		return int(val);
	}
	virtual double getDouble() const
	{
		double val;
		RVNGUnit unit;
		if (!findDouble(val, unit)) return 0;
		return val;
	}
	virtual RVNGUnit getUnit() const
	{
		double val;
		RVNGUnit unit=RVNG_UNIT_ERROR;
		findDouble(val, unit);
		return unit;
	}
	virtual RVNGString getStr() const;
	virtual RVNGProperty *clone() const;

private:
	bool findDouble(double &res, RVNGUnit &unit) const;
	RVNGString m_str;
};

class RVNGIntProperty : public RVNGProperty
{
public:
	RVNGIntProperty(const int val);
	~RVNGIntProperty() {}
	virtual int getInt() const;
	virtual double getDouble() const;
	virtual RVNGUnit getUnit() const
	{
		return RVNG_GENERIC;
	}
	virtual RVNGString getStr() const;
	virtual RVNGProperty *clone() const;

private:
	int m_val;
};

class RVNGBoolProperty : public RVNGIntProperty
{
public:
	RVNGBoolProperty(const bool val);
	~RVNGBoolProperty() {}
	virtual RVNGUnit getUnit() const
	{
		return RVNG_UNIT_ERROR;
	}
	virtual RVNGString getStr() const;
	virtual RVNGProperty *clone() const;
};

class RVNGDoubleProperty : public RVNGProperty
{
public:
	RVNGDoubleProperty(const double val);
	~RVNGDoubleProperty() {}
	virtual int getInt() const;
	virtual double getDouble() const;
	virtual RVNGUnit getUnit() const
	{
		return RVNG_GENERIC;
	}
	virtual RVNGString getStr() const;
	virtual RVNGProperty *clone() const;

private:
	double m_val;
};

class RVNGInchProperty : public RVNGDoubleProperty
{
public:
	RVNGInchProperty(const double val);
	~RVNGInchProperty() {}
	virtual RVNGUnit getUnit() const
	{
		return RVNG_INCH;
	}
	virtual RVNGString getStr() const;
	virtual RVNGProperty *clone() const;
};

class RVNGPercentProperty : public RVNGDoubleProperty
{
public:
	RVNGPercentProperty(const double val);
	~RVNGPercentProperty() {}
	virtual RVNGUnit getUnit() const
	{
		return RVNG_PERCENT;
	}
	virtual RVNGString getStr() const;
	virtual RVNGProperty *clone() const;
};

class RVNGPointProperty : public RVNGDoubleProperty
{
public:
	RVNGPointProperty(const double val);
	~RVNGPointProperty() {}
	virtual RVNGUnit getUnit() const
	{
		return RVNG_POINT;
	}
	virtual RVNGString getStr() const;
	virtual RVNGProperty *clone() const;
};

class RVNGTwipProperty : public RVNGDoubleProperty
{
public:
	RVNGTwipProperty(const double val);
	~RVNGTwipProperty() {}
	virtual RVNGUnit getUnit() const
	{
		return RVNG_TWIP;
	}
	virtual RVNGString getStr() const;
	virtual RVNGProperty *clone() const;
};

class RVNGGenericProperty : public RVNGDoubleProperty
{
public:
	RVNGGenericProperty(const double val);
	~RVNGGenericProperty() {}
	virtual RVNGString getStr() const;
	virtual RVNGProperty *clone() const;
};

RVNGProperty::~RVNGProperty()
{
}

RVNGStringProperty::RVNGStringProperty(const RVNGString &str) :
	m_str(str)
{
}
RVNGStringProperty::RVNGStringProperty(const char *str) :
	m_str(str)
{
}

bool RVNGStringProperty::findDouble(double &res, RVNGUnit &unit) const
{
	if (m_str.len()==0) return false;
	char firstC=m_str.cstr()[0];
	if (firstC!='-' && firstC!='.' && (firstC<'0' || firstC>'9'))
		return false;

	std::istringstream iss(m_str.cstr());
	res = 0.0;
	iss >> res;
	if (iss.fail())
		return false;
	if (iss.eof() || iss.peek() == std::char_traits<char>::eof())
	{
		unit=RVNG_GENERIC;
		return true;
	}
	std::string remain;
	iss >> remain;
	if (iss.peek() != std::char_traits<char>::eof())
		return false;
	if (remain=="pt")
		unit=RVNG_PERCENT;
	else if (remain=="in")
		unit=RVNG_INCH;
	else if (remain=="%")
	{
		res *= 100.;
		unit=RVNG_PERCENT;
	}
	else if (remain=="*")
		unit=RVNG_TWIP;
	else
		return false;
	return true;
}

RVNGString RVNGStringProperty::getStr() const
{
	return m_str;
}

RVNGProperty *RVNGStringProperty::clone() const
{
	return new RVNGStringProperty(m_str);
}

RVNGIntProperty::RVNGIntProperty(const int val)  :
	m_val(val)
{
}

int RVNGIntProperty::getInt() const
{
	return m_val;
}

double RVNGIntProperty::getDouble() const
{
	return (double)m_val;
}

RVNGString RVNGIntProperty::getStr() const
{
	RVNGString str;
	str.sprintf("%d", m_val);
	return str;
}

RVNGProperty *RVNGIntProperty::clone() const
{
	return new RVNGIntProperty(m_val);
}

RVNGBoolProperty::RVNGBoolProperty(const bool val)  :
	RVNGIntProperty(val)
{
}

RVNGString RVNGBoolProperty::getStr() const
{
	if (getInt())
		return "true";
	else
		return "false";
}

RVNGProperty *RVNGBoolProperty::clone() const
{
	return new RVNGBoolProperty(getInt() != 0);
}

RVNGDoubleProperty::RVNGDoubleProperty(const double val) :
	m_val(val)
{
}

int RVNGDoubleProperty::getInt() const
{
	return (int)m_val;
}

double RVNGDoubleProperty::getDouble() const
{
	return m_val;
}

RVNGString RVNGDoubleProperty::getStr() const
{
	RVNGString str = doubleToString(getDouble());
	return str;
}

RVNGProperty *RVNGDoubleProperty::clone() const
{
	return new RVNGDoubleProperty(getDouble());
}

RVNGInchProperty::RVNGInchProperty(const double val) :
	RVNGDoubleProperty(val)
{
}

RVNGString RVNGInchProperty::getStr() const
{
	RVNGString str = doubleToString(getDouble());
	str.append("in");
	return str;
}

RVNGProperty *RVNGInchProperty::clone() const
{
	return new RVNGInchProperty(getDouble());
}

RVNGPercentProperty::RVNGPercentProperty(const double val) :
	RVNGDoubleProperty(val)
{
}

RVNGString RVNGPercentProperty::getStr() const
{
	RVNGString str = doubleToString(getDouble()*100.0);
	str.append("%");
	return str;
}

RVNGProperty *RVNGPercentProperty::clone() const
{
	return new RVNGPercentProperty(getDouble());
}

RVNGPointProperty::RVNGPointProperty(const double val) :
	RVNGDoubleProperty(val)
{
}

RVNGString RVNGPointProperty::getStr() const
{
	RVNGString str = doubleToString(getDouble());
	str.append("pt");
	return str;
}

RVNGProperty *RVNGPointProperty::clone() const
{
	return new RVNGPointProperty(getDouble());
}

RVNGTwipProperty::RVNGTwipProperty(const double val) :
	RVNGDoubleProperty(val)
{
}

RVNGString RVNGTwipProperty::getStr() const
{
	RVNGString str;
	str.sprintf("%i*", getInt());
	return str;
}

RVNGProperty *RVNGTwipProperty::clone() const
{
	return new RVNGTwipProperty(getDouble());
}

RVNGProperty *RVNGPropertyFactory::newStringProp(const RVNGString &str)
{
	return static_cast<RVNGProperty *>(new RVNGStringProperty(str));
}

RVNGProperty *RVNGPropertyFactory::newStringProp(const char *str)
{
	return static_cast<RVNGProperty *>(new RVNGStringProperty(str));
}

RVNGProperty *RVNGPropertyFactory::newIntProp(const int val)
{
	return static_cast<RVNGProperty *>(new RVNGIntProperty(val));
}

RVNGProperty *RVNGPropertyFactory::newBoolProp(const bool val)
{
	return static_cast<RVNGProperty *>(new RVNGBoolProperty(val));
}

RVNGProperty *RVNGPropertyFactory::newDoubleProp(const double val)
{
	return static_cast<RVNGProperty *>(new RVNGDoubleProperty(val));
}

RVNGProperty *RVNGPropertyFactory::newInchProp(const double val)
{
	return static_cast<RVNGProperty *>(new RVNGInchProperty(val));
}

RVNGProperty *RVNGPropertyFactory::newPercentProp(const double val)
{
	return static_cast<RVNGProperty *>(new RVNGPercentProperty(val));
}

RVNGProperty *RVNGPropertyFactory::newPointProp(const double val)
{
	return static_cast<RVNGProperty *>(new RVNGPointProperty(val));
}

RVNGProperty *RVNGPropertyFactory::newTwipProp(const double val)
{
	return static_cast<RVNGProperty *>(new RVNGTwipProperty(val));
}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
