#ifndef MEDIA_HPP
#define MEDIA_HPP

#include "ids.h"

#define TAIL_FRAMES 26

#define MEDIA_BITMAP(n)		(reinterpret_cast<BITMAP *>(media[n].dat))
#define MEDIA_PALETTE(n)	(reinterpret_cast<RGB *>(media[n].dat))


extern DATAFILE *media;

extern BITMAP *devil_stand_bmp;
extern BITMAP *devil_left_shot1_bmp;
extern BITMAP *devil_left_shot2_bmp;
extern BITMAP *devil_left_shot3_bmp;
extern BITMAP *devil_right_shot1_bmp;
extern BITMAP *devil_right_shot2_bmp;
extern BITMAP *devil_right_shot3_bmp;
extern BITMAP *devil_right_bmp;
extern BITMAP *devil_up_bmp;
extern BITMAP *devil_down_bmp;
extern BITMAP *devil_stop_bmp;
extern BITMAP *devil_wait_bmp;


bool load_media();


#endif // MEDIA_HPP
