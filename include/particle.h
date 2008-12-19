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
