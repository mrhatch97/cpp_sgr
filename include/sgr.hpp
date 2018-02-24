#ifndef CPP_SGR_HPP
#define CPP_SGR_HPP

#include <iostream>
#include <string>

namespace cpp_sgr
{
  /**
   * Class representing a terminal SGR (Select Graphic Rendition)
   *
   * See https://en.wikipedia.org/wiki/ANSI_escape_code#SGR_(Select_Graphic_Rendition)_parameters
   */

  class SGR
  {

    /**
     * Insert SGR into stream
     *
     * @param ansiString ANSI SGR string
     */

    friend std::ostream & operator<<(std::ostream & out, const SGR & c)
    {
      return out << c.str;
    }

  public:

    /**
     * "reset" SGR
     */

    static const SGR NONE;

    enum SGRCode { BOLD = 1, UNDERLINE = 4, REVERSE = 7, NO_BOLD = 22,
      NO_UNDERLINE = 24, NO_REVERSE = 27 };

    /**
     * Construct an SGR with the given standard SGR code
     *
     * @param code SGR parameter number
     */

    SGR(const SGRCode code) : SGR(std::to_string(code)) { }

  protected:

    /**
     * Construct an SGR with the given ANSI string - this is unchecked, so if
     * the string is invalid, this is not verified by the class
     *
     * @param ansiString ANSI SGR string
     */

    SGR(const std::string ansiString) : str(escape + ansiString + "m") { }

  private:
    class SGRSentinel
    {
    public:
      SGRSentinel() = default;

      /**
       * Destructor called at program termination (static sentinel is destroyed)
       * causing SGRs to be cleared on all standard ostreams
       */

      ~SGRSentinel()
      {
        std::cerr << SGR::NONE;
        std::cout << SGR::NONE;
        std::clog << SGR::NONE;
      }
    };

    static const std::string escape;

    static SGRSentinel sentinel;
    std::string str;
  };

  const std::string SGR::escape = "\033[";
  const SGR SGR::NONE = SGR("0");

  /**
   * Class representing a terminal color SGR
   */

  class Color : public SGR
  {
  public:
    enum ANSIColor {
      BLACK=30, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE,
      BRIGHT_BLACK=90, BRIGHT_RED, BRIGHT_GREEN, BRIGHT_YELLOW,
      BRIGHT_BLUE, BRIGHT_MAGENTA, BRIGHT_CYAN, BRIGHT_WHITE
    };

  protected:

    /**
     * For use only by subclasses - forwards string to SGR ctor
     *
     * @param ansiString string representing SGR code
     */

    Color(const std::string ansiString) : SGR(ansiString) { };

    /**
     * Verify an RGB component is valid (i.e. in [0, 255])
     *
     * @param  c color component
     * @return   True if valid, else false
     */

    static bool verifyColorComponent(const int c) noexcept
    {
      return c <= 255 && c >= 0;
    }
  };

  /**
   * SGR to set terminal foreground color
   */

  class FGColor : public Color
  {
  public:

    /**
     * Construct a foreground color SGR from the given ANSI color constant
     *
     * @param c ANSI color constant
     */

     FGColor(const ANSIColor code) : Color(std::to_string(code)) { }

    /**
     * Construct a foreground color SGR from the given 8-bit RGB values
     *
     * @param r red component
     * @param g green component
     * @param b blue component
     */

     FGColor(const int r, const int g, const int b) :
     Color("38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";"
     + std::to_string(b))
     {
       if(!(verifyColorComponent(r) && verifyColorComponent(g) &&
         verifyColorComponent(b)))
       {
         throw std::runtime_error("Invalid color component");
       }
     }
  };

  /**
   * SGR to set terminal background color
   */

  class BGColor : public Color
  {
  public:

    /**
     * Construct a background color SGR from the given ANSI color constant
     *
     * @param c ANSI color constant
     */

     BGColor(const ANSIColor code) : Color(std::to_string(code + 10))
     {
     }

    /**
     * Construct a background color SGR from the given 8-bit RGB values
     *
     * @param r red component
     * @param g green component
     * @param b blue component
     */

     BGColor(const int r, const int g, const int b) :
     Color("48;2;" + std::to_string(r) + ";" + std::to_string(g) + ";"
     + std::to_string(b))
     {
       if(!(verifyColorComponent(r) && verifyColorComponent(g) &&
         verifyColorComponent(b)))
       {
         throw std::runtime_error("Invalid color component");
       }
     }
  };
}

#endif /* end of include guard: CPP_SGR_HPP */
