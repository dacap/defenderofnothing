// Defender Of Nothing
// Copyright (C) 2007 by David A. Capello
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
// * Neither the name of the Vaca nor the names of its contributors
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
#include "game.hpp"
#include "player.hpp"
#include "gameplay.hpp"
#include "level.hpp"
#include "util.hpp"
#include "media.hpp"
#include "particle.hpp"
#include "sprite.hpp"


#define INTRO_DURATION		(BPS*0.5)
#define DYING_DURATION		(BPS*0.5)
#define TO_BIG_CHARGE_DURATION	(BPS*0.25)
#define MAX_CHARGE_DURATION	(BPS*2.0)
#define FIRE_DURATION		(BPS*0.3)
#define BIG_FIRE_DURATION	(BPS*0.5)
#define TO_WAIT_DURATION	(BPS*2.0)
#define WAIT_DURATION		(BPS*1.0)
#define TAIL_CYCLE_DURATION	(BPS*1.5)
#define LANDING_DURATION	(BPS*0.5)
#define WALKING_DURATION	(BPS*1.0)


Player::Player()
{
  m_input = the_input;

  set_state(INIT_PLAYER);
}


Player::~Player()
{
}


void Player::update()
{
  if (m_state == INIT_PLAYER) {
    m_right = true;
    m_quiet_time = 0;
    m_tail_time = 0;
    m_charge_time = 0;
    m_fire_time = 0;
    m_fire_counter = 0;
    m_look = vector2d(8, 0);
//     m_smoke_time = GAME_T;

    m_pos = level()->get_start_pos();
    level()->look_at(m_pos + m_look);

    set_state(INTRO_PLAYER);
  }
  
  switch (m_state) {

    case INTRO_PLAYER:
      if (GAME_T - m_state_time > INTRO_DURATION) {
	set_state(FLOATING_PLAYER);
      }
      break;

    case FLOATING_PLAYER:
      update_fly();
      update_fire();
      break;

    case STANDING_PLAYER:
      update_walk();
      update_fire();
      break;

    case WALKING_PLAYER:
      update_walk();
      update_fire();
      break;

    case DYING_PLAYER:
      if (GAME_T - m_state_time > DYING_DURATION) {
	set_state(DEAD_PLAYER);
      }
      break;

    case DEAD_PLAYER:
      break;
  }

  // update halo
  if (m_halo_factor > 0.0) {
    m_halo_factor *= 0.9;
    if (m_halo_factor < 0.01)
      m_halo_factor = 0.0;
  }

  //////////////////////////////////////////////////////////////////////
  // update smoke
//   if (GAME_T - m_smoke_time > BPS*0.1) {
//     m_smoke_time = GAME_T;
//   Sprite *sprite, *tail;
//   get_sprites(sprite, tail);
//   if (sprite != NULL) {
//     if (tail != NULL) {
//       GAMEPLAY->add_particles_from_sprite(tail,
// 					  vector2d(0.0, -2.0),
// 					  vector2d(0.0, -0.1),
// 					  vector2d(0.5, 0.0),
// 					  vector2d(4.0, 0.0),
// 					  BPS/4, BPS*3/4,
// 					  makecol(128, 128, 128));
//       delete tail;
//     }
//     GAMEPLAY->add_particles_from_sprite(sprite,
// 					vector2d(0.0, -2.0),
// 					vector2d(0.0, -0.1),
// 					vector2d(2.5, 0.0),
// 					vector2d(8.0, 0.0),
// 					BPS/4, BPS*3/4,
// 					makecol(128, 128, 128));
//     delete sprite;
//   }
//   }
  //////////////////////////////////////////////////////////////////////
}


void Player::draw(BITMAP *bmp)
{
  int x, y;
  level()->to_screen(m_pos, x, y);

//   textprintf(bmp, font, 0, 0, makecol(255, 255, 255), "%dx%d", x, y);
//   textprintf(bmp, font, 0, 0, makecol(255, 255, 255), "player-state:%d", m_state);

  switch (m_state) {

    case INTRO_PLAYER: {
      double t = exp_ramp(m_state_time, INTRO_DURATION);

      trans_mode(128 * t);
      for (int c=0; c<4; ++c) {
	int r = 64.0*t-c*8;
	if (r > 0)
	  circlefill(bmp, x, y-8, r, makecol(255, 32*c, 0));
      }
      solid_mode();

      {
	int w = 80*t;
	int h = 30*t;
	draw_text(bmp, bmp->w/2-w/2, bmp->h/2-h/2, w, h,
		  makecol(120, 120, 255), "LEVEL %d",
		  level()->get_num());
      }

      // TODO
      break;
    }

    case DYING_PLAYER:
      // TODO
      break;

    default:
      break;
  }
  
  Sprite *sprite, *tail;
  get_sprites(sprite, tail);
  if (sprite != NULL) {

    // draw tail
    if (tail != NULL) {
      tail->draw(bmp);
      delete tail;
    }

    // draw body
    sprite->draw(bmp);

    // draw halo
    if (m_halo_factor > 0.0)
      draw_halo(bmp, sprite);

    // draw the sprite & charge-fire effect
    if (m_charge_time > 0 &&
	GAME_T - m_charge_time > TO_BIG_CHARGE_DURATION) {
      draw_charge(bmp,
		  sprite->x+sprite->flip_x(30),
		  sprite->y+15);
    }

    delete sprite;
  }

//   int u, v;
//   level()->to_screen(m_pos+m_look, u, v);
//   line(bmp, x, y, u, v, makecol(255, 255, 0));
//   ellipse(bmp, u, v, 8, 8, makecol(255, 255, 0));
}


void Player::get_sprites(Sprite *&sprite, Sprite *&tail_sprite)
{
  BITMAP *body_bmp = NULL;
  BITMAP *tail_bmp = NULL;
  bool fire = false;
  int x, y;
  int tail_x_offset = 0;
  int tail_y_offset = 0;

  sprite = NULL;
  tail_sprite = NULL;

  level()->to_screen(m_pos, x, y);

  switch (m_state) {

    case INTRO_PLAYER:
      break;

    case FLOATING_PLAYER: {
      // flying up
      if (m_vel.y < -3) {
	body_bmp = devil_fly_up_bmp;
	tail_bmp = MEDIA_BITMAP(TAIL06_PCX);
	m_tail_time = GAME_T-BPS/2.0;
      }
      // flying down
      else if (m_vel.y > 3) {
	body_bmp = devil_fly_down_bmp;
	tail_bmp = MEDIA_BITMAP(TAIL10_PCX);
	m_tail_time = GAME_T-BPS/2.0;
      }
      // flying to right/left
      else if (m_vel.x > 3 || m_vel.x < -3) {
	body_bmp = devil_fly_right_bmp;
	tail_bmp = MEDIA_BITMAP(TAIL08_PCX);
	m_tail_time = GAME_T-BPS/2.0;
      }
      // stop
      else if (m_vel.x > 0.1 || m_vel.x < -0.1)
	body_bmp = devil_fly_stop_bmp;
      // quit in the air
      else {
	if (m_quiet_time == 0 ||
	    GAME_T - m_quiet_time >= TO_WAIT_DURATION+WAIT_DURATION)
	  m_quiet_time = GAME_T;

	if (GAME_T - m_quiet_time < TO_WAIT_DURATION)
	  body_bmp = devil_floating_bmp;
	else
	  body_bmp = devil_floating_wait_bmp;
      }

      // tail movement
      if (tail_bmp == NULL) {
	int anilength = (TAIL25_PCX - TAIL00_PCX + 1);
	double t = (GAME_T - m_tail_time) / TAIL_CYCLE_DURATION;
	int c = static_cast<int>(t * anilength) % anilength;

	tail_bmp = MEDIA_BITMAP(TAIL00_PCX+c);
      }

      fire = true;
      break;
    }

    case STANDING_PLAYER:
      if (m_old_state == FLOATING_PLAYER &&
	  GAME_T - m_state_time < LANDING_DURATION) {

	body_bmp = devil_landing_bmp;
	tail_y_offset = 8;
      }
      else
	body_bmp = devil_stand_bmp;

      tail_bmp = MEDIA_BITMAP(TAIL08_PCX);

      fire = true;
      break;

    case WALKING_PLAYER: {
      int anilength = (DEVIL_WALKING07_PCX - DEVIL_WALKING00_PCX + 1);
      double t = (GAME_T - m_state_time) / WALKING_DURATION;
      int c = static_cast<int>(t * anilength) % anilength;

      body_bmp = MEDIA_BITMAP(DEVIL_WALKING00_PCX+c);
      tail_bmp = MEDIA_BITMAP(TAIL08_PCX);

      fire = true;
      break;
    }

    default:
      break;
  }

  // fire
  if (fire) {
    bool fly = (m_state == FLOATING_PLAYER);

    // charging
    if (m_charge_time > 0 &&
	GAME_T - m_charge_time > TO_BIG_CHARGE_DURATION) {
      body_bmp = fly ? devil_floating_big_shot1_bmp: devil_stand_big_shot1_bmp;

      tail_x_offset = 2;
    }
    // firing
    else if (m_fire_time > 0) {
      int t = GAME_T - m_fire_time;

      // small fire
      if (!m_fire_big && t < FIRE_DURATION) {
	int pair = (m_fire_counter & 1) == 0;

	if (t < FIRE_DURATION/4)
	  body_bmp =
	    (pair ?
	     (fly ? devil_floating_left_shot1_bmp: devil_stand_left_shot1_bmp):
	     (fly ? devil_floating_right_shot1_bmp: devil_stand_right_shot1_bmp));
	else if (t < FIRE_DURATION/2)
	  body_bmp =
	    (pair ?
	     (fly ? devil_floating_left_shot2_bmp: devil_stand_left_shot2_bmp):
	     (fly ? devil_floating_right_shot2_bmp: devil_stand_right_shot2_bmp));
	else
	  body_bmp =
	    (pair ?
	     (fly ? devil_floating_left_shot3_bmp: devil_stand_left_shot3_bmp):
	     (fly ? devil_floating_right_shot3_bmp: devil_stand_right_shot3_bmp));
      }
      // big fire
      else if (m_fire_big && t < BIG_FIRE_DURATION) {
	if (t < BIG_FIRE_DURATION/4)
	  body_bmp = (fly ? devil_floating_big_shot2_bmp: devil_stand_big_shot2_bmp);
	else
	  body_bmp = (fly ? devil_floating_big_shot3_bmp: devil_stand_big_shot3_bmp);
      }
    }
  }

  if (body_bmp != NULL) {
    sprite = new Sprite(body_bmp, x-body_bmp->w/2, y-body_bmp->h, !m_right);

    if (tail_bmp != NULL) {
      int u, v;
      u = sprite->x + sprite->flip_x(10+tail_x_offset) + (sprite->h_flip ? -tail_bmp->w+41: -41);
      v = sprite->y + 17 - 23 + tail_y_offset;
      tail_sprite = new Sprite(tail_bmp, u, v, sprite->h_flip);
    }
  }
}


vector2d Player::get_pos() const
{
  return m_pos;
}


void Player::inc_halo(double increment)
{
  m_halo_factor += increment;
  if (m_halo_factor > 1.0)
    m_halo_factor = 1.0;
}


void Player::inc_bad_fire()
{
  ++m_bad_fire_counter;
}


void Player::start_level()
{
  set_state(INIT_PLAYER);
}


void Player::update_fly()
{
  // horizontal acceleration
  if (m_input->left()) {
    if (m_right && m_charge_time == 0 && m_fire_time == 0) {
      m_right = false;
      m_tail_time = GAME_T-BPS/4.0;
    }
    m_vel.x -= 32.0 / BPS;
  }
  else if (m_input->right()) {
    if (!m_right && m_charge_time == 0 && m_fire_time == 0) {
      m_right = true;
      m_tail_time = GAME_T-BPS/4.0;
    }
    m_vel.x += 32.0 / BPS;
  }
  else
    m_vel /= 64.0 / BPS;

  // vertical acceleration
  if (m_input->up())
    m_vel.y -= 32.0 / BPS;
  else if (m_input->down())
    m_vel.y += 32.0 / BPS;
  else
    m_vel /= 64.0 / BPS;

  // limit velocity
  m_vel.x = MID(-10.0, m_vel.x, 10.0);
  m_vel.y = MID(-10.0, m_vel.y, 10.0);

  // apply velocity to the position (movement)
  m_pos += m_vel / BPS;

  m_pos.x = MID(0.5, m_pos.x, level()->get_w()-0.5);
  m_pos.y = MID(0.5, m_pos.y, level()->get_h()-0.5);

  // look-at (move scroll)
  double angle1 = m_vel.angle();
  double angle2 = m_look.angle();

  if (ABS(angle1 - angle2) > AL_PI/16.0) {
    m_look = vector2d(std::cos(angle1)*8.0, std::sin(angle1)*8.0);

    if (m_pos.y > level()->get_h()-LEVEL_H+2)
      m_look.y = 8.0;

    level()->look_at(m_pos + m_look);
  }

  // landing
  if (level()->touch_floor(m_pos)) {
    m_pos = level()->get_stand_point(m_pos);
    set_state(STANDING_PLAYER);
  }
}


void Player::update_walk()
{
  m_vel = vector2d(m_right ? 10.0: -10.0, -10.0);
  m_look = vector2d(m_vel.normalize().x * 8.0, 0.0);
  level()->look_at(m_pos + m_look);

  // fly again
  if (m_input->up())
    set_state(FLOATING_PLAYER);

  // the fire lock the walk
  if (m_fire_time > 0 || m_charge_time > 0)
    return;

  if (m_input->left()) {
    m_right = false;
    m_pos.x -= 4.0 / BPS;
    if (m_state == STANDING_PLAYER)
      set_state(WALKING_PLAYER);
  }
  else if (m_input->right()) {
    m_right = true;
    m_pos.x += 4.0 / BPS;
    if (m_state == STANDING_PLAYER)
      set_state(WALKING_PLAYER);
  }
  else if (m_state != STANDING_PLAYER)
    set_state(STANDING_PLAYER);
}


void Player::update_fire()
{
  // the player is charging?
  if (m_input->fire()) {
    if (m_state == WALKING_PLAYER)
      set_state(STANDING_PLAYER);

    // start charge
    if (m_charge_time == 0) {
      m_fire_big = false;
      m_charge_time = GAME_T;
    }
    // big fire?
    else if (!m_fire_big &&
	     GAME_T - m_charge_time > TO_BIG_CHARGE_DURATION) {
      m_fire_big = true;
    }
  }
  // was the player uphold the fire button?
  else if (m_charge_time > 0) {
    // shoot! generate the "Fire" particle
    int size = !m_fire_big ? 3: 10 * exp_ramp(m_charge_time, MAX_CHARGE_DURATION);
    double energy = !m_fire_big ? 2.0: 20.0 * exp_ramp(m_charge_time, MAX_CHARGE_DURATION);

    vector2d fire_pos = m_pos + vector2d(m_right ? 0.5: -0.5,
					 m_fire_big ? -1.2: -1.6);
    vector2d fire_vel;
    double vx = (m_fire_big ? 40.0: 30.0);

    fire_vel = vector2d(m_right ? (m_vel.x > 0.0 ? m_vel.x: -m_vel.x):
				  (m_vel.x < 0.0 ? m_vel.x: -m_vel.x),
			MID(-2.0, m_vel.y, 2.0));

    if (m_state == FLOATING_PLAYER && fire_vel.magnitude() > 4.0)
      fire_vel = fire_vel.normalize() * vx;
    else
      fire_vel = vector2d(vx * (m_right ? 1: -1), 0);

    // add the "fire" particle in the gameplay
    GAMEPLAY->add_particle(new Fire(fire_pos, fire_vel, size, energy));

    // fixup the tail time animation
    if (m_fire_big)
      m_tail_time = GAME_T-BPS/4.0;

    // fire animation
    m_charge_time = 0;
    m_fire_time = GAME_T;
    m_fire_counter++;
  }
  // end of fire
  else if (!m_fire_big && GAME_T - m_fire_time > FIRE_DURATION)
    m_fire_time = 0;
  else if (m_fire_big && GAME_T - m_fire_time > BIG_FIRE_DURATION)
    m_fire_time = 0;
}


void Player::draw_charge(BITMAP *bmp, int x, int y)
{
  double t1 = exp_ramp(m_charge_time+TO_BIG_CHARGE_DURATION, MAX_CHARGE_DURATION*0.5);
  double t2 = exp_ramp(m_charge_time+TO_BIG_CHARGE_DURATION, MAX_CHARGE_DURATION);
  double t3 = exp_ramp(m_charge_time+TO_BIG_CHARGE_DURATION, MAX_CHARGE_DURATION*0.4);
  int c1 = 240 * t1;
  int c2 = 255 * t2;
  int r1 = 10 * t1;
  int r2 = 9 * t2;
  int r3 = 64 * t3;

  if (r1 > 0) {
    trans_mode(32);
    circlefill(bmp,
	       x+rand_range(-1, 1),
	       y+rand_range(-1, 1), r1+1, makecol(255, 0, 0));

    trans_mode(255 * t1);
    circlefill(bmp, x, y, r1, makecol(c1, 0, 0));
  }

  if (r2 > 0) {
    trans_mode(255 * t2);
    circlefill(bmp, x, y, r2, makecol(c1, c2, c2/2));
  }

  if (r3 > 0) {
    trans_mode(255 * (1.0-t3));
    circle(bmp, x, y, r3, makecol(c1, c1, 0));
  }

  solid_mode();

  //     for (int c=0; c<16; ++c) {
  //       double angle = rand_range(-AL_PI, AL_PI);
  //       double length = rand_range(1.0, static_cast<double>(r2));
  //       int u = x + sin(angle) * length;
  //       int v = y + cos(angle) * length;

  //       GAMEPLAY->add_particle(new PixelParticle(vector2d(1.0*u/TILE_W, 1.0*v/TILE_H),
  // 					       vector2d(0, -2),
  // 					       vector2d(0, -2),
  // 					       rand_range(BPS, BPS*2),
  // 					       makecol(c1, c2, c2/2),
  // 					       makecol(c1, 0, 0)));
  //     }
}


void Player::draw_halo(BITMAP *bmp, Sprite *sprite)
{
  int x = sprite->x + sprite->flip_x(sprite->bmp->w/2-4);
  int y = sprite->y + sprite->bmp->h/2;

  double r = 16+16.0*(1-std::exp(-5.0*m_halo_factor));

  trans_mode(200 * m_halo_factor);
  circle(bmp, x, y, r, makecol(255, 255, 0));

  trans_mode(128 * m_halo_factor);
  circle(bmp, x, y, r+1, makecol(255, 255, 0));

  trans_mode(190 * m_halo_factor);
  circlefill(bmp, x, y, r,
	     makecol(128 - 64 * m_halo_factor, 0, 0));

  solid_mode();
}


void Player::set_state(PlayerState state)
{
  m_old_state = m_state;
  m_state = state;
  m_state_time = GAME_T;
  m_tail_time = GAME_T;
}


Level *Player::level()
{
  return GAMEPLAY->get_level();
}
