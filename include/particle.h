// Defender Of Nothing
// Copyright (C) 2007 by David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#ifndef PARTICLES_H_INCLUDED
#define PARTICLES_H_INCLUDED

#include "object.h"
#include "vector2d.h"


class Level;


// a partile
class Particle : public Object
{
protected:
  int m_lifetime;
  vector2d m_pos;
  vector2d m_vel;
  vector2d m_acc;

public:

  Particle(vector2d pos, vector2d vel, vector2d acc, int lifetime);
  virtual ~Particle() = 0;

  virtual void update();
  virtual void draw(BITMAP *bmp) = 0;

  virtual bool is_dead();
  void kill();
};


class PixelParticle : public Particle
{
  int m_start_time, m_start_lifetime;
  int m_color_from, m_color_to;

public:
  PixelParticle(vector2d pos, vector2d vel, vector2d acc, int lifetime,
		int color_from, int color_to);

  virtual ~PixelParticle();

  virtual void draw(BITMAP *bmp);
};



class Fire : public Particle
{
  int m_size;
  double m_energy;

public:
  Fire(vector2d pos, vector2d vel, int size, double energy);
  virtual ~Fire();

  virtual void update();
  virtual void draw(BITMAP *bmp);

private:
  void hit(double energy);
  void impact();
};


#endif // PARTICLES_H_INCLUDED
