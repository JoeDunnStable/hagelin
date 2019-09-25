//
///  \file M209Group_main.cpp
///  \package hagelin
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

#include <boost/program_options.hpp>

#include "config.h"
#include "M209Group.hpp"

int main(int argc, const char * argv[]) {
  using namespace boost::program_options;
  using std::cerr;
  using std::endl;
  string NetIndicator;
  string DateString;
  
  // Parse command-line arguments
  options_description desc("M209 Group KeylistIndicator Generator");
  desc.add_options()
  ("help,h", "produce help message")
  ("version,V", "print version and copyright")
  ("netIndicator,n",value<string>(&NetIndicator)->default_value("M209GROUP"),"NetIndicator")
  ("date,d", value<string>(&DateString)->default_value(to_simple_string(day_clock::universal_day())), "Date");
  
  variables_map vm;
  store(parse_command_line(argc, argv, desc), vm);
  notify(vm);
  
  if (vm.count("help")) {
    cerr << desc << endl;
    exit(0);
  }
  if (vm.count("version")) {
    cerr << VERSION << endl;
    exit(0);
  }
  date d = from_simple_string(DateString);
  // insert code here...
  std::cout << NetIndicator << " " << d << " "
            << Date2KeyListIndicator(NetIndicator, d) << std::endl;
  return 0;
}
