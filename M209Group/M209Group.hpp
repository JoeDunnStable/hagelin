//
//  M209Group.hpp
//  check_keylists
//
//  Created by Joseph Dunn on 9/23/19.
//  Copyright © 2019 Joseph Dunn. All rights reserved.
//

#ifndef M209Group_hpp
#define M209Group_hpp

#include <iostream>
#include <sstream>
using std::stringstream;
#include <string>
using std::string;
#include <boost/date_time/gregorian/gregorian.hpp>
using namespace boost::gregorian;

/// Return the day count sin 1/1/1 adjusted to match Python
int toordinal(date d);

/// Mark Blair's method to generata KeyList Indicator from date
string Date2KeyListIndicator(string NetIndicator, date d);

/// return most recent date for with the given KeyList
date KeyListIndicator2Date(string NetIndicator, string KeyListIndicator);

#endif /* M209Group_hpp */
