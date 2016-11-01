// Defender Of Nothing
// Copyright (C) 2007 by David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#ifndef GAMEPLAY_H_INCLUDED
#define GAMEPLAY_H_INCLUDED

#include <list>
#include "gamestate.h"
#include "lockedkey.h"
#include "vector2d.h"


// current game-play state
#define GAMEPLAY	(static_cast<GamePlay *>(the_game->get_state()))



class Object;
class Level;
class Player;
class Scorer;
class Particle;
class Person;
class Angel;
class Sprite;


enum {
  LEVEL_LAYER,
  PEOPLE_LAYER,
  ANGELS_LAYER,
  PLAYER_LAYER,
  PARTICLES_LAYER,
  SCORER_LAYER,
  MAX_LAYERS
};


typedef std::list<Object *> ObjectList;


// a game state
class GamePlay : public GameState
{
  Level *m_level;
  Player *m_player;
  Scorer *m_scorer;
  ObjectList m_layers[MAX_LAYERS];

  // quit menu
  LockedKey m_esc, m_enter, m_left, m_right;
  int m_quit_time;
  int m_selected;

public:
  GamePlay();
  virtual ~GamePlay();

  GameState *update();
  void draw(BITMAP *bmp);

  Level *get_level();
  Player *get_player();
  Scorer *get_scorer();
  const ObjectList &get_people() const;
  const ObjectList &get_angels() const;

  void add_particle(Particle *particle);
  void add_particles(std::list<Particle *> particles);
  void add_particles_from_sprite(Sprite *sprite,
				 vector2d velmin, vector2d velmax,
				 vector2d accmin, vector2d accmax,
				 int timemin, int timemax,
				 int color_dest);

  void add_person(Person *person);
  void add_angel(Angel *person);

private:
  void update_layers();
  void draw_layers(BITMAP *bmp);
  void draw_menu(BITMAP *bmp);
  void start_level();
};


#endif // GAMEPLAY_H_INCLUDED
