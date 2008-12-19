#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "object.hpp"
#include "vector2d.hpp"
#include "input.hpp"


class Level;


enum PlayerState {
  INIT_PLAYER,
  INTRO_PLAYER,
  FLOATING_PLAYER,
  STANDING_PLAYER,
  WALKING_PLAYER,
  RUNNING_PLAYER,
  DYING_PLAYER,
  DEAD_PLAYER
};


// the player
class Player : public Object
{
  BITMAP *m_sprite;
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

public:
  Player();
  virtual ~Player();

  virtual void update();
  virtual void draw(BITMAP *bmp);

  vector2d get_pos() const;
  void inc_halo(double increment);
  void inc_bad_fire();

private:
  void update_fly();
  void update_walk();
  void update_fire();
  void draw_charge(BITMAP *bmp, int x, int y);
  void set_state(PlayerState state);
  Level *level();
};


#endif // PLAYER_HPP
