#ifndef SCORER_HPP
#define SCORER_HPP


#include "object.hpp"
#include "vector2d.hpp"


class Scorer : public Object
{
  int m_people_in_heaven;
  int m_people_in_hell;
  int m_people_in_earth;
  
public:
  Scorer();
  virtual ~Scorer();

  virtual void update();
  virtual void draw(BITMAP *bmp);

  void one_to_heaven();
  void one_to_hell();
  void one_to_earth();

private:
  void draw_minimap(BITMAP *bmp, int x, int y, int w, int h);
  void to_minimap(vector2d pos, int x, int y, int w, int h, int &u, int &v);
};

#endif
