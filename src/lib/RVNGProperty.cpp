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
 */

#include <ctype.h>
#include <locale.h>

#include <sstream>
#include <string>

#include <librevenge/librevenge.h>

namespace librevenge
{

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
		return 0;
	}
	virtual double getDouble() const
	{
		return 0;
	}
	virtual RVNGUnit getUnit() const
	{
		return RVNG_UNIT_ERROR;
	}
	virtual RVNGString getStr() const;
	virtual RVNGProperty *clone() const;

private:
	RVNGString m_str;
};

class RVNGBinaryDataProperty : public RVNGProperty
{
public:
	RVNGBinaryDataProperty(const RVNGBinaryData &data);
	RVNGBinaryDataProperty(const unsigned char *buffer, const unsigned long bufferSize);
	~RVNGBinaryDataProperty() {}
	virtual int getInt() const
	{
		return 0;
	}
	virtual double getDouble() const
	{
		return 0;
	}
	virtual RVNGUnit getUnit() const
	{
		return RVNG_UNIT_ERROR;
	}
	virtual RVNGString getStr() const;
	virtual RVNGProperty *clone() const;

private:
	RVNGBinaryData m_data;
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

RVNGString RVNGStringProperty::getStr() const
{
	return m_str;
}

RVNGProperty *RVNGStringProperty::clone() const
{
	return new RVNGStringProperty(m_str);
}

RVNGBinaryDataProperty::RVNGBinaryDataProperty(const RVNGBinaryData &data) :
	m_data(data)
{
}
RVNGBinaryDataProperty::RVNGBinaryDataProperty(const unsigned char *buffer, const unsigned long bufferSize) :
	m_data(buffer, bufferSize)
{
}

RVNGString RVNGBinaryDataProperty::getStr() const
{
	return m_data.getBase64Data();
}

RVNGProperty *RVNGBinaryDataProperty::clone() const
{
	return new RVNGBinaryDataProperty(m_data);
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
	return new RVNGStringProperty(str);
}

RVNGProperty *RVNGPropertyFactory::newStringProp(const char *str)
{
	return new RVNGStringProperty(str);
}

RVNGProperty *RVNGPropertyFactory::newBinaryDataProp(const RVNGBinaryData &data)
{
	return new RVNGBinaryDataProperty(data);
}

RVNGProperty *RVNGPropertyFactory::newBinaryDataProp(const unsigned char *buffer, const unsigned long bufferSize)
{
	return new RVNGBinaryDataProperty(buffer, bufferSize);
}

RVNGProperty *RVNGPropertyFactory::newIntProp(const int val)
{
	return new RVNGIntProperty(val);
}

RVNGProperty *RVNGPropertyFactory::newBoolProp(const bool val)
{
	return new RVNGBoolProperty(val);
}

RVNGProperty *RVNGPropertyFactory::newDoubleProp(const double val)
{
	return new RVNGDoubleProperty(val);
}

RVNGProperty *RVNGPropertyFactory::newInchProp(const double val)
{
	return new RVNGInchProperty(val);
}

RVNGProperty *RVNGPropertyFactory::newPercentProp(const double val)
{
	return new RVNGPercentProperty(val);
}

RVNGProperty *RVNGPropertyFactory::newPointProp(const double val)
{
	return new RVNGPointProperty(val);
}

RVNGProperty *RVNGPropertyFactory::newTwipProp(const double val)
{
	return new RVNGTwipProperty(val);
}

}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
