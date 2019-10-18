//
/// \file C52CreateDataBase.cpp
/// \package hagelin
//
/// \author Joseph Dunn on 10/15/19.
/// \copyright opyright © 2019 Joseph Dunn.
//

/**************************************************************************
* Copyright (C) 2019 Jospeh Dunn
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
using std::cout;
using std::cerr;
using std::endl;
#include <string>
using std::string;
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
using boost::filesystem::path;
using boost::filesystem::is_directory;
using boost::filesystem::create_directories;
using boost::filesystem::ofstream;

bool Quiet = true;
bool Verbose = false;

#define SOURCE
#include "config.h"
#include "C52.hpp"

//! Print version.
//
void PrintVersion(ostream& os) {
  os << endl;
  os << "C-52 Create Key Data Base "
  << VERSION << " by Joseph Dunn" << endl;
  os << "Copyright (C) 2019 Joseph Dunn, Released under GPL v3." << endl;
  os << "Copyright (C) 2009-2013 Mark J. Blair. Released under GPLv3." << endl;
  os << endl;
  os << "Joseph Dunn source code hosted at GitHub:" << endl;
  os << "    https://github.com/JoeDunnStable/hagelin" << endl;
  os << "Mark Blair source code hosted at GitLab:" << endl;
  os << "    https://gitlab.com/NF6X_Crypto/hagelin" << endl;
}

int main(int argc, const char * argv[]) {
  using namespace boost::program_options;
  C52     c52;
  string  DataDir, NetIndicator, StartDate_str, EndDate_str;

  // Parse command-line arguments
  options_description desc("C52 options description");
  desc.add_options()
  ("help,h", "produce help message")
  ("version,V", "print version and copyright")
  (",d", value<string>(&DataDir), "the root directory of key databases")
  (",n", value<string>(&NetIndicator), "Specify a net indicator.\nMust be a single word consisting of only letters and/or numbers.")
  (",s", value<string>(&StartDate_str), "the start date for the database in ISO format")
  (",e", value<string>(&EndDate_str), "the end date for the database in ISO format")
  (",v", bool_switch(&Verbose), "Print verbose debug messages to stderr.");

  variables_map vm;
  store(parse_command_line(argc, argv, desc), vm);
  notify(vm);
  
  if (vm.count("help")) {
    PrintVersion(cerr);
    cerr << desc << endl;
    exit(0);
  }
  if (vm.count("version")) {
    PrintVersion(cerr);
    exit(0);
  }
  if (vm.count("-d")==0) {
    cerr << "Error: The -d options must be specified" << endl;
    exit(1);
  }
  if (vm.count("-n")==0) {
    cerr << "Error: The -n option must be specified" << endl;
    exit (1);
  }
  path p(DataDir + "/" + NetIndicator);
  create_directories(p);
  if (!is_directory(p)) {
    cerr << "Unable to create/open directory " << p << endl;
  }
  date StartDate = date_from_iso_string(StartDate_str);
  date EndDate = date_from_iso_string(EndDate_str);

  for (day_iterator d_itr{StartDate}; (*d_itr) <= EndDate; ++d_itr) {
    path fname{to_iso_string(*d_itr)+".c52key"};
    ofstream fout(p / fname);
    if (!fout) {
      cerr << "Unable to open " << (p / fname) << endl;
      exit(1);
    } else {
      cout << "Writing to file: " << (p / fname) << endl;

    }
    c52.GenKey();
    c52.PrintKey(NetIndicator, *d_itr, fout);
  }

  return 0;
}
