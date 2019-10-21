//
/// \file C52.cpp
/// \package hagelin
//
/// \author Joseph Dunn on 9/30/19.
/// \copyright © 2019 Joseph Dunn.
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
using std::cerr;
using std::endl;
#include <iomanip>
using std::setfill;
using std::setw;
using std::dec;
#include <fstream>
using std::ifstream;
#include <sstream>
using std::stringstream;
using std::istringstream;
using std::ostringstream;
#include <list>
using std::list;
#include <boost/algorithm/string/split.hpp>
using boost::algorithm::split;
using boost::algorithm::token_compress_on;
#include <boost/algorithm/string/classification.hpp>
using boost::algorithm::is_alpha;
#include <regex>
using std::regex;
using std::smatch;
using std::regex_match;

#include <config.h>
#include "C52.hpp"

inline int mod(int a, int b) {
  int ret = a % b;
  if (ret<0)
    ret += b;
  return ret;
}

bool is_alpha(string s) {
  bool ret = true;
  for (size_t i=0; i<s.size(); ++i){
    if (!isalpha(s.at(i))) {
      ret=false;
      break;
    } else {
      continue;
    }
  }
  return ret;
}

const array<vector<string>, 12 >  C52::wheel_labels ={
  vector<string> {"A","B","C","D","E","F","G","H","I","J","K","L","M","N",
                  "O","P","Q","R","S","T","U","V","X","Y","Z"},
  vector<string> {"A","B","C","D","E","F","G","H","I","J","K","L","M","N",
                  "O","P","Q","R","S","T","U","V","W","X","Y","Z"},
  vector<string> {"A","B","C","D","E","F","G","H","I","10","J","K","L","M","N",
                  "O","P","Q","R","20","S","T","U","V","W","X","Y","Z","29"},
  vector<string> {"A","B","C","D","E","06","F","G","H","I","J","12","K","L","M","N",
                  "O","P","19","Q","R","S","T","U","25","V","W","X","Y","Z","31"},
  vector<string> {"A","02","B","04","C","06","D","08","E","10","F","12","G","14","H","16",
                  "I","18","J","20","K","22","L","24","M","26","N","28","O","30","P","32",
                  "Q","34"},
  vector<string> {"A","B","C","04","D","E","07","F","G","10","H","I","J","14","K","L","17",
                  "M","N","20","O","P","Q","24","R","S","27","T","U","30","V","W","X","34",
                  "Y","Z","37"},
  vector<string> {"A","02","B","04","C","06","D","08","E","10","F","12","G","14","H","16",
                  "I","18","J","20","K","22","L","24","M","26","N","28","O","30","P","32",
                  "Q","34","R","36","S","38"},
  vector<string> {"A","B","03","C","D","06","E","F","09","G","11","H","I","14","J","K","17",
                  "L","M","20","N","22","O","P","25","Q","R","28","S","T","31","U","33",
                  "V","W","36","X","Y","39","Z","41"},
  vector<string> {"A","02","B","04","C","06","D","08","E","10","F","12","G","14","H","16",
                  "I","18","J","20","K","22","L","24","M","26","N","28","O","30","P","32",
                  "Q","34","R","36","S","38","T","40","U","42"},
  vector<string> {"A","B","03","C","05","D","E","08","F","10","G","H","13","I","15","J",
                  "K","18","L","20","M","N","23","O","25","P","Q","28","R","30","S","T","33",
                  "U","35","V","W","38","X","40","Y","Z","43"},
  vector<string> {"A","02","B","04","C","06","D","08","E","10","F","12","G","14","H","16",
                  "I","18","J","20","K","22","L","24","M","26","N","28","O","30","P","32",
                  "Q","34","R","36","S","38","T","40","U","42","V","44","W","46"},
  vector<string> {"A","B","03","C","05","D","07","E","09","F","G","12","H","14","I","16",
                  "J","18","K","L","21","M","23","N","O","26","27","P","29","Q","R","32",
                  "S","34","T","36","U","38","V","W","41","X","43","Y","45","Z","47"}
};

const array<int, 12> C52::offsets = {-15,-15,-17,-18,-20,-22,-22,-24,-24,-25,-27,-27};

C52::C52() {
  
  ClearKey();
}

char C52::Cipher(char c) {
  
  // Check for valid character
  if ((c < 'A') || (c > 'Z')) {
    throw std::invalid_argument("C52::Cipher(): invalid character");
  }
  
  // Get character number, 0..25
  int cnum = c - 'A';
  
  // Get mask of active pins at offsets from current code wheel positions.
  bitset<NUM_WHEELS> pins;
  for (size_t i=0; i<NUM_WHEELS; i++) {
    pins[i] = Wheels.at(i).ReadPinOffset();
  }
  
  // Compare active pins to each lug bar. For each bar where one or
  // more active lugs match an active pin, add one to the key value.
  int key = print_offset;
  for (size_t i=5; i<NUM_LUG_BARS; i++) {
    key += (Drum.at(i) & pins).any();
  }
  
  cnum = mod(cnum+key, 26);
  
  // Increment the letter counter
  ++LetterCounter;
  
  // Compute ciphered letter from reciprocal alphabet
  char c2 = 'Z' - cnum;
  
  if (Verbose) {
    cerr << "Counter: "
    << setfill('0') << setw(4) << dec << LetterCounter
    << "  Wheels: ";
    for (size_t i=0; i<NUM_WHEELS; i++) {
      cerr << Wheels.at(i).GetPosName();
    }
    cerr << "  Pin Positions: ";
    for (size_t i=0; i<NUM_WHEELS; i++) {
      cerr << Wheels.at(i).GetOffsetName();
    }
    cerr << "  Pin Values: ";
    for (size_t i=0; i<NUM_WHEELS; i++) {
      cerr << pins[i];
    }
    cerr << "  In: " << c
    << "  Key: " << setfill(' ') << setw(2) << key
    << "  Out: " << c2;
    cerr << endl;
  }
  // Advance first code wheel every time.
  Wheels.at(0).Rotate(1);
  for (size_t i=1; i<NUM_WHEELS; i++) {
    // Use the first 5 lugbars to determine advance of wheels 1 to 5
    Wheels.at(i).Rotate((Drum.at(i-1) & pins).any() ? 1 : 0);
  }
  
  return c2;
}

void C52::ClearKey(void) {

  for (size_t i=0; i<NUM_WHEELS; Wheels.at(i++).Clear());
  for (size_t i=0; i<NUM_LUG_BARS; i++) {
    for (size_t j=0; j<NUM_WHEELS; j++) {
      Drum.at(i)[j] = false;
    }
  }
  LetterCounter = 0;
}

void C52::PrintKey(const string& NetIndicator, date d, ostream& os) {
  os << "----------------------------------" << endl;
  string head = NetIndicator + "  " + to_simple_string(d);
  os << string((34-head.size())/2,' ') << head << endl;
  os << "----------------------------------" << endl;
  array<size_t, NUM_WHEELS> wheel_size;
  for (size_t i=0; i<NUM_WHEELS; ++i)
    wheel_size.at(i) = Wheels.at(i).GetWheelSize();
    
  for (size_t i=0; i<NUM_WHEELS; ++i)
    os << setw(2) << wheel_size.at(i) << " ";
  os << " NR  1 2 3 4 5 6" << endl;
  os << "----------------------------------" << endl;

  for (size_t i=0; i<NUM_WHEELS; i++) {
    Wheels[i].SetPosition(0);
  }
  
  size_t nrows = NUM_LUG_BARS;
  for (size_t i=0; i<NUM_WHEELS; ++i)
    nrows = std::max(nrows, wheel_size.at(i));
  for (size_t i = 0; i < nrows; i++) {
    for (size_t j = 0; j < NUM_WHEELS; j++) {
      if (i < wheel_size.at(j)) {
        if (Wheels.at(j).ReadPin())
          os << setfill('0') << setw(2) << i+1 << " ";
        else
          os << "-- ";
        Wheels.at(j).Rotate(1);
      } else {
        os << "   ";
      }
    }
    os << " " << setfill('0') << setw(2) << (i+1) << "  ";
    if (i < NUM_LUG_BARS) {
      for (size_t j=0; j<NUM_WHEELS; ++j)
        if (Drum.at(i)[j])
          os << setw(1) << j+1 << " ";
        else
          os << "- ";
    }
    
    os << endl;
  }
  
  os << "----------------------------------" << endl;
  
  for (size_t i=0; i<NUM_WHEELS; i++) {
    Wheels.at(i).SetPosition(0);
  }
  
  LetterCounter = 0;
  print_offset=0;
  for (size_t i=0; i<25; i++) {
    os << Cipher('A');
    if ((i % 5) == 4) {
      os << ' ';
    }
  }
  
  os << endl;
  os << "----------------------------------" << endl;

}

bool C52::LoadKey(const string& fname, string &NetIndicator, date d) {

  if (Verbose) {
    cerr << "Looking for key file " << fname << endl;
  }
  ifstream keyfile(fname, ifstream::in); // key file input stream
  if (!keyfile) return false;
  if (!Quiet) {
    cerr << "Loading key file " << fname << endl;
  }
  LoadKey(keyfile, NetIndicator, d);
  return true;
}

bool C52::LoadKey(date d, string& NetIndicator) {
  string root_dir;
  if (getenv("C52_KEYLIST_DIR") != nullptr)
    root_dir = getenv("C52_KEYLIST_DIR");
  else {
    cerr << "ERROR: Environmental variable C52_KEYLIST_DIR must be defined to";
    cerr << " use the -A mode." << endl;
    exit(1);
  }
  if (root_dir.back() != '/')
    root_dir += "/";
  string d_str = to_iso_string(d);
  string key_file = NetIndicator + "/" + d_str + ".c52key";
  bool ret = LoadKey(root_dir + key_file, NetIndicator, d);
  if (!ret) {
    cerr << "Could not load key from file " << (root_dir + key_file) << endl;
  }
  return ret;
}

void C52::LoadKey(istream& keyfile, string& NetIndicator, date d) {
  ClearKey();
  
  if (Verbose) {
    cerr << "Reading key file " << endl;
  }
  
  // Read key file one line at a time
  string line;
  getline(keyfile, line);  // Line of ---...
  getline(keyfile, line);
  if (!Quiet) {
    cerr << "Description: " << line << endl;
  }
  getline(keyfile, line);  // line of ---
  getline(keyfile, line);  // line with wheel sizes NR and lug heading
  vector<string> tokens;
  split(tokens, line, isspace, token_compress_on);
  if (tokens.size() < 2*NUM_WHEELS+1) {
    throw std::runtime_error("C52::LoadKey: Insufficent tokens on header line.");
  }
  array<size_t, NUM_WHEELS> wheel_size;
  for (size_t i = 0; i<NUM_WHEELS; ++i) {
    istringstream iss{tokens.at(i)};
    if (!(iss >> wheel_size.at(i))) {
      ostringstream oss;
      oss << "C52::LoadKey: wheel_size[" << i << "] is unreadable: "
          << tokens.at(i);
      throw std::runtime_error(oss.str());
    }
  }
  // Figure out the index of the wheels with the specified size
  for (size_t i=0; i<NUM_WHEELS; ++i) {
    size_t j;
    for (j=0; j<12; ++j) {
      if (wheel_size.at(i) == wheel_labels.at(j).size()) {
        wheel_idx.at(i) = j;
        break;
      }
    }
    if (j >= 12) {
      ostringstream oss;
      oss << "C52::LoadKey: Unable to find wheel of size " << wheel_size.at(i);
      throw std::runtime_error(oss.str());
    }
    Wheels.at(i).Clear();
    for (auto a : wheel_labels.at(wheel_idx.at(i))) {
      Wheels.at(i).AddPosition(a);
    }
    Wheels[i].SetReadOffset(offsets.at(wheel_idx.at(i)));

  }
  getline(keyfile, line);  // Line of ---
  size_t nrows = NUM_LUG_BARS;
  for (size_t j=0; j<NUM_WHEELS; ++j) {
    nrows = std::max(nrows, wheel_size.at(j));
    Wheels.at(j).SetPosition(0);
    Wheels.at(j).ClearAllPins();
  }
  for (size_t i=0; i<nrows; ++i) {
    ostringstream oss;
    oss << setfill('0') << setw(2) << (i+1);
    getline(keyfile, line);
    vector<string> tokens0;
    split(tokens0, line, isspace, token_compress_on);
    tokens.clear();
    for (string a : tokens0)
      if(a.size()>0) tokens.push_back(a);
    auto itr = tokens.begin();
    for (size_t j=0; j<NUM_WHEELS; ++j) {
      if (i < wheel_size.at(j)) {
        if ((*itr) == "--") {
          Wheels.at(j).SetPin(false);
          Wheels.at(j).Rotate(1);
          ++itr;
        } else if ((*itr) == oss.str()) {
          Wheels.at(j).SetPin(true);
          Wheels.at(j).Rotate(1);
          ++itr;
        } else {
          throw std::runtime_error("C52::LoadKey: Invalid pin indicator.");
        }
          
      }
    }
    if ((*itr) == oss.str()) {
      ++itr;
    } else {
      throw std::runtime_error("C52::LoadKey: Invalid row number.");
    }
    if (i < NUM_LUG_BARS) {
      for (size_t j=0; j<NUM_WHEELS; ++j) {
        ostringstream oss;
        oss << setw(1) << j+1;
        if ((*itr) == oss.str())
          Drum.at(i)[j]=true;
        else if ((*itr) == "-")
          Drum.at(i)[j]=false;
        else {
          throw std::runtime_error("C52::LoadKey: Invalid character for Drum.");
        }
        ++itr;
      }
    }
  } // for i
  getline(keyfile, line);  // Should be line of ---
  if (!getline(keyfile, line) || line.size() == 0) {
    if (!Quiet) {
      cerr << "C52::LoadKey: No 25 letter check is present." << endl;
    }
    // No check
    return;
  }
  if (Verbose) {
    cerr << "  letter check line \"" << line << "\"" << endl;
  }

  // Reassemble check line without group spacing
  string check_line;
  for (size_t i=0; i<line.size(); ++i) {
    if (!isspace(line[i]))
      check_line.push_back(line[i]);
  }
  
  if (Verbose) {
    cerr << "Running 25 letter check" << endl;
  }
  
  string test_line;
  print_offset=0;
  for (int n=0; n<NUM_WHEELS; n++) {
    Wheels[n].SetPosition(0);
  }
  for (int n=0; n<25; n++) {
    test_line += Cipher('A');
  }
  if (test_line != check_line) {
    throw std::range_error("C52::LoadKey(): Key file error:"
                           " 25 letter check failed.");
  }

  // Success! Set the machine to clean initial state
  LetterCounter = 0;
  for (int n=0; n<NUM_WHEELS; n++) {
    Wheels[n].SetPosition(0);
  }
}

void C52::ResetCounter(void) {
  
  LetterCounter = 0;
}


bool C52::SetWheels(vector<string> indicator) {
  size_t i,j;
  for (i=0, j=0; (i<NUM_WHEELS) && (j < indicator.size()); i++, j++) {
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

void C52::CipherStream(bool AutoKey,
                        bool AutoMsgIndicator,
                        date d,
                        string& NetIndicator,
                        string KeyDir,
                        bool CipherMode,
                        istream& InText, ostream& OutText) {
  
  // Output is buffered in OutBuf so that we can insert group count at
  // beginning of buffer after generating ciphertext if needed. This is
  // a bit of a kludge, but it works.
  
  
  // Prepare message indicator array;
  array<char, 15> ExtMsgInd;
  array<char, 15> IntMsgInd;
  
  // regexp matching net indicator and group count in ciphertext.
  // Tolerate some variation in the "GR" text.
  regex netind_regex;
  netind_regex = "[\\s]*"       // optional leading whitespace
  "([A-Z0-9]+)"         // net indicator
  "[\\s]+"              // whitespace
  "([A-Z0-9\\-]+)"      // date
  "[\\s]+"              // whitespace
  "(GR(P|PS|OUP|OUPS)?)"  // GR, GRP, GRPS, GROUP or GROUPS
  "[\\s]+"              // whitespace
  "([0-9]+)"            // group count
  "[\\s]*";             // optional trailing whitespace
  
  // Read entire input message into buffer. If we are deciphering
  // with automatic indicator extraction, we will look for repeated
  // message indicators at end of message, and snip them off.
  list<char> MsgText;
  while (InText.good()) {
    // Read a line
    string line;
    getline(InText, line);
    
    // upcase it
    boost::to_upper(line);
    
    if (Verbose) {
      cerr << "read message line \"" << line << "\"" << endl;
    }
    
    // If decipher mode get NetIndicator and date.
    bool FoundNetInd = false;
    if (!CipherMode && !FoundNetInd) {
      smatch matches;
      if (regex_match(line, matches, netind_regex)) {
        // Line looks like a net indicator line.
        if (AutoKey) {
          NetIndicator = matches[1].str();
          d = from_simple_string(matches[2].str());
          if (Verbose) {
            cerr << "Using NetIndicator \"" << NetIndicator << "\"" <<
            ", and date " << to_simple_string(d) << " from cipher text" << endl;
          }
        } else {
          if (Verbose) {
            cerr << "Discarding net indicator line \"" << line << "\"" << endl;
          }
        }
        FoundNetInd = true;
        continue;
      }
    }
    
    // Now copy the line into the message buffer, processing
    // it char by char
    for (string::iterator i=line.begin(); i < line.end(); i++) {
      char InC = *i;
      
      // When enciphering, translate space to X
      if (CipherMode && (InC == ' ')) {
        InC = 'X';
      }
      
      // Ignore if not alphabetic
      if (isalpha(InC)) {
        MsgText.push_back(InC);
      }
      
    }
  }
  
  stringstream OutBuf;
  // Are we automatically setting message indicators?
  if (AutoMsgIndicator) {
    
    if (CipherMode) {
      // Must generate random indicator, 6 character for start pos for
      // 6 characters of the starting position for the text;
      // encipher it, together with the trigram which is set to "XXA"
      // indicating that the print offset should be zero.
      
      if (AutoKey) {
        d = day_clock::universal_day();
        if (!LoadKey(d, NetIndicator)) {
          cerr << "ERROR: Unable to load key data base " << endl;
          exit(1);
        }
      } else {
        string Keyfile = KeyDir + "/" + to_iso_string(d) + KEYFILE_SUFFIX;

        if (LoadKey(Keyfile, NetIndicator, d)) {
        } else if (!Quiet) {
          cerr << "ERROR: Key file" + Keyfile +" not found." << endl;
          exit(1);
        }
      }

      // Clear letter counter and randomize wheel positions.
      // Store positions as external message indicator.
      LetterCounter = 0;
      for (size_t i=0; i<NUM_WHEELS; i++) {
        ui_dist dist(0,Wheels.at(i).GetWheelSize()-1);
        int r = dist(gen);
        Wheels.at(i).SetPosition(r);
        while (!is_alpha(Wheels.at(i).GetPosName())) {
          // the first posioon should always be alpha so r-1>=0 here
          Wheels.at(i).Rotate(1);
        }
        ExtMsgInd.at(i)=IntMsgInd.at(i)=Wheels.at(i).GetPosName().front();
      }
      
      // Pick six rnadom letters to encipher for
      // We can't tell whether they're legal until after encipherment
      for (size_t i =0; i<NUM_WHEELS; ++i) {
        ui_dist dist(0,Wheels.at(i).GetWheelSize()-1);
        int r = dist(gen);
        Wheels.at(i).SetPosition(r);
        while (!is_alpha(Wheels.at(i).GetPosName())) {
          // the first posioon should always be alpha so r-1>=0 here
          Wheels.at(i).Rotate(1);
        }
        IntMsgInd.at(i+6)=Wheels.at(i).GetPosName().front();
      }
      print_offset=0;
      // We need to reset the wheel positions to encrypt the starting pos
      for (size_t i=0; i<NUM_WHEELS; ++i) {
        string pos{IntMsgInd.at(i)};
        Wheels.at(i).SetPosByName(pos);
      }
      for (size_t i=NUM_WHEELS; i<2*NUM_WHEELS; ++i) {
        ExtMsgInd.at(i) = Cipher(IntMsgInd.at(i));
      }
      // Choose a random print offset & generate trigram
      ui_dist dist(0,25);
      int p_offset = dist(gen);
      IntMsgInd.at(12) = 'X';
      ExtMsgInd.at(12) = Cipher(IntMsgInd.at(12));
      IntMsgInd.at(13) = 'X';
      ExtMsgInd.at(13) = Cipher(IntMsgInd.at(13));
      IntMsgInd.at(14) = p_offset+'A';
      ExtMsgInd.at(14) = Cipher(IntMsgInd.at(14));
      
      for (size_t i=0; i<NUM_WHEELS; ++i) {
        string pos{IntMsgInd.at(i+NUM_WHEELS)};
        Wheels.at(i).SetPosByName(pos);
      }
      print_offset=p_offset;
      
      LetterCounter = 0;
      
      // Print system indicator, external message indicator
      // and key list indicator
      for (size_t i=0; i<ExtMsgInd.size(); i++) {
        OutBuf << ExtMsgInd[i];
        if (((i+1) % 5) == 0) {
          OutBuf << ' ';
        }
      }
      
    } else {
      // first load the key
      
      if (AutoKey) {
        if (!LoadKey(d, NetIndicator)) {
          cerr << "ERROR: Unable to load key from data base" << endl;
          exit(1);
        }
      } else {
        // See if corresponding key file exists
        // Try .txt extension first, then .m209key if not found.
        string Keyfile = KeyDir + "/" + to_iso_string(d) + KEYFILE_SUFFIX;

        if (LoadKey(Keyfile, NetIndicator, d)) {
        } else {
          cerr << "ERROR: Key file not found." << endl;
          exit(1);
        }

      }
      // Must extract message indicator from the ciphertext
      
      // First, check for valid message indicator at each
      // end of message.
      
      if (MsgText.size() < 15) {
        // Message is too small for message indicators
        // plus at least one 5-letter group.
        cerr << "ERROR: Message is too small."
        << endl;
        exit(1);
      }
      for (size_t i=0; i<ExtMsgInd.size(); i++) {
        ExtMsgInd.at(i) = MsgText.front();
        MsgText.pop_front();
      }
      if (Verbose) {
        cerr << "ExtMsgInd = \"";
        for (size_t i=0; i<ExtMsgInd.size(); i++) {
          cerr << ExtMsgInd.at(i);
        }
        cerr << "\"" << endl;
      }
      
      print_offset = 0;
      // Extract message indicator components
      for (size_t i=0; i<NUM_WHEELS; ++i) {
        string pos{ExtMsgInd.at(i)};
        Wheels.at(i).SetPosByName(pos);
      }
      for (size_t i=NUM_WHEELS; i< 2*NUM_WHEELS+3; ++i) {
        IntMsgInd.at(i) = Cipher(ExtMsgInd.at(i));
      }
      if (Verbose) {
        cerr << "IntMsgInd = \"";
        for (size_t i=NUM_WHEELS; i<IntMsgInd.size(); ++i)
          cerr << IntMsgInd.at(i);
        cerr << "\"" << endl;
      }
      for (size_t i=0; i<NUM_WHEELS; ++i) {
        string pos{IntMsgInd.at(i+NUM_WHEELS)};
        Wheels.at(i).SetPosByName(pos);
      }
      print_offset = IntMsgInd.at(14) - 'A';
      
      // Generate internal message indicator
      LetterCounter = 0;

    } // decipher
  } // if AutoMsgIndicator
  
  while (!MsgText.empty()) {
    char InC = MsgText.front();
    MsgText.pop_front();
    
    // Process the character
    char OutC = Cipher(InC);
    
    // In decipher mode, convert X to space
    if (!CipherMode && (OutC == 'X')) {
      OutC = ' ';
    }
    
    // Output the character
    OutBuf << OutC;
    
    // Add a space or line break every five letters in encipher mode.
    if (CipherMode && LetterCounter && (LetterCounter % 5 == 0)) {
      
      // Break line every 25 letters, also counting the initial 10 letter
      // message indicator when appropriate.
      if (((LetterCounter + ((AutoMsgIndicator && CipherMode)?15:0)) % 25) == 0) {
        OutBuf << endl;
      } else {
        OutBuf << ' ';
      }
    }
  }
  
  if (AutoMsgIndicator && CipherMode) {
    
    // Pad out message if necessary with unenciphered 'X's
    if (LetterCounter % 5) {
      while (LetterCounter % 5) {
        if (Verbose) {
          cerr << "Padding with unenciphered X" << endl;
        }
        OutBuf << 'X';
        ++LetterCounter;
      }
      
      if (((LetterCounter + 15) % 25) == 0) {
        OutBuf << endl;
      } else {
        OutBuf << ' ';
      }
    }
    
/*
    // Print system indicator, external message indicator
    // and key list indicator
    for (size_t i=0; i<ExtMsgInd.size(); i++) {
      OutBuf << ExtMsgInd[i];
      
      // Add a space or line break every five letters
      if (((i+1) % 5) == 0) {
        if (((LetterCounter + 15 + (i+1)) % 25) == 0) {
          OutBuf << endl;
        } else {
          OutBuf << ' ';
        }
      }
      
    }
 */
    
    // If NetIndicator is not empty, then add net indicator
    // and group count at beginning of output buffer.
    if (!NetIndicator.empty()) {
      string tempbuf  = OutBuf.str();
      OutBuf.clear();
      OutBuf.str(std::string());
      OutBuf << NetIndicator << " "
             << to_simple_string(d)
             << " GR " << ((LetterCounter/5)+3) << endl;
      OutBuf << tempbuf;
    }
    
  }
  
  OutBuf << endl;
  
  // Finally, send buffered output to output stream.
  OutText << OutBuf.str();
}

