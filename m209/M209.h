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

#include "Keywheel.h"


//! Number of pin wheels.
//
#define NUM_WHEELS	6

//! Number of lug bars in drum.
//
#define NUM_LUG_BARS	27

//! This defines how hard we are willing to work ar generating a key.
//
#define GUMPTION 50

//! Filename suffixes for key files
//
#define KEYFILE_SUFFIX1 ".txt"		// preferred extension
#define KEYFILE_SUFFIX2 ".m209key"	// alternate extension for backwards compatibility


extern bool Verbose;
extern bool Quiet;


using namespace std;

//! This class simulates an M209 series cipher machine.
//
class M209 {

private:

    //! Array of NUM_WHEELS key wheels.
    //
    vector <Keywheel>			Wheels;

    //! Array of NUM_LUG_BARS lug bars, each defined by NUM_WHEELS bits.
    //
    vector <bitset <NUM_WHEELS> >	Drum;

    //! Letter counter (a 4-digit counter in a real machine).
    //
    int					LetterCounter;

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
    void PrintKey(string KeyListIndicator, string NetIndicator);


    //! Load key from file.
    //
    void LoadKey(const string& fname);
    void LoadKey(const string& fname, string& KeyListIndicator, string& NetIndicator);


    //! Generate a random key.
    //
    void GenKey(void);


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

};


#endif // _M209_H_

// Local Variables: ***
// mode:c++ ***
// End: ***
