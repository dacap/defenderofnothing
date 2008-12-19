// Defender Of Nothing
// Copyright (C) 2007 by David A. Capello
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
// * Neither the name of the Vaca nor the names of its contributors
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

#ifndef PERSON_HPP
#define PERSON_HPP

#include "object.hpp"
#include "vector2d.hpp"


class Angel;


enum PersonState {
  STAND_PERSON,
  WALKING_PERSON,
  LOOKING_PERSON,
  CALLING_PERSON,
  ABDUCTING_PERSON,
  FALLING_PERSON,
  DEAD_PERSON
};


class Person : public Object
{
  // type of person
  int m_type;

  // position
  vector2d m_pos;
  double m_velx;
  bool m_right;

  // state of the person
  PersonState m_state;
  int m_state_time;
  double m_toast_factor;

  // who is abducting me?
  Angel *m_abductor;

public:
  Person(vector2d pos);
  virtual ~Person();

  virtual void update();
  virtual void draw(BITMAP *bmp);

  virtual bool is_dead();

  vector2d get_pos() const;
//   void abduct(vector2d vel);

  Angel *get_abductor();
  void set_abductor(Angel *angel);
  void catch_person();
  void throw_person();
  void kill();

private:
  void set_state(PersonState state);
  void burn();
  BITMAP *prepare_sprite();
};


#endif // PERSON_HPP
