/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
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
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#include "libwpd_internal.h"

class WPXStringProperty : public WPXProperty
{
public:
	WPXStringProperty(const WPXString &str);
	WPXStringProperty(const char *str);
	~WPXStringProperty() {}
	virtual int getInt() const;
	virtual double getDouble() const;
	virtual WPXString getStr() const;
	virtual WPXProperty *clone() const;

private:
	WPXString m_str;
};

class WPXIntProperty : public WPXProperty
{
public:
	WPXIntProperty(const int val);
	~WPXIntProperty() {}
	virtual int getInt() const;
	virtual double getDouble() const;
	virtual WPXString getStr() const;
	virtual WPXProperty *clone() const;

private:
	int m_val;
};

class WPXBoolProperty : public WPXIntProperty
{
public:
	WPXBoolProperty(const bool val);
	~WPXBoolProperty() {}
	virtual WPXString getStr() const;
	virtual WPXProperty *clone() const;
};

class WPXDoubleProperty : public WPXProperty
{
public:
	WPXDoubleProperty(const double val);
	~WPXDoubleProperty() {}
	virtual int getInt() const;
	virtual double getDouble() const;
	virtual WPXString getStr() const;
	virtual WPXProperty *clone() const;

private:
	double m_val;
};

class WPXInchProperty : public WPXDoubleProperty
{
public:
	WPXInchProperty(const double val);
	~WPXInchProperty() {}
	virtual WPXString getStr() const;
	virtual WPXProperty *clone() const;
};

class WPXPercentProperty : public WPXDoubleProperty
{
public:
	WPXPercentProperty(const double val);
	~WPXPercentProperty() {}
	virtual WPXString getStr() const;
	virtual WPXProperty *clone() const;
};

class WPXPointProperty : public WPXDoubleProperty
{
public:
	WPXPointProperty(const double val);
	~WPXPointProperty() {}
	virtual WPXString getStr() const;
	virtual WPXProperty *clone() const;
};

class WPXTwipProperty : public WPXDoubleProperty
{
public:
	WPXTwipProperty(const double val);
	~WPXTwipProperty() {}
	virtual WPXString getStr() const;
	virtual WPXProperty *clone() const;
};

class WPXGenericProperty : public WPXDoubleProperty
{
public:
	WPXGenericProperty(const double val);
	~WPXGenericProperty() {}
	virtual WPXString getStr() const;
	virtual WPXProperty *clone() const;
};

WPXProperty::~WPXProperty()
{
}

WPXStringProperty::WPXStringProperty(const WPXString &str) :
	m_str(str)
{
}
WPXStringProperty::WPXStringProperty(const char *str) :
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

WPXProperty *WPXStringProperty::clone() const
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
	WPXString str;
	str.sprintf("%d", m_val);
	return str;
}

WPXProperty *WPXIntProperty::clone() const
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

WPXProperty *WPXBoolProperty::clone() const
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

WPXProperty *WPXDoubleProperty::clone() const
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

WPXProperty *WPXInchProperty::clone() const
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

WPXProperty *WPXPercentProperty::clone() const
{
	return new WPXPercentProperty(getDouble());
}

WPXPointProperty::WPXPointProperty(const double val) :
	WPXDoubleProperty(val)
{
}

WPXString WPXPointProperty::getStr() const
{
	WPXString str = doubleToString(getDouble());
	str.append("pt");
	return str;
}

WPXProperty *WPXPointProperty::clone() const
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

WPXProperty *WPXTwipProperty::clone() const
{
	return new WPXTwipProperty(getDouble());
}

WPXProperty *WPXPropertyFactory::newStringProp(const WPXString &str)
{
	return static_cast<WPXProperty *>(new WPXStringProperty(str));
}

WPXProperty *WPXPropertyFactory::newStringProp(const char *str)
{
	return static_cast<WPXProperty *>(new WPXStringProperty(str));
}

WPXProperty *WPXPropertyFactory::newIntProp(const int val)
{
	return static_cast<WPXProperty *>(new WPXIntProperty(val));
}

WPXProperty *WPXPropertyFactory::newBoolProp(const bool val)
{
	return static_cast<WPXProperty *>(new WPXBoolProperty(val));
}

WPXProperty *WPXPropertyFactory::newDoubleProp(const double val)
{
	return static_cast<WPXProperty *>(new WPXDoubleProperty(val));
}

WPXProperty *WPXPropertyFactory::newInchProp(const double val)
{
	return static_cast<WPXProperty *>(new WPXInchProperty(val));
}

WPXProperty *WPXPropertyFactory::newPercentProp(const double val)
{
	return static_cast<WPXProperty *>(new WPXPercentProperty(val));
}

WPXProperty *WPXPropertyFactory::newPointProp(const double val)
{
	return static_cast<WPXProperty *>(new WPXPointProperty(val));
}

WPXProperty *WPXPropertyFactory::newTwipProp(const double val)
{
	return static_cast<WPXProperty *>(new WPXTwipProperty(val));
}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
