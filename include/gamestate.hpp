#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP


// a game state
class GameState
{
public:
  GameState();
  virtual ~GameState() = 0;

  // update the logic of the game and returns the new (or this) state
  // note: you can return NULL if the game ends completelly (e.g. return to OS)
  virtual GameState *update() = 0;

  // draw the game in the specified bitmap
  virtual void draw(BITMAP *bmp) = 0;

};


#endif // GAME_HPP
