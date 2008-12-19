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

#ifndef ANGEL_H_INCLUDED
#define ANGEL_H_INCLUDED

#include "object.h"
#include "vector2d.h"


class Sprite;
class Person;
class Level;


enum AngelState {
  INTRO_ANGEL,
  FLOATING_ANGEL,
  FLYTOANYWHERE_ANGEL,
  FLYTOPERSON_ANGEL,
  FLYUP_ANGEL,
  AVOIDFIRE_ANGEL,
  FIGHTING_ANGEL,
  ABDUCTING_ANGEL,
  HIT_ANGEL,
  DEAD_ANGEL
};


// the angel
class Angel : public Object
{
  vector2d m_pos;		// position of the angel in the level
  vector2d m_vel;		// velocity of the angel in the level

  // state of the angel
  AngelState m_state;
  int m_state_time;
  bool m_right;
  double m_energy;

  // hit
  int m_hit_num;

  // wings
  int m_wing_time;

  // target person
  Person *m_target;

public:
  Angel(vector2d pos);
  virtual ~Angel();

  virtual void update();
  virtual void draw(BITMAP *bmp);

  virtual bool is_dead();

  bool is_hittable();
  void hit(vector2d vel, double energy);
  void kill();
  void fire_event_person_die(Person *person);

  vector2d get_pos() const;
  vector2d get_vel() const;
  double get_energy() const;

  void get_sprites(Sprite *&sprite, Sprite *&wings_sprite);

private:
  void set_state(AngelState state);
  void burn();
  Person *find_target_person();
  vector2d calculate_vel_to_abduct();
  Level *level();
};


#endif // ANGEL_H_INCLUDED
