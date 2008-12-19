// Defender Of Nothing
// Copyright (C) 2007 by David Capello
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
// * Neither the name of the author nor the names of its contributors
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
