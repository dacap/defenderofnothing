#ifndef OBJECT_HPP
#define OBJECT_HPP


// the object
class Object
{
public:
  Object() { }
  virtual ~Object() { }

  virtual void update() = 0;
  virtual void draw(BITMAP *bmp) = 0;

  // returns true if the object is dead and can be removed
  virtual bool is_dead() { return false; }
};


#endif // OBJECT_HPP
