// Defender Of Nothing
// Copyright (C) 2007 by David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include <allegro.h>
#include "media.h"
#include "util.h"

DATAFILE *media;

FONT *gamefont;

BITMAP *devil_floating_bmp;
BITMAP *devil_floating_left_shot1_bmp;
BITMAP *devil_floating_left_shot2_bmp;
BITMAP *devil_floating_left_shot3_bmp;
BITMAP *devil_floating_right_shot1_bmp;
BITMAP *devil_floating_right_shot2_bmp;
BITMAP *devil_floating_right_shot3_bmp;
BITMAP *devil_floating_big_shot1_bmp;
BITMAP *devil_floating_big_shot2_bmp;
BITMAP *devil_floating_big_shot3_bmp;
BITMAP *devil_stand_bmp;
BITMAP *devil_stand_left_shot1_bmp;
BITMAP *devil_stand_left_shot2_bmp;
BITMAP *devil_stand_left_shot3_bmp;
BITMAP *devil_stand_right_shot1_bmp;
BITMAP *devil_stand_right_shot2_bmp;
BITMAP *devil_stand_right_shot3_bmp;
BITMAP *devil_stand_big_shot1_bmp;
BITMAP *devil_stand_big_shot2_bmp;
BITMAP *devil_stand_big_shot3_bmp;
BITMAP *devil_fly_right_bmp;
BITMAP *devil_fly_up_bmp;
BITMAP *devil_fly_down_bmp;
BITMAP *devil_fly_stop_bmp;
BITMAP *devil_floating_wait_bmp;
BITMAP *devil_floating_hit_bmp;
BITMAP *devil_landing_bmp;

BITMAP *angels_floating_bmp;
BITMAP *angels_fly_down_bmp;
BITMAP *angels_fly_up_bmp;
BITMAP *angels_fly_cover_bmp;
BITMAP *angels_fly_hit1_bmp;
BITMAP *angels_fly_hit2_bmp;
BITMAP *angels_fly_hit3_bmp;
BITMAP *angels_fly_kick_bmp;
BITMAP *angels_fly_shoot_right_bmp;
BITMAP *angels_fly_shoot_diagonal_bmp;
BITMAP *angels_fly_shoot_down_bmp;

bool load_media()
{
  std::string filename = redir("media/defnot.dat");

  // load the palette of color
  media = load_datafile_object(filename.c_str(), "PALETTE_PCX");
  if (!media)
    return false;
  set_palette(reinterpret_cast<RGB *>(media->dat));

  // load all data
  media = load_datafile(filename.c_str());
  if (!media)
    return false;

  gamefont = MEDIA_FONT(FONT_PCX);

  // trim devil bitmap

#undef SUB
#define SUB(x,y)						\
  create_sub_bitmap(MEDIA_BITMAP(DEVIL_PCX), x*32, y*36, 32, 36)

  devil_floating_bmp   	       	 = SUB(0, 0);
  devil_floating_left_shot1_bmp	 = SUB(1, 0);
  devil_floating_left_shot2_bmp	 = SUB(2, 0);
  devil_floating_left_shot3_bmp	 = SUB(3, 0);
  devil_floating_right_shot1_bmp = SUB(4, 0);
  devil_floating_right_shot2_bmp = SUB(5, 0);
  devil_floating_right_shot3_bmp = SUB(6, 0);
  devil_floating_big_shot1_bmp 	 = SUB(7, 0);
  devil_floating_big_shot2_bmp 	 = SUB(8, 0);
  devil_floating_big_shot3_bmp 	 = SUB(9, 0);
  devil_stand_bmp      	       	 = SUB(0, 1);
  devil_stand_left_shot1_bmp   	 = SUB(1, 1);
  devil_stand_left_shot2_bmp   	 = SUB(2, 1);
  devil_stand_left_shot3_bmp   	 = SUB(3, 1);
  devil_stand_right_shot1_bmp  	 = SUB(4, 1);
  devil_stand_right_shot2_bmp  	 = SUB(5, 1);
  devil_stand_right_shot3_bmp  	 = SUB(6, 1);
  devil_stand_big_shot1_bmp    	 = SUB(7, 1);
  devil_stand_big_shot2_bmp    	 = SUB(8, 1);
  devil_stand_big_shot3_bmp    	 = SUB(9, 1);
  devil_fly_right_bmp  	       	 = SUB(0, 2);
  devil_fly_up_bmp     	       	 = SUB(1, 2);
  devil_fly_down_bmp   	       	 = SUB(2, 2);
  devil_fly_stop_bmp   	       	 = SUB(3, 2);
  devil_floating_wait_bmp        = SUB(4, 2);
  devil_floating_hit_bmp         = SUB(5, 2);
  devil_landing_bmp    	       	 = SUB(6, 2);

  // trim angels bitmap

#undef SUB
#define SUB(x,y)							\
  create_sub_bitmap(MEDIA_BITMAP(ANGELS_PCX), x*32, y*36, 32, 36)

  angels_floating_bmp  	       	 = SUB(0, 0);
  angels_fly_down_bmp  	       	 = SUB(1, 0);
  angels_fly_up_bmp    	       	 = SUB(2, 0);
  angels_fly_cover_bmp 	       	 = SUB(3, 0);
  angels_fly_hit1_bmp  	       	 = SUB(4, 0);
  angels_fly_hit2_bmp  	       	 = SUB(5, 0);
  angels_fly_hit3_bmp  	       	 = SUB(6, 0);
  angels_fly_kick_bmp  	       	 = SUB(7, 0);
  angels_fly_shoot_right_bmp   	 = SUB(8, 0);
  angels_fly_shoot_diagonal_bmp	 = SUB(9, 0);
  angels_fly_shoot_down_bmp    	 = SUB(10, 0);

  return true;
}
