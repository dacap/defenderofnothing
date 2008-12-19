#ifndef MENU_HPP
#define MENU_HPP


// the menu
class Menu
{
public:
  Menu();
  virtual ~Menu();

  void update();
  void draw(BITMAP *bmp);
};


#endif // MENU_HPP
