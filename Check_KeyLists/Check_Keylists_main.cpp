//
/// \file Check_KeyLists_ main.cpp
/// \package hagelin
//
///  \author Joseph Dunn on 9/23/19.
///  \copyright © 2019 Joseph Dunn.
//

#include <iostream>
#include <iomanip>
using std::setprecision;
using std::fixed;

#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

#define SOURCE
#include <config.h>
#include "M209.h"
#include "M209Group.hpp"

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
  for (day_iterator d_itr{d_start}; (*d_itr) <= d_end; ++d_itr) {
    string d_str = to_simple_string(*d_itr);
    string y_str = d_str.substr(0,4);
    string m_str = d_str.substr(5,3);
    boost::to_upper(m_str);
    string KeyListIndicator = Date2KeyListIndicator(NetIndicator, *d_itr);
    M209 m209;
    string key_file = NetIndicator + "-" + y_str + "/" + m_str + "/keys/" + KeyListIndicator + ".txt";
    if(m209.LoadKey(root_dir + key_file)) {
      bool valid = m209.ValidateDrum(m209.getDrum());
      count++;
      count_good += valid;
    } else {
      throw runtime_error("File " + key_file + " not found.");
    }
    
//    cout << key_file << "  " << (valid ? "Good" : "Bad") << endl;
  }
  cout << "Total good drums = " << count_good <<" / " << count
  << " (" << fixed << setprecision(0) << (100.*count_good)/count << "%)" << endl;
  return 0;
}
