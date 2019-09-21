//
///  main.cpp
/// \package hagelin
//
//  Created by Joseph Dunn on 9/15/19.
//  Copyright © 2019 Joseph Dunn. All rights reserved.
//

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#include <fstream>
using std::ifstream;
#include <sstream>
using std::stringstream;
#define BOOST_TEST_MODULE test_m209
#include <boost/test/included/unit_test.hpp>
#include <boost/test/data/test_case.hpp>

#define SOURCE
#include "config.h"
#include "M209.h"

//! If true, enable verbose debugging messages to stderr.
//
bool Verbose = false;

//! If true, suppress informational messages to stderr.
//
bool Quiet = true;

BOOST_AUTO_TEST_CASE(test_construction){
  M209 m209;
  BOOST_TEST(true);
}

BOOST_AUTO_TEST_CASE(cipher_test){
  string src_dir(getenv("MESON_SOURCE_ROOT"));
  M209 m209;
  bool AutoIndicator = true;
  string KeyListIndicator = "";
  string NetIndicator = "";
  string KeyDir = src_dir + "/tests";
  bool CipherMode = false;
  ifstream in(src_dir + "/tests/cipher.txt");
  if (in) {
    stringstream out;
    
    m209.CipherStream(AutoIndicator,
                      KeyListIndicator,
                      NetIndicator,
                      KeyDir, CipherMode, in, out);
    
    ifstream chk(src_dir + "/tests/deciphered.txt");
    if (!chk) {
      cerr << "Unable to open check file: " <<(src_dir + "/tests/deciphered.txt") << endl;
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

BOOST_AUTO_TEST_CASE(keyread_test){
  string src_dir(getenv("MESON_SOURCE_ROOT"));
  M209 m209;
  string KeyFileName = src_dir + "/tests/mangled.m209key";
  string KeyListIndicator = "";
  string NetIndicator = "";
  stringstream out;
  if (m209.LoadKey(KeyFileName, KeyListIndicator, NetIndicator)) {
	  m209.PrintKey(KeyListIndicator, NetIndicator, out);
	  ifstream chk(src_dir + "/tests/MB.m209key");
	  bool f_okay = true;
	  while (chk) {
		  char c1, c2;
		  out >> c1;
		  chk >> c2;
		  if (c1 != c2)
			  f_okay = false;
	  }
	  BOOST_TEST(f_okay);
  }
  else {
	  cerr << "Unable to load key file: " << KeyFileName << endl;
	  BOOST_TEST(false);
  }
}

BOOST_AUTO_TEST_CASE(genkey_test){
  M209 m209;
  bool AutoIndicator = false;
  string KeyListIndicator = "";
  string NetIndicator = "";
  string KeyDir;
  bool CipherMode = true;
  stringstream key_stream;
  m209.GenKey1944();
  m209.PrintKey(KeyListIndicator, NetIndicator, key_stream);
  stringstream plain_text("HELLO WORLD");
  stringstream cipher_text;
  vector<string> initial_pos{"A","A","A","A","A","A"};
  m209.SetWheels(initial_pos);
  
  m209.CipherStream(AutoIndicator,
                    KeyListIndicator,
                    NetIndicator,
                    KeyDir, CipherMode, plain_text, cipher_text);
  
  m209.LoadKey(key_stream, KeyListIndicator, NetIndicator);
  
  CipherMode = false;
  stringstream deciphered_text;
  m209.SetWheels(initial_pos);
  
  m209.CipherStream(AutoIndicator,
                    KeyListIndicator,
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

BOOST_AUTO_TEST_CASE(automatic_mode_test){
  string src_dir(getenv("MESON_SOURCE_ROOT"));
  M209 m209;
  bool AutoIndicator = true;
  string KeyListIndicator = "MB";
  string NetIndicator = "TEST";
  bool CipherMode = true;
  string KeyDir = src_dir + "/tests";
  stringstream plain_text("HELLO WORLD");
  stringstream cipher_text;
  
  m209.CipherStream(AutoIndicator,
                    KeyListIndicator,
                    NetIndicator,
                    KeyDir, CipherMode, plain_text, cipher_text);
  
  CipherMode = false;
  stringstream deciphered_text;
  
  m209.CipherStream(AutoIndicator,
                    KeyListIndicator,
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

