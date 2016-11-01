// Defender Of Nothing
// Copyright (C) 2007 by David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#ifndef GAMESTATE_H_INCLUDED
#define GAMESTATE_H_INCLUDED


// a game state
class GameState
{
public:
  GameState();
  virtual ~GameState();

  // update the logic of the game and returns the new (or this) state
  // note: you can return NULL if the game ends completelly (e.g. return to OS)
  virtual GameState *update() = 0;

  // draw the game in the specified bitmap
  virtual void draw(BITMAP *bmp) = 0;

};


#endif // GAME_H_INCLUDED
