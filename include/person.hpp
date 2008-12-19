#ifndef PERSON_HPP
#define PERSON_HPP

#include "object.hpp"
#include "vector2d.hpp"


enum PersonState {
  STAND_PERSON,
  WALKING_PERSON,
  LOOKING_PERSON,
  CALLING_PERSON,
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

public:
  Person(vector2d pos);
  virtual ~Person();

  virtual void update();
  virtual void draw(BITMAP *bmp);

  virtual bool is_dead();

  vector2d get_pos() const;

private:
  void set_state(PersonState state);
  void burn_completely();
  BITMAP *prepare_sprite();
};


#endif // PERSON_HPP
