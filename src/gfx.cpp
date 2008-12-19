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

#include <allegro.h>
#include "gfx.h"
#include "util.h"
#include "media.h"

int _gfx_w = 0;

bool gfx_switched = false;
bool gfx_widescreen = true;
bool gfx_fullscreen = false;

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

  int gfx_mode = gfx_fullscreen ? GFX_AUTODETECT_FULLSCREEN:
				  GFX_AUTODETECT_WINDOWED;

  if (gfx_widescreen) {
    if (set_gfx_mode(gfx_mode, w, h, 0, 0) == 0 ||
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, w, h, 0, 0) == 0 ||
	set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, w, h, 0, 0) == 0) {
      _gfx_w = 360;
      return true;
    }
  }

  if (set_gfx_mode(gfx_mode, 640, 480, 0, 0) < 0 ||
      set_gfx_mode(gfx_mode, 320, 240, 0, 0) < 0 ||
      set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0) < 0) {
    // error, no graphics mode
    return false;
  }

  _gfx_w = 320;

  // success
  return true;
}

void switch_widescreen()
{
  if (!setup_gfx() != 0) {
    allegro_message("Unable to setup the graphics mode\n");
    exit(1);
  }
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
  int r = getr(from) + static_cast<int>((getr(to) - getr(from)) * t);
  int g = getg(from) + static_cast<int>((getg(to) - getg(from)) * t);
  int b = getb(from) + static_cast<int>((getb(to) - getb(from)) * t);
  int a = geta(from) + static_cast<int>((geta(to) - geta(from)) * t);
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

  textout_ex(bmp, gamefont, buf, x, y+1, blend_color(color, makecol(0, 0, 0), 0.5), -1);
  textout_ex(bmp, gamefont, buf, x, y, color, -1);
}

void draw_text_r(BITMAP *bmp, int x, int y, int color, const char *format, ...)
{
  char buf[512];
  va_list ap;

  va_start(ap, format);
  uvszprintf(buf, sizeof(buf), format, ap);
  va_end(ap);

  textout_right_ex(bmp, gamefont, buf, x, y+1, blend_color(color, makecol(0, 0, 0), 0.5), -1);
  textout_right_ex(bmp, gamefont, buf, x, y, color, -1);
}

void draw_text(BITMAP *bmp, int x, int y, int w, int h, int color, const char *format, ...)
{
  if (w > 0 && h > 0) {
    char buf[512];
    va_list ap;

    va_start(ap, format);
    uvszprintf(buf, sizeof(buf), format, ap);
    va_end(ap);

    BITMAP *tmp = create_bitmap(text_length(gamefont, buf),
				text_height(gamefont)+1);

    clear_to_color(tmp, bitmap_mask_color(tmp));
    textout_ex(tmp, gamefont, buf, 0, 1, blend_color(color, makecol(0, 0, 0), 0.5), -1);
    textout_ex(tmp, gamefont, buf, 0, 0, color, -1);

    masked_stretch_blit(tmp, bmp, 0, 0, tmp->w, tmp->h, x, y, w, h);
    destroy_bitmap(tmp);
  }
}

void draw_bg(BITMAP *bmp, BITMAP *bg, int x, int y)
{
  if (x < 0) {
    do {
      x += bg->w;
    } while (x < 0);
    x -= bg->w;
  }
  masked_blit(bg, bmp, 0, 0, x, y, bg->w, bg->h);

  x += bg->w;
  masked_blit(bg, bmp, 0, 0, x, y, bg->w, bg->h);
}
