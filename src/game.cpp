// Defender Of Nothing
// Copyright (C) 2007 by David Capello
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
// * Neither the name of the author nor the names of its contributors
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
#include "game.h"
#include "gameplay.h"
#include "menu.h"

Game *the_game = NULL;

void Game::start()
{
  // game time to control animations and time-outs
  m_time = 0;

  // create the main menu
  m_state = new Menu();
}

int Game::get_time()
{
  return m_time;
}

bool Game::update()
{
  // update input state
  poll_keyboard();
  if (num_joysticks > 0)
    poll_joystick();

  // update the current game-state, and hold the new state
  GameState *new_state = m_state->update();

  // the state changes?
  if (new_state != m_state) {
    // delete the old state
    delete m_state;
    // renew the state
    m_state = new_state;
  }

  // the time pass
  ++m_time;

  // end of game?
  return m_state != NULL;
}

void Game::draw(BITMAP *bmp)
{
  // draw the game
  if (m_state != NULL)
    m_state->draw(bmp);
}

GameState *Game::get_state()
{
  return m_state;
}
