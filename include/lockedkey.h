// Defender Of Nothing
// Copyright (C) 2007 by David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#ifndef LOCKEDKEY_H_INCLUDED
#define LOCKEDKEY_H_INCLUDED


class LockedKey
{
  int m_key;
  bool m_locked;

public:
  LockedKey(int k);
  bool pressed();
};


#endif // LOCKEDKEY_H_INCLUDED
