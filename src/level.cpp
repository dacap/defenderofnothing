#include <cmath>
#include <allegro.h>
#include "level.hpp"
#include "gfx.hpp"
#include "util.hpp"
#include "media.hpp"
#include "game.hpp"
#include "gameplay.hpp"
#include "angel.hpp"


#define LOOK_AT_DURATION  (BPS*2)


Level::Level(int num)
{
  m_num = num;
  m_level_angel_interval = 0.5;
  m_last_angel_time = 0;
  m_look_time = 0;

  set_tiles_size(30, 20);
  set_scroll(get_start_pos() - vector2d(GFX_CX, GFX_CY));

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

  if (m_last_angel_time == 0) {
//     if (rand_range(0, 4) == 0) {
      m_last_angel_time = GAME_T;

      GAMEPLAY->add_angel(new Angel(get_random_pos_for_angel()));
//     }
  }
  else if (GAME_T - m_last_angel_time > m_level_angel_interval * BPS) {
    m_last_angel_time = 0;
  }
}


void Level::draw(BITMAP *bmp)
{
//   textprintf(bmp, font, 0, 8, makecol(255, 255, 255), "%02fx%02f", m_scroll.x, m_scroll.y);

  int i, j, x, y, outx, outy;
  int x1, y1, x2, y2;

  get_first_visible_tile(x1, y1);

  x2 = x1 + LEVEL_W;
  y2 = y1 + LEVEL_H;

  outy = -std::fmod(std::floor(m_scroll.y), TILE_H);
  for (y=y1; y<=y2; ++y) {
    outx = -std::fmod(std::floor(m_scroll.x), TILE_W);
    for (x=x1; x<=x2; ++x) {
      Tile bg = m_tiles_bg[y*m_h+x];
      Tile fg = m_tiles_fg[y*m_w+x];

      Level::draw_tile(bmp, bg, outx, outy);
      Level::draw_tile(bmp, fg, outx, outy);
      outx += TILE_W;
    }
    outy += TILE_W;
  }
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
  return vector2d(m_w/2.0, m_h/2.0);
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
  x = - m_scroll.x + pos.x*TILE_W;
  y = - m_scroll.y + pos.y*TILE_H;
}


bool Level::touch_floor(vector2d pos)
{
  int x = static_cast<int>(std::floor(pos.x));
  int y = static_cast<int>(std::floor(pos.y));
  return (x >= 0 && x < m_w &&
	  y >= 0 && y < m_w &&
	  Level::tile_is_floor(m_tiles_fg[y*m_w+x]));
}


bool Level::touch_wall(vector2d pos)
{
  int x = static_cast<int>(pos.x);
  int y = static_cast<int>(pos.y);
  return (x >= 0 && x < m_w &&
	  y >= 0 && y < m_w &&
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
