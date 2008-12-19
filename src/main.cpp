#include <allegro.h>
#include "game.hpp"
#include "media.hpp"
#include "input.hpp"


//////////////////////////////////////////////////////////////////////
// a timer to control that beats increments 60 times per second

static volatile int beats = 0;

static void timer_control()
{
  ++beats;
}
END_OF_STATIC_FUNCTION(timer_control);


//////////////////////////////////////////////////////////////////////
// routine to be called when the close button of the window is pressed

static volatile bool continuing = true;

static void close_button()
{
  continuing = false;
}
END_OF_STATIC_FUNCTION(close_button);


//////////////////////////////////////////////////////////////////////
// the entry point of the game

static void game_loop();

int main()
{
  allegro_init();
  install_timer();
  install_keyboard();
  install_mouse();
  install_joystick(JOY_TYPE_AUTODETECT);

  if (!setup_gfx()) {
    allegro_message("Unable to setup the graphics mode\n");
    return 1;
  }

//   if (gfx_capabilities & GFX_HW_CURSOR) {
//     enable_hardware_cursor();
//     select_mouse_cursor(MOUSE_CURSOR_ARROW);
//     show_mouse(screen);
//   }

  if (!load_media()) {
    allegro_message("Unable to load data files to play the game\n");
    return 1;
  }

  // install the timer to control the game speed
  LOCK_VARIABLE(beats);
  LOCK_FUNCTION(timer_control);

  beats = 0;
  install_int_ex(timer_control, BPS_TO_TIMER(BPS));

  // insert the callback routine for the close-button
  LOCK_VARIABLE(continuing);
  LOCK_FUNCTION(close_button);
  
  set_close_button_callback(close_button);

  // play the game
  game_loop();
  
  remove_int(timer_control);
  allegro_exit();
  return 0;
}

END_OF_MAIN();


//////////////////////////////////////////////////////////////////////
// all the game loop (intro/menu/game/etc.)

static void game_loop()
{
  BITMAP *buffer = create_bitmap(GFX_W, GFX_H);

  // initialize the player input
  the_input = new Input();

  // start the game
  the_game = new Game();
  the_game->start();

  while (continuing) {
    while (beats > 0 && continuing) {
      // update game logic
      continuing = the_game->update();
      --beats;
    }
    the_game->draw(buffer);

    if (key[KEY_F12]) {
      make_screenshot(buffer);
      do {
	poll_keyboard();
      } while (key[KEY_F12]);
    }

    // flip to screen (double buffering technique)
    vsync();
    stretch_blit(buffer, screen, 0, 0, GFX_W, GFX_H, 0, 0, SCREEN_W, SCREEN_H);
  }

  // destroy the game
  delete the_game;
  the_game = NULL;

  delete the_input;

  destroy_bitmap(buffer);
}
