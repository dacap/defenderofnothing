#include <allegro.h>
#include "particle.hpp"
#include "util.hpp"
#include "level.hpp"
#include "game.hpp"
#include "gameplay.hpp"
#include "angel.hpp"


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


Fire::Fire(vector2d pos, vector2d vel, int size)
  : Particle(pos, vel, vector2d(0, 0), BPS*60)
  , m_size(size)
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

  if (m_pos.x*TILE_W+m_size < 0 || m_pos.x*TILE_W-m_size >= level->get_w()*TILE_W ||
      m_pos.y*TILE_H+m_size < 0 || m_pos.y*TILE_H-m_size >= level->get_h()*TILE_H) {
    kill();
  }

  // hit an angel
  ObjectList angels = gameplay->get_angels();
  ObjectList::const_iterator it;
  for (it = angels.begin(); it != angels.end(); ++it) {
    Angel *angel = static_cast<Angel *>(*it);
    if ((angel->get_pos() - m_pos).magnitude() < 1.0) {
//       GAMEPLAY->add_particles(PixelParticle::generate_from_bitmap());
      angel->kill();
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
