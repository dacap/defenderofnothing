#ifndef UTIL_HPP
#define UTIL_HPP

#include <cmath>
#include "game.hpp"


inline double exp_ramp(int start_time, double duration_in_beats)
{
  double t = (GAME_T - start_time) / duration_in_beats;
  return 1.0 - std::exp(-5.0 * t);
}


#endif
