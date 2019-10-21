//
/// \file C52GenKey.cpp
/// \package hagelin
//
/// \author Joseph Dunn on 10/1/19.
/// \copyright © 2019 Joseph Dunn.

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
using std::cerr;
using std::endl;
#include <iomanip>
using std::setw;
using std::setfill;
using std::dec;
#include <array>
using std::array;
#include <algorithm>
using std::sort;
using std::shuffle;
#include <random>
using std::bernoulli_distribution;
#include <numeric>
using std::accumulate;
using std::iota;
#include "config.h"
#include "C52.hpp"


//! Assumes no more than two lugs are active. Sorts based
//! on the text representation of the lug settings.
//
bool C52::CompareBars(bitset <NUM_WHEELS> a, bitset <NUM_WHEELS> b) {
  unsigned long ai = a.to_ulong();
  unsigned long bi = b.to_ulong();
  
  return (ai < bi);
}

struct Combo {
  int i1;
  int i2;
  array<int, NUM_WHEELS> NumArrayIn;
  int used;
};

vector<array<int,NUM_WHEELS> > C52::NumArrayA;
vector<array<int,NUM_WHEELS> > C52::NumArrayB;

/// Validate that a proposed drum satisfies the sum requirement
bool C52::ValidateDrum(DrumType drum) {
  bitset<NUM_LUG_BARS-NUM_WHEELS+2> Sums(0);
  for (size_t i=0; i< pow(2,NUM_WHEELS); ++i) {
    bitset<NUM_WHEELS> pins(i);
    int sum = 0;
    for (auto itr = drum.begin()+NUM_WHEELS-1; itr < drum.end(); ++itr)
      sum += ((*itr) & pins).any();
    Sums[sum] = 1;
  }
  return Sums.all();
}

/// return a list of all the lugbars that are consistent with the NumArray
/// and that satisfy the sum test
vector<ScoredDrum>
C52::GoodDrums(array<int, NUM_WHEELS> NumArray, int& tries) {
  tries = 0;
  vector<ScoredDrum> ret;
  // Goal is to determine all possible overlaps
  // limits the number of overlaps between any two positions to 4
  const size_t N_CIPHER_LUGS = NUM_LUG_BARS - NUM_WHEELS+1;
  int overlaps = accumulate(NumArray.begin(), NumArray.end(), 0)-N_CIPHER_LUGS;
  vector<Combo> combos;
  for (int i=0; i<NUM_WHEELS; ++i) {
    for (int j=i+1; j<NUM_WHEELS; ++j) {
      Combo combo;
      combo.i1 = i;
      combo.i2 = j;
      combo.used = 0;
      if (i==0 && j==1)
        combo.NumArrayIn = NumArray;
      combos.push_back(combo);
      
    }
  }
  int k=0;       //the index in combos
  while (k>=0) {
    Combo& c = combos.at(k);
    if (Verbose) {
      cerr << "level = " << k << ", i1 = " << c.i1 << ", i2 = " << c.i2;
      cerr << ", used = " << c.used << ", overlaps = " << overlaps;
      cerr << ", NumArrayIn = ";
      for (int i = 0; i<NUM_WHEELS; ++i)
        cerr << setw(3) << c.NumArrayIn[i];
      cerr << endl;
    }
    if (c.used > 4 ||
        c.used > c.NumArrayIn[c.i1] ||
        c.used > c.NumArrayIn[c.i2]) {
      if (k==0)
        break;
      else{
        overlaps += c.used;
        c.used = 0;
        k--;
        combos.at(k).used++;
        overlaps--;
        continue;
      }
    }
    if (k==combos.size()-1) {
      if (overlaps <= c.NumArrayIn[c.i1]
          && overlaps <= c.NumArrayIn[c.i2]
          && overlaps <= 4) {
        c.used = overlaps;
        overlaps=0;
      } else {
        // go back up
        overlaps += c.used;
        c.used = 0;
        k--;
        combos.at(k).used++;
        overlaps--;
        continue;

      }
    }
    if (overlaps == 0) {
      array<int, NUM_WHEELS> num = NumArray;
      ScoredDrum scored_drum;
      int j=0;
      // Determistic stepping lug bars.  Agrees with Dirk's example.
      for (j=0; j<NUM_WHEELS-1; ++j) {
        for (size_t i=0; i<NUM_WHEELS; ++i)
          scored_drum.drum.at(j)[i] = (i<=j);
        if (Verbose) {
          cerr << j << " " << scored_drum.drum.at(j) << endl;
        }
      }
      // put the overlaps on the drum
      if (Verbose) {
        cerr << "Overlaps: " << endl;
      }
      scored_drum.score = 0;
      for (int k1=0; k1<=k; ++k1) {
        scored_drum.score += combos.at(k1).used > 0;
        for (int i=0; i<combos.at(k1).used; ++i) {
          bitset<NUM_WHEELS> lugs(0);
          lugs[combos.at(k1).i1]=1;
          lugs[combos.at(k1).i2]=1;
          if (Verbose) {
            cerr << j << " " << lugs << endl;
          }
          scored_drum.drum.at(j++) = lugs;
          num.at(combos.at(k1).i1)--;
          num.at(combos.at(k1).i2)--;
        }
      }
      // put the singletons on the drum
      if (Verbose) {
        cerr << endl << "Singletons :" << endl;
      }
      for (int l=0; l<NUM_WHEELS; ++l) {
        for (int i=0; i<num.at(l); ++i) {
          bitset<NUM_WHEELS> lugs(0);
          lugs[l]=1;
          if (Verbose) {
            cerr << j << " " << lugs << endl;
          }
          scored_drum.drum.at(j++) = lugs;
        }
      }
      tries++;
      for (int i=0; i<NUM_WHEELS; ++i) {
        int sum =0;
        for (int j=NUM_WHEELS-1; j<NUM_LUG_BARS; ++j) {
          sum += scored_drum.drum.at(j)[i];
        }
        if (sum != NumArray[i])
          throw std::runtime_error("Drum sums don't match NumArray");

      }
      
      if (ValidateDrum(scored_drum.drum))
        ret.push_back(scored_drum);
      // Go back up
      overlaps += c.used;
      c.used = 0;
      k--;
      if (k<0)
        break;
      combos.at(k).used++;
      overlaps--;
      continue;

    } // overlaps == 0
    // Go down
    k++;
    combos.at(k).used = 0;
    // because we're starting the level with used == 0 there's no need to
    // update overlaps
    combos.at(k).NumArrayIn = c.NumArrayIn;
    combos.at(k).NumArrayIn[c.i1]-= c.used;
    combos.at(k).NumArrayIn[c.i2]-= c.used;
    continue;
    
  }
  
  return ret;
}

/// Generate a key using the method descibed in the Appendices of the
/// 1944 Technical Manual
void C52::GenKey(bool CX52) {
  /*
   Per Technical Manual Appendix I 1b
   Oreoare as set if 156 lettered cards, 78 of which are marked R (right)
   and the remainder L Left).  shuffle the cards thoroughtly and draw ones
   at a time.  Start with A on wheel number 1, and prepare the key list in
   accordance with the cards drawn: if a card bears an L, cross out the
   letter; if a card bears an R, do not cross out the letter.  Only letters
   with effective pins are then shown in the key list (table I, page 13).
   More than six consecutive effective or noneffective pins on any wheel must
   be rearranged to prevent us of such a sequence. A random arranggement,
   in which from 40 to 60 per cent of the pins are in the errectie positon
   is assured by this method.
*/
  if (NumArrayA.size()==0) {
    GenNumArrays();
  }
  array<size_t, 12> wheel_idx;
  if (CX52) {
    for (int i=0; i<NUM_WHEELS; ++i)
      wheel_idx.at(i) = 11;  // the whell with 47 positions
  } else {
    /* Unlike the M209 we have to select the wheels for a C52 from
     a list of 12 possibilities.
     */
    iota(wheel_idx.begin(), wheel_idx.end(), 0);
    shuffle(wheel_idx.begin(), wheel_idx.end(), gen);
  }
  for (int i=0; i<NUM_WHEELS; ++i){
    Wheels.at(i).Clear();
    for (auto a : wheel_labels.at(wheel_idx.at(i))) {
      Wheels.at(i).AddPosition(a);
    }
    Wheels.at(i).SetReadOffset(offsets.at(wheel_idx.at(i)));
    Wheels.at(i).Randomize();
  }
  /*
   Per Technical Manual Appendix I 2a
   Seclect a set of six numbers from either group A or Group B in Appendix II.
   Sets of numbers selected from group B must no exceed 10 percent of the
   initial sets selected.  the sets are selected at random from the table, and
   a set is not used a second time as long as other unused sets are available.
   Sets of numbers from group B should be used at irregular intervals and should
   not succeed each other in a key list.
   */
  
  // Find a NumArray that has at least one good drum
  vector<ScoredDrum> good_drums;
  int tries;
  do {
    bernoulli_distribution dist_A(.9);
    bool A = dist_A(gen);
    vector<array<int, NUM_WHEELS> >& NumArrays = A ? NumArrayA : NumArrayB;
    ui_dist dist(0, static_cast<int>(NumArrays.size()-1));
    array<int, NUM_WHEELS> NumArray = NumArrays[dist(gen)];
    shuffle(NumArray.begin(), NumArray.end(), gen);
    good_drums = GoodDrums(NumArray, tries);
  } while (good_drums.size() == 0);
  
  // Sort the good drums and randomly pick one from the best candidates.
  sort(good_drums.begin(), good_drums.end());
  int best_score = good_drums.back().score;
  int num_best_score=0;
  for (auto itr = good_drums.begin(); itr<good_drums.end(); ++itr){
    num_best_score += ((*itr).score == best_score);
  }
  int num_candidates = std::min(static_cast<int>(good_drums.size()),
                                std::max(25,num_best_score));
  
  // Randomly select from the list of candidates
  // The good ones are at the end
  ui_dist dist_drum(0,num_candidates-1);
  Drum = (*(good_drums.rbegin()+dist_drum(gen))).drum;
  
  // Sort the bars to make it easier for the operator to set them
  // in a real machine
  sort(Drum.begin()+NUM_WHEELS-1, Drum.end(), CompareBars);
  

}

/// Generate the Number Lists in Appendix II of the 1944 Technical Manual
void C52::GenNumArrays() {
  
  // Develop a table of numbers like Appendix II in the manual
  // Six numbers between one and thirteen.
  // One is always used
  // The sum of the numbers is between 28 and 39
  // At most one repeat
  
  const size_t N_CIPHER_BARS = NUM_LUG_BARS-NUM_WHEELS+1;
  const double N = NUM_WHEELS;
  
  NumArrayA.clear();
  NumArrayB.clear();
  
  for (int sum=N_CIPHER_BARS+1; sum <= N_CIPHER_BARS+12 ; sum++){
    array<int, NUM_WHEELS> NumArray;
    NumArray[0] = 1;            // 1 is always included
    int partial_sum = 1;
    int repeats = 0;
    int j = 1;
    int lower_bound = sum - partial_sum;
    lower_bound = 1;
    NumArray[1]=lower_bound;
    if (Verbose) {
      cerr << "Starting GenNUMArrays" << endl;
      cerr << "NumArray[0] = " << NumArray[0] << endl;
      cerr << "NumArray[" << j << "] = " << NumArray[j] << endl;
    }
    while (j>0) {
      // partial sum should be the sum of NumArray prior to j
      // repeats is the number of repeats prior to j
      repeats = 0;
      for (int k= 1; k<=j; ++k) {
        repeats += NumArray[k]==NumArray[k-1];
      }
      int upper_bound;
      if (repeats > 0)
        upper_bound = static_cast<int>((sum - partial_sum -((N-j)*(N-1-j))/2)/(N-j));
      else
        upper_bound = static_cast<int>((sum - partial_sum -((N-1-j)*(N-2-j))/2)/(N-j));
      upper_bound = std::min(13, upper_bound);
      if (Verbose) {
        cerr << "NumArray[" << j << "] = " << NumArray[j];
        cerr << ", partial sum = " << partial_sum;
        cerr << ", repeats = " << repeats;
        cerr << ", upper_bound = " << upper_bound << endl;
      }
      if (NumArray[j]>upper_bound) {
        j=j-1;
        partial_sum -= NumArray[j];
        NumArray[j]++;
        continue;
      }
      if (j == 4) {
        NumArray[j+1] = sum-partial_sum-NumArray[j];
        
        if (NumArray[j] == NumArray[j+1])
          repeats += 1;
        if (repeats) {
          NumArrayB.push_back(NumArray);
          if (Verbose) {
            cerr << "output to AppendixIIB : " ;
            for (auto a : NumArray)
              cerr << a << " ";
            cerr << endl;
          }
        } else {
          NumArrayA.push_back(NumArray);
          if (Verbose) {
            cerr << "output to NumArrayA : ";
            for (auto a : NumArray)
              cerr << a << " ";
            cerr << endl;
          }
        }
        NumArray[j]++;
        continue;
      } else {
        partial_sum += NumArray[j];
        j = j+1;
        int lower_bound = sum - partial_sum;
        lower_bound -= (13*14)/2-((13-5+j)*(13-5+j+1))/2;
        lower_bound = std::max(NumArray[j-1]+repeats, lower_bound);
        NumArray[j] = lower_bound;
      }
    } // while j
  } // sum
  if (Verbose || !Quiet) {
    cerr << "NumArrayA (size " << NumArrayA.size() << ") : " << endl;
    if (Verbose){
      for (auto a : NumArrayA) {
        int s = 0;
        for (auto b : a) {
          cerr << setw(3) << b;
          s += b;
        }
        cerr << " | "<< setw(3) << (s-NUM_LUG_BARS)<< endl;
      }
    }
    cerr << endl << "NumArrayB (size " << NumArrayB.size() << ") : " << endl;
    if (Verbose){
      for (auto a : NumArrayB) {
        int s = 0;
        for (auto b : a) {
          cerr << setw(3) << b;
          s += b;
        }
        cerr << " | " << setw(3) << s- N_CIPHER_BARS << endl;
      }
    }
  }
}




