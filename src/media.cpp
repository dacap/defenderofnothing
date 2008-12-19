#include <allegro.h>
#include "media.hpp"


DATAFILE *media;

BITMAP *devil_stand_bmp;
BITMAP *devil_left_shot1_bmp;
BITMAP *devil_left_shot2_bmp;
BITMAP *devil_left_shot3_bmp;
BITMAP *devil_right_shot1_bmp;
BITMAP *devil_right_shot2_bmp;
BITMAP *devil_right_shot3_bmp;
BITMAP *devil_right_bmp;
BITMAP *devil_up_bmp;
BITMAP *devil_down_bmp;
BITMAP *devil_stop_bmp;
BITMAP *devil_wait_bmp;


bool load_media()
{
  char exe[1024], buf[1024];

  get_executable_name(exe, sizeof(exe));
  replace_filename(buf, exe, "media/bedevil.dat", sizeof(buf));

  // load the palette of color
  media = load_datafile_object(buf, "PALETTE_PCX");
  if (!media)
    return false;
  set_palette(reinterpret_cast<RGB *>(media->dat));

  // load all data
  media = load_datafile(buf);
  if (!media)
    return false;

#define SUB(x,y)						\
  create_sub_bitmap(MEDIA_BITMAP(DEVIL_PCX), x*32, y*36, 32, 36)

  devil_stand_bmp       = SUB(0, 0);
  devil_left_shot1_bmp  = SUB(1, 0);
  devil_left_shot2_bmp  = SUB(2, 0);
  devil_left_shot3_bmp  = SUB(3, 0);
  devil_right_shot1_bmp = SUB(4, 0);
  devil_right_shot2_bmp = SUB(5, 0);
  devil_right_shot3_bmp = SUB(6, 0);
  devil_right_bmp       = SUB(0, 1);
  devil_up_bmp          = SUB(0, 2);
  devil_down_bmp        = SUB(0, 3);
  devil_stop_bmp        = SUB(0, 4);
  devil_wait_bmp        = SUB(1, 4);

//   create_color_tables();

  return true;
}


