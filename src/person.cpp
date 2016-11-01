// Defender Of Nothing
// Copyright (C) 2007 by David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include <allegro.h>
#include <assert.h>
#include "person.h"
#include "util.h"
#include "game.h"
#include "gameplay.h"
#include "level.h"
#include "media.h"
#include "player.h"
#include "particle.h"
#include "angel.h"
#include "scorer.h"

#define CALLING_DURATION	(BPS*1.0)

enum {
  ANI_FRAME_STAND,
  ANI_FRAME_WALK1,
  ANI_FRAME_WALK2,
  ANI_FRAME_LOOK,
  ANI_FRAME_HELP1,
  ANI_FRAME_HELP2,
  ANI_FRAME_HELP3
};

Person::Person(vector2d pos)
{
  m_type = rand_range(0, 5);
  m_right = rand_range(0, 1) == 1 ? true: false;
  m_pos = pos;
  m_velx = rand_range(1.0, 2.0);
  m_toast_factor = 0.0;
  m_abductor = NULL;

  set_state(STAND_PERSON);
}

Person::~Person()
{
}

void Person::update()
{
  GamePlay *gameplay = GAMEPLAY;

  switch (m_state) {

    case STAND_PERSON:
      if (rand_range(0, 10) == 0) {
	set_state(WALKING_PERSON);
      }
      else if (rand_range(0, 20) == 0) {
	set_state(LOOKING_PERSON);
      }
      break;

    case WALKING_PERSON:
      if (m_right) {
	m_pos.x += m_velx / BPS;
	if (m_pos.x > gameplay->get_level()->get_w()-0.5) {
	  m_right = false;
	  m_pos.x = gameplay->get_level()->get_w()-0.5;
	}
      }
      else {
	m_pos.x -= m_velx / BPS;
	if (m_pos.x < 0.5) {
	  m_right = true;
	  m_pos.x = 0.5;
	}
      }

      if (rand_range(0, 100) == 0) {
	set_state(STAND_PERSON);
      }
      break;

    case LOOKING_PERSON:
      if (rand_range(0, 10) == 0) {
	set_state(CALLING_PERSON);
      }
      else if (rand_range(0, 20) == 0) {
	set_state(STAND_PERSON);
      }
      break;

    case CALLING_PERSON:
      if (GAME_T - m_state_time > CALLING_DURATION) {
	set_state(STAND_PERSON);
      }
      break;

    case ABDUCTING_PERSON:
      assert(m_abductor != NULL);
      m_pos += m_abductor->get_vel() / BPS;
      break;

    case FALLING_PERSON:
      m_pos.y += 2.0 / BPS;
      if (gameplay->get_level()->touch_floor(m_pos)) {
	m_pos = gameplay->get_level()->get_stand_point(m_pos);
	set_state(STAND_PERSON);
      }
      break;

    case DEAD_PERSON:
      // nothing to do
      break;
  }

  if (m_state != ABDUCTING_PERSON) {
    // interaction with the player....
    Player *player = gameplay->get_player();
    vector2d player_pos = player->get_pos();

    // are the player toasting us?
    double dist = (player_pos - m_pos).magnitude();
    if (dist < 2.0) {
      // let's toast this guy
      m_toast_factor += (2.0-dist)/100.0;

      // add the halo effect to the Devil
      player->inc_halo(0.05 * (2.0-dist));

      // create toasty effect
      int c, num = static_cast<int>(4 * (2.0-dist));
      for (c=0; c<num; ++c)
	gameplay->add_particle
	  (new PixelParticle(m_pos + vector2d(rand_range(-0.2, 0.2),
					      rand_range(-0.5, 0.0)),
			     vector2d(0, rand_range(-2.0, -0.1)),
			     vector2d(0, 0),
			     rand_range(BPS/4, BPS*3/4),
			     makecol(255, 255, 0),
			     makecol(64, 0, 0)));

      // is this person completely burned?
      if (m_toast_factor >= 1.0) {
	GAMEPLAY->get_scorer()->one_to_hell();
	burn();
	kill();
      }

      // go away from devil
      if (m_state == STAND_PERSON ||
	  m_state == WALKING_PERSON ||
	  m_state == LOOKING_PERSON ||
	  m_state == CALLING_PERSON) {
	if (m_right && m_pos.x < player_pos.x) {
	  if (m_state != DEAD_PERSON &&
	      m_state != WALKING_PERSON)
	    set_state(WALKING_PERSON);
	  m_right = false;
	}
	else if (!m_right && m_pos.x > player_pos.x) {
	  if (m_state != DEAD_PERSON &&
	      m_state != WALKING_PERSON)
	    set_state(WALKING_PERSON);
	  m_right = true;
	}
      }
    }
  }
}

void Person::draw(BITMAP *bmp)
{
  if (m_state != DEAD_PERSON) {
    BITMAP *sprite = Person::prepare_sprite();
    int x, y;

    GAMEPLAY->get_level()->to_screen(m_pos, x, y);

    if (m_right)
      draw_sprite(bmp, sprite, x-TILE_W/2, y-TILE_H+2);
    else
      draw_sprite_h_flip(bmp, sprite, x-TILE_W/2, y-TILE_H+2);
  }
}

bool Person::is_dead()
{
  return m_state == DEAD_PERSON;
}

vector2d Person::get_pos() const
{
  return m_pos;
}

Angel *Person::get_abductor()
{
  return m_abductor;
}

void Person::set_abductor(Angel *angel)
{
  assert(!m_abductor);
  m_abductor = angel;
}

void Person::catch_person()
{
  assert(m_state != ABDUCTING_PERSON); // doble abducting? impossible
  assert(m_abductor && !m_abductor->is_dead()); // abductor isn't dead

  set_state(ABDUCTING_PERSON);
}

void Person::throw_person()
{
  assert(m_state == ABDUCTING_PERSON);

  set_state(FALLING_PERSON);
  if (m_abductor != NULL)
    m_abductor = NULL;
}

void Person::kill()
{
  if (m_abductor != NULL) {
    m_abductor->fire_event_person_die(this);
    m_abductor = NULL;
  }

  set_state(DEAD_PERSON);
}

void Person::fire_event_angel_die(Angel *angel)
{
  // did the abductor of this person die?
  if (m_abductor == angel) {
    m_abductor = NULL;

    if (m_state == ABDUCTING_PERSON)
      throw_person();
  }
}

void Person::set_state(PersonState state)
{
  m_state = state;
  m_state_time = GAME_T;
}

// converts a dead person in black particles
void Person::burn()
{
  GamePlay *gameplay = GAMEPLAY;
  BITMAP *sprite = prepare_sprite();
  int x, y, u, v;

  gameplay->get_level()->to_screen(m_pos, x, y);

  x = x-TILE_W/2;
  y = y-TILE_H+1;

  for (v=0; v<sprite->h; ++v)
    for (u=0; u<sprite->w; ++u) {
      int color, ou;

      if (m_right)
	color = getpixel(sprite, ou=u, v);
      else
	color = getpixel(sprite, ou=sprite->w-1-u, v);

      if (color != bitmap_mask_color(sprite))
	gameplay->add_particle
	  (new PixelParticle(vector2d((m_pos.x*TILE_W - TILE_W/2 + ou) / TILE_W,
				      (m_pos.y*TILE_H - TILE_H+1 + v) / TILE_H),
			     vector2d(0, rand_range(-2.0, -0.1)),
			     vector2d(rand_range(0.5, 4.0), 0),
			     rand_range(BPS/4, BPS*3/4),
			     makecol(0, 0, 0),
			     makecol(0, 0, 0)));

    }
}

// prepares the person_bmp
BITMAP *Person::prepare_sprite()
{
  static BITMAP *person_bmp = NULL;
  if (person_bmp == NULL)
    person_bmp = create_bitmap(TILE_W, TILE_H);

  int ani_frame = -1;

  switch (m_state) {

    case STAND_PERSON:
      ani_frame = ANI_FRAME_STAND;
      break;

    case WALKING_PERSON:
      ani_frame = ((GAME_T - m_state_time) % (BPS/4)) < (BPS/8) ?
	ANI_FRAME_WALK1: ANI_FRAME_WALK2;
      break;

    case LOOKING_PERSON:
      ani_frame = ANI_FRAME_LOOK;
      break;

    case CALLING_PERSON: {
      if (GAME_T - m_state_time < CALLING_DURATION*1/6)
	ani_frame = ANI_FRAME_HELP1;
      else if (GAME_T - m_state_time < CALLING_DURATION*2/6)
	ani_frame = ANI_FRAME_HELP2;
      else if (GAME_T - m_state_time < CALLING_DURATION*3/6)
	ani_frame = ANI_FRAME_HELP3;
      else if (GAME_T - m_state_time < CALLING_DURATION*4/6)
	ani_frame = ANI_FRAME_HELP3;
      else if (GAME_T - m_state_time < CALLING_DURATION*5/6)
	ani_frame = ANI_FRAME_HELP3;
      else
	ani_frame = ANI_FRAME_HELP1;
      break;
    }

    case ABDUCTING_PERSON:
      ani_frame = ANI_FRAME_HELP3;
      break;

    case FALLING_PERSON:
      ani_frame = ANI_FRAME_HELP2;
      break;

    case DEAD_PERSON:
      // impossible to be here
      break;
  }

  if (ani_frame < -1) {
    clear_to_color(person_bmp, bitmap_mask_color(person_bmp));
    return person_bmp;
  }

  blit(MEDIA_BITMAP(PEOPLE_PCX), person_bmp,
       ani_frame*TILE_W, m_type*TILE_H,
       0, 0, TILE_W, TILE_H);

  lit_mode(0, 0, 0);
  draw_lit_sprite(person_bmp, person_bmp, 0, 0,
		  static_cast<int>(255 * m_toast_factor));
  solid_mode();

  return person_bmp;
}
