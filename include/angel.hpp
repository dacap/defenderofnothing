#ifndef ANGEL_HPP
#define ANGEL_HPP

#include "object.hpp"
#include "vector2d.hpp"


enum AngelState {
  INTRO_ANGEL,
  FLOATING_ANGEL,
  FIGHTING_ANGEL,
  DEAD_ANGEL
};


// the angel
class Angel : public Object
{
  vector2d m_pos;		// position of the angel in the level

  // state of the angel
  AngelState m_state;
  int m_state_time;

  // wings
  int m_wing_time;

  // wave (it's like the Devil's fire... but of the angels)
  int m_wave_time;

public:
  Angel(vector2d pos);
  virtual ~Angel();

  virtual void update();
  virtual void draw(BITMAP *bmp);

  virtual bool is_dead();
  void kill();

  vector2d get_pos() const;

private:
  void set_state(AngelState state);
};


#endif // ANGEL_HPP
