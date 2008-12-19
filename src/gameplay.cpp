#include <cmath>
#include <allegro.h>
#include "gfx.hpp"
#include "game.hpp"
#include "gameplay.hpp"
#include "player.hpp"
#include "level.hpp"
#include "util.hpp"


#define MENU_FADEIN_DURATION (BPS/4.0)


GamePlay::GamePlay()
  : m_esc(KEY_ESC)
{
  m_quit_time = 0;

  m_level = new Level(this, 1);
  m_player = new Player(this);
}


GamePlay::~GamePlay()
{
  delete m_player;
  delete m_level;
}


Player *GamePlay::get_player()
{
  return m_player;
}


Level *GamePlay::get_level()
{
  return m_level;
}


GameState *GamePlay::update()
{
  bool esc = m_esc.pressed();

#if 0
  // are we in the menu to quit?
  if (m_quit_time > 0) {
    if (esc)
      m_quit_time = 0;
    else
      return this;
  }
  // go to the quit menu?
  else if (esc) {
    m_quit_time = get_time();
    return this;
  }
#else
  if (esc)
    return NULL;
#endif

  // playing...
  m_level->update();
  m_player->update();
  return this;
}


void GamePlay::draw(BITMAP *bmp)
{
  clear_to_color(bmp, makecol(0, 180, 200));

  // draw the game
  m_level->draw(bmp);
  m_player->draw(bmp);

  // draw the quit-menu
  if (m_quit_time > 0) {
    double t = exp_ramp(m_quit_time, MENU_FADEIN_DURATION);

    trans_mode(190*t);
    rectfill(bmp, 0, 0, GFX_W-1, GFX_H-1, makecol(0, 0, 0));
    solid_mode();

    textout_ex(bmp, font, "quit menu", 0, -8+8*t, makecol(255, 255, 255), -1);
  }
}
