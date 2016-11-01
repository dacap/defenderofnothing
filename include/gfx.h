// Defender Of Nothing
// Copyright (C) 2007 by David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#ifndef GFX_H_INCLUDED
#define GFX_H_INCLUDED


// pixels of the screen
#define GFX_W		_gfx_w	// 360 or 320
#define GFX_H		240
#define GFX_CX		(GFX_W/2)
#define GFX_CY		(GFX_H/2)


extern int _gfx_w;

extern bool gfx_switched;
extern bool gfx_widescreen;
extern bool gfx_fullscreen;


bool setup_gfx();
void switch_widescreen();

void make_screenshot(BITMAP *bmp);
void trans_mode(int a);
void lit_mode(int r, int g, int b);

int blend_color(int from, int to, double t);

void draw_text(BITMAP *bmp, int x, int y, int color, const char *format, ...);
void draw_text_r(BITMAP *bmp, int x, int y, int color, const char *format, ...);
void draw_text(BITMAP *bmp, int x, int y, int w, int h, int color, const char *format, ...);

void draw_bg(BITMAP *bmp, BITMAP *bg, int x, int y);


#endif // GFX_H_INCLUDED
