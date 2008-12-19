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

#ifndef MEDIA_H_INCLUDED
#define MEDIA_H_INCLUDED

#include "ids.h"

#define TAIL_FRAMES 26

#define MEDIA_BITMAP(n)		(reinterpret_cast<BITMAP *>(media[n].dat))
#define MEDIA_PALETTE(n)	(reinterpret_cast<RGB *>(media[n].dat))
#define MEDIA_FONT(n)		(reinterpret_cast<FONT *>(media[n].dat))


extern DATAFILE *media;

extern FONT *gamefont;

extern BITMAP *devil_floating_bmp;
extern BITMAP *devil_floating_left_shot1_bmp;
extern BITMAP *devil_floating_left_shot2_bmp;
extern BITMAP *devil_floating_left_shot3_bmp;
extern BITMAP *devil_floating_right_shot1_bmp;
extern BITMAP *devil_floating_right_shot2_bmp;
extern BITMAP *devil_floating_right_shot3_bmp;
extern BITMAP *devil_floating_big_shot1_bmp;
extern BITMAP *devil_floating_big_shot2_bmp;
extern BITMAP *devil_floating_big_shot3_bmp;
extern BITMAP *devil_stand_bmp;
extern BITMAP *devil_stand_left_shot1_bmp;
extern BITMAP *devil_stand_left_shot2_bmp;
extern BITMAP *devil_stand_left_shot3_bmp;
extern BITMAP *devil_stand_right_shot1_bmp;
extern BITMAP *devil_stand_right_shot2_bmp;
extern BITMAP *devil_stand_right_shot3_bmp;
extern BITMAP *devil_stand_big_shot1_bmp;
extern BITMAP *devil_stand_big_shot2_bmp;
extern BITMAP *devil_stand_big_shot3_bmp;
extern BITMAP *devil_fly_right_bmp;
extern BITMAP *devil_fly_up_bmp;
extern BITMAP *devil_fly_down_bmp;
extern BITMAP *devil_fly_stop_bmp;
extern BITMAP *devil_floating_wait_bmp;
extern BITMAP *devil_floating_hit_bmp;
extern BITMAP *devil_landing_bmp;

extern BITMAP *angels_floating_bmp;
extern BITMAP *angels_fly_down_bmp;
extern BITMAP *angels_fly_up_bmp;
extern BITMAP *angels_fly_cover_bmp;
extern BITMAP *angels_fly_hit1_bmp;
extern BITMAP *angels_fly_hit2_bmp;
extern BITMAP *angels_fly_hit3_bmp;
extern BITMAP *angels_fly_kick_bmp;
extern BITMAP *angels_fly_shoot_right_bmp;
extern BITMAP *angels_fly_shoot_diagonal_bmp;
extern BITMAP *angels_fly_shoot_down_bmp;


bool load_media();


#endif // MEDIA_H_INCLUDED
