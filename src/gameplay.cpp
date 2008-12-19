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

#include <cmath>
#include <iterator>
#include <algorithm>
#include <allegro.h>
#include "gfx.hpp"
#include "game.hpp"
#include "gameplay.hpp"
#include "player.hpp"
#include "level.hpp"
#include "util.hpp"
#include "particle.hpp"
#include "person.hpp"
#include "scorer.hpp"
#include "angel.hpp"
#include "sprite.hpp"
#include "final.hpp"
#include "menu.hpp"


#define MENU_FADEIN_DURATION (BPS/4.0)


GamePlay::GamePlay()
  : m_esc(KEY_ESC)
  , m_enter(KEY_ENTER)
  , m_left(KEY_LEFT)
  , m_right(KEY_RIGHT)
{
  m_quit_time = 0;

  m_level = new Level(1);
  m_player = new Player();
  m_scorer = new Scorer();

  m_layers[LEVEL_LAYER].push_back(m_level);
  m_layers[PLAYER_LAYER].push_back(m_player);
  m_layers[SCORER_LAYER].push_back(m_scorer);

  start_level();
}


GamePlay::~GamePlay()
{
  ObjectList::iterator it, end;
  int num;

  // destroy all layers
  for (num=0; num<MAX_LAYERS; ++num) {
    end = m_layers[num].end();
    for (it = m_layers[num].begin(); it != end; ++it)
      delete (*it);
  }
}


GameState *GamePlay::update()
{
  bool esc = m_esc.pressed();
  bool enter = m_enter.pressed();
  bool left = m_left.pressed();
  bool right = m_right.pressed();

  // are we in the menu to quit?
  if (m_quit_time > 0) {
    if (esc)
      m_quit_time = 0;
    else {
      // select option
      if (enter) {
	// return to game
	if (m_selected == 0) {
	  m_quit_time = 0;
	}
	// go to main menu
	else if (m_selected == 1) {
	  return new Menu();
	}
      }
      // change option
      else if (left || right) {
	m_selected = (m_selected+1) % 2;
      }
      return this;
    }
  }
  // go to the quit menu?
  else if (esc) {
    m_quit_time = GAME_T;
    m_selected = 0;
    return this;
  }

  if (m_level->is_finished()) {
    int num = m_level->get_num();
    if (num == 3) {
      return new Final();
    }
    else {
      m_level->set_old();
      Level *next_level = new Level(++num);

      m_level = next_level;

      m_layers[LEVEL_LAYER].clear();
      m_layers[LEVEL_LAYER].push_back(m_level);
      start_level();
    }
  }

  // update the logic of all objects in all layers
  update_layers();
  
  return this;
}


void GamePlay::draw(BITMAP *bmp)
{
  // draw the game
  draw_layers(bmp);

  // draw the quit-menu
  if (m_quit_time > 0)
    draw_menu(bmp);
}


Level *GamePlay::get_level()
{
  return m_level;
}


Player *GamePlay::get_player()
{
  return m_player;
}


Scorer *GamePlay::get_scorer()
{
  return m_scorer;
}


const ObjectList &GamePlay::get_people()
{
  return m_layers[PEOPLE_LAYER];
}


const ObjectList &GamePlay::get_angels()
{
  return m_layers[ANGELS_LAYER];
}


void GamePlay::add_particle(Particle *particle)
{
  m_layers[PARTICLES_LAYER].push_back(particle);
}


void GamePlay::add_particles(std::list<Particle *> particles)
{
  std::copy(particles.begin(), particles.end(),
	    std::back_inserter(m_layers[PARTICLES_LAYER]));
}


void GamePlay::add_particles_from_sprite(Sprite *sprite,
					 vector2d velmin, vector2d velmax,
					 vector2d accmin, vector2d accmax,
					 int timemin, int timemax,
					 int color_dest)
{
  int x, y, u, v, fx, fy, color;
  vector2d pos;

  pos = m_level->to_level(sprite->x, sprite->y);

  for (v=0; v<sprite->bmp->h; ++v) {
    for (u=0; u<sprite->bmp->w; ++u) {
      color = getpixel(sprite->bmp, u, v);
      fx = sprite->flip_x(u);
      fy = sprite->flip_y(v);

      if (color != bitmap_mask_color(sprite->bmp))
	add_particle(new PixelParticle(vector2d((pos.x + 1.0 * fx / TILE_W),
						(pos.y + 1.0 * fy / TILE_H)),
				       vector2d(rand_range(velmin.x, velmax.x),
						rand_range(velmin.y, velmax.y)),
				       vector2d(rand_range(accmin.x, accmax.x),
						rand_range(accmin.y, accmax.y)),
				       rand_range(timemin, timemax),
				       color,
				       color_dest));
    }
  }
}


void GamePlay::add_person(Person *person)
{
  m_layers[PEOPLE_LAYER].push_back(person);
}


void GamePlay::add_angel(Angel *angel)
{
  m_layers[ANGELS_LAYER].push_back(angel);
}


void GamePlay::update_layers()
{
  ObjectList::iterator it, end, next;
  int num;

  for (num=0; num<MAX_LAYERS; ++num) {
    end = m_layers[num].end();
    for (it = m_layers[num].begin(); it != end; it = next) {
      next = it;
      ++next;

      (*it)->update();
      if ((*it)->is_dead()) {
	delete (*it);
	m_layers[num].erase(it);
      }
    }
  }
}


void GamePlay::draw_layers(BITMAP *bmp)
{
  ObjectList::iterator it, end;
  int num;

  for (num=0; num<MAX_LAYERS; ++num) {
    end = m_layers[num].end();
    for (it = m_layers[num].begin(); it != end; ++it)
      (*it)->draw(bmp);
  }
}


void GamePlay::draw_menu(BITMAP *bmp)
{
  double t = exp_ramp(m_quit_time, MENU_FADEIN_DURATION);

  trans_mode(static_cast<int>(190*t));
  rectfill(bmp, 0, 0, GFX_W-1, GFX_H-1, makecol(0, 0, 0));
  solid_mode();

  // textout_ex(bmp, font, "quit menu", 0, -8+8*t, makecol(255, 255, 255), -1);

  draw_text(bmp, 32, 32, makecol(0, 180, 200), "do you want to quit?");
  Menu::draw_item(bmp, 30, 32+12, "no", (m_selected == 0));
  Menu::draw_item(bmp, 30+32, 32+12, "yes", (m_selected == 1));
}


void GamePlay::start_level()
{
  for (int c=0; c<16*m_level->get_num(); ++c)
    add_person(new Person(m_level->get_random_pos_for_people()));

  m_player->start_level();
  m_scorer->reset(m_level->get_angels_to_generate(),
		  get_people().size());
}

