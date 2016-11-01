// Defender Of Nothing
// Copyright (C) 2007 by David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include <allegro.h>
#include "final.h"
#include "game.h"
#include "menu.h"

Final::Final()
{
  m_final_time = GAME_T;
  m_enter_pressed = false;
}

Final::~Final()
{
}

GameState *Final::update()
{
  if (key[KEY_ENTER]) {
    m_enter_pressed = true;
  }
  else if (m_enter_pressed) {
    return new Menu();
  }

  return this;
}

void Final::draw(BITMAP *bmp)
{
  clear_bitmap(bmp);
  draw_text(bmp, 0, 0*12, makecol(255, 255, 255), "This is the final. Are you disappointed? So I am.");
//   draw_text(bmp, 0, 1*12, makecol(255, 255, 255), "You kill %d angels", 0);
//   draw_text(bmp, 0, 3*12, makecol(255, 255, 255), "You kill %d humans", 0);
//   draw_text(bmp, 0, 4*12, makecol(255, 255, 255), "You save %d humans", 0);
//   draw_text(bmp, 0, 5*12, makecol(255, 255, 255), "You shoot %d times", 0);
//   draw_text(bmp, 0, 7*12, makecol(255, 255, 255), "Average of kills/shoots: %3.02f", 0.0);
}
