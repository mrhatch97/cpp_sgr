#include <cpp_sgr/sgr.hpp>

#include <regex>
#include <sstream>
#include <string>

using namespace cpp_sgr;

int main()
{
  const std::regex chk_regex(R"REGEX(\x1b\x5b1mBold string\x1b\x5b0m)REGEX");

  std::ostringstream stream;

  stream << bold << "Bold string";

  const std::string target = stream.str();

  if(!std::regex_match(target, chk_regex))
  {
    return -1;
  }

  return 0;
}
