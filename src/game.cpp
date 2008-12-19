#include <allegro.h>
#include "game.hpp"
// #include "intro.hpp"
#include "gameplay.hpp"


Game *the_game = NULL;


void Game::start()
{
  // game time to control animations and time-outs
  m_time = 0;

  // create introduction game state
//   m_state = new Intro();
  m_state = new GamePlay();
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

