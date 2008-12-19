#include <allegro.h>
#include "scorer.hpp"
#include "media.hpp"
#include "level.hpp"
#include "gfx.hpp"
#include "game.hpp"
#include "gameplay.hpp"
#include "player.hpp"
#include "person.hpp"
#include "angel.hpp"


Scorer::Scorer()
{
  m_people_in_heaven = 0;
  m_people_in_hell = 0;
  m_people_in_earth = 0;
}


Scorer::~Scorer()
{
}


void Scorer::update()
{
}


void Scorer::draw(BITMAP *bmp)
{
//   masked_blit(MEDIA_BITMAP(PEOPLE_PCX), bmp, 0, 4, 0,  0, TILE_W, TILE_H-4);
//   masked_blit(MEDIA_BITMAP(PEOPLE_PCX), bmp, 0, 4, 0,  8, TILE_W, TILE_H-4);
//   masked_blit(MEDIA_BITMAP(PEOPLE_PCX), bmp, 0, 4, 0, 16, TILE_W, TILE_H-4);
//   draw_text(bmp, 16,  0, makecol(100, 100, 120), "x%02d", m_people_in_heaven);
//   draw_text(bmp, 16,  8, makecol(100, 100, 120), "x%02d", m_people_in_hell);
//   draw_text(bmp, 16, 16, makecol(100, 100, 120), "x%02d", m_people_in_earth);

  draw_minimap(bmp, bmp->w-50-3, 2, 50, 32);
}


void Scorer::one_to_heaven()
{
  --m_people_in_earth;
  ++m_people_in_heaven;
}


void Scorer::one_to_hell()
{
  --m_people_in_earth;
  ++m_people_in_hell;
}


void Scorer::one_to_earth()
{
  ++m_people_in_earth;
}


void Scorer::draw_minimap(BITMAP *bmp, int x, int y, int w, int h)
{
  GamePlay *gameplay = GAMEPLAY;
  vector2d pos;
  int u, v;

  rect(bmp, x, y, x+w-1, y+h-1, makecol(240, 0, 0));

  trans_mode(128);
  rectfill(bmp, x+1, y+1, x+w-2, y+h-2, makecol(128, 0, 0));
  solid_mode();

  pos = gameplay->get_player()->get_pos();
  to_minimap(pos, x, y, w, h, u, v);
  putpixel(bmp, u, v, makecol(255, 200, 0));

  ObjectList people = gameplay->get_people();
  ObjectList angels = gameplay->get_angels();
  ObjectList::const_iterator it;

  for (it = people.begin(); it != people.end(); ++it) {
    pos = static_cast<Person *>(*it)->get_pos();
    to_minimap(pos, x, y, w, h, u, v);
    putpixel(bmp, u, v, makecol(0, 100, 255));
  }

  for (it = angels.begin(); it != angels.end(); ++it) {
    pos = static_cast<Angel *>(*it)->get_pos();
    to_minimap(pos, x, y, w, h, u, v);
    putpixel(bmp, u, v, makecol(255, 255, 255));
  }
}


void Scorer::to_minimap(vector2d pos, int x, int y, int w, int h,
			int &u, int &v)
{
  u = x+1 + (w-2)*pos.x/(GAMEPLAY->get_level()->get_w()-1);
  v = y+1 + (h-2)*pos.y/(GAMEPLAY->get_level()->get_h()-1);
  u = MID(x+1, u, x+w-2);
  v = MID(y+1, v, y+h-2);
}
