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
	virtual float getFloat() const;
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
	virtual float getFloat() const;
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

class WPXFloatProperty : public WPXProperty
{
public:
	WPXFloatProperty(const float val);
	virtual int getInt() const;
	virtual float getFloat() const;

private:
	float m_val;
};

class WPXInchProperty : public WPXFloatProperty
{
public:
	WPXInchProperty(const float val);
	virtual WPXString getStr() const; 
	virtual WPXProperty * clone() const;
};

class WPXPercentProperty : public WPXFloatProperty
{
public:
	WPXPercentProperty(const float val);
	virtual WPXString getStr() const; 
	virtual WPXProperty * clone() const;
};

class WPXPointProperty : public WPXFloatProperty
{
public:
	WPXPointProperty(const float val);
	virtual WPXString getStr() const; 
	virtual WPXProperty * clone() const;
};

class WPXTwipProperty : public WPXFloatProperty
{
public:
	WPXTwipProperty(const float val);
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

float WPXStringProperty::getFloat() const 
{ 
	return 0.0f; 
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

float WPXIntProperty::getFloat() const 
{ 
	return (float)m_val; 
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
	return new WPXBoolProperty((const bool)(getInt() != 0));
}

WPXFloatProperty::WPXFloatProperty(const float val) :
	m_val(val) 
{
}

int WPXFloatProperty::getInt() const 
{ 
	return (int)m_val; 
}

float WPXFloatProperty::getFloat() const 
{ 
	return m_val; 
}

WPXInchProperty::WPXInchProperty(const float val) :
	WPXFloatProperty(val) 
{
}

WPXString WPXInchProperty::getStr() const 
{ 
	WPXString str = doubleToString(getFloat()); 
	str.append("in"); 
	return str; 
}

WPXProperty * WPXInchProperty::clone() const
{
	return new WPXInchProperty(getFloat());
}

WPXPercentProperty::WPXPercentProperty(const float val) :
	WPXFloatProperty(val) 
{
}

WPXString WPXPercentProperty::getStr() const
{
	WPXString str = doubleToString(getFloat()*100.0f); 
	str.append("%"); 
	return str; 
}

WPXProperty * WPXPercentProperty::clone() const
{
	return new WPXPercentProperty(getFloat());
}

WPXPointProperty::WPXPointProperty(const float val) :
	WPXFloatProperty(val) 
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
	return new WPXPointProperty(getFloat());
}

WPXTwipProperty::WPXTwipProperty(const float val) :
	WPXFloatProperty(val) 
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
	return new WPXTwipProperty(getFloat());
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

WPXProperty * WPXPropertyFactory::newInchProp(const float val) 
{ 
	return static_cast<WPXProperty *>(new WPXInchProperty(val)); 
}

WPXProperty * WPXPropertyFactory::newPercentProp(const float val) 
{ 
	return static_cast<WPXProperty *>(new WPXPercentProperty(val)); 
}

WPXProperty * WPXPropertyFactory::newPointProp(const float val) 
{ 
	return static_cast<WPXProperty *>(new WPXPointProperty(val)); 
}

WPXProperty * WPXPropertyFactory::newTwipProp(const float val) 
{ 
	return static_cast<WPXProperty *>(new WPXTwipProperty(val)); 
}

