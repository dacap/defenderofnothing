#include <cmath>
#include <allegro.h>
#include "level.hpp"
#include "gfx.hpp"
#include "util.hpp"


#define LOOK_AT_DURATION  (BPS*4)


Level::Level(GamePlay *gameplay, int num)
{
  m_gameplay = gameplay;
  m_num = num;

  for (int y=0; y<m_h; ++y)
    for (int x=0; x<m_w; ++x) {
      m_tiles_bg[y*m_w+x] = TILE_NOTHING;
      m_tiles_fg[y*m_w+x] = (y == m_h-1) ? TILE_FLOOR_EXTERN:
					   TILE_NOTHING;
    }

  set_tiles_size(30, 20);
  set_scroll(get_start_pos() - vector2d(GFX_CX, GFX_CY));

  m_look_time = 0;
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
}


void Level::draw(BITMAP *bmp)
{
  textprintf(bmp, font, 0, 8, makecol(255, 255, 255), "%02fx%02f", m_scroll.x, m_scroll.y);

  int i, j, x, y, outx, outy;
  int x1, y1, x2, y2;

  get_first_visible_tile(x1, y1);

  x2 = x1 + LEVEL_W;
  y2 = y1 + LEVEL_H;

  outy = -fmod(m_scroll.y, TILE_H);
  for (y=y1; y<=y2; ++y) {
    outx = -fmod(m_scroll.x, TILE_W);
    for (x=x1; x<=x2; ++x) {
      Tile bg = m_tiles_bg[y*m_h+x];
      Tile fg = m_tiles_fg[y*m_w+x];

      draw_tile(bmp, bg, outx, outy);
      draw_tile(bmp, fg, outx, outy);
      outx += TILE_W;
    }
    outy += TILE_W;
  }
}


// int Level::get_w()
// {
//   return m_w;
// }


int Level::get_h()
{
  return m_h;
}


// vector2d Level::get_center()
// {
//   return vector2d(m_w/2.0, m_h/2.0);
// }


vector2d Level::get_start_pos()
{
  return vector2d(m_w/2.0, m_h/2.0);
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
  x = - m_scroll.x + pos.x*TILE_W;
  y = - m_scroll.y + pos.y*TILE_H;
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
  x = std::floor(m_scroll.x) / TILE_W;
  y = std::floor(m_scroll.y) / TILE_H;
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
//   if (tile > TILE_NOTHING) {
    trans_mode(220);
    rect(bmp, x, y, x+TILE_W, y+TILE_H, makecol(255, 255, 255));
    solid_mode();
//   }
//   else {
//     trans_mode(255, 255, 255, 200);
//     rect(bmp, x, y, x+TILE_W, y+TILE_H, 0);
//     solid_mode();
//   }
}
