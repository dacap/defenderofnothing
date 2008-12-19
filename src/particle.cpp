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

#include <allegro.h>
#include "particle.h"
#include "util.h"
#include "level.h"
#include "game.h"
#include "gameplay.h"
#include "angel.h"
#include "sprite.h"

Particle::Particle(vector2d pos, vector2d vel, vector2d acc,
		   int lifetime)
  : m_pos(pos)
  , m_vel(vel)
  , m_acc(acc)
  , m_lifetime(lifetime)
{
}

Particle::~Particle()
{
}

void Particle::update()
{
  m_vel += m_acc / BPS;
  m_pos += m_vel / BPS;
  --m_lifetime;
}

bool Particle::is_dead()
{
  return (m_lifetime <= 0);
}

void Particle::kill()
{
  m_lifetime = 0;
}

//////////////////////////////////////////////////////////////////////

PixelParticle::PixelParticle(vector2d pos, vector2d vel, vector2d acc,
			     int lifetime, int color_from, int color_to)
  : Particle(pos, vel, acc, lifetime)
  , m_start_time(GAME_T)
  , m_start_lifetime(lifetime)
  , m_color_from(color_from)
  , m_color_to(color_to)
{
}

PixelParticle::~PixelParticle()
{
}

void PixelParticle::draw(BITMAP *bmp)
{
  int x, y;

  double t = 1.0 * (GAME_T - m_start_time) / m_start_lifetime;
  t = MIN(t, 1.0);

  GAMEPLAY->get_level()->to_screen(m_pos, x, y);
  putpixel(bmp, x, y, blend_color(m_color_from, m_color_to, t));
}

//////////////////////////////////////////////////////////////////////

Fire::Fire(vector2d pos, vector2d vel, int size, double energy)
  : Particle(pos, vel, vector2d(0, 0), BPS*60)
  , m_size(size)
  , m_energy(energy)
{
}

Fire::~Fire()
{
}

void Fire::update()
{
  GamePlay *gameplay = GAMEPLAY;

  Particle::update();

  // create fire tail in each "game-tick"
  for (int c=0; c<4; ++c) {
    double vx = m_vel.x > 0.0 ? rand_range(-5, -1):
				rand_range(1, 2);

    gameplay->add_particle
      (new PixelParticle(m_pos + vector2d(0.0, rand_range(-m_size/TILE_H, m_size/TILE_H)),
			 vector2d(vx, rand_range(-1.0, 1.0)),
			 vector2d(0, 0),
			 rand_range(BPS/4, BPS*3/4),
			 makecol(255, 255, 0),
			 makecol(255, 0, 0)));
  }

  // the fire is out of the scenary
  Level *level = gameplay->get_level();

  if (m_pos.x*TILE_W+m_size < 0 || m_pos.x*TILE_W-m_size >= level->get_w()*TILE_W)
    kill();

  // hit an angel
  int x, y;
  level->to_screen(m_pos, x, y);

  const ObjectList& angels = gameplay->get_angels();
  ObjectList::const_iterator it;
  for (it = angels.begin(); it != angels.end(); ++it) {
    Angel *angel = static_cast<Angel *>(*it);

    if (angel->is_hittable()) {
      Sprite *sprite, *wings;
      angel->get_sprites(sprite, wings);
      if (sprite != NULL) {
	if (sprite->collision(x-m_size, y) ||
	    sprite->collision(x+m_size, y) ||
	    sprite->collision(x, y-m_size) ||
	    sprite->collision(x, y-m_size)) {
	  double angel_energy = angel->get_energy();

	  angel->hit(m_vel, m_energy);
	  this->hit(angel_energy);
	}

	if (wings != NULL)
	  delete wings;
	delete sprite;

	if (is_dead())
	  break;
      }
    }
  }
}

void Fire::draw(BITMAP *bmp)
{
  int x, y;

  GAMEPLAY->get_level()->to_screen(m_pos, x, y);

  trans_mode(128);
  circlefill(bmp, x, y, m_size, makecol(255, 0, 0));
  solid_mode();

  x += rand_range(-1, 1);
  y += rand_range(-1, 1);
  circlefill(bmp, x, y, m_size-1, makecol(255, 255, 0));
}

void Fire::hit(double energy)
{
  m_energy -= energy;
  if (m_energy < 0.0) {
    impact();
    kill();
  }
}

void Fire::impact()
{
  for (int c=0; c<40; ++c) {
    double angle = rand_range(-AL_PI, AL_PI);
    double vel = rand_range(7.0, 8.2);

    GAMEPLAY->add_particle
      (new PixelParticle(m_pos,
			 vector2d(vel*cos(angle),
				  vel*sin(angle)),
			 vector2d(0, 0),
			 rand_range(BPS/4, BPS/2),
			 makecol(128, 0, 0),
			 makecol(255, 255, 0)));
  }
}
