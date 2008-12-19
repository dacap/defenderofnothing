#ifndef MEDIA_HPP
#define MEDIA_HPP

#include "ids.h"

#define TAIL_FRAMES 26

#define MEDIA_BITMAP(n)		(reinterpret_cast<BITMAP *>(media[n].dat))
#define MEDIA_PALETTE(n)	(reinterpret_cast<RGB *>(media[n].dat))


extern DATAFILE *media;

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


#endif // MEDIA_HPP
