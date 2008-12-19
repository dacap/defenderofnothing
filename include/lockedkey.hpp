#ifndef LOCKEDKEY_HPP
#define LOCKEDKEY_HPP


class LockedKey
{
  int m_key;
  bool m_locked;

public:
  LockedKey(int k);
  bool pressed();
};


#endif // LOCKEDKEY_HPP
