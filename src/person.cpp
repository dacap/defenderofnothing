#include <allegro.h>
#include "person.hpp"
#include "util.hpp"
#include "game.hpp"
#include "gameplay.hpp"
#include "level.hpp"
#include "media.hpp"
#include "player.hpp"
#include "particle.hpp"


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

    case DEAD_PERSON:
      // nothing to do
      break;
  }

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
    int c, num = 4 * (2.0-dist);
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
      set_state(DEAD_PERSON);
      burn_completely();
    }

    // go away from devil
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


void Person::set_state(PersonState state)
{
  m_state = state;
  m_state_time = GAME_T;
}


void Person::burn_completely()
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
  draw_lit_sprite(person_bmp, person_bmp, 0, 0, 255 * m_toast_factor);
  solid_mode();

  return person_bmp;
}
