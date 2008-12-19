// Defender Of Nothing
// Copyright (C) 2007 by David A. Capello
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
// * Neither the name of the Vaca nor the names of its contributors
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
#include "sprite.hpp"


Sprite::Sprite(BITMAP *bmp, int x, int y, int h_flip, int v_flip)
  : bmp(bmp)
  , x(x), y(y)
  , h_flip(h_flip)
  , v_flip(v_flip)
{
}


Sprite::~Sprite()
{
}


void Sprite::draw(BITMAP *bmp)
{
  if (h_flip) {
    if (v_flip)
      draw_sprite_vh_flip(bmp, this->bmp, x, y);
    else
      draw_sprite_h_flip(bmp, this->bmp, x, y);
  }
  else if (v_flip)
    draw_sprite_v_flip(bmp, this->bmp, x, y);
  else
    draw_sprite(bmp, this->bmp, x, y);
}


int Sprite::flip_x(int u)
{
  return !h_flip ? u: bmp->w-1-u;
}


int Sprite::flip_y(int v)
{
  return !v_flip ? v: bmp->h-1-v;
}


bool Sprite::collision(int u, int v)
{
  if (u < x || v < y || u >= x+bmp->w || v >= y+bmp->h)
    return false;
  else
    return getpixel(bmp, u-x, v-y) != bitmap_mask_color(bmp);
}


bool Sprite::collision(Sprite &spr)
{
  if (spr.x+spr.bmp->w <= x || spr.x >= x+bmp->w ||
      spr.y+spr.bmp->h <= y || spr.y >= y+bmp->h) {
    return false;
  }
  else {
    int u, v, x1, y1, x2, y2;

    x1 = MAX(x, spr.x);
    y1 = MAX(y, spr.y);
    x2 = MIN(x+bmp->w-1, spr.x+spr.bmp->w-1);
    y2 = MIN(y+bmp->h-1, spr.y+spr.bmp->h-1);

    for (v=y1; v<=y2; ++v)
      for (u=x1; u<=x2; ++u)
	if (getpixel(bmp, u-x, v-y) != bitmap_mask_color(bmp) &&
	    getpixel(spr.bmp, u-spr.x, v-spr.y) != bitmap_mask_color(bmp))
	  return true;

    return false;
  }
}


#if 0 				// a test
#include "gfx.hpp"

int main()
{
  allegro_init();
  install_timer();
  install_keyboard();
  install_joystick(JOY_TYPE_AUTODETECT);
  install_mouse();

  setup_gfx(true);

  BITMAP *buf = create_bitmap(GFX_W, GFX_H);
  BITMAP *bmp1 = create_bitmap(32, 32);
  BITMAP *bmp2 = create_bitmap(48, 48);
  clear_to_color(bmp1, bitmap_mask_color(bmp1));
  circlefill(bmp1, bmp1->w/2, bmp1->h/2, bmp1->w/2-1, makecol(255, 255, 0));
  clear_to_color(bmp2, bitmap_mask_color(bmp2));
  circlefill(bmp2, bmp2->w/2, bmp2->h/2, bmp2->w/2-1, makecol(255, 0, 0));

  Sprite spr1(bmp1, 128, 128, false, false);
  Sprite spr2(bmp2, 128, 128, false, false);

  do {
    clear(buf);
    if (key[KEY_LSHIFT]) { spr1.x = mouse_x; spr1.y = mouse_y; }
    else if (!key[KEY_LCONTROL]) { spr2.x = mouse_x; spr2.y = mouse_y; }
    spr1.draw(buf);
    spr2.draw(buf);
    textprintf_ex(buf, font, 0, 0, makecol(255,255,255), -1, "pixel: %d", spr1.collision(mouse_x, mouse_y));
    textprintf_ex(buf, font, 0, 8, makecol(255,255,255), -1, "sprte: %d", spr1.collision(spr2));
    putpixel(buf, mouse_x, mouse_y, makecol(0, 0, 255));
    stretch_blit(buf, screen, 0, 0, GFX_W, GFX_H, 0, 0, SCREEN_W, SCREEN_H);
  } while (!key[KEY_ESC]);
  
  allegro_exit();
  exit(1);
}

END_OF_MAIN();
#endif
