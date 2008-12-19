// Defender Of Nothing
// Copyright (C) 2007 by David A. Capello
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
// * Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in
//   the documentation and/or other materials provided with the
//   distribution.
// * Neither the name of the Vaca nor the names of its contributors
//   may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.

#include <allegro.h>
#include "final.hpp"
#include "game.hpp"
#include "menu.hpp"


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
