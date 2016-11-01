// Defender Of Nothing
// Copyright (C) 2007 by David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include <allegro.h>
#include "lockedkey.h"

LockedKey::LockedKey(int k)
{
  m_key = k;
  m_locked = key[m_key] != 0 ? true: false;
}

bool LockedKey::pressed()
{
  if (key[m_key]) {
    if (!m_locked) {
      m_locked = true;
      return true;
    }
  }
  else if (m_locked)
    m_locked = false;

  return false;
}
