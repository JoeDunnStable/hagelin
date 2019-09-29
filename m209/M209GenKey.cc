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
 * \file M209GenKey.cc
 * \brief M209 class GenKey() function
 * \package hagelin
 */

#include <iostream>
using std::cerr;
#include <iomanip>
using std::setw;
using std::setfill;
using std::dec;
#include <array>
using std::array;
#include <algorithm>
using std::sort;
using std::random_shuffle;
#include <numeric>
using std::accumulate;
#include "config.h"
#include "M209.h"

/*
//! Random number function for use with shuffle algorithm.
//
static ptrdiff_t doRandom(ptrdiff_t i) {
  ui_dist u_0_im1(0,static_cast<int>(i-1));
  return u_0_im1(gen);
}
*/
/// convert bitset representation to lugs
void M209::bitset2lugs(bitset<NUM_WHEELS> a, char& a1, char& a2) {
	vector<char> lugs;
	for (int i = 0; i < NUM_WHEELS; ++i)
		if (a[i]) lugs.push_back('1' + i);
	if (lugs.size() == 0) {
		a1 = '0';
		a2 = '0';
	}
	else if (lugs.size() == 1) {
		a1 = '0';
		a2 = lugs[0];
	}
	else if (lugs.size() == 2) {
		a1 = lugs[0];
		a2 = lugs[1];
	}
	else {
		throw std::runtime_error("M209::compare: More than two lugs present.");
	}
}

//! Compare two lug bars on the drum, for sorting.
//
//! Assumes no more than two lugs are active. Sorts based
//! on the text representation of the lug settings.
//
bool M209::CompareBars(bitset <NUM_WHEELS> a, bitset <NUM_WHEELS> b) {
  int   ai, bi;
  char  a1, a2, b1, b2;
  
  bitset2lugs(a, a1, a2);
  bitset2lugs(b, b1, b2);
  
  ai = ((int)a1 << 8) | a2;
  bi = ((int)b1 << 8) | b2;
  
  return (ai < bi);
}

struct Combo {
  int i1;
  int i2;
  array<int, NUM_WHEELS> NumArrayIn;
  int used;
};

/// Validate that a proposed drum satisfies the sum requirement
bool M209::ValidateDrum(DrumType drum) {
  bitset<NUM_LUG_BARS+1> Sums(0);
  for (size_t i=0; i< pow(2,NUM_WHEELS); ++i) {
    bitset<NUM_WHEELS> pins(i);
    int sum = 0;
    for (int i=0; i<NUM_LUG_BARS; ++i)
      sum += (drum.at(i) & pins).any();
    Sums[sum] = 1;
  }
  return Sums.all();
}

/// return a list of all the lugbars that are consistent with the NumArray
/// and that satisfy the sum test
vector<ScoredDrum>
M209::GoodDrums(array<int, NUM_WHEELS> NumArray, int& tries) {
  tries = 0;
  vector<ScoredDrum> ret;
  // Goal is to determine all possible overlaps
  // limits the number of overlaps between any two positions to 4
  int overlaps = accumulate(NumArray.begin(), NumArray.end(), 0)-NUM_LUG_BARS;
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
        for (int j=0; j<NUM_LUG_BARS; ++j) {
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
void M209::GenKey1944() {
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
  
  for (int i=0; i<NUM_WHEELS; ++i)
    Wheels[i].Randomize();
  
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
    vector<array<int, 6> >& NumArrays = A ? NumArrayAppendixIIA : NumArrayAppendixIIB;
    uniform_int_distribution<int> dist(0, static_cast<int>(NumArrays.size()-1));
    array<int, 6> NumArray = NumArrays[dist(gen)];
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
  uniform_int_distribution<int> dist_drum(0,num_candidates-1);
  Drum = (*(good_drums.rbegin()+dist_drum(gen))).drum;
  
  // Sort the bars to make it easier for the operator to set them
  // in a real machine
  sort(Drum.begin(), Drum.end(), CompareBars);
  

}

/// Generate the Number Lists in Appendix II of the 1944 Technical Manual
void M209::GenAppendixII(vector<array<int, 6> >& NumArrayA,
                         vector<array<int, 6> >& NumArrayB) {
  
  // Develop a table of numbers like Appendix II in the manual
  // Six numbers between one and thirteen.
  // One is always used
  // The sum of the numbers is between 28 and 39
  // At most one repeat
  
  NumArrayA.clear();
  NumArrayB.clear();
  
  for (int sum=28; sum <= 39 ; sum++){
    array<int, 6> NumArray;
    NumArray[0] = 1;            // 1 is always included
    int partial_sum = 1;
    int repeats = 0;
    int j = 1;
    int lower_bound = sum - partial_sum;
    lower_bound = 1;
    NumArray[1]=lower_bound;
    if (Verbose) {
      cerr << "Starting GenAppendixII" << endl;
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
        upper_bound = static_cast<int>((sum - partial_sum -((6-j)*(5-j))/2)/(6.-j));
      else
        upper_bound = static_cast<int>((sum - partial_sum -((5-j)*(4-j))/2)/(6.-j));
      upper_bound = std::min(13, upper_bound);
      if (j==1) upper_bound=2;
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
            cerr << "output to AppendixIIA : ";
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
        lower_bound = max(NumArray[j-1]+repeats, lower_bound);
        NumArray[j] = lower_bound;
      }
    } // while j
  } // sum
  if (!Quiet) {
    cerr << "Appendix II Group A (size " << NumArrayA.size() << ") : " << endl;
    for (auto a : NumArrayA) {
      int s = 0;
      for (auto b : a) {
        cerr << setw(3) << b;
        s += b;
      }
      cerr << " | "<< setw(3) << (s-27)<< endl;
    }
    cerr << endl << "Appendix II Group B (size " << NumArrayB.size() << ") : " << endl;
    for (auto a : NumArrayB) {
      int s = 0;
      for (auto b : a) {
        cerr << setw(3) << b;
        s += b;
      }
      cerr << " | " << setw(3) << s-27 << endl;
    }
  }
}




