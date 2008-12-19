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
