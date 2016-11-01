// Defender Of Nothing
// Copyright (C) 2007 by David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#ifndef FINAL_H_INCLUDED
#define FINAL_H_INCLUDED


#include "gamestate.h"


class Final : public GameState
{
  int m_final_time;
  bool m_enter_pressed;

public:
  Final();
  virtual ~Final();

  GameState *update();
  void draw(BITMAP *bmp);
};


#endif // FINAL_H_INCLUDED
