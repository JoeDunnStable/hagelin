//
/// \file C52Keywheel.cpp
/// \package hagelin
//
/// \author Joseph Dunn on 10/19/19.
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
#include <random>
using std::binomial_distribution;
#include <algorithm>
using std::shuffle;

#include "config.h"
#include "C52Keywheel.hpp"

inline int mod(int a,int b) {
  int c = a % b;
  if (c<0) c += b;
  return c;
}

void C52Keywheel::Randomize() {
  
  int n=GetWheelSize();
  
  binomial_distribution<int> dist(n, .5);
  int n_active;
  do {
    n_active = dist(gen);
  } while (n_active > .6 * n || n_active < .4 * n);
  
  vector<bool> pins(n,false);
  for (size_t i=0; i<n_active; ++i)
    pins.at(i)=true;
  shuffle(pins.begin(),pins.end(),gen);
  
  // Determine point where pins first changes
  int n0;
  for (n0=1; pins.at(n0-1)==pins.at(n0); ++n0);
  
  // Find rund lengths
  vector<int> run_length(n);
  for (int i=n0; i<n0+n; ++i) {
    if (pins.at(mod(i-1,n)) == pins.at(mod(i,n)))
      run_length.at(mod(i,n)) = run_length.at(mod(i-1,n)) + (pins.at(mod(i,n)) ? 1 : -1);
    else if (pins.at(mod(i-1,n)) != pins.at(mod(i,n)))
      run_length.at(mod(i,n)) = pins.at(mod(i,n)) ? 1 : -1;
  }
  
  do {
    // find positon of maximum run of active and inactiv pins
    int imax=0, imin=0, rmax=0, rmin=0;
    for (int i=0; i<n; ++i) {
      if (run_length.at(i) > rmax) {
        rmax = run_length.at(i);
        imax = i;
      } else if (run_length.at(i) < rmin) {
        rmin = run_length.at(i);
        imin = i;
      }
    }
    if (Verbose) {
      cerr << "imax = " << imax << ", rmax = " << rmax
           << ", imin = " << imin << ", rmin = " << rmin << endl;
    }
    if (rmax <= 3 && rmin >= -3) break;
    int jmax = mod(imax-rmax/2, n);
    int jmin = mod(imin+rmin/2, n);
    swap(pins.at(jmax), pins.at(jmin));
  
    // recaluclate run lengths for positions affected by swap
    for (int i=jmax; i<=jmax+rmax/2; ++i) {
      if (pins.at(mod(i-1,n)) == pins.at(mod(i,n)))
        run_length.at(mod(i,n)) = run_length.at(mod(i-1,n)) + (pins.at(mod(i,n)) ? 1 : -1);
      else if (pins.at(mod(i-1,n)) != pins.at(mod(i,n)))
        run_length.at(mod(i,n)) = pins.at(mod(i,n)) ? 1 : -1;
    }
    for (int i=jmin; i<=jmin-rmin/2; ++i) {
      if (pins.at(mod(i-1,n)) == pins.at(mod(i,n)))
        run_length.at(mod(i,n)) = run_length.at(mod(i-1,n)) + (pins.at(mod(i,n)) ? 1 : -1);
      else if (pins.at(mod(i-1,n)) != pins.at(mod(i,n)))
        run_length.at(mod(i,n)) = pins.at(mod(i,n)) ? 1 : -1;
    }
  } while(true);
  
  for (int i=0; i<n; ++i) {
    SetPosition(i);
    SetPin(pins.at(i));
  }
  
  SetPosition(0);
  
}
