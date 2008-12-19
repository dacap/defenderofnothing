// Defender Of Nothing
// Copyright (C) 2007 by David Capello
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
// * Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in
//   the documentation and/or other materials provided with the
//   distribution.
// * Neither the name of the author nor the names of its contributors
//   may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "object.h"
#include "vector2d.h"
#include "input.h"


class Level;
class Sprite;


enum PlayerState {
  INIT_PLAYER,
  INTRO_PLAYER,
  FLOATING_PLAYER,
  STANDING_PLAYER,
  WALKING_PLAYER,
  DYING_PLAYER,
  DEAD_PLAYER
};


// the player
class Player : public Object
{
  Input *m_input;

  // position
  vector2d m_pos;		// position of the player in the level
  vector2d m_vel;		// velocity
  bool m_right;			// true=look to right, false=look to left

  // look-at information (to move scroll)
  vector2d m_look;

  // state of the player
  PlayerState m_state;
  PlayerState m_old_state;
  int m_state_time;

  // subanimations
  int m_quiet_time;
  int m_tail_time;
  int m_charge_time;
  bool m_fire_big;
  int m_fire_time;
  int m_fire_counter;
  int m_bad_fire_counter;

  // effects
  double m_halo_factor;
//   int m_smoke_time;

public:
  Player();
  virtual ~Player();

  virtual void update();
  virtual void draw(BITMAP *bmp);

  void get_sprites(Sprite *&sprite, Sprite *&tail_sprite);

  vector2d get_pos() const;
  void inc_halo(double increment);
  void inc_bad_fire();

  void start_level();

private:
  void update_fly();
  void update_walk();
  void update_fire();
  void draw_charge(BITMAP *bmp, int x, int y);
  void draw_halo(BITMAP *bmp, Sprite *sprite);
  void set_state(PlayerState state);
  Level *level();
};


#endif // PLAYER_H_INCLUDED
