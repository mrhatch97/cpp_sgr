/**
 *  cpp_sgr library.
 *
 *  @file sgr.hpp
 */

/*

  MIT License

  Copyright (c) 2018 Matthew Hatch

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

 */

#ifndef CPP_SGR_HPP
#define CPP_SGR_HPP

#include <exception>
#include <iostream>
#include <memory>
#include <string>

/**
 *  Library namespace.
 *
 *  @namespace cpp_sgr
 */

namespace cpp_sgr
{
  /**
   * Class representing a terminal SGR (Select Graphic Rendition).
   *
   * @class sgr
   * Wraps a std::string and provides operations for creating SGR escape
   * sequences using easy to remember mnemonics.
   *
   * See https://en.wikipedia.org/wiki/ANSI_escape_code#SGR_(Select_Graphic_Rendition)_parameters
   * for more information.
   */

  class sgr
  {

    /**
     * Combines the given SGRs into a single SGR.
     *
     * @param  left  Left SGR to be combined
     * @param  right Right SGR to be combined
     * @return     Combined SGR
     * @see operator,(const sgr & left, const sgr & right)
     */

    friend sgr operator+(const sgr & left, const sgr & right)
    {
      return sgr(left.str + ";" + right.str);
    }

    /**
     * Combines the given SGRs into a single SGR.
     *
     * @param  left  Left SGR to be combined
     * @param  right Right SGR to be combined
     * @return     Combined SGR
     * @see operator+(const sgr & left, const sgr & right)
     */

    friend sgr operator,(const sgr & left, const sgr & right)
    {
      return left + right;
    }

  public:

    sgr() = delete;
    ~sgr() = default;

    /**
     * Non-color SGR codes.
     */

    enum SGRCode { RESET = 0, BOLD = 1, FAINT = 2, ITALIC = 3, UNDERLINE = 4,
      BLINK_SLOW = 5, BLINK_FAST = 6, REVERSE = 7, CONCEAL = 8, STRIKE = 9,
      FRAME = 51, ENCIRCLE = 52, OVERLINE = 53 };

    /**
     * Construct an sgr with the given SGR code.
     *
     * @param code SGR code
     */

    sgr(const SGRCode code) : sgr(std::to_string(code)) { }

    /**
     * Retrieve the escape sequence string represented by this sgr.
     *
     * The std::string produced by this method can be directly printed,
     * but this does not provide the automatic reset functionality of
     * stream insertion of the sgr class. It is thus the programmer's
     * responsibility to print a reset SGR (or not).
     *
     * @return std::string representing this sgr
     */

    std::string toString() const
    {
      return escape + str + terminator;
    }

  protected:

    /**
     * Construct an SGR with the given std::string.
     *
     * This constructor does not verify that the string is a valid escape
     * sequence. Marked explicit to prevent automatic conversion of strings
     * to SGRs when not desired.
     *
     * @param str SGR string representation
     */

    explicit sgr(const std::string str) : str(str) { }

  private:
    static const std::string escape;
    static const std::string terminator;

    std::string str;
  };

  const std::string sgr::escape = "\033[";
  const std::string sgr::terminator = "m";

  const sgr reset = sgr(sgr::RESET); /**< Clear all SGRs */

  // most commonly supported SGRs
  const sgr underline = sgr(sgr::UNDERLINE); /**< Underlined text */
  const sgr bold = sgr(sgr::BOLD); /**< Bold text */
  const sgr reverse = sgr(sgr::REVERSE); /**< Swapped foreground and background colors */

  // rarely supported SGRs
  const sgr faint = sgr(sgr::FAINT); /**< Faint text */
  const sgr italic = sgr(sgr::ITALIC); /**< Italic text */
  const sgr blink_slow = sgr(sgr::BLINK_SLOW); /**< Slow-blinking text */
  const sgr blink_fast = sgr(sgr::BLINK_FAST); /**< Fast-blinking text */
  const sgr conceal = sgr(sgr::CONCEAL); /**< Concealed text */
  const sgr strike = sgr(sgr::STRIKE); /**< Struckthrough text */
  const sgr frame = sgr(sgr::FRAME); /**< Framed text */
  const sgr encircle = sgr(sgr::ENCIRCLE); /**< Encircled text */
  const sgr overline = sgr(sgr::OVERLINE); /**< Overlined text */

  /**
   * Exception indicating a color component outside the range [0,255] was
   * passed to a 24-bit color constructor.
   *
   * @class invalid_color_component
   */
  struct invalid_color_component : public std::exception
  {

    /**
     * Returns a text description of the error causing this exception.
     *
     * @return description of the error causing this exception
     */
    const char * what() const throw()
    {
      return "initialize 24-bit color sgr with color component outside [0,255]";
    }
  };

  /**
   * Class representing a terminal color SGR.
   *
   * @class color
   * See https://en.wikipedia.org/wiki/ANSI_escape_code#Colors
   * for more information.
   */
  class color : public sgr
  {
  public:

    color() = delete;
    ~color() = default;

    /**
     * 3/4 bit color codes.
     */
    enum ANSIColor {
      BLACK=30, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE,
      BRIGHT_BLACK=90, BRIGHT_RED, BRIGHT_GREEN, BRIGHT_YELLOW,
      BRIGHT_BLUE, BRIGHT_MAGENTA, BRIGHT_CYAN, BRIGHT_WHITE
    };

    /**
     * Construct a 3/4 bit foreground color SGR.
     *
     * @param  code 3/4 bit color code
     * @return      sgr to set the foreground to the given color
     */
    static color fg(const ANSIColor code)
    {
      return color(code, true);
    }

    /**
     * Construct a 24-bit foreground color SGR.
     *
     * The color components must be integers between 0 and 255 inclusive.
     *
     * @param  r 8-bit red component
     * @param  g 8-bit green component
     * @param  b 8-bit blue component
     * @return   sgr to set the foreground to the given color
     */
    static color fg(const int r, const int g, const int b)
    {
      return color(r, g, b, true);
    }

    /**
     * Construct a 3/4 bit background color SGR.
     *
     * @param  code 3/4 bit color code
     * @return      sgr to set the background to the given color
     */
    static const color bg(const ANSIColor code)
    {
      return color(code, false);
    }

    /**
     * Construct a 24-bit background color SGR.
     *
     * The color components must be integers between 0 and 255 inclusive.
     *
     * @param  r 8-bit red component
     * @param  g 8-bit green component
     * @param  b 8-bit blue component
     * @return   sgr to set the background to the given color
     */
    static color bg(const int r, const int g, const int b)
    {
      return color(r, g, b, false);
    }

  private:

    /**
     * Private constructor for 3/4 bit color SGRs.
     *
     * @param  code 3/4 bit color code
     * @param foreground true if foreground color, else false
     * @return      sgr to set the given color
     */
    color(const ANSIColor code, bool foreground) :
    sgr(std::to_string(foreground ? code : (code + 10)))
    {
    }

    /**
     * Private constructor for 24-bit color SGRs.
     *
     * The color components must be integers between 0 and 255 inclusive.
     *
     * @param  r 8-bit red component
     * @param  g 8-bit green component
     * @param  b 8-bit blue component
     * @param foreground true if foreground color, else false
     * @return      sgr to set the given color
     */
    color(const int r, const int g, const int b, bool foreground) :
    sgr((foreground ? "38;2;" : "48;2;") + std::to_string(r) + ";" +
    std::to_string(g) + ";" + std::to_string(b))
    {
      if(!(verifyColorComponent(r) && verifyColorComponent(g) &&
        verifyColorComponent(b)))
      {
        throw invalid_color_component();
      }
    }

    /**
     * Verify an RGB component in the range [0,255].
     *
     * @param  c 8-bit color component
     * @return   True if valid, else false
     */
    static bool verifyColorComponent(const int c) noexcept
    {
      return c <= 255 && c >= 0;
    }
  };

  // syntactic sugar - shorthands for the constructors

  const sgr black_fg = color::fg(color::BLACK); /**< Black foreground */
  const sgr red_fg = color::fg(color::RED); /**< Red foreground */
  const sgr green_fg = color::fg(color::GREEN); /**< Green foreground */
  const sgr yellow_fg = color::fg(color::YELLOW); /**< Yellow foreground */
  const sgr blue_fg = color::fg(color::BLUE); /**< Blue foreground */
  const sgr magenta_fg = color::fg(color::MAGENTA); /**< Magenta foreground */
  const sgr cyan_fg = color::fg(color::CYAN); /**< Cyan foreground */
  const sgr white_fg = color::fg(color::WHITE); /**< White foreground */

  const sgr b_black_fg = color::fg(color::BRIGHT_BLACK); /**< Bright black foreground */
  const sgr b_red_fg = color::fg(color::BRIGHT_RED); /**< Bright red foreground */
  const sgr b_green_fg = color::fg(color::BRIGHT_GREEN); /**< Bright green foreground */
  const sgr b_yellow_fg = color::fg(color::BRIGHT_YELLOW); /**< Bright yellow foreground */
  const sgr b_blue_fg = color::fg(color::BRIGHT_BLUE); /**< Bright blue foreground */
  const sgr b_magenta_fg = color::fg(color::BRIGHT_MAGENTA); /**< Bright magenta foreground */
  const sgr b_cyan_fg = color::fg(color::BRIGHT_CYAN); /**< Bright cyan foreground */
  const sgr b_white_fg = color::fg(color::BRIGHT_WHITE); /**< Bright white foreground */

  const sgr black_bg = color::bg(color::BLACK); /**< Black background */
  const sgr red_bg = color::bg(color::RED); /**< Red background */
  const sgr green_bg = color::bg(color::GREEN); /**< Green background */
  const sgr yellow_bg = color::bg(color::YELLOW); /**< Yellow background */
  const sgr blue_bg = color::bg(color::BLUE); /**< Blue background */
  const sgr magenta_bg = color::bg(color::MAGENTA); /**< Magenta background */
  const sgr cyan_bg = color::bg(color::CYAN); /**< Cyan background */
  const sgr white_bg = color::bg(color::WHITE); /**< White background */

  const sgr b_black_bg = color::bg(color::BRIGHT_BLACK); /**< Bright black background */
  const sgr b_red_bg = color::bg(color::BRIGHT_RED); /**< Bright red background */
  const sgr b_green_bg = color::bg(color::BRIGHT_GREEN); /**< Bright green background */
  const sgr b_yellow_bg = color::bg(color::BRIGHT_YELLOW); /**< Bright yellow background */
  const sgr b_blue_bg = color::bg(color::BRIGHT_BLUE); /**< Bright blue background */
  const sgr b_magenta_bg = color::bg(color::BRIGHT_MAGENTA); /**< Bright magenta background */
  const sgr b_cyan_bg = color::bg(color::BRIGHT_CYAN); /**< Bright cyan background */
  const sgr b_white_bg = color::bg(color::BRIGHT_WHITE); /**< Bright white background */

  /**
   * Wrapper for std::ostream that automatically clears SGRs when disposed
   *
   * @class sgr_ostream_wrapper
   * This class wraps std::ostream for SGR management; upon destruction,
   * it inserts the reset SGR into its underlying stream. Thus the user is
   * relieved of the need to explicitly clear SGRs from streams. For typical
   * usage, the ostream is destroyed at the end of a series of stream
   * insertions, so the SGR will be cleared after a single chain of insertions.
   */

  class sgr_ostream_wrapper
  {
  public:

    /**
     * Construct an sgr_ostream_wrapper by moving the provided std::ostream, and
     * insert the specified sgr into the newly created stream.
     *
     * @param stream Stream to be moved
     */
    sgr_ostream_wrapper(std::ostream & stream) :
      stream(stream.rdbuf()), shouldReset(true)
    {
    }

    /**
     * Construct an sgr_ostream_wrapper by taking ownership of the other stream's
     * internal std::ostream and assuming reset responsibility.
     *
     * @param other Stream to be moved
     */
    sgr_ostream_wrapper(sgr_ostream_wrapper && other) :
      stream(other.stream.rdbuf()), shouldReset(other.shouldReset)
    {
      other.shouldReset = false;
    }

    sgr_ostream_wrapper(const sgr_ostream_wrapper & other) = delete;

    /**
     * Destructor that inserts a reset SGR into the stream if necessary.
     */
    ~sgr_ostream_wrapper()
    {
      this->kill();
    }

    /**
     * Insert the given argument into the underlying std::ostream. T must
     * overload operator<<.
     *
     * @param t Object to insert into stream
     * @return Reference to this wrapper
     */
    template<class T>
    sgr_ostream_wrapper & operator<<(const T & t)
    {
      stream << t;
      return *this;
    }

  private:
    std::ostream stream;

    bool shouldReset = true;

    /**
     * Mark this stream as having been reset, and insert a reset sgr if
     * needed.
     */
    void kill()
    {
      if(shouldReset)
      {
        shouldReset = false;
        *this << reset.toString();
      }
    }
  };

  /**
   * Specialization of wrapper insertion for sgrs. Converts the given sgr into
   * its string representation and inserts it into the underlying stream.
   *
   * @param c sgr to insert into stream
   * @return Reference to this wrapper
   */
  template<>
  sgr_ostream_wrapper & sgr_ostream_wrapper::operator<<(const sgr & c)
  {
    stream << c.toString();
    return *this;
  }

  /**
   * Insert an sgr into a std::ostream, setting the active sgr.
   * This operation returns an sgr_ostream_wrapper, which will automatically
   * clear the active sgr when the stream being inserted into is destroyed.
   * The buffer of the std::ostream inserted into is referenced from the
   * returned wrapper from a new internal std::ostream.
   *
   * @param  out std::ostream to insert into
   * @param  c   sgr to insert
   * @return     sgr_ostream replacing the std::ostream
   */
  sgr_ostream_wrapper operator<<(std::ostream & out, const sgr & c)
  {
    sgr_ostream_wrapper wrapper(out);
    wrapper << c;
    return std::move(wrapper);
  }
}

#endif /* end of include guard: CPP_SGR_HPP */
