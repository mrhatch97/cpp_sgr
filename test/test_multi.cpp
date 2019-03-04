#include <cpp_sgr/sgr.hpp>

#include <regex>
#include <sstream>
#include <string>

using namespace cpp_sgr;

int main()
{
  const std::regex chk_regex(
    R"REGEX(\x1b\[1;31;47mBold red string white background\x1b\[0m)REGEX");

  std::ostringstream stream;

  stream << (bold, red_fg, white_bg) << "Bold red string white background";

  const std::string target = stream.str();

  if(!std::regex_match(target, chk_regex))
  {
    return -1;
  }

  return 0;
}
