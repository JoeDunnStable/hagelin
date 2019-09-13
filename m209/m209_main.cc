/**************************************************************************
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
 * \file
 * \brief Main entry point of m209 executable.
 *
 */

#include <iostream>
using std::cerr;

#include <getopt.h>

#include <boost/algorithm/string.hpp>

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
  << VERSION << " by Mark J. Blair, NF6X" << endl;
  os << "Copyright (C) 2009-2013 Mark J. Blair. Released under GPLv3." << endl;
  os << endl;
  os << "Source code hosted at GitLab:" << endl;
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

//! Long command line option definitions.
//
static struct option longopts[] = {
  {"help",  no_argument,  NULL,  'h'},
  {"version",  no_argument,  NULL,  'V'},
  {NULL,  0,    NULL,  0}
};


//! Main entry point.
//
int main(int argc, char **argv)
{
  M209    m209;
  bool    PrintKey = false;
  bool    CipherMode = true;
  bool    DoCipher = false;
  bool    AutoIndicator = false;
  int      opt;
  vector<string *>  indicator;
  string    KeyListIndicator;
  int      i;
  string    KeyDir;
  string    NetIndicator;
  size_t      SkipChars = 0;
  
  // Misc. initialization
  
  for (i=0; i<NUM_WHEELS; i++) {
    indicator.push_back(new string());
    indicator[i]->assign("A");
  }
  
  KeyListIndicator.clear();
  NetIndicator.clear();
  
  KeyDir = ".";
  
  //     SRAND;
  
  // Disable getopt()'s internal error messages
  opterr = 0;
  
  // Parse command-line arguments
  while ((opt = getopt_long(argc, argv, ":acdghik:l:n:ps:t:vVq",
                            longopts, NULL)) != -1) {
    
    switch (opt) {
        
      case 'a':      // automatic message indicator
        AutoIndicator = true;
        break;
        
      case 'c':      // cipher mode
        CipherMode = true;
        DoCipher   = true;
        break;
        
      case 'd':      // decipher mode
        CipherMode = false;
        DoCipher   = true;
        break;
        
      case 'g':      // generate random key
        m209.GenKey();
        break;
        
      case 'h':      // help
        PrintHelp(cerr);
        exit(0);
        break;
        
      case 'i':      // specify message indicator
        AutoIndicator = false;
        for (i=0; i<NUM_WHEELS; i++) {
          
          if (optind >= argc) {
            cerr << "ERROR: -i requires "
            << dec << NUM_WHEELS
            << " arguments."
            << endl;
            PrintHelp(cerr);
            exit(1);
          }
          
          if ((strlen(argv[optind]) != 1) || !isalpha(argv[optind][0])) {
            cerr << "ERROR: -i requires single-letter alphabetic arguments"
            << endl;
            PrintHelp(cerr);
            exit(1);
          }
          
          argv[optind][0] = toupper(argv[optind][0]);
          
          indicator[i]->assign((const char *)argv[optind++]);
        }
        break;
        
      case 'k':      // load key from file
        m209.LoadKey(optarg, KeyListIndicator, NetIndicator);
        break;
        
      case 'l':      // key list indicator
        if (strlen(optarg) != 2) {
          cerr << "ERROR: Key List Indicator must consist of two letters."
          << endl;
          PrintHelp(cerr);
          exit(1);
        }
        optarg[0] = toupper(optarg[0]);
        optarg[1] = toupper(optarg[1]);
        if (!isalpha(optarg[0]) || !isalpha(optarg[1])) {
          cerr << "ERROR: Key List Indicator must consist of two letters."
          << endl;
          PrintHelp(cerr);
          exit(1);
        }
        KeyListIndicator.assign(optarg);
        break;
        
      case 'n':      // net indicator
        NetIndicator.assign(optarg);
        boost::to_upper(NetIndicator);
        break;
        
      case 'p':      // print key to stdout
        PrintKey = true;
        break;
        
      case 's':      // skip leading characters
        SkipChars = strtoul(optarg, NULL, 0);
        break;
        
      case 't':      // directory for key lists
        KeyDir.assign(optarg);
        break;
        
      case 'v':      // verbose mode
        Verbose = true;
        break;
        
      case 'V':      // version
        PrintVersion(cerr);
        exit(0);
        break;
        
      case 'q':      // quiet mode
        Quiet = true;
        break;
        
      case ':':      // missing argument
        cerr << "ERROR: option -"
        << (char)optopt
        << " requires an argument."
        << endl;
        PrintHelp(cerr);
        exit(1);
        break;
        
      case '?':      // unknown option
        cerr << "ERROR: option -"
        << (char)optopt
        << " is not valid."
        << endl;
        PrintHelp(cerr);
        exit(1);
        break;
        
      default:
        cerr << "ERROR: unknown error during argument parsing."
        << endl;
        PrintHelp(cerr);
        exit(1);
        break;
    }
    
  }
  
  if (Verbose) {
    cerr << "Using "
    << "std::random_device"
    << " for random number generation."
    << endl;
  }
  
  //argc -= optind;
  //argv += optind;
  
  if (PrintKey) {
    m209.PrintKey(KeyListIndicator, NetIndicator);
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
