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


#define MENU_FADEIN_DURATION (BPS/4.0)


GamePlay::GamePlay()
  : m_esc(KEY_ESC)
{
  m_quit_time = 0;

  m_level = new Level(1);
  m_player = new Player();
  m_scorer = new Scorer();

  m_layers[LEVEL_LAYER].push_back(m_level);
  m_layers[PLAYER_LAYER].push_back(m_player);
  m_layers[SCORER_LAYER].push_back(m_scorer);

  for (int c=0; c<16; ++c)
    add_person(new Person(m_level->get_random_pos_for_people()));
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

#if 0
  // are we in the menu to quit?
  if (m_quit_time > 0) {
    if (esc)
      m_quit_time = 0;
    else
      return this;
  }
  // go to the quit menu?
  else if (esc) {
    m_quit_time = GAME_T;
    return this;
  }
#else
  if (esc)
    return NULL;
#endif

  // update the logic of all objects in all layers
  update_layers();
  
  return this;
}


void GamePlay::draw(BITMAP *bmp)
{
  clear_to_color(bmp, makecol(0, 180, 200));

  // draw the game
  draw_layers(bmp);

  // draw the quit-menu
  if (m_quit_time > 0) {
    double t = exp_ramp(m_quit_time, MENU_FADEIN_DURATION);

    trans_mode(190*t);
    rectfill(bmp, 0, 0, GFX_W-1, GFX_H-1, makecol(0, 0, 0));
    solid_mode();

    // textout_ex(bmp, font, "quit menu", 0, -8+8*t, makecol(255, 255, 255), -1);
  }
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

