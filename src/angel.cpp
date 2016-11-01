// Defender Of Nothing
// Copyright (C) 2007 by David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include <allegro.h>
#include <assert.h>
#include "angel.h"
#include "game.h"
#include "gameplay.h"
#include "level.h"
#include "util.h"
#include "media.h"
#include "person.h"
#include "sprite.h"
#include "scorer.h"

#define WING_CYCLE_DURATION	(BPS*1.0)

Angel::Angel(vector2d pos)
{
  m_pos = pos;
  m_wing_time = GAME_T;
  m_hit_num = rand_range(0, 2);
  m_target = NULL;

  set_state(INTRO_ANGEL);
}

Angel::~Angel()
{
}

void Angel::update()
{
  switch (m_state) {

    case INTRO_ANGEL:
      m_pos.y += 2.0 / BPS;
      if (m_pos.y > 1.0) {
	set_state(FLOATING_ANGEL);
	m_energy = rand_range(0, 5) == 0 ? level()->get_max_angel_energy():
					   level()->get_min_angel_energy();
	m_vel = vector2d(0, 0);
      }
      break;

    case FLOATING_ANGEL: {
      int option = rand_range(0, 10);
      switch (option) {
	case 0:
	case 1:
	case 2:	{		// fly anywhere
	  double angle = rand_range(0.0, AL_PI);
	  double length = rand_range(2.0, 8.0);
	  set_state(FLYTOANYWHERE_ANGEL);
	  m_vel = vector2d(length*cos(angle),
			   length*sin(angle));
	  m_right = (m_vel.x >= 0.0);
	  break;
	}
	case 3:
	case 4:
	case 5:	{		// fly to one person
	  Person *target = find_target_person();
	  if (target != NULL) {
	    set_state(FLYTOPERSON_ANGEL);
	    m_target = target;
	    m_target->set_abductor(this);

	    m_vel = calculate_vel_to_abduct();
	    m_right = (m_vel.x >= 0.0);
	  }
	  break;
	}
	case 6:	{		// change direction, but do nothing
	  m_right = !m_right;
	  break;
	}
	default:
	  // continue floating
	  break;
      }
      break;
    }

    case FLYTOANYWHERE_ANGEL:
      break;

    case FLYTOPERSON_ANGEL:
      m_vel = calculate_vel_to_abduct();
      break;

    case HIT_ANGEL:
      m_pos += m_vel / BPS;
      if (GAME_T - m_state_time > BPS*0.1) {
	if (m_energy < 0.0) {
	  GAMEPLAY->get_scorer()->one_to_killed_angel();
	  burn();
	  kill();
	}
	else {
	  set_state(FLOATING_ANGEL);
	  m_vel = vector2d(0, 0);
	}
      }
      break;

    case FLYUP_ANGEL:
      if (GAME_T - m_state_time > BPS) {
	set_state(FLOATING_ANGEL);
	m_vel = vector2d(0.0, 0.0);
      }
      break;

    case FIGHTING_ANGEL:
      break;

    case ABDUCTING_ANGEL:
      m_vel = vector2d(0.0, -2);
      break;

    case DEAD_ANGEL:
      break;
  }

  m_pos += m_vel / BPS;

  if (m_pos.x < 0.5) {
    if (!m_right) m_right = true;
    m_pos.x = 0.5;
    m_vel.x = m_vel.x < 0.0 ? -m_vel.x: 2.0;
  }
  else if (m_pos.x > level()->get_w()-0.5) {
    if (m_right) m_right = false;
    m_pos.x = level()->get_w()-0.5;
    m_vel.x = m_vel.x > 0.0 ? -m_vel.x: -2.0;
  }

  if (m_pos.y > level()->get_h()-2.0) {
    m_pos.y = level()->get_h()-2.0;

    if (m_state == FLYTOANYWHERE_ANGEL) {
      set_state(FLYUP_ANGEL);
      m_vel = vector2d(rand_range(-2, 2), -8);
    }
  }

  // can this angel abduct the target person?
  if (m_target != NULL) {
    double d = level()->get_max_distance_to_abduct();
    vector2d v = ((m_target->get_pos() - vector2d(0, -2.0)) - m_pos);

    if (ABS(v.x) < 1.5 && v.magnitude() < d) {
      if (m_state != ABDUCTING_ANGEL) {
	set_state(ABDUCTING_ANGEL);
	m_target->catch_person();
      }
    }

    if (m_target->get_pos().y < 0.0) {
      m_target->kill();		// kill this person
      set_state(FLOATING_ANGEL);

      GAMEPLAY->get_scorer()->one_to_heaven();
    }
  }

//   AVOIDFIRE_ANGEL,
//   FIGHTING_ANGEL,
//   ABDUCTING_ANGEL,
//   DEAD_ANGEL
}

void Angel::draw(BITMAP *bmp)
{
  Sprite *sprite, *wings;
  get_sprites(sprite, wings);
  if (sprite != NULL) {
    if (wings != NULL) {
      wings->draw(bmp);
      delete wings;
    }
    sprite->draw(bmp);
    delete sprite;
  }

  if (m_state == ABDUCTING_ANGEL &&
      m_target != NULL) {
    int x, y, u, v;

    level()->to_screen(m_pos, x, y);
    level()->to_screen(m_target->get_pos(), u, v);

    y -= 8;

    trans_mode(64);
//     line(bmp, x, y, u, v, makecol(0, 0, 255));
//     circlefill(bmp, u, v, 20, makecol(0, 0, 255));
    vector2d w = vector2d(u, v).get_normal().normalize();
    triangle(bmp,
	     x, y,
	     u+4*w.x,
	     v+4*w.y,
	     u-4*w.x,
	     v-4*w.y,
	     makecol(0, 0, 255));
    solid_mode();
  }
}

bool Angel::is_dead()
{
  return m_state == DEAD_ANGEL;
}

bool Angel::is_hittable()
{
  return
    m_state == FLOATING_ANGEL ||
    m_state == FLYTOANYWHERE_ANGEL ||
    m_state == FLYTOPERSON_ANGEL ||
    m_state == FLYUP_ANGEL ||
    m_state == AVOIDFIRE_ANGEL ||
    m_state == FIGHTING_ANGEL ||
    m_state == ABDUCTING_ANGEL;
}

void Angel::hit(vector2d vel, double energy)
{
  if (m_state == ABDUCTING_ANGEL && m_target != NULL) {
    m_target->throw_person();
    m_target = NULL;
  }

  set_state(HIT_ANGEL);
  m_hit_num = (m_hit_num+1) % 3;
  m_energy -= energy;
  m_vel = vel.normalize() * 9.0;
  m_right = vel.x < 0.0 ? true: false; // it's inverted
}

void Angel::kill()
{
  if (m_target != NULL) {
    m_target->fire_event_angel_die(this);
    m_target = NULL;
  }
  set_state(DEAD_ANGEL);
}

void Angel::fire_event_person_die(Person *person)
{
  // did the target of this angel die?
  if (m_target == person) {
    m_target = NULL;

    if (m_state == FLYTOPERSON_ANGEL ||
	m_state == ABDUCTING_ANGEL)
      set_state(FLYTOANYWHERE_ANGEL);
  }
}

vector2d Angel::get_pos() const
{
  return m_pos;
}

vector2d Angel::get_vel() const
{
  return m_vel;
}

double Angel::get_energy() const
{
  return m_energy;
}

void Angel::get_sprites(Sprite *&sprite, Sprite *&wings_sprite)
{
  BITMAP *body_bmp = NULL;
  BITMAP *wings_bmp = NULL;
  int x, y, u, v;

  sprite = NULL;
  wings_sprite = NULL;

  level()->to_screen(m_pos, x, y);

  switch (m_state) {

    case INTRO_ANGEL:
      body_bmp = angels_fly_down_bmp;
      break;

    case FLOATING_ANGEL:
      body_bmp = angels_floating_bmp;
      break;

    case FLYTOANYWHERE_ANGEL:
    case FLYTOPERSON_ANGEL: {
      if (m_vel.y > 0.5)
	body_bmp = angels_fly_down_bmp;
      else if (m_vel.y < -0.5)
	body_bmp = angels_fly_up_bmp;
      else
	body_bmp = angels_floating_bmp;
      break;
    }

    case FLYUP_ANGEL:
      body_bmp = angels_fly_up_bmp;
      break;

    case FIGHTING_ANGEL:
      break;

    case ABDUCTING_ANGEL:
      body_bmp = angels_fly_shoot_down_bmp;
      break;

    case HIT_ANGEL:
      body_bmp = m_hit_num == 0 ?
	(angels_fly_hit1_bmp):
        (m_hit_num == 1 ? angels_fly_hit2_bmp:
			  angels_fly_hit3_bmp);
      break;

    case DEAD_ANGEL:
      break;
  }

  if (body_bmp == NULL)
    return;

  x = x - body_bmp->w/2;
  y = y - body_bmp->h+1;
  sprite = new Sprite(body_bmp, x, y, !m_right, 0);

  // wings
  int anilength = (WINGS11_PCX - WINGS00_PCX + 1);
  double t = (GAME_T - m_wing_time) / WING_CYCLE_DURATION;
  int c = static_cast<int>(t * anilength) % anilength;

  wings_bmp = MEDIA_BITMAP(WINGS00_PCX+c);

  u = sprite->x + sprite->flip_x(12) + (sprite->h_flip ? -wings_bmp->w+38: -38);
  v = sprite->y + 15 - 24;
  wings_sprite = new Sprite(wings_bmp, u, v, sprite->h_flip);
}

void Angel::set_state(AngelState state)
{
  m_state = state;
  m_state_time = GAME_T;
}

void Angel::burn()
{
  Sprite *sprite, *wings;
  get_sprites(sprite, wings);
  if (sprite != NULL) {
    if (wings != NULL) {
      GAMEPLAY->add_particles_from_sprite(wings,
					  vector2d(0.0, -2.0),
					  vector2d(0.0, -0.1),
					  vector2d(0.5, 0.0),
					  vector2d(4.0, 0.0),
					  BPS/4, BPS*3/4,
					  makecol(128, 128, 128));
      delete wings;
    }
    GAMEPLAY->add_particles_from_sprite(sprite,
					vector2d(0.0, -2.0),
					vector2d(0.0, -0.1),
					vector2d(2.5, 0.0),
					vector2d(8.0, 0.0),
					BPS/4, BPS*3/4,
					makecol(128, 128, 128));
    delete sprite;
  }
}

Person *Angel::find_target_person()
{
  Person *target = NULL;
  double target_dist = 1000.0;

  const ObjectList& people = GAMEPLAY->get_people();
  ObjectList::const_iterator it;
  for (it = people.begin(); it != people.end(); ++it) {
    Person *person = static_cast<Person *>(*it);
    if (person->get_abductor() == NULL && !person->is_dead()) {
      double dist = (person->get_pos() - m_pos).magnitude();

      if (target_dist > dist) {
	target_dist = dist;
	target = person;
      }
    }
  }

  return target;
}

vector2d Angel::calculate_vel_to_abduct()
{
  assert(m_target != NULL);

  double vel = rand_range(3.0, 4.0);
  return vel * ((m_target->get_pos() - vector2d(0, -2.0)) - m_pos).normalize();
}

Level *Angel::level()
{
  return GAMEPLAY->get_level();
}
