#include <allegro.h>
#include "game.hpp"
#include "player.hpp"
#include "gameplay.hpp"
#include "level.hpp"
#include "util.hpp"
#include "media.hpp"
#include "particle.hpp"


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
  m_sprite = create_bitmap(100, 100);

  m_input = the_input;
  m_right = true;
  m_quiet_time = 0;
  m_tail_time = 0;
  m_charge_time = 0;
  m_fire_time = 0;
  m_fire_counter = 0;
  m_look = vector2d(8, 0);

  set_state(INTRO_PLAYER);
}


Player::~Player()
{
  destroy_bitmap(m_sprite);
}


void Player::update()
{
  if (m_state == INIT_PLAYER) {
    m_state = INTRO_PLAYER;
    m_pos = level()->get_start_pos();
    level()->look_at(m_pos + m_look);
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

    case RUNNING_PLAYER:
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
}


void Player::draw(BITMAP *bmp)
{
  BITMAP *sprite = NULL;
  BITMAP *tail_sprite = NULL;
  bool fire = false;
  int x, y;

  level()->to_screen(m_pos, x, y);
//   textprintf(bmp, font, 0, 0, makecol(255, 255, 255), "%dx%d", x, y);
//   textprintf(bmp, font, 0, 0, makecol(255, 255, 255), "player-state:%d", m_state);

  switch (m_state) {

    case INTRO_PLAYER: {
      double t = exp_ramp(m_state_time, INTRO_DURATION);
      trans_mode(255 * t);
      circle(bmp, x, y, 32.0*t, makecol(255, 255*t, 0));
      solid_mode();
      break;
    }

    case FLOATING_PLAYER: {
      // flying up
      if (m_vel.y < -3) {
	sprite = devil_fly_up_bmp;
	tail_sprite = MEDIA_BITMAP(TAIL06_PCX);
	m_tail_time = GAME_T-BPS/2.0;
      }
      // flying down
      else if (m_vel.y > 3) {
	sprite = devil_fly_down_bmp;
	tail_sprite = MEDIA_BITMAP(TAIL10_PCX);
	m_tail_time = GAME_T-BPS/2.0;
      }
      // flying to right/left
      else if (m_vel.x > 3 || m_vel.x < -3) {
	sprite = devil_fly_right_bmp;
	tail_sprite = MEDIA_BITMAP(TAIL08_PCX);
	m_tail_time = GAME_T-BPS/2.0;
      }
      // stop
      else if (m_vel.x > 0.1 || m_vel.x < -0.1)
	sprite = devil_fly_stop_bmp;
      // quit in the air
      else {
	if (m_quiet_time == 0 ||
	    GAME_T - m_quiet_time >= TO_WAIT_DURATION+WAIT_DURATION)
	  m_quiet_time = GAME_T;

	if (GAME_T - m_quiet_time < TO_WAIT_DURATION)
	  sprite = devil_floating_bmp;
	else
	  sprite = devil_floating_wait_bmp;
      }

      // tail movement
      if (tail_sprite == NULL) {
	int anilength = (TAIL25_PCX - TAIL00_PCX + 1);
	double t = (GAME_T - m_tail_time) / TAIL_CYCLE_DURATION;
	int c = static_cast<int>(t * anilength) % anilength;

	tail_sprite = MEDIA_BITMAP(TAIL00_PCX+c);
      }

      fire = true;
      break;
    }

    case STANDING_PLAYER:
      if (m_old_state == FLOATING_PLAYER &&
	  GAME_T - m_state_time < LANDING_DURATION)
	sprite = devil_landing_bmp;
      else
	sprite = devil_stand_bmp;

      fire = true;
      break;

    case WALKING_PLAYER: {
      int anilength = (DEVIL_WALKING07_PCX - DEVIL_WALKING00_PCX + 1);
      double t = (GAME_T - m_state_time) / WALKING_DURATION;
      int c = static_cast<int>(t * anilength) % anilength;

      sprite = MEDIA_BITMAP(DEVIL_WALKING00_PCX+c);
      fire = true;
      break;
    }

    case RUNNING_PLAYER:
      ellipse(bmp, x, y, 8, 16, makecol(0, 200, 0));
      break;

    case DYING_PLAYER:
      ellipse(bmp, x, y, 8, 16, makecol(0, 200, 0));
      break;

    case DEAD_PLAYER:
      // nothing to draw
      break;
  }

  // fire
  if (fire) {
    bool fly = (m_state == FLOATING_PLAYER);

    // charge
    if (m_charge_time > 0 &&
	GAME_T - m_charge_time > TO_BIG_CHARGE_DURATION) {
      sprite = fly ? devil_floating_big_shot1_bmp: devil_stand_big_shot1_bmp;
    }
    else if (m_fire_time > 0) {
      int t = GAME_T - m_fire_time;

      // small fire
      if (!m_fire_big && t < FIRE_DURATION) {
	int pair = (m_fire_counter & 1) == 0;

	if (t < FIRE_DURATION/4)
	  sprite = (pair ?
		    (fly ? devil_floating_left_shot1_bmp: devil_stand_left_shot1_bmp):
		    (fly ? devil_floating_right_shot1_bmp: devil_stand_right_shot1_bmp));
	else if (t < FIRE_DURATION/2)
	  sprite = (pair ?
		    (fly ? devil_floating_left_shot2_bmp: devil_stand_left_shot2_bmp):
		    (fly ? devil_floating_right_shot2_bmp: devil_stand_right_shot2_bmp));
	else
	  sprite = (pair ?
		    (fly ? devil_floating_left_shot3_bmp: devil_stand_left_shot3_bmp):
		    (fly ? devil_floating_right_shot3_bmp: devil_stand_right_shot3_bmp));
      }
      // big fire
      else if (m_fire_big && t < BIG_FIRE_DURATION) {
	if (t < BIG_FIRE_DURATION/4)
	  sprite = (fly ? devil_floating_big_shot2_bmp: devil_stand_big_shot2_bmp);
	else
	  sprite = (fly ? devil_floating_big_shot3_bmp: devil_stand_big_shot3_bmp);
      }
    }
  }

  // draw the sprite?
  if (sprite != NULL) {
    int u, v;

    // prerender "sprite" in "m_sprite"
    clear_to_color(m_sprite, bitmap_mask_color(m_sprite));

    if (tail_sprite != NULL) {
      u = m_sprite->w/2 - sprite->w/2 + 12 - 41;
      v = m_sprite->h/2 - sprite->h   + 17 - 23;
      draw_sprite(m_sprite, tail_sprite, u, v);
    }

    u = m_sprite->w/2 - sprite->w/2;
    v = m_sprite->h/2 - sprite->h + 1;
//     lit_mode(0, 0, 0);
//     draw_lit_sprite(m_sprite, sprite, u, v+1, 255);
//     solid_mode();
    draw_sprite(m_sprite, sprite, u, v);

    // draw halo
    if (m_halo_factor > 0.0) {
      double r = 16+16.0*(1-std::exp(-5.0*m_halo_factor));
      trans_mode(200 * m_halo_factor);
      circle(bmp, x, y-sprite->h/2, r, makecol(255, 255, 0));
      trans_mode(128 * m_halo_factor);
      circle(bmp, x, y-sprite->h/2, r+1, makecol(255, 255, 0));
      trans_mode(190 * m_halo_factor);
      circlefill(bmp,
		 x, y-sprite->h/2, r,
		 makecol(128 - 64 * m_halo_factor, 0, 0));
      solid_mode();
    }

    // draw the sprite & charge-fire effect
    if (m_right) {
      draw_sprite(bmp, m_sprite, x-m_sprite->w/2, y-m_sprite->h/2);
      draw_charge(bmp,
		  x-sprite->w/2+30,
		  y-sprite->h+1+15);
    }
    else {
      draw_sprite_h_flip(bmp, m_sprite, x-m_sprite->w/2, y-m_sprite->h/2);
      draw_charge(bmp,
		  x-sprite->w/2+sprite->w-30,
		  y-sprite->h+1+15);
    }
  }

  int u, v;
  level()->to_screen(m_pos+m_look, u, v);
  line(bmp, x, y, u, v, makecol(255, 255, 0));
  ellipse(bmp, u, v, 8, 8, makecol(255, 255, 0));
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


void Player::update_fly()
{
  // horizontal acceleration
  if (m_input->left()) {
    if (m_right) {
      m_right = false;
      m_tail_time = GAME_T-BPS/4.0;
    }
    m_vel.x -= 32.0 / BPS;
  }
  else if (m_input->right()) {
    if (!m_right) {
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

    vector2d fire_pos = m_pos + vector2d(m_right ? 0.5: -0.5,
					 m_fire_big ? -1.2: -1.6);
    vector2d fire_vel;
    double vx = (m_fire_big ? 40.0: 30.0);

    fire_vel = vector2d(m_vel.x, MID(-2.0, m_vel.y, 2.0));

    if (m_state == FLOATING_PLAYER && fire_vel.magnitude() > 4.0)
      fire_vel = fire_vel.normalize() * vx;
    else
      fire_vel = vector2d(vx * (m_right ? 1: -1), 0);

    // add the "fire" particle in the gameplay
    GAMEPLAY->add_particle(new Fire(fire_pos, fire_vel, size));

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
  if (m_charge_time > 0 &&
      GAME_T - m_charge_time > TO_BIG_CHARGE_DURATION) {
    double t1 = exp_ramp(m_charge_time+TO_BIG_CHARGE_DURATION, MAX_CHARGE_DURATION/2);
    double t2 = exp_ramp(m_charge_time+TO_BIG_CHARGE_DURATION, MAX_CHARGE_DURATION);
    int c1 = 240 * t1;
    int c2 = 255 * t2;
    int r1 = 10 * t1;
    int r2 = 9 * t2;

    if (r1 > 0) {
      trans_mode(255 * t1);
      circlefill(bmp, x, y, r1, makecol(c1, 0, 0));
    }

    if (r2 > 0) {
      trans_mode(255 * t2);
      circlefill(bmp, x, y, r2, makecol(c1, c2, c2/2));
    }

    solid_mode();

//     for (int c=0; c<16; ++c) {
//       double angle = rand_range(-AL_PI, AL_PI);
//       double length = rand_range(1.0, static_cast<double>(r2));
//       int u = x + sin(angle) * length;
//       int v = y + cos(angle) * length;

//       particles.push_back(new PixelParticle(vector2d(1.0*u/TILE_W, 1.0*v/TILE_H),
// 					    vector2d(0, -2),
// 					    vector2d(0, -2),
// 					    rand_range(BPS, BPS*2),
// 					    makecol(c1, c2, c2/2),
// 					    makecol(c1, 0, 0)));
//     }
  }
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
