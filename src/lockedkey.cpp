#include <allegro.h>
#include "lockedkey.hpp"


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
