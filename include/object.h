// Defender Of Nothing
// Copyright (C) 2007 by David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED


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


#endif // OBJECT_H_INCLUDED
