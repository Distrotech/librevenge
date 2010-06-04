/* libwpd
 * Copyright (C) 2004 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2005 Net Integration Technologies (http://www.net-itech.com)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#include "WPXProperty.h"
#include "libwpd_internal.h"

class WPXStringProperty : public WPXProperty
{
public:
	WPXStringProperty(const WPXString &str);
	WPXStringProperty(const char * str);
	virtual int getInt() const;
	virtual double getDouble() const;
	virtual WPXString getStr() const;
	virtual WPXProperty * clone() const;

private:
	WPXString m_str;
};

class WPXIntProperty : public WPXProperty
{
public:
	WPXIntProperty(const int val);
	virtual int getInt() const;
	virtual double getDouble() const;
	virtual WPXString getStr() const;
	virtual WPXProperty * clone() const;

private:
	int m_val;
};

class WPXBoolProperty : public WPXIntProperty
{
public:
	WPXBoolProperty(const bool val);
	virtual WPXString getStr() const;
	virtual WPXProperty * clone() const;
};

class WPXDoubleProperty : public WPXProperty
{
public:
	WPXDoubleProperty(const double val);
	virtual int getInt() const;
	virtual double getDouble() const;
	virtual WPXString getStr() const; 
	virtual WPXProperty * clone() const;

private:
	double m_val;
};

class WPXInchProperty : public WPXDoubleProperty
{
public:
	WPXInchProperty(const double val);
	virtual WPXString getStr() const; 
	virtual WPXProperty * clone() const;
};

class WPXPercentProperty : public WPXDoubleProperty
{
public:
	WPXPercentProperty(const double val);
	virtual WPXString getStr() const; 
	virtual WPXProperty * clone() const;
};

class WPXPointProperty : public WPXDoubleProperty
{
public:
	WPXPointProperty(const double val);
	virtual WPXString getStr() const; 
	virtual WPXProperty * clone() const;
};

class WPXTwipProperty : public WPXDoubleProperty
{
public:
	WPXTwipProperty(const double val);
	virtual WPXString getStr() const; 
	virtual WPXProperty * clone() const;
};

class WPXGenericProperty : public WPXDoubleProperty
{
public:
	WPXGenericProperty(const double val);
	virtual WPXString getStr() const; 
	virtual WPXProperty * clone() const;
};

WPXProperty::~WPXProperty()
{
}

WPXStringProperty::WPXStringProperty(const WPXString &str) :
	m_str(str) 
{
}
WPXStringProperty::WPXStringProperty(const char * str) :
	m_str(str) 
{
}
int WPXStringProperty::getInt() const 
{	
	return 0; 
}

double WPXStringProperty::getDouble() const 
{ 
	return 0.0; 
}

WPXString WPXStringProperty::getStr() const 
{ 
	return m_str; 
}

WPXProperty * WPXStringProperty::clone() const
{
	return new WPXStringProperty(m_str);
}

WPXIntProperty::WPXIntProperty(const int val)  :
	m_val(val) 
{
}

int WPXIntProperty::getInt() const 
{ 
	return m_val; 
}

double WPXIntProperty::getDouble() const 
{ 
	return (double)m_val; 
}

WPXString WPXIntProperty::getStr() const 
{ 
	WPXString str; str.sprintf("%d", m_val); 
	return str; 
}

WPXProperty * WPXIntProperty::clone() const
{
	return new WPXIntProperty(m_val);
}

WPXBoolProperty::WPXBoolProperty(const bool val)  :
	WPXIntProperty(val) 
{
}

WPXString WPXBoolProperty::getStr() const 
{ 
	if (getInt())
		return "true"; 
	else
		return "false"; 
}

WPXProperty * WPXBoolProperty::clone() const
{
	return new WPXBoolProperty(getInt() != 0);
}

WPXDoubleProperty::WPXDoubleProperty(const double val) :
	m_val(val) 
{
}

int WPXDoubleProperty::getInt() const 
{ 
	return (int)m_val; 
}

double WPXDoubleProperty::getDouble() const 
{ 
	return m_val; 
}

WPXString WPXDoubleProperty::getStr() const 
{ 
	WPXString str = doubleToString(getDouble()); 
	return str; 
}

WPXProperty * WPXDoubleProperty::clone() const
{
	return new WPXDoubleProperty(getDouble());
}

WPXInchProperty::WPXInchProperty(const double val) :
	WPXDoubleProperty(val) 
{
}

WPXString WPXInchProperty::getStr() const 
{ 
	WPXString str = doubleToString(getDouble()); 
	str.append("in"); 
	return str; 
}

WPXProperty * WPXInchProperty::clone() const
{
	return new WPXInchProperty(getDouble());
}

WPXPercentProperty::WPXPercentProperty(const double val) :
	WPXDoubleProperty(val) 
{
}

WPXString WPXPercentProperty::getStr() const
{
	WPXString str = doubleToString(getDouble()*100.0); 
	str.append("%"); 
	return str; 
}

WPXProperty * WPXPercentProperty::clone() const
{
	return new WPXPercentProperty(getDouble());
}

WPXPointProperty::WPXPointProperty(const double val) :
	WPXDoubleProperty(val) 
{
}

WPXString WPXPointProperty::getStr() const
{
	WPXString str; 
	str.sprintf("%ipt", getInt()); 
	return str; 
}

WPXProperty * WPXPointProperty::clone() const
{
	return new WPXPointProperty(getDouble());
}

WPXTwipProperty::WPXTwipProperty(const double val) :
	WPXDoubleProperty(val) 
{
}

WPXString WPXTwipProperty::getStr() const
{
	WPXString str; 
	str.sprintf("%i*", getInt()); 
	return str; 
}

WPXProperty * WPXTwipProperty::clone() const
{
	return new WPXTwipProperty(getDouble());
}

WPXProperty * WPXPropertyFactory::newStringProp(const WPXString &str) 
{ 
	return static_cast<WPXProperty *>(new WPXStringProperty(str)); 
}

WPXProperty * WPXPropertyFactory::newStringProp(const char *str) 
{ 
	return static_cast<WPXProperty *>(new WPXStringProperty(str));
}

WPXProperty * WPXPropertyFactory::newIntProp(const int val) 
{ 
	return static_cast<WPXProperty *>(new WPXIntProperty(val)); 
}

WPXProperty * WPXPropertyFactory::newBoolProp(const bool val) 
{ 
	return static_cast<WPXProperty *>(new WPXBoolProperty(val)); 
}

WPXProperty * WPXPropertyFactory::newDoubleProp(const double val) 
{ 
	return static_cast<WPXProperty *>(new WPXDoubleProperty(val)); 
}

WPXProperty * WPXPropertyFactory::newInchProp(const double val) 
{ 
	return static_cast<WPXProperty *>(new WPXInchProperty(val)); 
}

WPXProperty * WPXPropertyFactory::newPercentProp(const double val) 
{ 
	return static_cast<WPXProperty *>(new WPXPercentProperty(val)); 
}

WPXProperty * WPXPropertyFactory::newPointProp(const double val) 
{ 
	return static_cast<WPXProperty *>(new WPXPointProperty(val)); 
}

WPXProperty * WPXPropertyFactory::newTwipProp(const double val) 
{ 
	return static_cast<WPXProperty *>(new WPXTwipProperty(val)); 
}

