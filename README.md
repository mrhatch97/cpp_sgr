cpp_sgr
======
[![pipeline status](https://gitlab.com/mrhatch/cpp_sgr/badges/master/pipeline.svg)](https://gitlab.com/mrhatch/cpp_sgr/commits/master) [Doxygen Reference](https://mrhatch.gitlab.io/cpp_sgr)

cpp_sgr is a library to ease the use of SGRs in programs producing output to
a terminal. SGRs (Select Graphic Renditions) are special character sequences
that signal the terminal emulator to change its behavior; this library is
concerned with SGRs controlling text styling and coloring. Thus it provides
an interface that abstracts the ugly magic strings specifying SGRs behind
convenient mnemonics, and automatically handles clearing SGRs when they
are no longer needed.

SGRs in action:

![SGR Demonstration](docs/img/demo.png)

## Basics

Since this library is designed with ease of use in mind, usage is simple.
To use non-color SGRs, simply find the name of the appropriate sgr constant
and insert it into a stream.

An example:
```cpp
std::cerr << cpp_sgr::bold << "This text is bold\n";
```

To apply multiple SGRs, combine them with the comma or + operator like so:
```cpp
std::cerr << (cpp_sgr::bold, cpp_sgr::underline) << "This text is bold and underlined\n";
std::cerr << cpp_sgr::italic + cpp_sgr::reverse << "This text is italic and reversed\n";
```
or simply insert multiple SGRs:
```cpp
std::cerr << cpp_sgr::bold << cpp_sgr::italic << "This text is bold and italic\n";
```

## Color
SGRs can color either the foreground (text) or background of terminal output.
Two types of color SGR are supported by this library, and their differences are
outlined below.

### 3/4-Bit Color

The original SGR specification allowed 3-bit color, enumerating 8 color
constants. This was later extended to 4-bit color, allowing the addition of
"bright" variants of the original 8 colors. Those colors are:

* Black
* Red
* Green
* Yellow
* Blue
* Magenta
* Cyan
* White

The actual rendering of these colors is determined by the terminal emulator
to which the output is sent, and the color scheme applied to it. Colors may
not even correspond to their name. Bright colors may be implemented as a bold
version of the normal color. For more precise color control, see the next
section.

Usage of 3/4-bit color is similar to non-color SGRs. Find the appropriate
mnemonic constant, and insert it into a stream.

Examples:
```cpp
std::cerr << cpp_sgr::red_fg << "This text is red\n";
std::cerr << cpp_sgr::b_green_bg << "This text has a bright green background";
```

### 24-bit Color

The SGR specification now allows the use of traditional 24-bit (RGB) colors to
exactly control what color is rendered. To use 24-bit color, the `fg(r, g, b)`
and `bg(r, g, b)` functions respectively create foreground and background colors
 with the specified red, green, and blue components.

 Example:
 ```cpp
 std::cerr << cpp_sgr::fg(255, 255, 0) << "This text is yellow\n";
 ```

## Clearing SGRs

The most convenient feature of this library is its ability to automatically
clear SGRs, preventing accidental bleed of SGR effects into text that was
not intended to be affected, even post-program-termination.

Insertion of an SGR into a std::ostream produces a modified ostream
that automatically inserts the reset SGR into itself upon its destruction.
Thus, the following is possible:
```cpp
std::cerr << cpp_sgr::red_fg << "This text is red\n";
std::cerr << "This text is not\n";
```

The ostream's lifetime ends with the final chained insertion; thus SGRs
are active only for the chain in which they are first inserted.

SGRs can of course be manually cleared, using the reset SGR:
```cpp
std::cerr << cpp_sgr::red_fg << "This text is red\n" << cpp_sgr::reset <<
"This text is not\n";
```

## Other Useful Information

### Windows Support
SGRs should work out of the box on Linux terminal emulators (e.g. Git Bash's 
MINGW terminal) on Windows.

To get SGRs working with the Windows Command Processor (`cmd.exe`), a call to
cpp_sgr::enable_vterm_processing is required before `cmd` will interpret
SGRs as commands rather than as plaintext. 

### Terminal Emulator Support
Of the non-color SGRs, `bold`, `underline`, and `reverse` are the most widely
supported by terminal emulators. YMMV when using other SGRs, as they may have
no effect or even completely unexpected effects.

### Unexpected Background Color Behavior
If strange behavior is experienced when using background colors,
this is (probably) not a library bug. The particular behavior is that
text that sets a background color may result in whitespace between the end
of the printed text and the end of the line (and even whitespace between text
on the next line and the end of that line, but not the text on that line) being
rendered in the specified background color, even though it should be reset. It
may also seem non-deterministic; sometimes this occurs, sometimes it does not.

This is actually a quirk of how terminals handle scrolling. See
 [this](https://stackoverflow.com/questions/30060029/colored-terminal-output-does-not-reset/30064906#30064906)
Stack Overflow answer for an explanation of the behavior. To avoid this issue,
avoid inserting newlines in text blocks with an active background SGR.

For example, replace this code:
```cpp
std::cerr << cpp_sgr::red_bg << "Some text\n";
```
with this:
```cpp
std::cerr << cpp_sgr::red_bg << "Some text";
std::cerr << "\n";
```
or this:
```cpp
std::cerr << cpp_sgr::red_bg << "Some text" << cpp_sgr::reset << "\n";
```

## Installation

To install this library for use in a project, simply copy `include/sgr.hpp` to a
project-specific include directory, or use CMake to install the library to the
system include directory and use find_package to import it into a CMake project.
Include `cpp_sgr/sgr.hpp` to make it available.

A demonstration program has been provided, built as `demo`, which showcases the
functionality of the library.
