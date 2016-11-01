// Defender Of Nothing
// Copyright (C) 2007 by David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include <allegro.h>
#include "scorer.h"
#include "media.h"
#include "level.h"
#include "gfx.h"
#include "game.h"
#include "gameplay.h"
#include "player.h"
#include "person.h"
#include "angel.h"

Scorer::Scorer()
{
  m_people_in_heaven = 0;
  m_people_in_hell = 0;
  m_people_in_earth = 0;

  m_killed_angels = 0;
  m_total_angels = 0;
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

  draw_text(bmp, 2,  2, makecol(  0, 0, 255), "%02d in Heaven", m_people_in_heaven);
  draw_text(bmp, 2, 14, makecol(255, 0,   0), "%02d in Hell", m_people_in_hell);

  line(bmp, 80, 2, 80, 14+12, makecol(255, 255, 0));
  draw_text(bmp, 81, (2+12+2+12+2)/2-6, makecol(255, 255, 0), "%02d", m_people_in_earth);

  draw_minimap(bmp, bmp->w-50-3, 2, 50, 32);
  draw_text_r(bmp, bmp->w-3, 2+32+2, makecol(255, 255, 255), "Angels");
  draw_text_r(bmp, bmp->w-3, 2+32+2+12, makecol(255, 255, 255), "%02d/%02d",
	      m_killed_angels, m_total_angels);
}

void Scorer::reset(int angels, int people)
{
  m_people_in_earth = people;
  m_people_in_heaven = 0;
  m_people_in_hell = 0;

  m_total_angels = angels;
  m_killed_angels = 0;
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

void Scorer::one_to_killed_angel()
{
  m_killed_angels++;
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

  const ObjectList& people = gameplay->get_people();
  const ObjectList& angels = gameplay->get_angels();
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
  u = static_cast<int>(x+1 + (w-2)*pos.x/(GAMEPLAY->get_level()->get_w()-1));
  v = static_cast<int>(y+1 + (h-2)*pos.y/(GAMEPLAY->get_level()->get_h()-1));
  u = MID(x+1, u, x+w-2);
  v = MID(y+1, v, y+h-2);
}
