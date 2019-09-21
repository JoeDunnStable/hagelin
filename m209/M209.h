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
 * \file M209.h
 * \brief Definition of the M209 class.
 * \package hagelin
 */

#ifndef _M209_H_
#define _M209_H_

#include <bitset>
using std::bitset;
#include "Keywheel.h"

#include <iostream>
#include <vector>
#include <array>


//! Number of pin wheels.
//
#define NUM_WHEELS  6

//! Number of lug bars in drum.
//
#define NUM_LUG_BARS  27

//! This defines how hard we are willing to work ar generating a key.
//
#define GUMPTION 1000

//! Filename suffixes for key files
//
#define KEYFILE_SUFFIX1 ".txt"    // preferred extension
#define KEYFILE_SUFFIX2 ".m209key"  // alternate extension for backwards compatibility


extern bool Verbose;
extern bool Quiet;


using namespace std;

/// q e4um or lug bars together iwth a score for their fit with Appendix II of the
/// Technical Manual
struct ScoredDrum {
  array<bitset<NUM_WHEELS>, NUM_LUG_BARS> drum;
  int score;
  friend bool operator< (const ScoredDrum& lhs, const ScoredDrum& rhs)
    {return lhs.score < rhs.score;}
};


//! This class simulates an M209 series cipher machine.
//
class M209 {
  
private:
  
  //! Array of NUM_WHEELS key wheels.
  //
  vector <Keywheel>      Wheels;
  
  //! Array of NUM_LUG_BARS lug bars, each defined by NUM_WHEELS bits.
  //
  array<bitset <NUM_WHEELS>, NUM_LUG_BARS >  Drum;
  
  //! Letter counter (a 4-digit counter in a real machine).
  //
  int          LetterCounter;
  
  /// The NumArrays contained in Appendix II Group A of the 1944 Tecnical
  /// Manual. Group A are the arrays without repeats.
  static vector<array<int,6> > NumArrayAppendixIIA;
  
  /// the array in Appendix II Group B of the 1944 Technical Manual
  /// These are the arrays with one repeat.
  static vector<array<int,6> > NumArrayAppendixIIB;
  
  
public:
  
  //! Default constructor.
  //
  M209();
  
  
  //! Destructor.
  //
  ~M209() {}
  
  
  //! Encipher/Decipher one letter.
  //
  char Cipher(char c);
  
  
  //! Clear current key settings.
  //
  void ClearKey(void);
  
  
  //! Print current key setting.
  //
  void PrintKey(string KeyListIndicator, string NetIndicator,
                ostream& os = cout);
  
  
  //! Load key from file.
  //
  bool LoadKey(const string& fname);
  
  /// Load key from file using indicated KeyListIndicator and NetIndicator
  bool LoadKey(const string& fname, string& KeyListIndicator, string& NetIndicator);
  
  /// Load key for an istream using designated KeyListIndicator and NetIndicator
  void LoadKey(istream& keyfile, string&KeyListIndicator, string& NetIndicator);
  
  
  /// Generaate a random key using mehtod in Appendices of 1944 Technical Manual
  void GenKey1944(void);
  
  //! Generate arrays similar to those in Appendix II of the 1944l
  /// Technical Manual
  void GenAppendixII(vector<array<int, 6> >& NumArrayA,
                     vector<array<int, 6> >& NumArrayB);
  
  /// Validate that a proposed drum satisfies the sum condition
  bool ValidateDrum(array<bitset<NUM_WHEELS>, NUM_LUG_BARS> drum);
  
  /// Geneate a list of all of the drums that are consistem with NumArray
  /// and which satisfy the sum dest
  vector<ScoredDrum>
  GoodDrums(array<int, 6> NumArray, int& tries);
  
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
  
  
  //! Encipher/Decipher a stream, exit() on EOF.
  //
  void CipherStream(bool AutoIndicator,
                    string& KeyListIndicator,
                    string& NetIndicator,
                    string KeyDir, bool CipherMode,
                    istream& InText, ostream& OutText);
  
  /// Convert bitset representation lugbar to lug positions
  static void bitset2lugs(bitset<NUM_WHEELS> a, char& a1, char& a2);
  
  /// Compare two lugbars for sorting purposes
  static bool CompareBars(bitset<NUM_WHEELS> a, bitset<NUM_WHEELS> b);
};


#endif // _M209_H_

// Local Variables: ***
// mode:c++ ***
// End: ***
