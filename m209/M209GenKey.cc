/**************************************************************************
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
 * \file
 * \brief M209 class GenKey() function
 */


#include "hagelin.h"

//! Random number function for use with shuffle algorithm.
//
static ptrdiff_t doRandom(ptrdiff_t i) {
  ui_dist u_0_im1(0,static_cast<int>(i-1));
  return u_0_im1(gen);
}


//! Compare two lug bars on the drum, for sorting.
//
//! Assumes no more than two lugs are active. Sorts based
//! on the text representation of the lug settings.
//
static bool CompareBars(bitset <NUM_WHEELS> a, bitset <NUM_WHEELS> b) {
  int    i, ai, bi;
  char  a1, a2, b1, b2;
  
  
  for (i=0; i<NUM_WHEELS && !a[i]; i++);
  a1 = a[i] ? '1'+i : '0';
  
  for (++i; i<NUM_WHEELS && !a[i]; i++);
  a2 = a[i] ? '1'+i : '0';
  
  if (a1 > a2) {
    swap(a1, a2);
  }
  
  for (i=0; i<NUM_WHEELS && !b[i]; i++);
  b1 = b[i] ? '1'+i : '0';
  
  for (++i; i<NUM_WHEELS && !b[i]; i++);
  b2 = b[i] ? '1'+i : '0';
  
  if (b1 > b2) {
    swap(b1, b2);
  }
  
  ai = ((int)a1 << 8) | a2;
  bi = ((int)b1 << 8) | b2;
  
  return (ai < bi);
}


void M209::GenKey() {
  int          i, j, k, x, n; // loop counters
  vector<int>        NumList;
  int          sum, overlaps, repeats;
  int          pain;
  vector<vector <bool> >    LugTable;
  bool        DidRotation, DidScramble;
  bitset<NUM_LUG_BARS+1>    Sums;
  bool        Success;
  
  // Set up lug table
  LugTable.resize(NUM_WHEELS);
  for (i=0; i<NUM_WHEELS; i++) {
    LugTable[i].resize(NUM_LUG_BARS);
  }
  
  
  // For each wheel, randomly set pin positions, repeating
  // until 40%-60% of pins are active and no more than 6 consecutive
  // pins have the same setting.
  for (i=0; i<NUM_WHEELS; i++) {
    Wheels[i].Randomize();
  }
  
  // Now, generate lug settings. This part is a bit harder.
  // Procedure is derived from a combination of instructions in
  // an online CSP-1500 manual and an original 1944 M209 manual.
  
  NumList.resize(NUM_WHEELS);
  pain = 0;
  Success = false;
  
  while (!Success) {
    // Avoid infinite loop
    if (pain++ >= GUMPTION) {
      cerr << "ERROR: Gave up trying to generate lug settings." << endl;
      exit(1);
    }
    
    // First, pick six numbers 1-13 such that:
    // * Three are odd
    // * Three are even
    // * Sum is between 28 and 39 (inclusive)
    // * Allow one repeated number
    // * 1 is always in the set
    
    NumList[0] = sum = 1;
    repeats = 0;
    
    // Two odd numbers (we already inserted one in the list)
    for (i=1; i<3; i++) {
      ui_dist u_0_6(0,6);
      NumList[i] = (u_0_6(gen) * 2) + 1;
      sum += NumList[i];
    }
    
    // Three even numbers
    for ( ; i<NUM_WHEELS; i++) {
      ui_dist u_0_nwm1(1,NUM_WHEELS);
      NumList[i] = (u_0_nwm1(gen) * 2);
      sum += NumList[i];
    }
    
    // How many overlaps?
    overlaps = max(sum - NUM_LUG_BARS, 0);
    
    // How many repeats?
    for (i=0; i<5; i++) {
      for (j=i+1; j<NUM_WHEELS; j++) {
        if (NumList[i] == NumList[j]) {
          ++repeats;
        }
      }
    }
    
    // Sort the list
    sort(NumList.begin(), NumList.end());
    
    if (Verbose) {
      cerr << "Lug number list "
      << dec << setw(3) << setfill(' ') << pain << ':';
      for (i=0; i<NUM_WHEELS; i++) {
        cerr << ' ' << dec << setw(2) << setfill(' ') << NumList[i];
      }
      cerr << " sum " << setw(2) << sum;
      cerr << " overlaps " << setw(2) << overlaps;
      cerr << " repeats " << setw(1) << repeats << endl;
    }
    
    // Start over if sum isn't in range
    if ((sum < 28) || (sum > 39)) {
      continue;
    }
    
    // Allow no more than one repeat
    if (repeats > 1) {
      continue;
    }
    
    // Now load up the numbers into a table which allows
    // easy rotating and shuffling of either rows or columns
    for (i=0; i<NUM_WHEELS; i++) {
      for (j=0; j<NUM_LUG_BARS; j++) {
        LugTable[i][j] = (j < NumList[i]) ? true : false;
      }
    }
    
    // Rotate columns to get a preliminary overlap distribution
    for (i=1, j=0, k=overlaps; i<NUM_WHEELS; i++) {
      
      // Rotation amount
      j += NumList[i-1];
      
      // overlap adjacent columns by one if overlaps remain,
      // but don't overlap first and second columns if second
      // column contains 1 or 2.
      if ((k>0) && !((i==1) && (NumList[i] < 3))) {
        j--;
        k--;
      }
      
      // Keep rotation amount in bounds
      // Also, don't rotate ones past the end of the column.
      j = j % NUM_LUG_BARS;
      j = min(j, (NUM_LUG_BARS - NumList[i]));
      
      rotate(LugTable[i].begin(),
             LugTable[i].end() - j,
             LugTable[i].end());
    }
    
    // Print preliminary rotation results
    if (Verbose) {
      cerr << "First Rotation " << pain << ":" << endl;
      for (j=0; j<NUM_LUG_BARS; j++) {
        cerr << "  ";
        for (i=0; i<NUM_WHEELS; i++) {
          cerr << LugTable[i][j];
        }
        cerr << endl;
      }
    }
    
    // Look for overlaps of more than 4 between columns, and
    // rows with more than two lugs.
    do {
      // Avoid infinite loop
      ++pain;
      if (pain >= GUMPTION) {
        cerr << "ERROR: Gave up trying to generate lug settings."
        << endl;
        exit(1);
      }
      
      // Now look for any overlaps of more than four between
      // consecutive columns.
      DidRotation = false;
      for (i=1; i<NUM_WHEELS; i++) {
        for (j=0, k=0; j<NUM_LUG_BARS; j++) {
          if (LugTable[i][j] && (LugTable[i][j] == LugTable[i-1][j])) {
            ++k;
          }
        }
        if (k > 4) {
          k = (k-4) % NUM_LUG_BARS;
          DidRotation = true;
          rotate(LugTable[i].begin(),
                 LugTable[i].end() - k,
                 LugTable[i].end());
        }
      }
      
      if (Verbose && DidRotation) {
        cerr << "Overlap Fix Rotation " << pain << ":" << endl;
        for (j=0; j<NUM_LUG_BARS; j++) {
          cerr << "  ";
          for (i=0; i<NUM_WHEELS; i++) {
            cerr << LugTable[i][j];
          }
          cerr << endl;
        }
      }
      
      // While eliminating excessive overlaps between consecutive columns,
      // we might have created rows with more than two active lugs.
      // Look for them and scramble lugs to try to correct this. Keep
      // trying until we succeed, or pain exceeds gumption.
      
      DidScramble = false;
      for (j=0; j<NUM_LUG_BARS; j++){
        for (i=0, k=0; i<NUM_WHEELS; i++) {
          if (LugTable[i][j]) {
            ++k;
            if (k>2) {
              break;
            }
          }
        }
        if (k>2) {
          // We found a row with more than one active lugs.
          // j points to the row, and i points to the third
          // active column since we broke out of the i loop.
          // Let's recycle k to locate a 0 in column
          // i, and then swap that 0 with the 1 that we just found.
          DidScramble = true;
          
          for (k=0; k<NUM_LUG_BARS
               && LugTable[i][(j+k)%NUM_LUG_BARS]; k++);
          iter_swap(LugTable[i].begin()+j,
                    LugTable[i].begin()+((j+k)%NUM_LUG_BARS));
        }
      }
      
      // Print scramble results
      if (Verbose && DidScramble) {
        cerr << "Excess Lug Scramble " << pain << ":" << endl;
        for (j=0; j<NUM_LUG_BARS; j++) {
          cerr << "  ";
          for (i=0; i<NUM_WHEELS; i++) {
            cerr << LugTable[i][j];
          }
          cerr << endl;
        }
      }
      
    } while (DidRotation || DidScramble);
    
    // Now, check that all numbers from 1 to 27 can be
    // generated with these lug settings. If not, start over.
    // We use an array of flags (Sums[]) to mark each number
    // that can be generated. Then, we look for holes.
    
    // First, clear flags.
    for (i=1; i<=NUM_LUG_BARS; Sums[i++] = false);
    
    // Next, set flags for all single columns.
    for (i=0; i<NUM_WHEELS; i++) {
      Sums[NumList[i]] = true;
    }
    
    // Next, try pairs of columns.
    for (i=0; i<(NUM_WHEELS-1); i++) {
      for (j=i+1; j<NUM_WHEELS; j++) {
        for (n=0, sum=0; n<NUM_LUG_BARS; n++) {
          if (LugTable[i][n] || LugTable[j][n]) {
            ++sum;
          }
          Sums[sum] = true;
        }
      }
    }
    
    // Next, try sets of three columns.
    for (i=0; i<(NUM_WHEELS-2); i++) {
      for (j=i+1; j<(NUM_WHEELS-1); j++) {
        for (k=j+1; k<NUM_WHEELS; k++) {
          for (n=0, sum=0; n<NUM_LUG_BARS; n++) {
            if (LugTable[i][n] || LugTable[j][n]
                || LugTable[k][n]) {
              ++sum;
            }
            Sums[sum] = true;
          }
        }
      }
    }
    
    
    // Next, try sets of four columns. Is there a smarter way to do this?
    for (i=0; i<(NUM_WHEELS-3); i++) {
      for (j=i+1; j<(NUM_WHEELS-2); j++) {
        for (k=j+1; k<(NUM_WHEELS-1); k++) {
          for (x=k+1; x<NUM_WHEELS; x++) {
            for (n=0, sum=0; n<NUM_LUG_BARS; n++) {
              if (LugTable[i][n] || LugTable[j][n]
                  || LugTable[k][n] || LugTable[x][n]) {
                ++sum;
              }
              Sums[sum] = true;
            }
          }
        }
      }
    }
    
    
    // Next, try sets of five columns.
    for (i=0; i<NUM_WHEELS; i++) {
      for (n=0, sum=0; n<NUM_LUG_BARS; n++) {
        for (j=0, k=0; j<NUM_WHEELS; j++) {
          if ((j!=i) && LugTable[j][n]) {
            k=1;
          }
        }
        if (k) {
          ++sum;
        }
      }
      Sums[sum] = true;
    }
    
    
    // (yawn) Now check sum of all six columns.
    // We could also check for any rows with no lugs instead.
    for (n=0, sum=0; n<NUM_LUG_BARS; n++) {
      for (i=0, j=0; i<NUM_WHEELS; i++) {
        if (LugTable[i][n]) {
          j=1;
        }
      }
      if (j) {
        ++sum;
      }
    }
    Sums[sum] = true;
    
    
    if (Verbose) {
      cerr << "Sums " << pain << ": ";
      for (i=1; i<=NUM_LUG_BARS; i++) {
        cerr << Sums[i];
      }
      cerr << endl;
    }
    
    // If we can't make all 27 numbers, start over.
    for (i=1, n=0; i<=NUM_LUG_BARS; i++) {
      if (!Sums[i]) {
        n=1;
      }
    }
    if (n) {
      continue;
    }
    
    Success = true;
  };
  
  
  // We've selected a preliminary set of lug settings that meets criteria.
  // Now shuffle the columns for more variety.
  random_shuffle(LugTable.begin(), LugTable.end(), doRandom);
  
  if (Verbose) {
    cerr << "Lug Shuffle:" << endl;
    for (j=0; j<NUM_LUG_BARS; j++) {
      cerr << "  ";
      for (i=0; i<NUM_WHEELS; i++) {
        cerr << LugTable[i][j];
      }
      cerr << endl;
    }
  }
  
  // Load up the array of lug settings
  for (i=0; i<NUM_LUG_BARS; i++) {
    for (j=0; j<NUM_WHEELS; j++) {
      Drum[i][j] = LugTable[j][i];
    }
  }
  
  
  // Sort the bars to make it easier for the operator to set them
  // in a real machine
  sort(Drum.begin(), Drum.end(), CompareBars);
  
  if (Verbose) {
    cerr << "Bar Sort:" << endl;
    for (i=0; i<NUM_LUG_BARS; i++) {
      cerr << "  ";
      for (j=0; j<NUM_WHEELS; j++) {
        cerr << Drum[i][j];
      }
      cerr << endl;
    }
  }
}


