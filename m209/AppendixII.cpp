//
///array<int,6>{\file AppendixII.cpp
///  \package hagelin
//
///  \author Joseph Dunn on 9/19/19.
///  \copyright © 2019 Joseph Dunn.
//

/**************************************************************************
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

#include "M209.h"


/// The NumArrays contained in Appendix II Group A of the 1944 Tecnical
/// Manual. Group A are the arrays without repeats.
vector<array<int,6> > M209::NumArrayAppendixIIA = {
  array<int,6>{1, 2, 3, 4, 5, 13},
  array<int,6>{1, 2, 3, 4, 6, 12},
  array<int,6>{1, 2, 3, 4, 7, 11},
  array<int,6>{1, 2, 3, 4, 8, 10},
  array<int,6>{1, 2, 3, 5, 6, 11},
  array<int,6>{1, 2, 3, 5, 7, 10},
  array<int,6>{1, 2, 3, 5, 8, 9},
  array<int,6>{1, 2, 3, 6, 7, 9},
  array<int,6>{1, 2, 4, 5, 6, 10},
  array<int,6>{1, 2, 4, 5, 7, 9},
  array<int,6>{1, 2, 3, 4, 6, 13},
  array<int,6>{1, 2, 3, 4, 7, 12},
  array<int,6>{1, 2, 3, 4, 8, 11},
  array<int,6>{1, 2, 3, 4, 9, 10},
  array<int,6>{1, 2, 3, 5, 6, 12},
  array<int,6>{1, 2, 3, 5, 7, 11},
  array<int,6>{1, 2, 3, 5, 8, 10},
  array<int,6>{1, 2, 3, 6, 7, 10},
  array<int,6>{1, 2, 3, 6, 8, 9},
  array<int,6>{1, 2, 4, 5, 6, 11},
  array<int,6>{1, 2, 4, 5, 7, 10},
  array<int,6>{1, 2, 4, 5, 8, 9},
  array<int,6>{1, 2, 4, 6, 7, 9},
  array<int,6>{1, 2, 3, 4, 7, 13},
  array<int,6>{1, 2, 3, 4, 8, 12},
  array<int,6>{1, 2, 3, 4, 9, 11},
  array<int,6>{1, 2, 3, 5, 6, 13},
  array<int,6>{1, 2, 3, 5, 7, 12},
  array<int,6>{1, 2, 3, 5, 8, 11},
  array<int,6>{1, 2, 3, 5, 9, 10},
  array<int,6>{1, 2, 3, 6, 7, 11},
  array<int,6>{1, 2, 3, 6, 8, 10},
  array<int,6>{1, 2, 3, 7, 8, 9},
  array<int,6>{1, 2, 4, 5, 6, 12},
  array<int,6>{1, 2, 4, 5, 7, 11},
  array<int,6>{1, 2, 4, 5, 8, 10},
  array<int,6>{1, 2, 4, 6, 7, 10},
  array<int,6>{1, 2, 4, 6, 8, 9},
  array<int,6>{1, 2, 3, 4, 8, 13},
  array<int,6>{1, 2, 3, 4, 9, 12},
  array<int,6>{1, 2, 3, 4, 10, 11},
  array<int,6>{1, 2, 3, 5, 7, 13},
  array<int,6>{1, 2, 3, 5, 8, 12},
  array<int,6>{1, 2, 3, 5, 9, 11},
  array<int,6>{1, 2, 3, 6, 7, 12},
  array<int,6>{1, 2, 3, 6, 8, 11},
  array<int,6>{1, 2, 3, 6, 9, 10},
  array<int,6>{1, 2, 3, 7, 8, 10},
  array<int,6>{1, 2, 4, 5, 6, 13},
  array<int,6>{1, 2, 4, 5, 7, 12},
  array<int,6>{1, 2, 4, 5, 8, 11},
  array<int,6>{1, 2, 4, 5, 9, 10},
  array<int,6>{1, 2, 4, 6, 7, 11},
  array<int,6>{1, 2, 4, 6, 8, 10},
  array<int,6>{1, 2, 4, 7, 8, 9},
  array<int,6>{1, 2, 3, 4, 9, 13},
  array<int,6>{1, 2, 3, 4, 10, 12},
  array<int,6>{1, 2, 3, 5, 8, 13},
  array<int,6>{1, 2, 3, 5, 9, 12},
  array<int,6>{1, 2, 3, 5, 10, 11},
  array<int,6>{1, 2, 3, 6, 7, 13},
  array<int,6>{1, 2, 3, 6, 8, 12},
  array<int,6>{1, 2, 3, 6, 9, 11},
  array<int,6>{1, 2, 3, 7, 8, 11},
  array<int,6>{1, 2, 3, 7, 9, 10},
  array<int,6>{1, 2, 4, 5, 7, 13},
  array<int,6>{1, 2, 4, 5, 8, 12},
  array<int,6>{1, 2, 4, 5, 9, 11},
  array<int,6>{1, 2, 4, 6, 7, 12},
  array<int,6>{1, 2, 4, 6, 8, 11},
  array<int,6>{1, 2, 4, 6, 9, 10},
  array<int,6>{1, 2, 4, 7, 8, 10},
  array<int,6>{1, 2, 3, 4, 10, 13},
  array<int,6>{1, 2, 3, 4, 11, 12},
  array<int,6>{1, 2, 3, 5, 9, 13},
  array<int,6>{1, 2, 3, 5, 10, 12},
  array<int,6>{1, 2, 3, 6, 8, 13},
  array<int,6>{1, 2, 3, 6, 9, 12},
  array<int,6>{1, 2, 3, 6, 10, 11},
  array<int,6>{1, 2, 3, 7, 8, 12},
  array<int,6>{1, 2, 3, 7, 9, 11},
  array<int,6>{1, 2, 4, 5, 8, 13},
  array<int,6>{1, 2, 4, 5, 9, 12},
  array<int,6>{1, 2, 4, 5, 10, 11},
  array<int,6>{1, 2, 4, 6, 7, 13},
  array<int,6>{1, 2, 4, 6, 8, 12},
  array<int,6>{1, 2, 4, 6, 9, 11},
  array<int,6>{1, 2, 4, 7, 8, 11},
  array<int,6>{1, 2, 4, 7, 9, 10},
  array<int,6>{1, 2, 3, 4, 11, 13},
  array<int,6>{1, 2, 3, 5, 10, 13},
  array<int,6>{1, 2, 3, 5, 11, 12},
  array<int,6>{1, 2, 3, 6, 9, 13},
  array<int,6>{1, 2, 3, 6, 10, 12},
  array<int,6>{1, 2, 3, 7, 8, 13},
  array<int,6>{1, 2, 3, 7, 9, 12},
  array<int,6>{1, 2, 3, 7, 10, 11},
  array<int,6>{1, 2, 4, 5, 9, 13},
  array<int,6>{1, 2, 4, 5, 10, 12},
  array<int,6>{1, 2, 4, 6, 8, 13},
  array<int,6>{1, 2, 4, 6, 9, 12},
  array<int,6>{1, 2, 4, 6, 10, 11},
  array<int,6>{1, 2, 4, 7, 8, 12},
  array<int,6>{1, 2, 4, 7, 9, 11},
  array<int,6>{1, 2, 4, 8, 9, 10},
  array<int,6>{1, 2, 3, 5, 11, 13},
  array<int,6>{1, 2, 3, 6, 10, 13},
  array<int,6>{1, 2, 3, 6, 11, 12},
  array<int,6>{1, 2, 3, 7, 9, 13},
  array<int,6>{1, 2, 3, 7, 10, 12},
  array<int,6>{1, 2, 4, 5, 10, 13},
  array<int,6>{1, 2, 4, 5, 11, 12},
  array<int,6>{1, 2, 4, 6, 9, 13},
  array<int,6>{1, 2, 4, 6, 10, 12},
  array<int,6>{1, 2, 4, 7, 8, 13},
  array<int,6>{1, 2, 4, 7, 9, 12},
  array<int,6>{1, 2, 4, 7, 10, 11},
  array<int,6>{1, 2, 4, 8, 9, 11},
  array<int,6>{1, 2, 3, 5, 12, 13},
  array<int,6>{1, 2, 3, 6, 11, 13},
  array<int,6>{1, 2, 3, 7, 10, 13},
  array<int,6>{1, 2, 3, 7, 11, 12},
  array<int,6>{1, 2, 4, 5, 11, 13},
  array<int,6>{1, 2, 4, 6, 10, 13},
  array<int,6>{1, 2, 4, 6, 11, 12},
  array<int,6>{1, 2, 4, 7, 9, 13},
  array<int,6>{1, 2, 4, 7, 10, 12},
  array<int,6>{1, 2, 4, 8, 9, 12},
  array<int,6>{1, 2, 4, 8, 10, 11},
  array<int,6>{1, 2, 3, 6, 12, 13},
  array<int,6>{1, 2, 3, 7, 11, 13},
  array<int,6>{1, 2, 4, 5, 12, 13},
  array<int,6>{1, 2, 4, 6, 11, 13},
  array<int,6>{1, 2, 4, 7, 10, 13},
  array<int,6>{1, 2, 4, 7, 11, 12},
  array<int,6>{1, 2, 4, 8, 9, 13},
  array<int,6>{1, 2, 4, 8, 10, 12},
  array<int,6>{1, 2, 3, 7, 12, 13},
  array<int,6>{1, 2, 4, 6, 12, 13},
  array<int,6>{1, 2, 4, 7, 11, 13},
  array<int,6>{1, 2, 4, 8, 10, 13},
  array<int,6>{1, 2, 4, 8, 11, 12},
  array<int,6>{1, 2, 4, 7, 12, 13},
  array<int,6>{1, 2, 4, 8, 11, 13},
};

/// The NumArrays from Appendix II Group B of the 1944 Technical Manual
/// These are the arrays with one repeat.
vector<array<int, 6> > M209::NumArrayAppendixIIB = {
  array<int, 6>{  1, 1, 2, 3, 8, 13},
  array<int, 6>{  1, 1, 2, 4, 9, 11},
  array<int, 6>{  1, 1, 2, 4, 8, 12},
  array<int, 6>{  1, 1, 2, 4, 7, 13},
  array<int, 6>{  1, 1, 2, 5, 9, 10},
  array<int, 6>{  1, 1, 2, 5, 8, 11},
  array<int, 6>{  1, 1, 2, 5, 7, 12},
  array<int, 6>{  1, 1, 2, 5, 6, 13},
  array<int, 6>{  1, 1, 3, 4, 9, 10},
  array<int, 6>{  1, 1, 3, 4, 8, 11},
  array<int, 6>{  1, 1, 3, 4, 7, 12},
  array<int, 6>{  1, 1, 3, 4, 6, 13},
  array<int, 6>{  1, 1, 3, 5, 8, 10},
  array<int, 6>{  1, 1, 3, 5, 7, 11},
  array<int, 6>{  1, 1, 3, 5, 6, 12},
  array<int, 6>{  1, 1, 3, 6, 8, 9},
  array<int, 6>{  1, 1, 3, 6, 7, 10},
  array<int, 6>{  1, 2, 2, 3, 9, 11},
  array<int, 6>{  1, 2, 2, 3, 8, 12},
  array<int, 6>{  1, 2, 2, 3, 7, 13},
  array<int, 6>{  1, 2, 2, 4, 8, 11},
  array<int, 6>{  1, 2, 2, 4, 7, 12},
  array<int, 6>{  1, 2, 2, 4, 6, 13},
  array<int, 6>{  1, 2, 2, 5, 8, 10},
  array<int, 6>{  1, 2, 2, 5, 7, 11},
  array<int, 6>{  1, 2, 2, 6, 8, 9},
  array<int, 6>{  1, 2, 2, 6, 7, 10},
  array<int, 6>{  1, 2, 3, 3, 9, 10},
  array<int, 6>{  1, 2, 3, 3, 8, 11},
  array<int, 6>{  1, 2, 3, 3, 7, 12},
  array<int, 6>{  1, 2, 3, 3, 6, 13},
  array<int, 6>{  1, 2, 3, 4, 9, 9},
  array<int, 6>{  1, 2, 3, 5, 5, 12},
  array<int, 6>{  1, 2, 3, 6, 6, 10},
  array<int, 6>{  1, 2, 4, 4, 8, 9},
  array<int, 6>{  1, 2, 4, 5, 5, 11},
  array<int, 6>{  1, 2, 4, 6, 6, 9},
  array<int, 6>{  1, 1, 2, 4, 9, 12},
  array<int, 6>{  1, 1, 2, 4, 8, 13},
  array<int, 6>{  1, 1, 2, 5, 9, 11},
  array<int, 6>{  1, 1, 2, 5, 8, 12},
  array<int, 6>{  1, 1, 2, 5, 7, 13},
  array<int, 6>{  1, 1, 3, 4, 9, 11},
  array<int, 6>{  1, 1, 3, 4, 8, 12},
  array<int, 6>{  1, 1, 3, 4, 7, 13},
  array<int, 6>{  1, 1, 3, 5, 9, 10},
  array<int, 6>{  1, 1, 3, 5, 8, 11},
  array<int, 6>{  1, 1, 3, 5, 7, 12},
  array<int, 6>{  1, 1, 3, 5, 6, 13},
  array<int, 6>{  1, 1, 3, 6, 8, 10},
  array<int, 6>{  1, 1, 3, 6, 7, 11},
  array<int, 6>{  1, 2, 2, 3, 9, 12},
  array<int, 6>{  1, 2, 2, 3, 8, 13},
  array<int, 6>{  1, 2, 2, 4, 9, 11},
  array<int, 6>{  1, 2, 2, 4, 7, 13},
  array<int, 6>{  1, 2, 2, 5, 9, 10},
  array<int, 6>{  1, 2, 2, 5, 8, 11},
  array<int, 6>{  1, 2, 2, 5, 7, 12},
  array<int, 6>{  1, 2, 2, 5, 6, 13},
  array<int, 6>{  1, 2, 2, 6, 8, 10},
  array<int, 6>{  1, 2, 2, 6, 7, 11},
  array<int, 6>{  1, 2, 3, 3, 9, 11},
  array<int, 6>{  1, 2, 3, 3, 8, 12},
  array<int, 6>{  1, 2, 3, 3, 7, 13},
  array<int, 6>{  1, 2, 3, 5, 9, 9},
  array<int, 6>{  1, 2, 3, 5, 5, 13},
  array<int, 6>{  1, 2, 3, 6, 6, 11},
  array<int, 6>{  1, 2, 3, 7, 7, 9},
  array<int, 6>{  1, 2, 4, 4, 7, 11},
  array<int, 6>{  1, 2, 4, 4, 5, 13},
  array<int, 6>{  1, 2, 4, 5, 5, 12},
  array<int, 6>{  1, 1, 2, 4, 9, 13},
  array<int, 6>{  1, 1, 2, 5, 10, 11},
  array<int, 6>{  1, 1, 2, 5, 9, 12},
  array<int, 6>{  1, 1, 2, 5, 8, 13},
  array<int, 6>{  1, 1, 3, 4, 10, 11},
  array<int, 6>{  1, 1, 3, 4, 9, 12},
  array<int, 6>{  1, 1, 3, 4, 8, 13},
  array<int, 6>{  1, 1, 3, 5, 9, 11},
  array<int, 6>{  1, 1, 3, 5, 8, 12},
  array<int, 6>{  1, 1, 3, 5, 7, 13},
  array<int, 6>{  1, 1, 3, 6, 9, 10},
  array<int, 6>{  1, 1, 3, 6, 8, 11},
  array<int, 6>{  1, 1, 3, 6, 7, 12},
  array<int, 6>{  1, 2, 2, 3, 9, 13},
  array<int, 6>{  1, 2, 2, 4, 10, 11},
  array<int, 6>{  1, 2, 2, 4, 9, 12},
  array<int, 6>{  1, 2, 2, 4, 8, 13},
  array<int, 6>{  1, 2, 2, 5, 9, 11},
  array<int, 6>{  1, 2, 2, 5, 8, 12},
  array<int, 6>{  1, 2, 2, 5, 7, 13},
  array<int, 6>{  1, 2, 2, 6, 9, 10},
  array<int, 6>{  1, 2, 2, 6, 8, 11},
  array<int, 6>{  1, 2, 2, 6, 7, 12},
  array<int, 6>{  1, 2, 3, 3, 10, 11},
  array<int, 6>{  1, 2, 3, 3, 9, 12},
  array<int, 6>{  1, 2, 3, 3, 8, 13},
  array<int, 6>{  1, 2, 3, 4, 10, 10},
  array<int, 6>{  1, 2, 3, 6, 6, 12},
  array<int, 6>{  1, 2, 3, 6, 9, 9},
  array<int, 6>{  1, 2, 3, 7, 7, 10},
  array<int, 6>{  1, 2, 4, 4, 9, 10},
  array<int, 6>{  1, 2, 4, 4, 8, 11},
  array<int, 6>{  1, 2, 4, 4, 7, 12},
  array<int, 6>{  1, 2, 4, 4, 6, 13},
  array<int, 6>{  1, 2, 4, 5, 5, 13},
  array<int, 6>{  1, 2, 4, 5, 9, 9},
  array<int, 6>{  1, 2, 4, 6, 6, 11},
  array<int, 6>{  1, 2, 4, 7, 7, 9},
  array<int, 6>{  1, 1, 2, 5, 10, 12},
  array<int, 6>{  1, 1, 2, 5, 9, 13},
  array<int, 6>{  1, 1, 3, 4, 10, 12},
  array<int, 6>{  1, 1, 3, 4, 9, 13},
  array<int, 6>{  1, 1, 3, 5, 10, 11},
  array<int, 6>{  1, 1, 3, 5, 9, 12},
  array<int, 6>{  1, 1, 3, 5, 8, 13},
  array<int, 6>{  1, 1, 3, 6, 9, 11},
  array<int, 6>{  1, 1, 3, 6, 8, 12},
  array<int, 6>{  1, 1, 3, 6, 7, 13},
  array<int, 6>{  1, 2, 2, 4, 9, 13},
  array<int, 6>{  1, 2, 2, 5, 10, 11},
  array<int, 6>{  1, 2, 2, 5, 9, 12},
  array<int, 6>{  1, 2, 2, 5, 8, 13},
  array<int, 6>{  1, 2, 2, 6, 9, 11},
  array<int, 6>{  1, 2, 2, 6, 7, 13},
  array<int, 6>{  1, 2, 3, 3, 10, 12},
  array<int, 6>{  1, 2, 3, 3, 9, 13},
  array<int, 6>{  1, 2, 3, 5, 10, 10},
  array<int, 6>{  1, 2, 3, 6, 6, 13},
  array<int, 6>{  1, 2, 3, 7, 9, 9},
  array<int, 6>{  1, 2, 3, 7, 7, 11},
  array<int, 6>{  1, 2, 4, 4, 9, 11},
  array<int, 6>{  1, 2, 4, 4, 7, 13},
  array<int, 6>{  1, 2, 4, 6, 9, 9},
  array<int, 6>{  1, 2, 4, 7, 7, 10},
  array<int, 6>{  1, 1, 2, 5, 10, 13},
  array<int, 6>{  1, 1, 3, 4, 10, 13},
  array<int, 6>{  1, 1, 3, 5, 10, 12},
  array<int, 6>{  1, 1, 3, 5, 9, 13},
  array<int, 6>{  1, 1, 3, 6, 10, 11},
  array<int, 6>{  1, 1, 3, 6, 9, 12},
  array<int, 6>{  1, 1, 3, 6, 8, 13},
  array<int, 6>{  1, 2, 2, 4, 10, 13},
  array<int, 6>{  1, 2, 2, 5, 10, 12},
  array<int, 6>{  1, 2, 2, 5, 9, 13},
  array<int, 6>{  1, 2, 2, 6, 9, 12},
  array<int, 6>{  1, 2, 2, 6, 8, 13},
  array<int, 6>{  1, 2, 3, 3, 10, 13},
  array<int, 6>{  1, 2, 3, 4, 11, 11},
  array<int, 6>{  1, 2, 3, 6, 10, 10},
  array<int, 6>{  1, 2, 3, 7, 7, 12},
  array<int, 6>{  1, 2, 4, 4, 10, 11},
  array<int, 6>{  1, 2, 4, 4, 9, 12},
  array<int, 6>{  1, 2, 4, 4, 8, 13},
  array<int, 6>{  1, 2, 4, 6, 6, 13},
  array<int, 6>{  1, 2, 4, 7, 9, 9},
  array<int, 6>{  1, 2, 4, 7, 7, 11},
  array<int, 6>{  1, 2, 4, 8, 8, 9},
  array<int, 6>{  1, 1, 3, 5, 11, 12},
  array<int, 6>{  1, 1, 3, 5, 10, 13},
  array<int, 6>{  1, 1, 3, 6, 10, 12},
  array<int, 6>{  1, 1, 3, 6, 9, 13},
  array<int, 6>{  1, 2, 2, 4, 11, 13},
  array<int, 6>{  1, 2, 2, 5, 11, 12},
  array<int, 6>{  1, 2, 2, 5, 10, 13},
  array<int, 6>{  1, 2, 2, 6, 9, 13},
  array<int, 6>{  1, 2, 3, 3, 11, 13},
  array<int, 6>{  1, 2, 3, 5, 11, 11},
  array<int, 6>{  1, 2, 3, 7, 10, 10},
  array<int, 6>{  1, 2, 3, 7, 7, 13},
  array<int, 6>{  1, 2, 4, 7, 7, 12},
  array<int, 6>{  1, 2, 4, 8, 9, 9},
  array<int, 6>{  1, 1, 3, 5, 11, 13},
  array<int, 6>{  1, 1, 3, 6, 11, 12},
  array<int, 6>{  1, 1, 3, 6, 10, 13},
  array<int, 6>{  1, 2, 2, 4, 12, 13},
  array<int, 6>{  1, 2, 2, 5, 11, 13},
  array<int, 6>{  1, 2, 2, 6, 11, 12},
  array<int, 6>{  1, 2, 2, 6, 10, 13},
  array<int, 6>{  1, 2, 3, 6, 11, 11},
  array<int, 6>{  1, 2, 4, 4, 11, 12},
  array<int, 6>{  1, 2, 4, 4, 10, 13},
  array<int, 6>{  1, 2, 4, 5, 11, 11},
  array<int, 6>{  1, 2, 4, 7, 10, 10},
  array<int, 6>{  1, 2, 4, 7, 7, 13},
  array<int, 6>{  1, 2, 4, 8, 8, 11},
  array<int, 6>{  1, 1, 3, 6, 11, 13},
  array<int, 6>{  1, 2, 2, 6, 11, 13},
  array<int, 6>{  1, 2, 3, 5, 12, 12},
  array<int, 6>{  1, 2, 4, 4, 11, 13},
  array<int, 6>{  1, 2, 4, 6, 11, 11},
  array<int, 6>{  1, 1, 3, 6, 12, 13},
  array<int, 6>{  1, 2, 2, 6, 12, 13},
  array<int, 6>{  1, 2, 3, 6, 12, 12},
  array<int, 6>{  1, 2, 4, 4, 12, 13},
  array<int, 6>{  1, 2, 4, 5, 12, 12},
  array<int, 6>{  1, 2, 4, 7, 11, 11},
  array<int, 6>{  1, 2, 4, 8, 8, 13},
  array<int, 6>{  1, 2, 2, 6, 13, 13},
  array<int, 6>{  1, 2, 3, 5, 13, 13},
  array<int, 6>{  1, 2, 4, 8, 11, 11},
  array<int, 6>{  1, 2, 3, 6, 13, 13},
  array<int, 6>{  1, 2, 4, 7, 12, 12},
  array<int, 6>{  1, 2, 3, 7, 13, 13}
};