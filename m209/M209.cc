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

/*!
 * \file M209.cc
 * \brief Implementation of M209 class member functions.
 * \package hagelin
 */

#include <iostream>
using std::cerr;
using std::endl;
#include <iomanip>
using std::setw;
using std::setfill;
#include <fstream>
using std::ifstream;

#include <regex>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include "config.h"
#include "M209.h"

//! Array of position names for each of the six key wheels.
//
static const char *pnames[NUM_WHEELS][27] = {
  
  {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J",
    "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T",
    "U", "V", "W", "X", "Y", "Z", NULL},
  
  {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J",
    "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T",
    "U", "V", "X", "Y", "Z", NULL, NULL},
  
  {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J",
    "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T",
    "U", "V", "X", NULL, NULL, NULL, NULL},
  
  {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J",
    "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T",
    "U", NULL, NULL, NULL, NULL, NULL, NULL},
  
  {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J",
    "K", "L", "M", "N", "O", "P", "Q", "R", "S", NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL},
  
  {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J",
    "K", "L", "M", "N", "O", "P", "Q", NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL},
};

//! Array of read offsets for each of the six key wheels.
//
static const int offsets[] = {-11, -11, -10, -9, -8, -7};


M209::M209() {
  int    i;
  
//  Drum.resize(NUM_LUG_BARS);
  
  Wheels.resize(NUM_WHEELS);
  for (i = 0; i < NUM_WHEELS; i++) {
    for (int j = 0; pnames[i][j]; j++) {
      Wheels[i].AddPosition(pnames[i][j]);
    }
    Wheels[i].SetReadOffset(offsets[i]);
  }
  
  ClearKey();
}


char M209::Cipher(char c) {
  int      cnum, i, j, key;
  char           c2;
  bool    match;
  vector <bool>  pins;
  
  // Check for valid character
  if ((c < 'A') || (c > 'Z')) {
    throw std::invalid_argument("M209::Cipher(): invalid character");
  }
  
  // Get character number, 0..25
  cnum = c - 'A';
  
  // Get mask of active pins at offsets from current code wheel positions.
  pins.resize(NUM_WHEELS);
  for (i=0; i<NUM_WHEELS; i++) {
    pins[i] = Wheels[i].ReadPinOffset();
  }
  
  // Compare active pins to each lug bar. For each bar where one or
  // more active lugs match an active pin, add one to the key value.
  for (i=0, key=0; i<NUM_LUG_BARS; i++) {
    for (j=0, match=0; j<NUM_WHEELS; j++) {
      if (Drum[i][j] & pins[j]) {
        match = true;
      }
    }
    if (match) {
      ++key;
    }
  }
  
  // Subtract calculated key (0-27) from input character, modulo 26
  cnum -= key;
  while (cnum < 0) {
    cnum += 26;
  }
  cnum = cnum % 26;
  
  // Increment the letter counter
  ++LetterCounter;
  
  // Compute ciphered letter from reciprocal alphabet
  c2 = 'Z' - cnum;
  
  if (Verbose) {
    cerr << "Counter: "
    << setfill('0') << setw(4) << dec << LetterCounter
    << "  Wheels: ";
    for (i=0; i<NUM_WHEELS; i++) {
      cerr << Wheels[i].GetPosName();
    }
    cerr << "  Pin Positions: ";
    for (i=0; i<NUM_WHEELS; i++) {
      cerr << Wheels[i].GetOffsetName();
    }
    cerr << "  Pin Values: ";
    for (i=0; i<NUM_WHEELS; i++) {
      cerr << pins[i];
    }
    cerr << "  In: " << c
    << "  Key: " << setfill(' ') << setw(2) << key
    << "  Out: " << c2;
    cerr << endl;
  }
  
  // Advance each code wheel.
  for (i=0; i<NUM_WHEELS; i++) {
    Wheels[i].Rotate(1);
  }
  
  return c2;
}



void M209::ClearKey(void) {
  int    i, j;
  
  for (i=0; i<NUM_WHEELS; Wheels[i].SetPosition(0), Wheels[i++].ClearAllPins());
  for (i=0; i<NUM_LUG_BARS; i++) {
    for (j=0; j<NUM_WHEELS; j++) {
      Drum[i][j] = false;
    }
  }
  LetterCounter = 0;
}


void M209::PrintKey(string KeyListIndicator, string NetIndicator,
                    ostream& os) {
  int    i, j;
  char  c1, c2;
  
  if (!NetIndicator.empty()) {
    os << "NET INDICATOR:      " << NetIndicator << endl;
  }
  if (KeyListIndicator.length() == 2) {
    os << "KEY LIST INDICATOR: " << KeyListIndicator << endl;
  }
  
  os << "-------------------------------" << endl;
  os << "NR LUGS  1  2  3  4  5  6" << endl;
  os << "-------------------------------" << endl;
  
  for (i=0; i<NUM_WHEELS; i++) {
    Wheels[i].SetPosition(0);
  }
  
  for (i = 0; i < NUM_LUG_BARS; i++) {
    os << setfill('0') << setw(2) << (i+1);
    
    bitset2lugs(Drum[i], c1, c2);
    // by popular demand
	if (c1 == '0' && (c2 == '1' || c2 == '2'))
		swap(c1, c2);

    os << ' ' << c1 << '-' << c2 << ' ';
    
    for (j = 0; j < NUM_WHEELS; j++) {
      os << "  ";
      if (i >= Wheels[j].GetWheelSize()) {
        os << ' ';
      } else if (Wheels[j].ReadPin()) {
        os << Wheels[j].GetPosName();
      } else {
        os << '-';
      }
      Wheels[j].Rotate(1);
    }
    
    os << endl;
  }
  
  os << "-------------------------------" << endl;
  os << "26 LETTER CHECK" << endl;
  os << endl;
  
  for (i=0; i<NUM_WHEELS; i++) {
    Wheels[i].SetPosition(0);
  }
  
  LetterCounter = 0;
  
  for (i=0; i<26; i++) {
    os << Cipher('A');
    if ((i % 5) == 4) {
      os << ' ';
    }
  }
  
  os << endl;
  os << "-------------------------------" << endl;
  
  
}


bool M209::LoadKey(const string& fname) {
  string    dummy1;
  string    dummy2;
  
  return LoadKey(fname, dummy1, dummy2);
}

bool M209::LoadKey(const string& fname, string& KeyListIndicator, string &NetIndicator) {

  if (Verbose) {
    cerr << "Looking for key file " << fname << endl;
  }
  ifstream keyfile(fname, ifstream::in); // key file input stream
  if (!keyfile) return false;
  if (!Quiet) {
    cerr << "Loading key file " << fname << endl;
  }
  LoadKey(keyfile, KeyListIndicator, NetIndicator);
  return true;
}

void M209::LoadKey(istream& keyfile, string& KeyListIndicator, string& NetIndicator) {

  string              line;                         // single line read from key file
  std::regex  netind_regex;          // regex matching net indicator line
  std::regex  keyind_regex;          // regex matching key list indicator line
  std::regex        key_regex;                    // regex matching pin/lug setting line
  std::regex        check_regex;                  // regex matching 26 letter check
  std::smatch       matches;                      // matches returned by regex_match
  bool    found_check;          // found 26 letter check?
  string    check_line;          // reformatted 26 letter check line
  bool    found_lugbar[NUM_LUG_BARS];   // found pin/lug line?
  
  
  // Regular expression matching net indicator line.
  netind_regex = "[\\s]*"    // optional leading whitespace
  "NET[\\s]+INDICATOR[\\s]*:"  // net indicator label text
  "[\\s]*"      // optional whitespace
  "([A-Za-z0-9]+)"    // net indicator
  "[\\s]*";      // optional trailing whitespace
  
  // Regular expression matching key list indicator line.
  keyind_regex = "[\\s]*"    // optional leading whitespace
  "(M-209[\\s]+)?"    // optional word M-209 (present in older key tables)
  "KEY[\\s]+LIST[\\s]*"    // key list label text
  "(INDICATOR[\\s]*)?"    // optional word INDICATOR (added in newer tables)
  ":[\\s]*"      // colon, optional space
  "([A-Za-z][A-Za-z])"    // key list indicator
  "[\\s]*";      // optional trailing whitespace
  
  // Regular expression matching key table pin/lug setting line.
  key_regex = "[\\s]*"    // optional leading whitespace
  "([0-2]?[0-9])"      // lug bar number
  ":?[\\s]+"      // optional colon, whitespace
  "([0-6])[\\s]*-?[\\s]*([0-6])"  // two lug numbers
  "[\\s]+"      // whitespace
  "((?:[A-Z\\-][\\s]*){0,6})"  // 0-6 pin settings
  "[\\s]*";      // optional trailing whitespace
  
  // Regular expression matching 26 letter check line.
  check_regex = "[\\s]*"    // optional leading whitespace
  "([A-Z]{5})[\\s]*"    // group 1, optional whitespace
  "([A-Z]{5})[\\s]*"    // group 2, optional whitespace
  "([A-Z]{5})[\\s]*"    // group 3, optional whitespace
  "([A-Z]{5})[\\s]*"    // group 4, optional whitespace
  "([A-Z]{5})[\\s]*"    // group 5, optionalwhitespace
  "([A-Z])"      // final letter
  "[\\s]*";      // optional trailing whitespace
  
  // Init flags and clear old key
  found_check = false;
  for (int n = 0; n < NUM_LUG_BARS; n++) {
    found_lugbar[n] = false;
  }
  this->ClearKey();
  
  if (Verbose) {
    cerr << "Reading key file " << endl;
  }
  
  // Read key file one line at a time
  while (getline(keyfile, line)) {
    
    if (std::regex_match(line, matches, key_regex)) {
      // Found a pin/lug setting line
      
      if (Verbose) {
        cerr << "  pin/lug line \"" << line << "\"";
      }
      
      // sanity check
      if (matches.size() != 5) {
        throw std::out_of_range("M209::LoadKey(): Internal error:"
                                " Bad number of pin/lug expression matches.");
      }
      
      // decode lugbar number
      int lugbar_num = boost::lexical_cast<int>(matches[1].str()) - 1;
      if (Verbose) {
        cerr << "  lugbar " << (lugbar_num + 1);
      }
      if ((lugbar_num < 0) || (lugbar_num >= NUM_LUG_BARS)) {
        throw std::range_error("M209::LoadKey(): Key file error:"
                               " Lug bar number out of range.");
      }
      
      // check for duplicate lugbar number
      if (found_lugbar[lugbar_num]) {
        throw std::range_error("M209::LoadKey(): Key file error:"
                               " Lug bar set multiple times.");
      }
      found_lugbar[lugbar_num] = true;
      
      // decode and sanity-check lug settings
      int lug1 = boost::lexical_cast<int>(matches[2].str());
      int lug2 = boost::lexical_cast<int>(matches[3].str());
      if (Verbose) {
        cerr << "  lugs " << lug1 << " " << lug2;
      }
      if ((lug1 < 0) || (lug1 > 6) || (lug2 < 0) || (lug2  >6)) {
        throw std::range_error("M209::LoadKey(): Key file error:"
                               " Lug number out of range.");
      }
      if ((lug1 > 0) && (lug1 == lug2)) {
        throw std::range_error("M209::LoadKey(): Key file error:"
                               " Invalid lug settings.");
      }
      
      // Set lugs
      if (lug1 > 0) {
        Drum[lugbar_num][lug1 - 1] = true;
      }
      if (lug2 > 0) {
        Drum[lugbar_num][lug2 - 1] = true;
      }
      
      // strip spaces from pin settings
      string pins = matches[4].str();
      for (int n = 0; n < pins.length(); ) {
        if (isspace(pins[n])) {
          pins.erase(n,1);
        } else {
          ++n;
        }
      }
      size_t num_pins = pins.length();
      
      // check for correct number of pins
      int expected_pins=0;
      for (int j=0; j<NUM_WHEELS; ++j)
        expected_pins += Wheels[j].GetWheelSize() > lugbar_num;
      
      if (num_pins != expected_pins) {
        throw std::range_error("M209::LoadKey(): Key file error:"
                               " Incorrect number of pins found.");
      }
      
      
      // Set pins
      if (Verbose) {
        cerr << "  pins ";
      }
      for (int n=0; n < expected_pins; n++) {
        if (pins[n] != '-') {
          Wheels[n].SetPosition(lugbar_num);
          if (pins.substr(n,1) != Wheels[n].GetPosName()) {
            throw std::range_error("M209::LoadKey(): Key file error:"
                                   " Incorrect pin position letter.");
          }
          Wheels[n].SetPin(1);
          if (Verbose) {
            cerr << "+";
          }
        } else {
          if (Verbose) {
            cerr << "-";
          }
        }
      }
      if (Verbose) {
        cerr << endl;
      }
      
      
    } else if (std::regex_match(line, matches, check_regex)) {
      // Found the 26 letter check line
      
      if (Verbose) {
        cerr << "  letter check line \"" << line << "\"" << endl;
      }
      
      // sanity check
      if (matches.size() != 7) {
        throw std::out_of_range("M209::LoadKey(): Internal error:"
                                " Bad number of letter check expression matches.");
      }
      
      if (found_check) {
        // Found a second 26 letter check line. There should be
        // only zero or one of them found in a key file.
        //
        // TODO: Create more specific exception(s) to use for key file
        //       syntax errors? Or print an error message and return
        //       an error code?
        throw std::range_error("M209::LoadKey(): Key file error:"
                               " Multiple 26-letter check lines found.");
      }
      
      found_check = true;
      
      // Reassemble check line without group spacing
      check_line = "";
      for (int n = 1; n < 7; n++) {
        check_line = check_line + matches[n].str();
      }
      
    } else if (std::regex_match(line, matches, netind_regex)) {
      // Found a net indicator
      if (Verbose) {
        cerr << "  net indicator line \"" << line << "\"" << endl;
      }
      
      NetIndicator.assign(matches[1].str());
      boost::to_upper(NetIndicator);
      
    } else if (std::regex_match(line, matches, keyind_regex)) {
      // Found a key list indicator
      if (Verbose) {
        cerr << "  key list indicator line \"" << line << "\"" << endl;
      }
      
      KeyListIndicator.assign(matches[3].str());
      
      
    } else {
      if (Verbose) {
        cerr << "  discarding line \"" << line << "\"" << endl;
      }
    }
    
  }
  
  // Verify that we found all of the pin/lug setting lines
  for (int n = 0; n < NUM_LUG_BARS; n++) {
    if (!found_lugbar[n]) {
      throw std::range_error("M209::LoadKey(): Key file error:"
                             " Did not find enough pin/lug setting lines.");
    }
  }
  
  // If we found a 26 letter check line, then verify the key settings.
  if (found_check) {
    if (Verbose) {
      cerr << "Running 26 letter check" << endl;
    }
    
    string test_line;
    for (int n=0; n<NUM_WHEELS; n++) {
      Wheels[n].SetPosition(0);
    }
    for (int n=0; n<26; n++) {
      test_line += Cipher('A');
    }
    if (test_line != check_line) {
      throw std::range_error("M209::LoadKey(): Key file error:"
                             " 26 letter check failed.");
    }
  } else {
    if (Verbose) {
      cerr << "Skipping 26 letter check" << endl;
    }
  }
  
  // Success! Set the machine to clean initial state
  LetterCounter = 0;
  for (int n=0; n<NUM_WHEELS; n++) {
    Wheels[n].SetPosition(0);
  }
}


void M209::ResetCounter(void) {
  int   i;
  
  for (i = 0; i<NUM_WHEELS; i++) {
    Wheels[i].Rotate(-LetterCounter);
  }
  
  LetterCounter = 0;
}


bool M209::SetWheels(vector<string> indicator) {
  int    i, j;
  
  
  for (i=0, j=0; (i<NUM_WHEELS) && (j < (int)indicator.size()); i++, j++) {
    try {
      if (Verbose) {
        cerr << "Trying wheel "
        << dec << i
        << " setting "
        << indicator[j]
        << endl;
      }
      Wheels[i].SetPosByName(indicator[j]);
    } catch (...) {
      // reset counters to force retry with next indicator value
      --i;
    }
  }
  
  if (i < NUM_WHEELS) {
    // Unable to set wheels as requested
    return false;
  }
  
  if (Verbose) {
    cerr << "Wheels set to: ";
    for (i=0; i<NUM_WHEELS; i++) {
      cerr << Wheels[i].GetPosName();
    }
    cerr << endl;
  }
  return true;
}




void M209::CipherStream(bool AutoIndicator,
                        string& KeyListIndicator,
                        string& NetIndicator,
                        string KeyDir,
                        bool CipherMode,
                        istream& InText, ostream& OutText) {
  
  int      i;
  int      r;
  int      tries;
  char    MsgIndLtr='A';  // Letter enciphered to get IntMsgInd
  char    InC, OutC;  // Input and Output characters
  vector<string>  ExtMsgInd;  // External Message Indicator
  vector<string>  IntMsgInd;  // Internal Message Indicator
  list<char>    MsgText;  // Text of input message
  vector<char>  MsgInd1, MsgInd2;
  string    MyKLI;    // Key list indicator
  stringstream  OutBuf;    // Output buffer
  string    line;    // Line buffer for input stream
  std::regex  netind_regex;  // regex matching net indicator line of message
  std::smatch       matches;        // matches returned by regex_match
  bool    FoundNetInd = false;
  
  
  // Output is buffered in OutBuf so that we can insert group count at
  // beginning of buffer after generating ciphertext if needed. This is
  // a bit of a kludge, but it works.
  
  
  // Prepare message indicator vectors
  MsgInd1.resize(10);
  MsgInd2.resize(10);
  /*
  for (i=0; i<6; i++) {
    ExtMsgInd.push_back(new string());
  }
   */
  ExtMsgInd.resize(6);
  IntMsgInd.resize(12);
  /*
  for (i=0; i<12; i++) {
    IntMsgInd.push_back(new string());
  )
   */
  
  // regexp matching net indicator and group count in ciphertext.
  // Tolerate some variation in the "GR" text.
  netind_regex = "[\\s]*"       // optional leading whitespace
  "[A-Z0-9]+"         // net indicator
  "[\\s]+"         // whitespace
  "GR(P|PS|OUP|OUPS)?"       // GR, GRP, GRPS, GROUP or GROUPS
  "[\\s]+"         // whitespace
  "[0-9]+"         // group count
  "[\\s]*";         // optional trailing whitespace
  
  // Read entire input message into buffer. If we are deciphering
  // with automatic indicator extraction, we will look for repeated
  // message indicators at end of message, and snip them off.
  while (InText.good()) {
    // Read a line
    getline(InText, line);
    
    // upcase it
    boost::to_upper(line);
    
    if (Verbose) {
      cerr << "read message line \"" << line << "\"" << endl;
    }
    
    // If decipher mode, discard it if it looks like
    // a net indicator line. Only discard the first such
    // line found.
    if (!CipherMode && !FoundNetInd) {
      if (std::regex_match(line, matches, netind_regex)) {
        // Line looks like a net indicator line. Discard it.
        if (Verbose) {
          cerr << "discarding net indicator line \"" << line << "\"" << endl;
        }
        FoundNetInd = true;
        continue;
      }
    }
    
    // Now copy the line into the message buffer, processing
    // it char by char
    for (string::iterator i=line.begin(); i < line.end(); i++) {
      InC = *i;
      
      // When enciphering, translate space to Z
      if (CipherMode && (InC == ' ')) {
        InC = 'Z';
      }
      
      // Ignore if not alphabetic
      if (isalpha(InC)) {
        MsgText.push_back(InC);
      }
      
    }
  }
  
  // Are we automatically setting message indicators?
  if (AutoIndicator) {
    
    if (CipherMode) {
      // Must generate random indicator, encipher it, and include it
      // in the ciphertext. If 2-char key list indicator passed in,
      // use it, and also look for a keylist file named
      // {keylist indicator}.txt or {keylist indicator}.m209key.
      // If not passed in, then
      // repeat system indicator in place of key list indicator.
      
      
      if (KeyListIndicator.length() == 2) {
        MyKLI = KeyListIndicator;
        
        string Keyfile1 = KeyDir + "/" + MyKLI + KEYFILE_SUFFIX1;
        string Keyfile2 = KeyDir + "/" + MyKLI + KEYFILE_SUFFIX2;

        if (LoadKey(Keyfile1, KeyListIndicator, NetIndicator)) {
        } else if (LoadKey(Keyfile2, KeyListIndicator, NetIndicator)) {
        } else if (!Quiet) {
          cerr << "Key file not found; using current key settings."
          << endl;
        }
      }
      
      // Generate random internal and external message indicators
      tries = 0;
      do {
        // Avoid infinite loop
        if (tries++ >= 100) {
          cerr << "ERROR: Cannot generate message indicator!"
          << " Is random() broken?" << endl;
          exit(1);
        }
        
        // Clear letter counter and randomize wheel positions.
        // Store positions as external message indicator.
        LetterCounter = 0;
        for (i=0; i<NUM_WHEELS; i++) {
          ui_dist dist(0,Wheels[i].GetWheelSize()-1);
          r = dist(gen);
          Wheels[i].SetPosition(r);
          ExtMsgInd[i]=Wheels[i].GetPosName();
        }
        
        // Pick a random letter to be enciphered to produce external
        // message indicator
        ui_dist u_0_25(0,25);
        MsgIndLtr = u_0_25(gen) + 'A';
        
        // Encipher letter 12 times to get internal message indicator
        for (i=0; i<(int)IntMsgInd.size(); i++) {
          IntMsgInd[i]=Cipher(MsgIndLtr);
        }
        
        if (Verbose) {
          cerr << "Try "
          << dec << tries
          << " EMI=";
          for (i=0; i<(int)ExtMsgInd.size(); i++) {
            cerr << ExtMsgInd[i];
          }
          cerr << " letter=" << MsgIndLtr << " IMI=";
          for (i=0; i<(int)IntMsgInd.size(); i++) {
            cerr << IntMsgInd[i];
          }
          cerr << endl;
        }
        
      } while (!SetWheels(IntMsgInd));
      
      if (KeyListIndicator.length() != 2) {
        MyKLI.clear();
        MyKLI.push_back(MsgIndLtr);
        MyKLI.push_back(MsgIndLtr);
      }
      
      
      
      LetterCounter = 0;
      
      // Print system indicator, external message indicator
      // and key list indicator
      OutBuf << MsgIndLtr << MsgIndLtr;
      for (i=0; i<(int)ExtMsgInd.size(); i++) {
        OutBuf << ExtMsgInd[i];
        if (((i+3) % 5) == 0) {
          OutBuf << ' ';
        }
      }
      OutBuf << MyKLI << ' ';
      
      
      
    } else {
      // Must extract message indicator from the ciphertext
      
      // First, check for valid message indicator at each
      // end of message.
      
      if (MsgText.size() < 25) {
        // Message is too small for message indicators
        // plus at least one 5-letter group.
        cerr << "ERROR: Message is too small."
        << endl;
        exit(1);
      }
      for (i=0; i<(int)MsgInd1.size(); i++) {
        MsgInd1[i] = MsgText.front();
        MsgInd2[MsgInd2.size()-(i+1)] = MsgText.back();
        MsgText.pop_front();
        MsgText.pop_back();
      }
      if (Verbose) {
        cerr << "MsgInd1 = \"";
        for (i=0; i<(int)MsgInd1.size(); i++) {
          cerr << MsgInd1[i];
        }
        cerr << "\"" << endl;
        cerr << "MsgInd2 = \"";
        for (i=0; i<(int)MsgInd2.size(); i++) {
          cerr << MsgInd2[i];
        }
        cerr << "\"" << endl;
      }
      if (MsgInd1 != MsgInd2) {
        if (!Quiet) {
          cerr << "WARNING: Duplicate message indicator does not match." << endl;
        }
      }
      if (MsgInd1[0] != MsgInd1[1]) {
        cerr << "ERROR: System indicator not found."
        << endl;
        exit(1);
      }
      
      // Extract message indicator components
      MsgIndLtr = MsgInd1[0];
      for (i=0; i<(int)ExtMsgInd.size(); i++) {
        ExtMsgInd[i]=MsgInd1[i+2];
      }
      MyKLI.clear();
      i += 2;
      MyKLI.push_back(MsgInd1[i++]);
      MyKLI.push_back(MsgInd1[i++]);
      
      
      // See if corresponding key file exists
      // Try .txt extension first, then .m209key if not found.
      string Keyfile1 = KeyDir + "/" + MyKLI + KEYFILE_SUFFIX1;
      string Keyfile2 = KeyDir + "/" + MyKLI + KEYFILE_SUFFIX2;

      if (LoadKey(Keyfile1)) {
      } else if (LoadKey(Keyfile2)) {
      } else  if (!Quiet) {
        cerr << "Key file not found; using current key settings."
        << endl;
      }
      
      // Generate internal message indicator
      LetterCounter = 0;
      if (!SetWheels(ExtMsgInd)) {
        cerr << "ERROR: Could not set wheels to external message indicator."
        << endl;
        exit(1);
      }
      for (i=0; i<(int)IntMsgInd.size(); i++) {
        IntMsgInd[i]=Cipher(MsgIndLtr);
      }
      
      // Reset letter counter and attempt to set wheels
      LetterCounter = 0;
      if (!SetWheels(IntMsgInd)) {
        cerr << "ERROR: Failed to set internal message indicator."
        << endl;
        exit(1);
      }
    }
  }
  
  while (!MsgText.empty()) {
    InC = MsgText.front();
    MsgText.pop_front();
    
    // Process the character
    OutC = Cipher(InC);
    
    // In decipher mode, convert Z to space
    if (!CipherMode && (OutC == 'Z')) {
      OutC = ' ';
    }
    
    // Output the character
    OutBuf << OutC;
    
    // Add a space or line break every five letters in encipher mode.
    if (CipherMode && LetterCounter && (LetterCounter % 5 == 0)) {
      
      // Break line every 25 letters, also counting the initial 10 letter
      // message indicator when appropriate.
      if (((LetterCounter + ((AutoIndicator && CipherMode)?10:0)) % 25) == 0) {
        OutBuf << endl;
      } else {
        OutBuf << ' ';
      }
    }
  }
  
  if (AutoIndicator && CipherMode) {
    
    // Pad out message if necessary with unenciphered 'X's
    if (LetterCounter % 5) {
      while (LetterCounter % 5) {
        if (Verbose) {
          cerr << "Padding with unenciphered X" << endl;
        }
        OutBuf << 'X';
        ++LetterCounter;
      }
      
      if (((LetterCounter + 10) % 25) == 0) {
        OutBuf << endl;
      } else {
        OutBuf << ' ';
      }
    }
    
    
    // Print system indicator, external message indicator
    // and key list indicator
    OutBuf << MsgIndLtr << MsgIndLtr;
    for (i=0; i<(int)ExtMsgInd.size(); i++) {
      OutBuf << ExtMsgInd[i];
      
      // Add a space or line break every five letters
      if (((i+3) % 5) == 0) {
        if (((LetterCounter + 10 + (i+3)) % 25) == 0) {
          OutBuf << endl;
        } else {
          OutBuf << ' ';
        }
      }
      
    }
    OutBuf << MyKLI << ' ' << endl;
    
    // If NetIndicator is not empty, then add net indicator
    // and group count at beginning of output buffer.
    if (!NetIndicator.empty()) {
      string tempbuf  = OutBuf.str();
      OutBuf.clear();
      OutBuf.str(std::string());
      OutBuf << NetIndicator << " GR " << ((LetterCounter/5)+4) << endl;
      OutBuf << tempbuf;
    }
    
  }
  
  OutBuf << endl;
  
  // Finally, send buffered output to output stream.
  OutText << OutBuf.str();
}

