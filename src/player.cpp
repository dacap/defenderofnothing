#include <allegro.h>
#include "game.hpp"
#include "player.hpp"
#include "gameplay.hpp"
#include "level.hpp"
#include "util.hpp"
#include "media.hpp"


#define INTRO_DURATION (BPS/2)
#define DYING_DURATION (BPS/2)
#define FIRE_DURATION  (BPS/3)


Player::Player(GamePlay *gameplay)
{
  m_sprite = create_bitmap(100, 100);

  m_input = the_input;
  m_gameplay = gameplay;
  m_right = true;
  m_tail_time = 0;
  m_fire_time = 0;
  m_fire_counter = 0;

  set_state(INTRO_PLAYER);

  m_pos = level()->get_start_pos();
  level()->look_at(m_pos);
}


Player::~Player()
{
  destroy_bitmap(m_sprite);
}


void Player::update()
{
  bool fly = false;
  bool walk = false;

  switch (m_state) {

    case INTRO_PLAYER:
      if (GAME_T - m_state_time > INTRO_DURATION) {
	set_state(FLOATING_PLAYER);
      }
      break;

    case FLOATING_PLAYER:
      fly = true;
      break;

    case STANDING_PLAYER:
      walk = true;
      break;

    case WALKING_PLAYER:
      walk = true;
      break;

    case RUNNING_PLAYER:
      walk = true;
      break;

    case JUMPING_PLAYER:
      walk = true;
      break;

    case DYING_PLAYER:
      if (GAME_T - m_state_time > DYING_DURATION) {
	set_state(DEAD_PLAYER);
      }
      break;

    case DEAD_PLAYER:
      break;
  }

  if (fly) {
    // horizontal acceleration
    if (m_input->left()) {
      if (m_right)
	m_tail_time = GAME_T-BPS/4.0;
      m_right = false;
      m_vel.x -= 32.0 / BPS;
    }
    else if (m_input->right()) {
      if (!m_right)
	m_tail_time = GAME_T-BPS/4.0;
      m_right = true;
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
    m_vel.x = MID(-8.0, m_vel.x, 8.0);
    m_vel.y = MID(-8.0, m_vel.y, 8.0);

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

    // fire!
    if (GAME_T - m_fire_time > FIRE_DURATION) {
      if (m_input->fire()) {
	m_fire_time = GAME_T;
	m_fire_counter++;
      }
      else
	m_fire_time = 0;
    }
  }

  if (walk) {
    if (m_state == WALKING_PLAYER) {
      if (m_input->left())
	m_pos -= vector2d(-32.0, 0.0) / BPS;
      else if (m_input->right())
	m_pos += vector2d(32.0, 0.0) / BPS;
    }
    else {
    }
  }
}


void Player::draw(BITMAP *bmp)
{
  BITMAP *sprite = NULL;
  BITMAP *tail_sprite = NULL;
  int x, y;

  level()->to_screen(m_pos, x, y);
  textprintf(bmp, font, 0, 0, makecol(255, 255, 255), "%dx%d", x, y);

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
	sprite = devil_up_bmp;
	tail_sprite = MEDIA_BITMAP(TAIL06_PCX);
	m_tail_time = GAME_T-BPS/2.0;
      }
      // flying down
      else if (m_vel.y > 3) {
	sprite = devil_down_bmp;
	tail_sprite = MEDIA_BITMAP(TAIL10_PCX);
	m_tail_time = GAME_T-BPS/2.0;
      }
      // flying to right/left
      else if (m_vel.x > 3 || m_vel.x < -3) {
	sprite = devil_right_bmp;
	tail_sprite = MEDIA_BITMAP(TAIL08_PCX);
	m_tail_time = GAME_T-BPS/2.0;
      }
      // stop
      else if (m_vel.x > 0.1 || m_vel.x < -0.1)
	sprite = devil_stop_bmp;
      // stand
      else
	sprite = devil_stand_bmp;

      if (GAME_T - m_fire_time < FIRE_DURATION) {
	int t = GAME_T - m_fire_time;
	int pair = (m_fire_counter & 1) == 0;

	if (t < FIRE_DURATION/4)
	  sprite = pair ? devil_left_shot1_bmp: devil_right_shot1_bmp;
	else if (t < FIRE_DURATION/2)
	  sprite = pair ? devil_left_shot2_bmp: devil_right_shot2_bmp;
	else
	  sprite = pair ? devil_left_shot3_bmp: devil_right_shot3_bmp;
      }

      // tail movement for stop/stand sprites
      if (tail_sprite == NULL) {
	double t = (GAME_T - m_tail_time) / (BPS*1.5);
	int c = t * (TAIL25_PCX - TAIL00_PCX + 1);
	c %= (TAIL25_PCX - TAIL00_PCX + 1);

	tail_sprite = MEDIA_BITMAP(TAIL00_PCX+c);
      }
      break;
    }

    case STANDING_PLAYER:
      ellipse(bmp, x, y, 8, 16, makecol(0, 200, 0));
      break;

    case WALKING_PLAYER:
      ellipse(bmp, x, y, 8, 16, makecol(0, 200, 0));
      break;

    case RUNNING_PLAYER:
      ellipse(bmp, x, y, 8, 16, makecol(0, 200, 0));
      break;

    case JUMPING_PLAYER:
      break;

    case DYING_PLAYER:
      ellipse(bmp, x, y, 8, 16, makecol(0, 200, 0));
      break;

    case DEAD_PLAYER:
      // nothing to draw
      break;
  }

  if (sprite != NULL) {
    int u, v;

    clear_to_color(m_sprite, bitmap_mask_color(m_sprite));

//     lit_mode(0, 0, 0);
//     draw_lit_sprite(m_sprite, sprite, u, v+1, 255);
//     solid_mode();

    if (tail_sprite != NULL) {
      u = m_sprite->w/2 - tail_sprite->w/2;
      v = m_sprite->h/2 - tail_sprite->h/2;
      draw_sprite(m_sprite, tail_sprite, u-13, u+10);
    }

    u = m_sprite->w/2 - sprite->w/2;
    v = m_sprite->h/2 - sprite->h/2;
    draw_sprite(m_sprite, sprite, u, v);

    if (m_right)
      draw_sprite(bmp, m_sprite, x-m_sprite->w/2, y-m_sprite->h/2);
    else
      draw_sprite_h_flip(bmp, m_sprite, x-m_sprite->w/2, y-m_sprite->h/2);
  }

  level()->to_screen(m_pos+m_look, x, y);
  ellipse(bmp, x, y, 8, 8, makecol(255, 255, 0));
}


void Player::set_state(PlayerState state)
{
  m_state = state;
  m_state_time = GAME_T;
  m_tail_time = GAME_T;
}


Level *Player::level()
{
  return m_gameplay->get_level();
}
