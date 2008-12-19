#ifndef GAMEPLAY_HPP
#define GAMEPLAY_HPP

#include "gamestate.hpp"
#include "lockedkey.hpp"


class Level;
class Player;


// a game state
class GamePlay : public GameState
{
  Level *m_level;
  Player *m_player;

  // quit menu
  LockedKey m_esc;
  int m_quit_time;

public:
  GamePlay();
  virtual ~GamePlay();

  Player *get_player();
  Level *get_level();

  GameState *update();
  void draw(BITMAP *bmp);
};


#endif // GAMEPLAY_HPP
