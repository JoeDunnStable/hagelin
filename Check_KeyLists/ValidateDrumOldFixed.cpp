//
//  ValidateDrumOldFixed.cpp
//  Check_KeyLists
//
//  Created by Joseph Dunn on 9/25/19.
//  Copyright © 2019 Joseph Dunn. All rights reserved.
//

#include "ValidateDrumOldFixed.hpp"

// Now, check that all numbers from 1 to 27 can be
// generated with these lug settings. If not, start over.
// We use an array of flags (Sums[]) to mark each number
// that can be generated. Then, we look for holes.

bool ValidateDrumOldFixed(M209::DrumType drum) {
  int i, j, k, x, n, sum;
  array<int,NUM_WHEELS> NumList;
  for (i=0; i<NUM_WHEELS; ++i) {
    sum =0;
    for (j=0; j<NUM_LUG_BARS; ++j)
      sum += drum.at(j)[i];
    NumList.at(i) = sum;
  }
  vector<vector <bool> >    LugTable;
  LugTable.resize(NUM_WHEELS, vector<bool>(NUM_LUG_BARS, false));
  for (i=0; i<NUM_WHEELS; ++i)
    for (j=0; j<NUM_LUG_BARS; ++j)
      LugTable.at(i).at(j) = drum.at(j)[i];
  
  bitset<NUM_LUG_BARS+1>    Sums;
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
      }
      Sums[sum] = true;
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
        }
        Sums[sum] = true;
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
          }
          Sums[sum] = true;
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
    cerr << "Sums " << ": ";
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
  return !n;
}
