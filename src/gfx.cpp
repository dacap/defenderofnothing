#include <allegro.h>
#include "gfx.hpp"


// #define TRANS_TABLES	5


// static COLOR_MAP *trans_table[TRANS_TABLES];


// changes to some widescreen or fullscreen mode
bool setup_gfx()
{
  int dsk_w, dsk_h;
  int w = 720;
  int h = 480;
  int bpp = 16;

  if (get_desktop_resolution(&dsk_w, &dsk_h) == 0) {
    bpp = desktop_color_depth();
    if (bpp == 8)
      bpp = 16;

  if (dsk_w >= 720 && dsk_h >= 480) {
      w = 720;
      h = 480;
    }
    else {
      w = 360;
      h = 240;
    }
  }

  set_color_depth(bpp);

  // widescreen window
  if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, w, h, 0, 0) < 0) {
    // fullscreen
    if (set_gfx_mode(GFX_AUTODETECT, 320, 240, 0, 0) < 0)
      if (set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0) < 0) {
	// error, no graphics mode
	return false;
      }
  }

  // success
  return true;
}


// void create_color_tables()
// {
//   PALETTE pal;
//   get_palette(pal);

//   // create color maps
//   rgb_map = new RGB_MAP;
//   create_rgb_table(rgb_map, pal, NULL);

//   for (int c=0; c<TRANS_TABLES; ++c) {
//     int g = c;

//     trans_table[c] = new COLOR_MAP;
//     create_trans_table(trans_table[c], pal, g, g, g, NULL);
//   }
// }


void make_screenshot(BITMAP *bmp)
{
  char exe[1024], name[1024], buf[1024];

  get_executable_name(exe, sizeof(exe));

  for (int c=0; c<10000; ++c) {
    sprintf(name, "shot%04d.bmp", c);
    replace_filename(buf, exe, name, sizeof(buf));
    if (!exists(buf)) {
      PALETTE pal;
      get_palette(pal);
      save_bitmap(buf, bmp, pal);
      break;
    }
  }
}


void trans_mode(int a)
{
//   if (bitmap_color_depth(screen) > 8) {
    drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
    set_trans_blender(0, 0, 0, a);
//   }
//   else {
//     // TODO
//   }
}

void lit_mode(int r, int g, int b)
{
//   if (bitmap_color_depth(screen) > 8) {
    set_trans_blender(r, g, b, 0);
//   }
//   else {
//     // TODO
//   }
}
