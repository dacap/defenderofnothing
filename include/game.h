// Defender Of Nothing
// Copyright (C) 2007 by David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "gfx.h"
#include "gamestate.h"


// game speed (beats per second)
#define BPS	60

// game time
#define GAME_T	(the_game->get_time())


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

  GameState *get_state();
};


// the current game
extern Game *the_game;


#endif // GAME_H_INCLUDED
