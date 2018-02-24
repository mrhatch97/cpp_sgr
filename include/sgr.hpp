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
      return out << escape << c.str << "m";
    }

    /**
     * Chain SGRs with + operator
     *
     * @param  left  Left hand SGR operand
     * @param  right Right hand SGR operand
     * @return     Combined SGR
     */

    friend SGR operator+(const SGR & left, const SGR & right)
    {
      return SGR(left.str + "m" + escape + right.str);
    }

    /**
     * Chain SGRs with comma operator; this is an alternative to the + operator
     *
     * @param  left  Left hand SGR operand
     * @param  right Right hand SGR operand
     * @return     Combined SGR
     */

    friend SGR operator,(const SGR & left, const SGR & right)
    {
      return left + right;
    }

  public:

    enum SGRCode { RESET = 0, BOLD = 1, UNDERLINE = 4, REVERSE = 7,
      NO_BOLD = 22, NO_UNDERLINE = 24, NO_REVERSE = 27, FG_DEFAULT = 39,
      BG_DEFAULT = 49 };

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

    SGR(const std::string ansiString) : str(ansiString) { }

  private:
    class SGRSentinel
    {
    public:
      SGRSentinel() = default;

      /**
       * Destructor called at program termination (static sentinel is destroyed)
       * causing SGRs to be cleared on all standard ostreams
       */

      ~SGRSentinel();
    };

    static const std::string escape;

    static SGRSentinel sentinel;
    std::string str;
  };

  const std::string SGR::escape = "\033[";

  /**
   * "reset" SGR
   */

  const SGR reset = SGR(SGR::RESET);

  const SGR underline = SGR(SGR::UNDERLINE);
  const SGR bold = SGR(SGR::BOLD);
  const SGR reverse = SGR(SGR::REVERSE);
  const SGR no_underline = SGR(SGR::NO_UNDERLINE);
  const SGR no_bold = SGR(SGR::NO_BOLD);
  const SGR no_reverse = SGR(SGR::NO_REVERSE);
  const SGR fg_default = SGR(SGR::FG_DEFAULT);
  const SGR bg_default = SGR(SGR::BG_DEFAULT);

  SGR::SGRSentinel::~SGRSentinel()
  {
    std::cerr << reset;
    std::cout << reset;
    std::clog << reset;
  }

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

  // syntactic sugar - shorthands for the constructors

  const SGR black_fg = FGColor(Color::BLACK);
  const SGR red_fg = FGColor(Color::RED);
  const SGR green_fg = FGColor(Color::GREEN);
  const SGR yellow_fg = FGColor(Color::YELLOW);
  const SGR blue_fg = FGColor(Color::BLUE);
  const SGR magenta_fg = FGColor(Color::MAGENTA);
  const SGR cyan_fg = FGColor(Color::CYAN);
  const SGR white_fg = FGColor(Color::WHITE);

  const SGR b_black_fg = FGColor(Color::BRIGHT_BLACK);
  const SGR b_red_fg = FGColor(Color::BRIGHT_RED);
  const SGR b_green_fg = FGColor(Color::BRIGHT_GREEN);
  const SGR b_yellow_fg = FGColor(Color::BRIGHT_YELLOW);
  const SGR b_blue_fg = FGColor(Color::BRIGHT_BLUE);
  const SGR b_magenta_fg = FGColor(Color::BRIGHT_MAGENTA);
  const SGR b_cyan_fg = FGColor(Color::BRIGHT_CYAN);
  const SGR b_white_fg = FGColor(Color::BRIGHT_WHITE);

  const SGR black_bg = BGColor(Color::BLACK);
  const SGR red_bg = BGColor(Color::RED);
  const SGR green_bg = BGColor(Color::GREEN);
  const SGR yellow_bg = BGColor(Color::YELLOW);
  const SGR blue_bg = BGColor(Color::BLUE);
  const SGR magenta_bg = BGColor(Color::MAGENTA);
  const SGR cyan_bg = BGColor(Color::CYAN);
  const SGR white_bg = BGColor(Color::WHITE);

  const SGR b_black_bg = BGColor(Color::BRIGHT_BLACK);
  const SGR b_red_bg = BGColor(Color::BRIGHT_RED);
  const SGR b_green_bg = BGColor(Color::BRIGHT_GREEN);
  const SGR b_yellow_bg = BGColor(Color::BRIGHT_YELLOW);
  const SGR b_blue_bg = BGColor(Color::BRIGHT_BLUE);
  const SGR b_magenta_bg = BGColor(Color::BRIGHT_MAGENTA);
  const SGR b_cyan_bg = BGColor(Color::BRIGHT_CYAN);
  const SGR b_white_bg = BGColor(Color::BRIGHT_WHITE);
}

#endif /* end of include guard: CPP_SGR_HPP */
