#include <allegro.h>
#include "angel.hpp"
#include "game.hpp"
#include "gameplay.hpp"
#include "level.hpp"
// #include "util.hpp"
#include "media.hpp"
// #include "particle.hpp"


#define WING_CYCLE_DURATION	(BPS*1.0)


Angel::Angel(vector2d pos)
{
  m_pos = pos;
  m_wing_time = GAME_T;
}


Angel::~Angel()
{
}


void Angel::update()
{
  m_pos.y += 2.0 / BPS;
}


void Angel::draw(BITMAP *bmp)
{
  BITMAP *sprite;
  BITMAP *wing_sprite;
  int x, y;

  GAMEPLAY->get_level()->to_screen(m_pos, x, y);

  sprite = angels_fly_down_bmp;

  {
    int anilength = (WINGS11_PCX - WINGS00_PCX + 1);
    double t = (GAME_T - m_wing_time) / WING_CYCLE_DURATION;
    int c = static_cast<int>(t * anilength) % anilength;

    wing_sprite = MEDIA_BITMAP(WINGS00_PCX+c);
    draw_sprite(bmp, wing_sprite,
		x - sprite->w/2 + 12 - 38,
		y - sprite->h+1 + 15 - 24);
  }
  
  draw_sprite(bmp, sprite, x - sprite->w/2, y - sprite->h+1);
}


bool Angel::is_dead()
{
  return m_state == DEAD_ANGEL;
}


void Angel::kill()
{
  m_state = DEAD_ANGEL;
}


vector2d Angel::get_pos() const
{
  return m_pos;
}
