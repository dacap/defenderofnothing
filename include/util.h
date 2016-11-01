// Defender Of Nothing
// Copyright (C) 2007 by David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <cmath>
#include <cstdlib>
#include <string>
#include "game.h"


inline double exp_ramp(int start_time, double duration_in_beats)
{
  if (GAME_T - start_time > duration_in_beats)
    return 1.0;
  else {
    double t = (GAME_T - start_time) / duration_in_beats;
    return 1.0 - std::exp(-5.0 * t);
  }
}


inline int rand_range(int min, int max)
{
  return min + (std::rand() % (max-min+1));
}


inline double rand_range(double min, double max)
{
  return rand_range(static_cast<int>(min * 1000),
		    static_cast<int>(max * 1000)) / 1000.0;
}


std::string redir(const std::string &filename);


#endif
