#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "vector2d.hpp"
#include "input.hpp"


class GamePlay;
class Level;


enum PlayerState {
  INTRO_PLAYER,
  FLOATING_PLAYER,
  STANDING_PLAYER,
  WALKING_PLAYER,
  RUNNING_PLAYER,
  JUMPING_PLAYER,
  DYING_PLAYER,
  DEAD_PLAYER
};


// the player
class Player
{
  BITMAP *m_sprite;
  GamePlay *m_gameplay;
  Input *m_input;
  vector2d m_pos;		// position of the player in the level
  vector2d m_vel;		// velocity

  bool m_right;			// true=look to right, false=look to left

  // look-at information (to move scroll)
  vector2d m_look;

  // state of the player
  PlayerState m_state;
  int m_state_time;

  // subanimations
  int m_tail_time;
  int m_fire_time;
  int m_fire_counter;

public:
  Player(GamePlay *gameplay);
  virtual ~Player();

  void update();
  void draw(BITMAP *bmp);

private:
  void set_state(PlayerState state);
  Level *level();
};


#endif // PLAYER_HPP
