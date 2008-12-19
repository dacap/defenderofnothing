#include <allegro.h>
#include "gfx.hpp"
#include "util.hpp"


int _gfx_w;


// changes to some widescreen or fullscreen mode
bool setup_gfx(bool widescreen)
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

  // widescreen
  if (widescreen) {
    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, w, h, 0, 0) == 0 ||
	set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, w, h, 0, 0) == 0) {
      _gfx_w = 360;
      return true;
    }
  }

  // fullscreen
  if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0) < 0)
    if (set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0) < 0)
      if (set_gfx_mode(GFX_AUTODETECT, 320, 240, 0, 0) < 0) {
	// error, no graphics mode
	return false;
      }

  _gfx_w = 320;

  // success
  return true;
}


void switch_gfx_mode()
{
  if (_gfx_w == 360)
    setup_gfx(false);
  else
    setup_gfx(true);
}


void make_screenshot(BITMAP *bmp)
{
  std::string filename;
  char buf[1024];

  for (int c=0; c<10000; ++c) {
    sprintf(buf, "shot%04d.bmp", c);
    filename = redir(buf);
    if (!exists(filename.c_str())) {
      PALETTE pal;
      get_palette(pal);
      save_bitmap(filename.c_str(), bmp, pal);
      break;
    }
  }
}


void trans_mode(int a)
{
  drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
  set_trans_blender(0, 0, 0, a);
}


void lit_mode(int r, int g, int b)
{
  set_trans_blender(r, g, b, 0);
}


int blend_color(int from, int to, double t)
{
  int r = getr(from) + (getr(to) - getr(from)) * t;
  int g = getg(from) + (getg(to) - getg(from)) * t;
  int b = getb(from) + (getb(to) - getb(from)) * t;
  int a = geta(from) + (geta(to) - geta(from)) * t;
  return makeacol(MID(0, r, 255),
		  MID(0, g, 255),
		  MID(0, b, 255),
		  MID(0, a, 255));
}


void draw_text(BITMAP *bmp, int x, int y, int color, const char *format, ...)
{
  char buf[512];
  va_list ap;

  va_start(ap, format);
  uvszprintf(buf, sizeof(buf), format, ap);
  va_end(ap);

  textout_ex(bmp, font, buf, x, y+1, blend_color(color, makecol(0, 0, 0), 0.5), -1);
  textout_ex(bmp, font, buf, x, y, color, -1);
}
