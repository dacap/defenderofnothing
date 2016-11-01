// Defender Of Nothing
// Copyright (C) 2007 by David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#ifndef SCORER_H_INCLUDED
#define SCORER_H_INCLUDED


#include "object.h"
#include "vector2d.h"


class Scorer : public Object
{
  int m_people_in_heaven;
  int m_people_in_hell;
  int m_people_in_earth;

  int m_killed_angels;
  int m_total_angels;

public:
  Scorer();
  virtual ~Scorer();

  virtual void update();
  virtual void draw(BITMAP *bmp);

  void reset(int angels, int people);

  void one_to_heaven();
  void one_to_hell();
  void one_to_killed_angel();

private:
  void draw_minimap(BITMAP *bmp, int x, int y, int w, int h);
  void to_minimap(vector2d pos, int x, int y, int w, int h, int &u, int &v);
};

#endif
