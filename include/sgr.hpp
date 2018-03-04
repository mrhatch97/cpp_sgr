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

  class sgr
  {

    /**
     * Combines the given SGRs into a single SGR; this is an alternative to the
     * comma operator
     *
     * @param  left  Left hand SGR operand
     * @param  right Right hand SGR operand
     * @return     Combined SGR
     * @see operator,
     */

    friend sgr operator+(const sgr & left, const sgr & right)
    {
      return sgr(left.str + ";" + right.str);
    }

    /**
     * Combines the given SGRs into a single SGR; this is an alternative to the
     * + operator
     *
     * @param  left  Left hand SGR operand
     * @param  right Right hand SGR operand
     * @return     Combined SGR
     * @see operator+
     */

    friend sgr operator,(const sgr & left, const sgr & right)
    {
      return left + right;
    }

  public:

    sgr() = delete;
    ~sgr() = default;

    /**
     * Enumeration of non-color SGR codes
     *
     * @param code [description]
     */

    enum SGRCode { RESET = 0, BOLD = 1, FAINT = 2, ITALIC = 3, UNDERLINE = 4,
      BLINK_SLOW = 5, BLINK_FAST = 6, REVERSE = 7, CONCEAL = 8, STRIKE = 9,
      FRAME = 51, ENCIRCLE = 52, OVERLINE = 53 };

    /**
     * Construct an SGR with the given standard SGR code
     *
     * @param code SGR parameter number
     */

    sgr(const SGRCode code) : sgr(std::to_string(code)) { }

    /**
     * Convert the SGR to a string suitable for output
     *
     * @return String representation of the SGR
     */

    std::string toString() const
    {
      return escape + str + terminator;
    }

  protected:

    /**
     * Construct an SGR with the given ANSI string - this is unchecked, so if
     * the string is invalid, this is not verified by the class
     *
     * @param ansiString ANSI SGR string
     */

    explicit sgr(const std::string ansiString) : str(ansiString) { }

  private:
    static const std::string escape;
    static const std::string terminator;

    std::string str;
  };

  const std::string sgr::escape = "\033[";
  const std::string sgr::terminator = "m";

  /**
   * "reset" SGR
   */

  const sgr reset = sgr(sgr::RESET);

  // most commonly supported SGRs
  const sgr underline = sgr(sgr::UNDERLINE);
  const sgr bold = sgr(sgr::BOLD);
  const sgr reverse = sgr(sgr::REVERSE);

  // rarely supported SGRs
  const sgr faint = sgr(sgr::FAINT);
  const sgr italic = sgr(sgr::ITALIC);
  const sgr blink_slow = sgr(sgr::BLINK_SLOW);
  const sgr blink_fast = sgr(sgr::BLINK_FAST);
  const sgr conceal = sgr(sgr::CONCEAL);
  const sgr strike = sgr(sgr::STRIKE);
  const sgr frame = sgr(sgr::FRAME);
  const sgr encircle = sgr(sgr::ENCIRCLE);
  const sgr overline = sgr(sgr::OVERLINE);

  /**
   * Class representing a terminal color SGR
   */

  class color : public sgr
  {
  public:

    color() = delete;
    ~color() = default;

    /**
     * Enumeration of 3/4 bit color codes
     */

    enum ANSIColor {
      BLACK=30, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE,
      BRIGHT_BLACK=90, BRIGHT_RED, BRIGHT_GREEN, BRIGHT_YELLOW,
      BRIGHT_BLUE, BRIGHT_MAGENTA, BRIGHT_CYAN, BRIGHT_WHITE
    };

    /**
     * Construct a 3/4 bit foreground color SGR
     *
     * @param  code 3/4 bit color code
     * @return      SGR to set the foreground to the given color
     */

    static sgr fg(const ANSIColor code)
    {
      return color(code, true);
    }

    /**
     * Construct a 24-bit foreground color SGR
     *
     * @param  r 8-bit red component
     * @param  g 8-bit green component
     * @param  b 8-bit blue component
     * @return   SGR to set the foreground to the given color
     */

    static sgr fg(const int r, const int g, const int b)
    {
      return color(r, g, b, true);
    }

    /**
     * Construct a 3/4 bit background color SGR
     *
     * @param  code 3/4 bit color code
     * @return      SGR to set the background to the given color
     */

    static sgr bg(const ANSIColor code)
    {
      return color(code, false);
    }

    /**
     * Construct a 24-bit background color SGR
     *
     * @param  r 8-bit red component
     * @param  g 8-bit green component
     * @param  b 8-bit blue component
     * @return   SGR to set the background to the given color
     */

    static sgr bg(const int r, const int g, const int b)
    {
      return color(r, g, b, false);
    }

  private:

    /**
     * Private constructor for 3/4 bit color SGRs
     *
     * @param  code 3/4 bit color code
     * @param foreground true if foreground color, else false
     * @return      SGR to set the given color
     */

    color(const ANSIColor code, bool foreground) :
    sgr(std::to_string(foreground ? code : (code + 10)))
    {
    }

    /**
     * Private constructor for 24-bit color SGRs
     *
     * @param  r 8-bit red component
     * @param  g 8-bit green component
     * @param  b 8-bit blue component
     * @param foreground true if foreground color, else false
     * @return      SGR to set the given color
     */

    color(const int r, const int g, const int b, bool foreground) :
    sgr((foreground ? "38;2;" : "48;2;") + std::to_string(r) + ";" +
    std::to_string(g) + ";" + std::to_string(b))
    {
      if(!(verifyColorComponent(r) && verifyColorComponent(g) &&
        verifyColorComponent(b)))
      {
        throw std::runtime_error("Invalid color component");
      }
    }

    /**
     * Verify an RGB component is valid (between [0,255])
     *
     * @param  c color component
     * @return   True if valid, else false
     */

    static bool verifyColorComponent(const int c) noexcept
    {
      return c <= 255 && c >= 0;
    }
  };

  // syntactic sugar - shorthands for the constructors

  const sgr black_fg = color::fg(color::BLACK);
  const sgr red_fg = color::fg(color::RED);
  const sgr green_fg = color::fg(color::GREEN);
  const sgr yellow_fg = color::fg(color::YELLOW);
  const sgr blue_fg = color::fg(color::BLUE);
  const sgr magenta_fg = color::fg(color::MAGENTA);
  const sgr cyan_fg = color::fg(color::CYAN);
  const sgr white_fg = color::fg(color::WHITE);

  const sgr b_black_fg = color::fg(color::BRIGHT_BLACK);
  const sgr b_red_fg = color::fg(color::BRIGHT_RED);
  const sgr b_green_fg = color::fg(color::BRIGHT_GREEN);
  const sgr b_yellow_fg = color::fg(color::BRIGHT_YELLOW);
  const sgr b_blue_fg = color::fg(color::BRIGHT_BLUE);
  const sgr b_magenta_fg = color::fg(color::BRIGHT_MAGENTA);
  const sgr b_cyan_fg = color::fg(color::BRIGHT_CYAN);
  const sgr b_white_fg = color::fg(color::BRIGHT_WHITE);

  const sgr black_bg = color::bg(color::BLACK);
  const sgr red_bg = color::bg(color::RED);
  const sgr green_bg = color::bg(color::GREEN);
  const sgr yellow_bg = color::bg(color::YELLOW);
  const sgr blue_bg = color::bg(color::BLUE);
  const sgr magenta_bg = color::bg(color::MAGENTA);
  const sgr cyan_bg = color::bg(color::CYAN);
  const sgr white_bg = color::bg(color::WHITE);

  const sgr b_black_bg = color::bg(color::BRIGHT_BLACK);
  const sgr b_red_bg = color::bg(color::BRIGHT_RED);
  const sgr b_green_bg = color::bg(color::BRIGHT_GREEN);
  const sgr b_yellow_bg = color::bg(color::BRIGHT_YELLOW);
  const sgr b_blue_bg = color::bg(color::BRIGHT_BLUE);
  const sgr b_magenta_bg = color::bg(color::BRIGHT_MAGENTA);
  const sgr b_cyan_bg = color::bg(color::BRIGHT_CYAN);
  const sgr b_white_bg = color::bg(color::BRIGHT_WHITE);

  /**
   * Extension of std::ostream to allow automatic clearing of SGRs after a print
   * is complete
   */

  class sgr_ostream : public std::ostream
  {
  public:
    /**
     * Construct an sgr_ostream using the buffer of the given ostream and
     * inserting the given SGR into that stream
     *
     * @param stream Stream to replace
     * @param sgr    SGR to insert into stream
     */
    sgr_ostream(std::ostream & stream, const sgr & sgr) :
    std::ostream(stream.rdbuf())
    {
      *this << sgr.toString();
    }

    /**
     * Copy constructor
     *
     * @param stream sgr_ostream to copy - no reinsertion is performed
     */

    sgr_ostream(const sgr_ostream & stream) :
    std::ostream(stream.rdbuf())
    {
    }

    /**
     * Destructor - flushes the stream with a reset sequence
     */

    ~sgr_ostream()
    {
      this->kill();
    }

    /**
     * If another SGR is inserted into this sgr_ostream, replace this stream's
     * SGR with the new one
     */

    sgr_ostream operator<<(const sgr & c)
    {
      // By pre-emptively killing this stream, we ensure the reset occurs before
      // the new stream is created; without this, the second insertion occurs
      // before this stream is destroyed, causing its SGR to be reset
      this->kill();
      return sgr_ostream(*this, c);
    }

  private:
    bool dead = false;

    /**
     * Mark this stream as dead and wipe the SGR applied to it
     */

    void kill()
    {
      if(!dead)
      {
        dead = true;
        *this << reset.toString();
      }
    }
  };

  /**
   * Insert an SGR into an ostream, returning an sgr_ostream which
   * manages clearing the SGR from the stream automatically
   *
   * @param  out ostream in which to insert
   * @param  c   SGR to insert
   * @return     sgr_ostream replacing the ostream
   */

  sgr_ostream operator<<(std::ostream & out, const sgr & c)
  {
    return sgr_ostream(out, c);
  }
}

#endif /* end of include guard: CPP_SGR_HPP */
