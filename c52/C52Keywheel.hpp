//
/// \file C52Keywheel.hpp
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

#ifndef C52Keywheel_hpp
#define C52Keywheel_hpp

#include "Keywheel.h"

/// Simulate a Keywheel on the C-52 cipher machine.  Inherits everytthing from the
/// m209 Kywheel class except the randomize routine, which has been modified
/// to limit the run lengths to 3 instead of seven
class C52Keywheel : public Keywheel {

public:
  //! Randomize pin settings.
  //
  //! Randomize such that 40%-60% are active and no
  //! more than three consecutive pins have the same setting.
  //
  void Randomize(void);

};

#endif /* C52Keywheel_hpp */
