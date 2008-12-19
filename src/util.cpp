#include <allegro.h>
#include "util.hpp"


std::string redir(const std::string &filename)
{
  char exe[1024], buf[1024];

  get_executable_name(exe, sizeof(exe));
  replace_filename(buf, exe, filename.c_str(), sizeof(buf));

  return std::string(buf);
}

