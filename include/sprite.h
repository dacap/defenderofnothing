// Defender Of Nothing
// Copyright (C) 2007 by David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED


class Sprite
{
public:
  BITMAP *bmp;
  int x, y;
  bool h_flip : 1;
  bool v_flip : 1;

  Sprite(BITMAP *bmp, int x, int y, int h_flip = false, int v_flip = false);
  virtual ~Sprite();

  void draw(BITMAP *bmp);

  int flip_x(int u);
  int flip_y(int v);

  bool collision(int u, int v);
  bool collision(Sprite &spr);
};


#endif
