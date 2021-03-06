// Defender Of Nothing
// Copyright (C) 2007 by David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include <allegro.h>
#include "menu.h"
#include "game.h"
#include "gfx.h"
#include "gameplay.h"
#include "media.h"

static const char *gametitle = "Defender Of Nothing";

Menu::Menu()
  : m_up(KEY_UP)
  , m_down(KEY_DOWN)
  , m_enter(KEY_ENTER)
  , m_esc(KEY_ESC)
{
  m_start_time = GAME_T;
  m_selected = 0;
  m_options = 4;

  m_logo = create_bitmap(text_length(gamefont, gametitle),
			 text_height(gamefont));
  clear_to_color(m_logo, bitmap_mask_color(m_logo));
  draw_text(m_logo, 0, 0, makecol(200, 180, 0), gametitle);
}

Menu::~Menu()
{
  destroy_bitmap(m_logo);
}

GameState *Menu::update()
{
  if (m_up.pressed()) {
    m_selected = m_selected == 0 ? m_options-1: --m_selected;
  }

  if (m_down.pressed()) {
    m_selected = m_selected == m_options-1 ? 0: ++m_selected;
  }

  if (m_enter.pressed()) {
    switch (m_selected) {
      case 0:
	return new GamePlay();
      case 1:
	gfx_widescreen = !gfx_widescreen;
	gfx_switched = true;
	break;
      case 2:
	gfx_fullscreen = !gfx_fullscreen;
	gfx_switched = true;
	break;
      case 3:
	return NULL;
    }
  }

  if (m_esc.pressed()) {
    return NULL;
  }

  return this;
}

void Menu::draw(BITMAP *bmp)
{
  static char *options[4] = { "play", "widescreen", "fullscreen", "quit" };
  int x = 32;
  int y = 64;

  clear_to_color(bmp, makecol(0, 0, 32));

  masked_stretch_blit(m_logo, bmp, 0, 0, m_logo->w, m_logo->h, 2, 2, m_logo->w*2, m_logo->h*2);

  for (int c=0; c<m_options; ++c) {
    Menu::draw_item(bmp, x, y, options[c], (m_selected == c));
    if ((c == 1 && gfx_widescreen) ||
	(c == 2 && gfx_fullscreen))
      circlefill(bmp, x-8, y+6, 2, makecol(190, 0, 0));
    y += 12;
    x -= 2;
  }

  draw_text_r(bmp, bmp->w-3, bmp->h-14, makecol(0, 180, 200), "(C) 2007 David Capello");

  draw_sprite(bmp, MEDIA_BITMAP(DISQUAL_PCX),
	      bmp->w-32-MEDIA_BITMAP(DISQUAL_PCX)->w,
	      bmp->h/2-MEDIA_BITMAP(DISQUAL_PCX)->h);
}

void Menu::draw_item(BITMAP *bmp, int x, int y, const char *text, bool selected)
{
  if (selected) {
    draw_text(bmp, x, y, makecol(190, 0, 0), text);
    draw_text(bmp, x+2, y-2, makecol(255, 255, 0), text);
  }
  else
    draw_text(bmp, x, y, makecol(190, 0, 0), text);
}
