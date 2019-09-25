//
//  M209Group.cpp
//  check_keylists
//
//  Created by Joseph Dunn on 9/23/19.
//  Copyright © 2019 Joseph Dunn. All rights reserved.
//

#include "M209Group.hpp"

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
  IndicatorNum = IndicatorNum % 26;
  int IndicatorNumKL = 26 * (KeyListIndicator[0]-'A') + KeyListIndicator[1]-'A';
  // adj is the # of days from kl date to present date
  int adj = IndicatorNum-IndicatorNumKL;
  if (adj < 0) adj=adj+676;
  return now - days(adj);
}

string M209GroupKeyFile (string NetIndicator,
                         string KeyListIndicator) {
  string root_dir = "../../m209group-key-lists/";
  date d = KeyListIndicator2Date(NetIndicator, KeyListIndicator);
  string d_str = to_simple_string(d);
  string y_str = d_str.substr(0,4);
  string m_str = d_str.substr(5,3);
  boost::to_upper(m_str);
  string key_file = NetIndicator + "-" + y_str + "/" + m_str + "/keys/" + KeyListIndicator + ".txt";
  return root_dir + key_file;
}
  

