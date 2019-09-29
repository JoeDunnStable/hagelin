//
/// \file KeyListDataBase.cpp
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

#include "KeyListDataBase.hpp"

/// Returns days since 1/1/01.  Replicated Python
int toordinal(date d) {
  stringstream ss;
  ss << days(730120) + (d-date(2000,1,1));
  int ret;
  ss >> ret;
  return ret;
}

string Date2KeyListIndicator(string NetIndicator, date d) {
  int IndicatorNum = toordinal(d);
  for (int i=0; i<NetIndicator.size(); ++i)
    IndicatorNum += NetIndicator[i];
  IndicatorNum = IndicatorNum % 676;
  string ret;
  ret.push_back(IndicatorNum / 26 +'A');
  ret.push_back(IndicatorNum % 26 +'A');
  return ret;
}

date KeyListIndicator2Date(string NetIndicator, string KeyListIndicator) {
  date now = day_clock::universal_day();
  int IndicatorNum = toordinal(now);
  for (int i=0; i<NetIndicator.size(); ++i)
    IndicatorNum += NetIndicator[i];
  IndicatorNum = IndicatorNum % 676;
  int IndicatorNumKL = 26 * (KeyListIndicator[0]-'A') + KeyListIndicator[1]-'A';
  // adj is the # of days from kl date to present date
  int adj = IndicatorNum-IndicatorNumKL;
  if (adj < 0) adj=adj+676;
  return now - days(adj);
}

