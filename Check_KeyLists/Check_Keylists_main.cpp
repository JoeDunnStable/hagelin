//
/// \file Check_KeyLists_ main.cpp
/// \package hagelin
//
///  \author Joseph Dunn on 9/23/19.
///  \copyright © 2019 Joseph Dunn.
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

#include <iostream>
#include <iomanip>
using std::setprecision;
using std::fixed;

#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

#define SOURCE
#include <config.h>
#include "M209.h"
#include "KeyListDataBase.hpp"
#include "ValidateDrumOldBroken.hpp"
#include "ValidateDrumOldFixed.hpp"

bool Verbose = false;
bool Quiet = true;

/// a one-off program to check the sums of the M209GROUP keylists
/// Mark Balai's GenKey contained a bug that allowed bad drums to excape
/// detection.
/// The M209GROUP keylists are assumed to be in ../../m209group-key-lists
/// That would be a sister directory of hagelin is the current directory == build
/// directory.
int main(int argc, const char * argv[]) {
  using namespace boost::program_options;
  string root_dir = "../../m209group-key-lists/";
  string NetIndicator{"M209GROUP"};
  string KeyListRootDir;
  date d_start{2013,1,1};
  date d_end{2020,12,31};
  int count = 0;
  int count_good = 0;
  int count_broken_good = 0;
  int count_fixed_good = 0;
  for (day_iterator d_itr{d_start}; (*d_itr) <= d_end; ++d_itr) {
    string d_str = to_simple_string(*d_itr);
    string y_str = d_str.substr(0,4);
    string m_str = d_str.substr(5,3);
    boost::to_upper(m_str);
    string KeyListIndicator = Date2KeyListIndicator(NetIndicator, *d_itr);
    M209 m209;
    string key_file = NetIndicator + "-" + y_str + "/" + m_str + "/keys/" + KeyListIndicator + ".txt";
    bool valid;
    bool valid_old_broken;
    bool valid_old_fixed;
    if(m209.LoadKey(root_dir + key_file)) {
      valid = m209.ValidateDrum(m209.getDrum());
      valid_old_broken = ValidateDrumOldBroken(m209.getDrum());
      valid_old_fixed = ValidateDrumOldFixed(m209.getDrum());
      count++;
      count_good += valid;
      count_broken_good += valid_old_broken;
      count_fixed_good += valid_old_fixed;
    } else {
      throw runtime_error("File " + key_file + " not found.");
    }
    
    cout << key_file << "  " << (valid_old_broken ? "Good" : "Bad") << "  "
                             << (valid_old_fixed ? "Good" : "Bad") << "  "
                             << (valid ? "Good" : "Bad") << endl;
  }
  cout << "Total good drums (old broken) = " << count_broken_good << " / " << count
  << " (" << fixed << setprecision(0) << (100.*count_broken_good)/count << "%)" << endl;
  cout << "Total good drums (old fixed) = " << count_fixed_good << " / " << count
  << " (" << fixed << setprecision(0) << (100.*count_fixed_good)/count << "%)" << endl;
  cout << "Total good drums =             " << count_good <<" / " << count
  << " (" << fixed << setprecision(0) << (100.*count_good)/count << "%)" << endl;
  return 0;
}
