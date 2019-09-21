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

/*!
 * \file m209_main.cc
 * \brief Main entry point of m209 executable.
 * \package hagelin
 *
 */

#include <iostream>
using std::cerr;

#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>

#define SOURCE
#include "config.h"
#include "M209.h"

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
  os << "M209 Simulator version "
  << VERSION << " by Mark J. Blair, NF6X and Joseph Dunn" << endl;
  os << "Copyright (C) 2019 Joseph Dunn, Released under GPL v3." << endl;
  os << "Copyright (C) 2009-2013 Mark J. Blair. Released under GPLv3." << endl;
  os << endl;
  os << "Joseph Dunn source code hosted at GitHub:" << endl;
  os << "    https://github.com/JoeDunnStable/hagelin" << endl;
  os << "Mark Blair source code hosted at GitLab:" << endl;
  os << "    https://gitlab.com/NF6X_Crypto/hagelin" << endl;
}


//! Print version and usage instructions.
//
void PrintHelp(ostream& os) {
  PrintVersion(os);
  os << endl;
  os << "Usage:" << endl;
  os << "  m209 [-v] -c|-d [-k {keyfile}] -i {i1} {i2} {i3} {i4} {i5} {i6}"
  << endl;
  os << "  m209 [-v] -c|-d [-k {keyfile}] [-l {ki}] [-n {ni}] -a"
  << endl;
  os << "  m209 [-v] -g -p [-l {ki}] [-n {ni}]" << endl;
  os << endl;
  os << "Options:" << endl;
  os << "  -a: Automatically generate/extract message indicators." << endl;
  os << "      If key list indicator is specified in cipher mode," << endl
  << "      automatically try to load key from either \"{ki}.txt\"" << endl
  << "      or \"{ki}.m209key\"." << endl << endl
  << "      In decipher mode, try to load key file based on key" << endl
  << "      list indicator in ciphertext. In either case, if key" << endl
  << "      file not found, use current setting, which is blank if" << endl
  << "      -k option is not specified." << endl << endl
  << "      In cipher mode, if -n option is specified or if net" << endl
  << "      indicator is found in key file, then add a line to beginning" << endl
  << "      of message with net indicator and group count."
  << endl << endl;
  os << "  -c: Encipher text from stdin to stdout." << endl << endl;
  os << "  -d: Decipher text from stdin to stdout." << endl << endl;
  os << "  -g: Generate random key settings." << endl << endl;
  os << "  -i: Set initial wheel positions from following six arguments."
  << endl << endl;
  os << "  -k: Load key settings from specified file." << endl << endl;
  os << "  -l: Use specified two-letter key list indicator." << endl << endl;
  os << "  -n: Specify a net indicator for use in -a or -p modes." << endl
  << "      Must be a single word consisting of only letters and/or numbers."
  << endl << endl;
  os << "  -p: Print key settings to stdout." << endl << endl;
  os << "  -s: Skip number of leading characters specified in following argument."
  << endl << endl;
  os << "  -t: Specify directory containing key files for -a mode." << endl
  << "      Default is current directory." << endl << endl;
  os << "  -v: Print verbose debug messages to stderr." << endl << endl;
  os << "  -q: Suppress informational messages." << endl << endl;
  os << "  --version | -V:" << endl;
  os << "      Print program version and copyright information to stderr."
  << endl << endl;
  os << "  --help | -h:" << endl;
  os << "      Print version and help information to stderr."
  << endl << endl;
  os << "Examples:" << endl;
  os << "  m209 -g -l AB -p >AB.txt" << endl << endl;
  os << "  m209 -a -c -l AB <plain.txt >cipher.txt" << endl
  << "    (note: requires file \"AB.txt\" or \"AB.m209key\" in current directory)"
  << endl << endl;
  os << "  m209 -a -d <cipher.txt >deciphered.txt" << endl
  << "    (note: requires suitable key list in current directory)"
  << endl << endl;
  os << "  m209 -c -k AB.txt -i a b c d e f <plain.txt >cipher.txt"
  << endl;
}

//! Main entry point.
//
int main(int argc, char **argv)
{
  using namespace boost::program_options;
  M209    m209;
  bool    CipherMode = true;
  bool    DoCipher = false;
  bool    AutoIndicator = false;
  vector<string>  indicator(NUM_WHEELS,"A");
  string KeyFileName;
  string    KeyListIndicator;
  int      i;
  string    KeyDir = ".";
  string    NetIndicator;
  size_t      SkipChars = 0;
  
  // Parse command-line arguments
  options_description desc("M209 options description");
  desc.add_options()
  ("help,h", "produce help message")
  ("version,V", "print version and copyright")
  (",a", bool_switch(&AutoIndicator), "Automatically generate/extract message indicators")
  (",c", "Encipher text from cin to cout")
  (",d", "Decipher text from cin to cout")
  (",g", "generate random key setting")
  (",i", value<vector<string> >(&indicator)->multitoken(),"Set initial wheel positions from following six arguments" )
  (",k", value<string>(&KeyFileName), "Load key setting from specified file.")
  (",l", value<string>(&KeyListIndicator), "Use specified two-letter key list indicator.")
  (",n", value<string>(&NetIndicator), "Specify a net indicator for use in -a or -p modes.\n   Must be a single word consisting of only letters and/or numbers.")
  (",p", "print key settings to cout")
  (",s", value<size_t>(&SkipChars),"Skip number of leading characters specified in following argument.")
  (",t", value<string>(&KeyDir), "Specify directory containing key files for -a mode.\n   Default is current directory.")
  (",q", bool_switch(&Quiet), "Suppress informational messages.")
  (",v", bool_switch(&Verbose), "Print verbose debug messages to stderr.");
  
  variables_map vm;
  store(parse_command_line(argc, argv, desc), vm);
  notify(vm);
  
  if (vm.count("help")) {
    PrintHelp(cerr);
    exit(0);
  }
  if (vm.count("version")) {
    PrintVersion(cerr);
    exit(0);
  }
  if (vm.count("-c") + vm.count("-d") > 1) {
    cerr << "m209: at most one of the option c and d may be used" << endl;
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
    m209.GenKey1944();
  }
  
  if (vm.count("-i")) {
    AutoIndicator = false;
    if (indicator.size() != NUM_WHEELS) {
      cerr << "ERROR: -i requires "
      << dec << NUM_WHEELS
      << " arguments."
      << endl;
      PrintHelp(cerr);
      exit(1);
    }
    for (i=0; i<NUM_WHEELS; i++) {
      
      if (indicator[i].size() != 1 || !isalpha(indicator[i][0])) {
        cerr << "ERROR: -i requires single-letter alphabetic arguments"
        << endl;
        PrintHelp(cerr);
        exit(1);
      }
      indicator[i][0] = toupper(indicator[i][0]);
    }
  }
  if (vm.count("-k")){
    m209.LoadKey(KeyFileName, KeyListIndicator, NetIndicator);
  }
  if (vm.count("-l")) {
    if (KeyListIndicator.size() != 2) {
      cerr << "ERROR: Key List Indicator must consist of two letters."
      << endl;
      PrintHelp(cerr);
      exit(1);
    }
    boost::to_upper(KeyListIndicator);
    if (!isalpha(KeyListIndicator[0]) || !isalpha(KeyListIndicator[1])) {
      cerr << "ERROR: Key List Indicator must consist of two letters."
      << endl;
      PrintHelp(cerr);
      exit(1);
    }
  }
  if (vm.count("-n")) {
    boost::to_upper(NetIndicator);
  }
  if (vm.count("-p")) {
    m209.PrintKey(KeyListIndicator, NetIndicator);
  }
  
  if (Verbose) {
    cerr << "Using "
    << "std::random_device"
    << " for random number generation."
    << endl;
  }
  
  if (DoCipher) {
    if (!AutoIndicator) {
      if (!m209.SetWheels(indicator)) {
        cerr << "ERROR: Invalid wheel position(s) specified"
        << endl;
        exit(1);
      }
    }
    
    if (!Quiet) {
      cerr << "Reading from stdin..." << endl;
    }
    
    if (SkipChars > 0) {
      if (!Quiet) {
        cerr << "(Skipping first " << SkipChars << " characters)" << endl;
      }
      char c;
      for (int n = 0; n< SkipChars; ) {
        if (!cin.good()) {
          break;
        }
        c = cin.get();
        if (!isspace(c)) {
          ++n;
        }
      }
    }
   
    m209.CipherStream(AutoIndicator,
                      KeyListIndicator,
                      NetIndicator,
                      KeyDir, CipherMode, cin, cout);
  }
  
  return 0;
}
