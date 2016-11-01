// Defender Of Nothing
// Copyright (C) 2007 by David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include <allegro.h>
#include "util.h"

std::string redir(const std::string &filename)
{
  char exe[1024], buf[1024];

  get_executable_name(exe, sizeof(exe));
  replace_filename(buf, exe, filename.c_str(), sizeof(buf));

  return std::string(buf);
}
