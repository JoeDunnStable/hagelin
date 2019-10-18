/**************************************************************************
 * Copyright (C) 2019 Joseph dunn
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
 * \file keywheel.h
 * \brief Definition of the Keywheel class.
 * \package hagelin
 */

#ifndef _KEYWHEEL_H_
#define _KEYWHEEL_H_

#include <vector>
using std::vector;
#include <string>
using std::string;
#include <map>
using std::map;

extern bool Verbose;
extern bool Quiet;



/*!
 * \brief This class simulates a key wheel in a Hagelin pin-and-lug
 *        cipher machine.
 *
 */
class Keywheel {

private:

    //! Bitmap of pin state corresponding to each indicated wheel position.
    //
    vector <bool>	Pins;


    //! Number of wheel positions (also, number of pins).
    //
    int			WheelSize;


    //! Offset between indicated position and pin to be read.

    //! Offset between indicated wheel position and pin which is read during
    //! encode/decode operations. I.e., when indicated wheel position is "A",
    //! the pin adjacent to the "A" marking is not necessarily the one which
    //! is read out for comparison to the lug basket.
    //
    int			ReadOffset;


    //! Current position of wheel. 0 <= Position < WheelSize.
    //
    int			Position;

    //! Names of each key wheel position (i.e., "A", "B", "10", "11", etc.).
    //
    vector<string>	PosNames;

    //! Hash allowing decode of position from position name
    map<string, int>	PosByName;


public:

    //! Default constructor
    //
    Keywheel();


    //! Destructor
    //
    ~Keywheel() {}

    /// Clear the wheel
    void Clear();
  
    //! Add position to wheel with specified name (name must be unique).
    //
    void AddPosition(const string& name);


    //! Set read offset.
    //
    void SetReadOffset(int offset);


    //! Set wheel to specific position. 0 <= pos < WheelSize
    //
    void SetPosition(int pos);


    //! Set wheel to specific position, by name.
    //
    void SetPosByName(const string& name);


    //! Rotate wheel by specified number of positions.

    int Rotate(int num);


    //! Get current position.
    //
    int GetPosition(void);


    //! Get name of current position.
    //
    string &GetPosName(void);


    //! Get name of offset position (letter next to pin which will be read)
    //
    string &GetOffsetName(void);


    //! Return wheel size (number of pins).
    //
    int GetWheelSize(void);


    //! Read pin at current indicated position.
    //
    bool ReadPin(void);


    //! Read pin at offset from indicated position.
    //
    bool ReadPinOffset(void);
    

    //! Set pin at current indicated position.
    //
    void SetPin(bool active);


    //! Set all pins to inactive state.
    //
    void ClearAllPins(void);


    //! Randomize pin settings.
    //
    //! Randomize such that 40%-60% are active and no
    //! more than six consecutive pins have the same setting.
    //
    void Randomize(void);


    //! Return number of active pins.
    //
    int GetWeight(void);
	    
};


#endif // _KEYWHEEL_H_

// Local Variables: ***
// mode:c++ ***
// End: ***
