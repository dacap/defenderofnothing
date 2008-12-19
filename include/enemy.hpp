#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "vector2d.hpp"


class GamePlay;


// the enemy
class Enemy
{
  Input *m_input;
  vector2d m_pos;		// position of the enemy in the level

public:
  Enemy();
  virtual ~Enemy();

  void update(GamePlay *gameplay);
  void draw(GamePlay *gameplay, BITMAP *bmp);
};


#endif // ENEMY_HPP
