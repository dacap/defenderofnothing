#ifndef GAME_HPP
#define GAME_HPP

#include "gfx.hpp"
#include "gamestate.hpp"


// game speed (beats per second)
#define BPS 60

// game time
#define GAME_T (the_game->get_time())


// the game
class Game
{
  int m_time;
  GameState *m_state;

public:
  void start();
  int get_time();

  bool update();
  void draw(BITMAP *bmp);
};


// the current game running
extern Game *the_game;


#endif // GAME_HPP
