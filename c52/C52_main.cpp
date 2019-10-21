//
/// \file C52_main.cpp
/// \package hagelin
//
/// \author Joseph Dunn on 10/1/19.
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

#include <iostream>
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
#include <iomanip>
using std::dec;
#include <fstream>
using std::ifstream;
using std::ofstream;
#include <exception>
using std::exception;

#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>

#define SOURCE
#include "config.h"
#include "C52.hpp"

//! If true, enable verbose debugging messages to stderr.
//
bool Verbose = false;

//! If true, suppress informational messages to stderr.
//
bool Quiet = false;


//! Print version.
//
void PrintVersion(ostream& os) {
  os << endl;
  os << "C-52 Simulator version "
  << VERSION << " by Joseph Dunn" << endl;
  os << "Copyright (C) 2019 Joseph Dunn, Released under GPL v3." << endl;
  os << "Copyright (C) 2009-2013 Mark J. Blair. Released under GPLv3." << endl;
  os << endl;
  os << "Joseph Dunn source code hosted at GitHub:" << endl;
  os << "    https://github.com/JoeDunnStable/hagelin" << endl;
  os << "Mark Blair source code hosted at GitLab:" << endl;
  os << "    https://gitlab.com/NF6X_Crypto/hagelin" << endl;
}

//! Main entry point.
int main(int argc, char **argv)
{
  using namespace boost::program_options;
  C52     c52;
  bool    CipherMode = true;
  bool    DoCipher = false;
  string  FileIn;
  string  FileOut;
  bool    AutoKey = false;
  bool    AutoMsgIndicator = false;
  vector<string>  indicator(NUM_WHEELS,"A");
  char    print_offset;
  date   d;
  string KeyFileName;
  string    d_str;
  int      i;
  string    KeyDir = ".";
  string    NetIndicator;
  bool CX52 = false;
  size_t      SkipChars = 0;
  
  // Parse command-line arguments
  options_description desc("C52 options description");
  desc.add_options()
  ("help,h", "produce help message")
  ("version,V", "print version and copyright")
  ("AutoKey,A", bool_switch(&AutoKey), "Automatically retrieve key from database\nusing NetIndicator and date for enciphering\nand the info in message header when deciphering.\nImplies autoMsg")
  ("autoMsg,a", bool_switch(&AutoMsgIndicator), "Automatically generate/extract message indicators")
  (",c", "Encipher text from cin to cout")
  (",d", "Decipher text from cin to cout")
  (",g", "generate random key setting")
  (",x",  bool_switch(&CX52), "Generate keys for CX52")
  ("fileIn",value<string>(&FileIn),"File of text input file, cin if omitted")
  ("fileOut",value<string>(&FileOut), "File name of text output file, cout if omitted")
  (",i", value<vector<string> >(&indicator)->multitoken(),"Set initial wheel positions from following six arguments" )
  (",o", value<char>(&print_offset)->default_value('A'), "print offset.  A indicates no offset")
  (",k", value<string>(&KeyFileName), "Load key setting from specified file.")
  (",l", value<string>(&d_str), "Use specified date to lookup key in database.")
  (",n", value<string>(&NetIndicator), "Specify a net indicator for use in -a or -p modes.\nMust be a single word consisting of only letters and/or numbers.")
  (",p", "print key settings to cout")
  (",s", value<size_t>(&SkipChars),"Skip number of leading characters specified in following argument.")
  (",t", value<string>(&KeyDir), "Specify directory containing key files for -a mode.\nDefault is current directory.")
  (",q", bool_switch(&Quiet), "Suppress informational messages.")
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
  if (vm.count("-c") + vm.count("-d")+ vm.count("-p") > 1) {
    // There can be atmost one owner of out
    cerr << "m209: at most one of the options c, d and p may be used" << endl;
    exit(1);
  }
  if (vm.count("-c")) {
    CipherMode = true;
    DoCipher = true;
  }
  if (vm.count("-d")) {
    CipherMode = false;
    DoCipher = true;
  }
  
  if (vm.count("-g")) {
    c52.GenKey(CX52);
  }
  
  if (AutoKey) {
    AutoMsgIndicator = true;
    if (vm.count("-k")){
      cerr << "ERROR: -k connot be specified in AutoKey mode" << endl;
      cerr << desc << endl;
      exit(1);
    }
    if ((vm.count("-c") || vm.count("-p")) && NetIndicator.size()==0) {
      NetIndicator = "C52NET";
    }
  } else if (AutoMsgIndicator) {
    if (CipherMode && vm.count("-l")==0) {
      cerr << "ERROR: -l must be specified for enciphering when -a is specified" << endl;
      cerr << desc << endl;
      exit(1);
    }
  } else {
    if (DoCipher && (vm.count("-k")==0 || vm.count("-i")==0)) {
      cerr << "ERROR: Both -k and -i must be specified in manual mode" << endl;
      cerr << desc << endl;
      exit(1);
    }
  }
  if (DoCipher && ((vm.count("-i")+AutoMsgIndicator) !=1 )){
    cerr << "ERROR: Either -i or autoMsg must be specified but not both" << endl;
    cerr << desc << endl;
    exit(1);
  }
  if (vm.count("-i")) {
    if (indicator.size() != NUM_WHEELS) {
      cerr << "ERROR: -i requires "
      << dec << NUM_WHEELS
      << " arguments."
      << endl;
      cerr << desc << endl;
      exit(1);
    }
    for (i=0; i<NUM_WHEELS; i++) {
      
      if (indicator[i].size() != 1 || !isalpha(indicator[i][0])) {
        cerr << "ERROR: -i requires single-letter alphabetic arguments"
        << endl;
        cerr << desc << endl;
        exit(1);
      }
      indicator[i][0] = toupper(indicator[i][0]);
    }
  }
  if (vm.count("-l")) {
    try {
      d = date_from_iso_string(d_str);
    } catch (exception& e) {
      cerr << "ERROR: Date must be in iso format YYYYMMDD" << endl;
      exit(1);
    }
  } else {
    d = day_clock::universal_day();
  }
  if (vm.count("-n")) {
    boost::to_upper(NetIndicator);
  }
  if (vm.count("-k")){
    if (!c52.LoadKey(KeyFileName, NetIndicator, d)) {
      cerr << "ERROR: Key file " << KeyFileName << " not found." << endl;
      exit (1);
    };
  }

  // open the input and output file or use cin and cout
  ifstream fin;
  if (vm.count("fileIn")) {
    fin.open(FileIn);
    if (!fin) {
      cerr << "ERROR: Unable to open input file " << FileIn << endl;
      exit(1);
    }
  }
  istream& in = (vm.count("fileIn")) ? fin : cin;
  ofstream fout;
  if (vm.count("fileOut")) {
    fout.open(FileOut);
    if (!fout) {
      cerr << "ERROR: Unable to open output file " << FileOut << endl;
      exit(1);
    }
  }
  ostream& out = (vm.count("fileOut")) ? fout : cout;

  if (vm.count("-p")) {
    if (AutoKey) {
      date now = day_clock::universal_day();
      if (!c52.LoadKey(now, NetIndicator)) {
        cerr << "ERROR: Unable to load key from data base" << endl;
        exit(1);
      }
    }
    c52.PrintKey(NetIndicator, d, out);
  }
  
  if (DoCipher) {
    if (!AutoMsgIndicator) {
      c52.SetPrintOffset(print_offset-'A');
      if (!c52.SetWheels(indicator)) {
        cerr << "ERROR: Invalid wheel position(s) specified"
        << endl;
        exit(1);
      }
    }
    
    if (!Quiet) {
      cerr << "Reading from text..." << endl;
    }
    
    if (SkipChars > 0) {
      if (!Quiet) {
        cerr << "(Skipping first " << SkipChars << " characters)" << endl;
      }
      char c;
      for (int n = 0; n< SkipChars; ) {
        if (!in.good()) {
          break;
        }
        c = in.get();
        if (!isspace(c)) {
          ++n;
        }
      }
    }
   
    c52.CipherStream(AutoKey,
                      AutoMsgIndicator,
                      d,
                      NetIndicator,
                      KeyDir, CipherMode, in, out);
  }
  
  return 0;
}
