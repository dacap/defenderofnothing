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

#include <allegro.h>
#include "input.h"

Input *the_input = NULL;

Input::Input()
{
  m_left = INPUT_KEY_LEFT;
  m_right = INPUT_KEY_RIGHT;
  m_up = INPUT_KEY_UP;
  m_down = INPUT_KEY_DOWN;
  m_fire = INPUT_KEY_LCONTROL;
  m_fire2 = INPUT_KEY_SPACE;
  m_target = INPUT_KEY_LSHIFT;
  m_demonize = INPUT_KEY_ALT;
}

Input::~Input()
{
}

//////////////////////////////////////////////////////////////////////
// mapper input_id -> joystick input

#define _INPUT_JOYSTICK_STICK(joy,stk)		\
    { joy, 0, stk, 0 },				\
    { joy, 1, stk, 0 },				\
    { joy, 0, stk, 1 },				\
    { joy, 1, stk, 1 },				\
    { joy, 0, stk, 2 },				\
    { joy, 1, stk, 2 }
#define _INPUT_JOYSTICK(joy)			\
    _INPUT_JOYSTICK_STICK(joy, 0),		\
    _INPUT_JOYSTICK_STICK(joy, 1),		\
    _INPUT_JOYSTICK_STICK(joy, 2),		\
    _INPUT_JOYSTICK_STICK(joy, 3),		\
    { joy, 2, 0, 0 },				\
    { joy, 2, 1, 0 },				\
    { joy, 2, 2, 0 },				\
    { joy, 2, 3, 0 },				\
    { joy, 2, 4, 0 },				\
    { joy, 2, 5, 0 },				\
    { joy, 2, 6, 0 },				\
    { joy, 2, 7, 0 },				\
    { joy, 2, 8, 0 },				\
    { joy, 2, 9, 0 },				\
    { joy, 2, 10, 0 },				\
    { joy, 2, 11, 0 },				\
    { joy, 2, 12, 0 },				\
    { joy, 2, 13, 0 },				\
    { joy, 2, 14, 0 },				\
    { joy, 2, 15, 0 }

static struct JOYMAP {
  int joy : 2;
  int type : 2;			// 0=stick d1, 1=stick d2, 2=button
  int index : 4;		// stick or button index
  int axis : 2;			// axis
} joymap[] = {
  _INPUT_JOYSTICK(0),
  _INPUT_JOYSTICK(1),
  _INPUT_JOYSTICK(2),
  _INPUT_JOYSTICK(3),
};

bool Input::check_input_state(int input_id)
{
  // keyboard
  if (input_id >= INPUT_KEY_FIRST && input_id <= INPUT_KEY_LAST)
    return key[input_id];
  // joysticks
  else if (input_id >= INPUT_JOY_FIRST && input_id <= INPUT_JOY_LAST) {
    JOYMAP *map = &joymap[input_id - INPUT_JOY_FIRST];
    if (map->joy < num_joysticks) {
      switch (map->type) {
	case 0: return joy[map->joy].stick[map->index].axis[map->axis].d1;
	case 1: return joy[map->joy].stick[map->index].axis[map->axis].d2;
	case 2: return joy[map->joy].button[map->index].b;
      }
    }
  }
  return false;
}
