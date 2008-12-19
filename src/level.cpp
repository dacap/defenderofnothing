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

#include <cmath>
#include <allegro.h>
#include "level.h"
#include "gfx.h"
#include "util.h"
#include "media.h"
#include "game.h"
#include "gameplay.h"
#include "angel.h"

#define LOOK_AT_DURATION  (BPS*2)

Level::Level(int num)
{
  int w, h;

  switch (num) {
    case 1:
      w = 40;
      h = 30;
      m_angels_to_generate = 10;
      m_level_angel_interval = 2.0;
      m_max_dist_to_abduct = 3.0;
      m_min_angel_energy = 4.0;
      m_max_angel_energy = 10.0;
      break;
    case 2:
      w = 50;
      h = 20;
      m_angels_to_generate = 15;
      m_level_angel_interval = 1.5;
      m_max_dist_to_abduct = 5.0;
      m_min_angel_energy = 8.0;
      m_max_angel_energy = 20.0;
      break;
    case 3:
      w = 60;
      h = 15;
      m_angels_to_generate = 20;
      m_level_angel_interval = 1.0;
      m_max_dist_to_abduct = 10.0;
      m_min_angel_energy = 15.0;
      m_max_angel_energy = 40.0;
      break;
  }

  m_is_old = false;
  m_num = num;
  m_last_angel_time = 0;
  m_look_time = 0;

  set_tiles_size(w, h);
//   set_scroll(get_start_pos() - vector2d(GFX_CX, GFX_CY));
  set_scroll(get_start_pos() + vector2d(-GFX_CX, GFX_CY));

  for (int y=0; y<m_h; ++y)
    for (int x=0; x<m_w; ++x) {
      m_tiles_bg[y*m_w+x] = TILE_NOTHING;
      m_tiles_fg[y*m_w+x] = (y == m_h-1) ? TILE_FLOOR_EXTERN:
					   TILE_NOTHING;
    }
}

Level::~Level()
{
}

void Level::update()
{
  if (m_look_time > 0) {
    double t = exp_ramp(m_look_time, LOOK_AT_DURATION);
    set_scroll(m_look_from + (m_look_to - m_look_from) * t);
    if (GAME_T - m_look_time > LOOK_AT_DURATION) {
      m_look_time = 0;
      set_scroll(m_look_to);
    }
  }

  if (m_angels_to_generate > 0) {
    if (m_last_angel_time == 0) {
      m_last_angel_time = GAME_T;
      m_angels_to_generate--;

      GAMEPLAY->add_angel(new Angel(get_random_pos_for_angel()));
    }
    else if (GAME_T - m_last_angel_time > m_level_angel_interval * BPS) {
      m_last_angel_time = 0;
    }
  }
}

void Level::draw(BITMAP *bmp)
{
//   textprintf(bmp, font, 0, 8, makecol(255, 255, 255), "%02fx%02f", m_scroll.x, m_scroll.y);

  clear_to_color(bmp, makecol(0, 180, 190));

  BITMAP *bg1 = MEDIA_BITMAP(BG_PCX);
  BITMAP *bg2 = MEDIA_BITMAP(BG2_PCX);
  BITMAP *bg3 = MEDIA_BITMAP(BG3_PCX);

  draw_bg(bmp, bg1, -m_scroll.x/3, m_h*TILE_H-m_scroll.y-bg1->h-8);
  draw_bg(bmp, bg2, -m_scroll.x/2, m_h*TILE_H-m_scroll.y-bg2->h);
  draw_bg(bmp, bg3, -m_scroll.x/4, -m_scroll.y);

  int i, j, x, y, outx, outy;
  int x1, y1, x2, y2;

  get_first_visible_tile(x1, y1);

  x2 = x1 + LEVEL_W;
  y2 = y1 + LEVEL_H;

  outy = static_cast<int>(-std::fmod(std::floor(m_scroll.y), TILE_H));
  for (y=y1; y<=y2; ++y) {
    outx = static_cast<int>(-std::fmod(std::floor(m_scroll.x), TILE_W));
    for (x=x1; x<=x2; ++x) {
      if (x < 0 || x >= m_w ||
	  y < 0 || y >= m_h) break;

      Tile bg = m_tiles_bg[y*m_w+x];
      Tile fg = m_tiles_fg[y*m_w+x];

      Level::draw_tile(bmp, bg, outx, outy);
      Level::draw_tile(bmp, fg, outx, outy);
      outx += TILE_W;
    }
    outy += TILE_W;
  }
}

bool Level::is_dead()
{
  return m_is_old;
}

bool Level::is_finished()
{
  return m_angels_to_generate == 0 && GAMEPLAY->get_angels().empty();
}

int Level::get_num()
{
  return m_num;
}

int Level::get_w()
{
  return m_w;
}

int Level::get_h()
{
  return m_h;
}

vector2d Level::get_start_pos()
{
  return vector2d(m_w/2.0, m_h-LEVEL_CY);
}

vector2d Level::get_random_pos_for_people()
{
  std::vector<vector2d> positions;
  
  for (int y=1; y<m_h; ++y)
    for (int x=0; x<m_w; ++x)
      if (Level::tile_is_floor(m_tiles_fg[y*m_w+x]) &&
	  !Level::tile_is_floor(m_tiles_fg[(y-1)*m_w+x])) {
	positions.push_back(vector2d(x+0.5, y-0.01));
      }

  return positions[rand_range(0, positions.size()-1)];
}

vector2d Level::get_random_pos_for_angel()
{
  return vector2d(rand_range(0.5, m_w-0.5), -1.5);
}

// tries to look at the specified center
void Level::look_at(vector2d center)
{
  // time when the look-at animation start
  m_look_time = GAME_T;

  // from where
  m_look_from = m_scroll;

  // to where
  center.x = center.x*TILE_W - GFX_CX;
  center.y = center.y*TILE_H - GFX_CY;
  center.x = MID(0, center.x, m_w*TILE_W-GFX_W);
  center.y = MID(0, center.y, m_h*TILE_H-GFX_H);

  m_look_to = center;
}

void Level::to_screen(vector2d pos, int &x, int &y)
{
  x = static_cast<int>(-m_scroll.x + pos.x*TILE_W);
  y = static_cast<int>(-m_scroll.y + pos.y*TILE_H);
}

vector2d Level::to_level(int x, int y)
{
  return vector2d(1.0 * (x + m_scroll.x) / TILE_W,
		  1.0 * (y + m_scroll.y) / TILE_H);
}

bool Level::touch_floor(vector2d pos)
{
  int x = static_cast<int>(std::floor(pos.x));
  int y = static_cast<int>(std::floor(pos.y));
  return (x >= 0 && x < m_w &&
	  y >= 0 && y < m_h &&
	  Level::tile_is_floor(m_tiles_fg[y*m_w+x]));
}

bool Level::touch_wall(vector2d pos)
{
  int x = static_cast<int>(pos.x);
  int y = static_cast<int>(pos.y);
  return (x >= 0 && x < m_w &&
	  y >= 0 && y < m_h &&
	  Level::tile_is_touchable(m_tiles_fg[y*m_w+x]));
}

vector2d Level::get_stand_point(vector2d pos)
{
  pos.y = std::floor(pos.y);
  while (!touch_floor(pos)) {
    pos.y -= 1.0 / (TILE_H+1);	// pixel by pixel go to top
  }
  return pos;
}

double Level::get_max_distance_to_abduct()
{
  return m_max_dist_to_abduct;
}

double Level::get_min_angel_energy()
{
  return m_min_angel_energy;
}

double Level::get_max_angel_energy()
{
  return m_max_angel_energy;
}

int Level::get_angels_to_generate()
{
  return m_angels_to_generate;
}

void Level::set_old()
{
  m_is_old = true;
}

void Level::set_scroll(vector2d pos)
{
  pos.x = MID(0, pos.x, m_w*TILE_W-GFX_W);
  pos.y = MID(0, pos.y, m_h*TILE_H-GFX_H);

  // set the scroll
  m_scroll = pos;
}

void Level::get_first_visible_tile(int &x, int &y)
{
  x = static_cast<int>(std::floor(m_scroll.x) / TILE_W);
  y = static_cast<int>(std::floor(m_scroll.y) / TILE_H);
}

void Level::set_tiles_size(int w, int h)
{
  m_w = w;
  m_h = h;
  m_tiles_fg.resize(w*h);
  m_tiles_bg.resize(w*h);
}

void Level::draw_tile(BITMAP *bmp, Tile tile, int x, int y)
{
  if (tile != TILE_NOTHING)
    masked_blit(MEDIA_BITMAP(TILES_PCX), bmp,
		tile*TILE_W, 0, x, y, TILE_W, TILE_H);
}

bool Level::tile_is_floor(Tile tile)
{
  return
    tile == TILE_FLOOR_EXTERN ||
    tile == TILE_FLOOR_INTERN;
}

bool Level::tile_is_touchable(Tile tile)
{
  return tile != TILE_NOTHING;
}
