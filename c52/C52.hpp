//
/// \file C52.hpp
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

#ifndef C52_hpp
#define C52_hpp

#include <vector>
using std::vector;
#include <array>
using std::array;
#include <string>
using std::string;

#include <bitset>
using std::bitset;
#include <boost/date_time/gregorian/gregorian.hpp>
using namespace boost::gregorian;

#include "C52Keywheel.hpp"

#include <iostream>
using std::cout;
using std::ostream;
using std::istream;
#include <vector>
#include <array>

//! Number of pin wheels.
#define NUM_WHEELS  6

//! Number of lug bars in drum.
#define NUM_LUG_BARS  32

//! This defines how hard we are willing to work ar generating a key.
//
#define GUMPTION 1000

//! Filename suffixes for key files
//
#define KEYFILE_SUFFIX ".c52key"  // alternate extension for backwards compatibility


extern bool Verbose;
extern bool Quiet;


/// struct with lug bars together iwth a score for their fit with Appendix II of the
/// Technical Manual
struct ScoredDrum {
  array<bitset<NUM_WHEELS>, NUM_LUG_BARS> drum;
  int score;
  friend bool operator< (const ScoredDrum& lhs, const ScoredDrum& rhs)
    {return lhs.score < rhs.score;}
};


//! This class simulates an C52 series cipher machine.
//
class C52 {
public:
  
  typedef array<bitset<NUM_WHEELS>, NUM_LUG_BARS> DrumType;
  
private:
  
  /// Which wheel size in in each position
  array<size_t, NUM_WHEELS> wheel_idx;

      //! Array of NUM_WHEELS key wheels.
  array<C52Keywheel, NUM_WHEELS>      Wheels;
  
  //! Array of NUM_LUG_BARS lug bars, each defined by NUM_WHEELS bits.
  //
  DrumType  Drum;
      
  /// Print offset
  int print_offset;
  
  //! Letter counter (a 4-digit counter in a real machine).
  //
  int          LetterCounter;
  
  /// The NumArrays contained in Appendix II Group A of the 1944 Tecnical
  /// Manual. Group A are the arrays without repeats.
  static vector<array<int,NUM_WHEELS> > NumArrayA;
  
  /// the array in Appendix II Group B of the 1944 Technical Manual
  /// These are the arrays with one repeat.
  static vector<array<int,NUM_WHEELS> > NumArrayB;
  static const array<vector<string>, 12 > wheel_labels;
  static const array<int, 12> offsets;


public:
  
  //! Default constructor.
  //
  C52();
  
  
  //! Destructor.
  //
  ~C52() {}
  
  
  //! Encipher/Decipher one letter.
  //
  char Cipher(char c);
  
  
  //! Clear current key settings.
  //
  void ClearKey(void);
  
  
  //! Print current key setting.
  //
  void PrintKey(const string& NetIndicator, date d, ostream& os = cout);
  
  /// Return the Drum
  DrumType getDrum() { return Drum;}
  
  /// Load key from file using indicated KeyListIndicator and NetIndicator
  bool LoadKey(const string& fname, string& NetIndicator, date d);
      
  /// Load key from database based on date and NetIndicator
      bool LoadKey(date d, string& NetIndicator);
  
  /// Load key for an istream using designated KeyListIndicator and NetIndicator
  void LoadKey(istream& keyfile, string& NetIndicator, date d);
  
  /// Generaate a random key
  void GenKey(bool CX52=false);
  
  //! Generate arrays similar to those in Appendix II of the 1944l
  /// Technical Manual
  void GenNumArrays();
  
  /// Validate that a proposed drum satisfies the sum condition
  bool ValidateDrum(DrumType drum);
  
  /// Geneate a list of all of the drums that are consistem with NumArray
  /// and which satisfy the sum dest
  vector<ScoredDrum>
  GoodDrums(array<int, NUM_WHEELS> NumArray, int& tries);
  
  //! Reset Letter Counter and Code Wheels.
  //
  void ResetCounter(void);
  
  
  //! Set code wheel positions.
  //
  //! Supply at least six indicators. If more than
  //! six are provided, unusable indicators will be discarded.
  //! Returns true if successful, false if not.
  //
  bool SetWheels(vector<string> indicator);
  
  /// Set the print offset
  void SetPrintOffset(int offset) {
    print_offset = offset;
  }
  
  //! Encipher/Decipher a stream, exit() on EOF.
  void CipherStream(bool AutoKey,
                    bool AutoMsgIndicator,
                    date d,
                    string& NetIndicator,
                    string KeyDir, bool CipherMode,
                    istream& InText, ostream& OutText);
  
  /// Compare two lugbars for sorting purposes
  static bool CompareBars(bitset<NUM_WHEELS> a, bitset<NUM_WHEELS> b);
};


#endif /* C52_hpp */
