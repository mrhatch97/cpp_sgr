#include "sgr.hpp"

#include <iomanip>
#include <iostream>
#include <string>

using namespace cpp_sgr;

/**
 * Demonstrate the standard non-color SGR codes
 */

void codeTest()
{
    std::cout << (bold, underline) << "Testing common SGR codes:\n";
    std::cout << (bold) << "Bold text\n";
    std::cout << (underline) << "Underlined text\n";
    std::cout << (reverse) << "Reversed text\n\n";
}

/**
 * Demonstrate the 3/4-bit color SGRs
 */

void ansiTest()
{
    std::cout << (bold, underline) << "Testing colors:\n";
    std::cout << (red_fg) << "Red foreground";
    std::cout << "\n";
    std::cout << (cyan_bg) << "Cyan fackground";
    std::cout << "\n";
    std::cout << (white_fg, black_bg) << "White foreground, black background";
    std::cout << "\n";
    std::cout << (blue_fg) << "Blue foreground";
    std::cout << "\n";
    std::cout << (b_green_fg) << "Bright green foreground";
    std::cout << "\n\n";
}

/**
 * Demonstrate 24-bit color SGRs
 */

void iteratorTest()
{
    std::cout << (bold, underline) << "Testing 24-bit color:\n";

    for(int r = 0; r <= 255; r+=51)
    {
      for(int g = 0; g <= 255; g+=51)
      {
        for(int b = 0; b <= 255; b+=51)
        {
          int color = (b + (g << 8) + (r << 16));
          std::cout << color::fg(r,g,b) + bold << std::setfill('0') <<
          std::showbase << std::setw(8) << std::hex << std::internal << color;
          std::cout << " ";
        }
        std::cout << "\n";
      }
      std::cout << "\n";
    }

}

/**
 * Demonstrate SGRs that are not commonly supported; results may vary
 * depending on terminal emulator.
 */

void exoticTest()
{
  std::cout << (bold, underline) << "Testing unusual codes:\n";

  std::cout << (faint) << "Faint";
  std::cout << "\n";
  std::cout << (italic) << "Italic";
  std::cout << "\n";
  std::cout << (blink_slow) << "Blinking slowly";
  std::cout << "\n";
  std::cout << (blink_fast) << "Blinking quickly";
  std::cout << "\n";
  std::cout << (conceal) << "Concealed";
  std::cout << " (concealed)";
  std::cout << "\n";
  std::cout << (strike) << "Crossed out";
  std::cout << "\n";
  std::cout << (frame) << "Framed";
  std::cout << "\n";
  std::cout << (encircle) << "Encircled";
  std::cout << "\n";
  std::cout << (overline) << "Overlined";
  std::cout << "\n";
}

int main(int argc, char * * argv)
{
    codeTest();
    ansiTest();
    iteratorTest();
    exoticTest();

    return 0;
}
