// Defender Of Nothing
// Copyright (C) 2007 by David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <stack>
#include "gamestate.h"
#include "lockedkey.h"


// the menu
class Menu : public GameState
{
  int m_start_time;
  int m_selected;
  int m_options;
  LockedKey m_up;
  LockedKey m_down;
  LockedKey m_enter;
  LockedKey m_esc;
  BITMAP *m_logo;

public:
  Menu();
  virtual ~Menu();

  virtual GameState *update();
  virtual void draw(BITMAP *bmp);

  static void draw_item(BITMAP *bmp, int x, int y, const char *text, bool selected);
};


#endif // MENU_H_INCLUDED
