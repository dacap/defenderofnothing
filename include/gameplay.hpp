#ifndef GAMEPLAY_HPP
#define GAMEPLAY_HPP

#include <list>
#include "gamestate.hpp"
#include "lockedkey.hpp"


// current game-play state
#define GAMEPLAY	(static_cast<GamePlay *>(the_game->get_state()))


enum {
  LEVEL_LAYER,
  PEOPLE_LAYER,
  ANGELS_LAYER,
  PLAYER_LAYER,
  PARTICLES_LAYER,
  SCORER_LAYER,
  MAX_LAYERS
};


class Object;
class Level;
class Player;
class Scorer;
class Particle;
class Person;
class Angel;


typedef std::list<Object *> ObjectList;


// a game state
class GamePlay : public GameState
{
  Level *m_level;
  Player *m_player;
  Scorer *m_scorer;
  ObjectList m_layers[MAX_LAYERS];

  // quit menu
  LockedKey m_esc;
  int m_quit_time;

public:
  GamePlay();
  virtual ~GamePlay();

  GameState *update();
  void draw(BITMAP *bmp);

  Level *get_level();
  Player *get_player();
  Scorer *get_scorer();
  const ObjectList &get_people();
  const ObjectList &get_angels();

  void add_particle(Particle *particle);
  void add_particles(std::list<Particle *> particles);
  void add_person(Person *person);
  void add_angel(Angel *person);

private:
  void update_layers();
  void draw_layers(BITMAP *bmp);
};


#endif // GAMEPLAY_HPP
