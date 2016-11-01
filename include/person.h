// Defender Of Nothing
// Copyright (C) 2007 by David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#ifndef PERSON_H_INCLUDED
#define PERSON_H_INCLUDED

#include "object.h"
#include "vector2d.h"


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

  void fire_event_angel_die(Angel *angel);

private:
  void set_state(PersonState state);
  void burn();
  BITMAP *prepare_sprite();
};


#endif // PERSON_H_INCLUDED
