//
/// \file  KeyListDataBase.hpp
/// \package hagelin
//
/// \author Joseph Dunn on 9/23/19.
/// \copyright © 2019 Joseph Dunn.
//

/**************************************************************************
* Copyright (C) 2019 Joseph Dunn
* Copyright (C) 2009-2013 Mark J. Blair, NF6X
*
* This file is part of Hagelin.
*
*  Hagelin is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  Hagelin is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with Hagelin.  If not, see <http://www.gnu.org/licenses/>.
**************************************************************************/

#ifndef KeyListDataBase_hpp
#define KeyListDataBase_hpp

#include <iostream>
#include <sstream>
using std::stringstream;
#include <string>
using std::string;
#include <boost/date_time/gregorian/gregorian.hpp>
using namespace boost::gregorian;

/// Return the day count since 1/1/1 adjusted to match Python
int toordinal(date d);

/// Mark Blair's method to generata KeyList Indicator from date
string Date2KeyListIndicator(string NetIndicator, date d);

/// return most recent date for  the given KeyList
date KeyListIndicator2Date(string NetIndicator, string KeyListIndicator);

#endif /* KeyListDataBase_hpp */
