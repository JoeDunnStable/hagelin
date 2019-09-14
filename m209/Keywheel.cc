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
 * \file Keywheel.h
 * \brief Implementation of Keywheel class member functions.
 * \package hagelin
 */

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#include <iomanip>
using std::setw;
using std::setfill;
using std::dec;



#include "config.h"
#include "Keywheel.h"

Keywheel::Keywheel() {
  WheelSize = Position = ReadOffset = 0;
}


void Keywheel::AddPosition(const string& name) {
  if (PosByName.count(name)) {
    throw std::invalid_argument(
                                "Keywheel::AddPosition(): duplicate name");
  }
  PosNames.push_back(name);
  PosByName[name] = WheelSize;
  WheelSize++;
  Pins.resize(WheelSize);
}


void Keywheel::SetReadOffset(int offset) {
  ReadOffset = offset;
}


void Keywheel::SetPosition(int pos) {
  if ((pos < 0) || (pos >= WheelSize)) {
    throw std::invalid_argument(
                                "Keywheel::SetPosition(): position out of bounds");
  }
  Position = pos;
}


void Keywheel::SetPosByName(const string& name) {
  if (PosByName.count(name) != 1) {
    throw std::invalid_argument(
                                "Keywheel::SetPosByName(): name not found");
  }
  
  Position = PosByName[name];
}


int Keywheel::Rotate(int num) {
  Position += num;
  for ( ; Position < 0; Position += WheelSize);
  Position %= WheelSize;
  return Position;
}


int Keywheel::GetPosition(void) {
  return Position;
}


string &Keywheel::GetPosName(void) {
  return PosNames[Position];
}


string &Keywheel::GetOffsetName(void) {
  int  p;
  for (p = Position + ReadOffset; p<0; p += WheelSize);
  return PosNames[p % WheelSize];
}


int Keywheel::GetWheelSize(void) {
  return WheelSize;
}


bool Keywheel::ReadPin(void) {
  return Pins[Position];
}


bool Keywheel::ReadPinOffset(void) {
  int  p;
  for (p = Position + ReadOffset; p<0; p += WheelSize);
  return Pins[p % WheelSize];
}


void Keywheel::SetPin(bool active) {
  Pins[Position] = active;
}


void Keywheel::ClearAllPins(void) {
  for (int i=0; i<WheelSize; Pins[i++]=false);
}


int Keywheel::GetWeight(void) {
  int    i, w;
  
  for (i=0, w=0; i < WheelSize; i++) {
    if (Pins[i]) {
      ++w;
    }
  }
  
  return w;
}


void Keywheel::Randomize(void) {
  int    i;
  double  ratio;      // ratio of active/inactive pins
  int    tries;      // number of attempts to randomize
  int    c, lastpin;    // used in consecutive pins check
  int    max_c;      // maximum consecutive pins
  int    pin;
  
  tries = 0;
  
  do {
    // Avoid infinite loop
    if (tries++ >= 100) {
      cerr << "ERROR: Cannot randomize pins! Is random() broken?" << endl;
      exit(1);
    }
    
    // Randomize the pins, counting run lengths
    for (i=0, max_c=1, c=1, lastpin=2; i<WheelSize; i++) {
      ui_dist u_0_1(0,1);
      pin = u_0_1(gen);
      Pins[i] = pin;
      
      if (pin == lastpin) {
        ++c;
      } else {
        if (c > max_c) {
          max_c = c;
        }
        c = 1;
      }
      lastpin = pin;
    }
    
    if (c > max_c) {
      max_c = c;
    }
    
    // If first and last bits equal, look for long overlapping run
    if (Pins[0] == Pins[WheelSize-1]) {
      
      lastpin = Pins[0];
      
      // Count right half of run
      for (c=0; Pins[c] == lastpin; c++);
      
      // Count left half of run
      for (i=WheelSize-1; Pins[i] == lastpin; i--, c++);
      
      if (c > max_c) {
        max_c = c;
      }
      
    }
    
    // Calculate percentage of active pins
    ratio = (double)GetWeight() / (double)WheelSize;
    
    // Print debug information
    if (Verbose) {
      cerr << "Wheel " << dec << setw(2) << setfill(' ') << WheelSize
      << " try " << dec << setw(2) << setfill(' ') << tries
      << " ratio " << dec << setw(2) << (int)(ratio*100.0) << "%"
      << " max run length " << dec << setfill(' ') << max_c;
      
      cerr << " Pins ";
      for (i=0; i<WheelSize; i++) {
        cerr << Pins[i];
      }
      
      cerr << endl;
    }
    
  } while ((ratio < 0.4) || (ratio > 0.6) || (max_c > 6));
  
  
}


