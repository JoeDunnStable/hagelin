//
/// \file test_c52.cpp
/// \package hagelin
//
/// \author Joseph Dunn on 10/17/19.
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
using std::cout;
using std::cerr;
using std::endl;
#include <fstream>
using std::ifstream;
#include <sstream>
using std::stringstream;
#define BOOST_TEST_MODULE test_c52
#include <boost/test/included/unit_test.hpp>
#include <boost/test/data/test_case.hpp>

#define SOURCE
#include "config.h"
#include "C52.hpp"

//! If true, enable verbose debugging messages to stderr.
//
bool Verbose = false;

//! If true, suppress informational messages to stderr.
//
bool Quiet = true;

BOOST_AUTO_TEST_CASE(test_construction){
  C52 c52;
  BOOST_TEST(true);
}


BOOST_AUTO_TEST_CASE(cipher_test){
  string src_dir(getenv("MESON_SOURCE_ROOT"));
  C52 c52;
  bool AutoKey = false;
  bool AutoMsgIndicator = false;
  date d = date_from_iso_string("20191015");
  string fname = src_dir + "/tests/20191015.c52key";
  string NetIndicator = "";
  c52.LoadKey(fname, NetIndicator, d);
  string KeyDir = ".";
  bool CipherMode = false;
  ifstream in(src_dir + "/tests/cipher2_c52.txt");
  if (in) {
    stringstream out;
    
    c52.CipherStream(AutoKey,
                      AutoMsgIndicator,
                      d,
                      NetIndicator,
                      KeyDir, CipherMode, in, out);
    
    ifstream chk(src_dir + "/tests/deciphered.txt");
    if (!chk) {
      cerr << "Unable to open check file: " <<(src_dir + "/tests/plain.txt") << endl;
      BOOST_TEST(false);
    } else {
      bool f_okay = true;
      while (chk) {
        char c1, c2;
        out >> c1;
        chk >> c2;
        if (c1 !=c2)
          f_okay=false;
      }
      BOOST_TEST(f_okay);
    }
  } else {
    cerr << "Unable to open input file: " << (src_dir + "/tests/cipher.txt") << endl;
    BOOST_TEST(false);
  }
  
}

BOOST_AUTO_TEST_CASE(genkey_test){
  C52 c52;
  bool AutoKey = false;
  bool AutoMsgIndicator = false;
  date d = day_clock::universal_day();
  string NetIndicator = "TEST";
  string KeyDir;
  bool CipherMode = true;
  stringstream key_stream;
  c52.GenKey();
  c52.PrintKey(NetIndicator, d, key_stream);
  stringstream plain_text("HELLO WORLD");
  stringstream cipher_text;
  vector<string> initial_pos{"A","A","A","A","A","A"};
  c52.SetWheels(initial_pos);
  
  c52.CipherStream(AutoKey,
                    AutoMsgIndicator,
                    d,
                    NetIndicator,
                    KeyDir, CipherMode, plain_text, cipher_text);
  
  c52.LoadKey(key_stream, NetIndicator, d);
  
  CipherMode = false;
  stringstream deciphered_text;
  c52.SetWheels(initial_pos);
  
  c52.CipherStream(AutoKey,
                    AutoMsgIndicator,
                    d,
                    NetIndicator,
                    KeyDir, CipherMode, cipher_text, deciphered_text);
  
  plain_text.seekg(0);
  bool f_okay = true;
  char c1;
  while (plain_text >> c1) {
    char c2;
    deciphered_text >> c2;
    if (c1 != c2)
      f_okay = false;
  }
  BOOST_TEST(f_okay);
}

BOOST_AUTO_TEST_CASE(auto_msg_test){
  string src_dir(getenv("MESON_SOURCE_ROOT"));
  C52 c52;
  bool AutoKey = false;
  bool AutoMsgIndicator = true;
  date d = date_from_iso_string("20191015");
  string NetIndicator = "TEST";
  bool CipherMode = true;
  string KeyDir = src_dir + "/tests";
  stringstream plain_text("HELLO WORLD");
  stringstream cipher_text;
  
  c52.CipherStream(AutoKey,
                    AutoMsgIndicator,
                    d,
                    NetIndicator,
                    KeyDir, CipherMode, plain_text, cipher_text);
  
  CipherMode = false;
  stringstream deciphered_text;
  
  c52.CipherStream(AutoKey,
                    AutoMsgIndicator,
                    d,
                    NetIndicator,
                    KeyDir, CipherMode, cipher_text, deciphered_text);
  
  plain_text.seekg(0);
  bool f_okay = true;
  char c1;
  while (plain_text >> c1) {
    char c2;
    deciphered_text >> c2;
    if (c1 != c2)
      f_okay = false;
  }
  BOOST_TEST(f_okay);
}

BOOST_AUTO_TEST_CASE(auto_key_test){
  C52 c52;
  bool AutoKey = true;
  bool AutoMsgIndicator = true;
  string NetIndicator = "C52NET";
  date d;
  bool CipherMode = true;
  string KeyDir = ".";
  stringstream plain_text("HELLO WORLD");
  stringstream cipher_text;
  
  c52.CipherStream(AutoKey,
                    AutoMsgIndicator,
                    d,
                    NetIndicator,
                    KeyDir, CipherMode, plain_text, cipher_text);
  
  c52.ClearKey();
  CipherMode = false;
  stringstream deciphered_text;
  
  c52.CipherStream(AutoKey,
                    AutoMsgIndicator,
                    d,
                    NetIndicator,
                    KeyDir, CipherMode, cipher_text, deciphered_text);
  
  plain_text.seekg(0);
  bool f_okay = true;
  char c1;
  while (plain_text >> c1) {
    char c2;
    deciphered_text >> c2;
    if (c1 != c2)
      f_okay = false;
  }
  BOOST_TEST(f_okay);
}

BOOST_AUTO_TEST_CASE(cx52_auto_key_test){
  C52 c52;
  bool AutoKey = true;
  bool AutoMsgIndicator = true;
  string NetIndicator = "CX52NET";
  date d;
  bool CipherMode = true;
  string KeyDir = ".";
  stringstream plain_text("HELLO WORLD");
  stringstream cipher_text;
  
  c52.CipherStream(AutoKey,
                    AutoMsgIndicator,
                    d,
                    NetIndicator,
                    KeyDir, CipherMode, plain_text, cipher_text);
  
  c52.ClearKey();
  CipherMode = false;
  stringstream deciphered_text;
  
  c52.CipherStream(AutoKey,
                    AutoMsgIndicator,
                    d,
                    NetIndicator,
                    KeyDir, CipherMode, cipher_text, deciphered_text);
  
  plain_text.seekg(0);
  bool f_okay = true;
  char c1;
  while (plain_text >> c1) {
    char c2;
    deciphered_text >> c2;
    if (c1 != c2)
      f_okay = false;
  }
  BOOST_TEST(f_okay);
}

