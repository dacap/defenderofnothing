#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <vector>

#include "object.hpp"
#include "vector2d.hpp"


// 16 [pixels] = 1.0 [logical unit]
#define TILE_W		16
#define TILE_H		16

// in logical units of the screen
#define LEVEL_W		((GFX_W / TILE_W) + ((GFX_W % TILE_W) == 0 ? 0: 1))
#define LEVEL_H		((GFX_H / TILE_H) + ((GFX_H % TILE_H) == 0 ? 0: 1))
#define LEVEL_CX	(LEVEL_W/2.0)
#define LEVEL_CY	(LEVEL_H/2.0)


enum Tile {
  TILE_NOTHING,
  TILE_FLOOR_EXTERN,
  TILE_FLOOR_INTERN,

  TILE_BUILDING1_W,
  TILE_BUILDING1_N,
  TILE_BUILDING1_S,
  TILE_BUILDING1_E,
  TILE_BUILDING1_NW,
  TILE_BUILDING1_NE,
  TILE_BUILDING1_SW,
  TILE_BUILDING1_SE,
  TILE_BUILDING1_O,

  TILE_BUILDING2_W,
  TILE_BUILDING2_N,
  TILE_BUILDING2_S,
  TILE_BUILDING2_E,
  TILE_BUILDING2_NW,
  TILE_BUILDING2_NE,
  TILE_BUILDING2_SW,
  TILE_BUILDING2_SE,
  TILE_BUILDING2_O,

  TILE_BUILDING3_W,
  TILE_BUILDING3_N,
  TILE_BUILDING3_S,
  TILE_BUILDING3_E,
  TILE_BUILDING3_NW,
  TILE_BUILDING3_NE,
  TILE_BUILDING3_SW,
  TILE_BUILDING3_SE,
  TILE_BUILDING3_O,
};


// a position of the level is modeled in this way:
//   (0,0) is the origin of the first tile
//   (1,0) is the origin of the next tile
//   (m_w-0.01, m_h-0.01) is the right-bottom corner of the last tile
//   all the level is repeated horizontally
class Level : public Object
{
  // general level information
  int m_num;

  // level tiles
  int m_w, m_h;
  std::vector<Tile> m_tiles_bg;
  std::vector<Tile> m_tiles_fg;

//   std::vector<Object *> m_objects;

  // the scroll is the left-top center of the screen in pixels
  vector2d m_scroll;

  // look_at information
  vector2d m_look_from;
  vector2d m_look_to;
  int m_look_time;

  // angels generator
  double m_level_angel_interval; // in seconds
  int m_last_angel_time;

public:
  Level(int num);
  virtual ~Level();

  virtual void update();
  virtual void draw(BITMAP *bmp);

  int get_w();
  int get_h();

  vector2d get_start_pos();
  vector2d get_random_pos_for_people();
  vector2d get_random_pos_for_angel();
  
  void look_at(vector2d center);
  void to_screen(vector2d pos, int &x, int &y);

  bool touch_floor(vector2d pos);
  bool touch_wall(vector2d pos);
  vector2d get_stand_point(vector2d pos);

private:
  void set_scroll(vector2d pos);
  void get_first_visible_tile(int &x, int &y);
  void set_tiles_size(int w, int h);
  static void draw_tile(BITMAP *bmp, Tile tile, int x, int y);
  static bool tile_is_floor(Tile tile);
  static bool tile_is_touchable(Tile tile);
};


#endif // LEVEL_HPP
