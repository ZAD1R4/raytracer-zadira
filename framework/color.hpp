// -----------------------------------------------------------------------------
// Copyright  : (C) 2014-2017 Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// Color
// -----------------------------------------------------------------------------

#ifndef BUW_COLOR_HPP
#define BUW_COLOR_HPP

#include <iostream>

struct Color
{
  friend std::ostream& operator<<(std::ostream& os, Color const& c)
  {
    os << "(" << c.r << "," << c.g << "," << c.b << ")\n";
    return os;
  }

  Color& operator+=(Color const& other)
  {
    r += other.r;
    g += other.g;
    b += other.b;
    return *this;
  }

  Color& operator-=(Color const& other)
  {
    r -= other.r;
    g -= other.g;
    b -= other.b;
    return *this;
  }

  // NEU: Zuweisungsmultiplikation (Color *= Color)
  Color& operator*=(Color const& other)
  {
    r *= other.r;
    g *= other.g;
    b *= other.b;
    return *this;
  }

  // NEU: Skalarmultiplikation (Color *= float)
  Color& operator*=(float scalar)
  {
    r *= scalar;
    g *= scalar;
    b *= scalar;
    return *this;
  }

  friend Color operator+(Color const& a, Color const& b)
  {
    auto tmp(a);
    tmp += b;
    return tmp;
  }

  friend Color operator-(Color const& a, Color const& b)
  {
    auto tmp(a);
    tmp -= b;
    return tmp;
  }

  // NEU: Komponentenweise Multiplikation (Color * Color)
  friend Color operator*(Color const& a, Color const& b)
  {
    auto tmp(a);
    tmp *= b;
    return tmp;
  }

  // NEU: Skalarmultiplikation (Color * float)
  friend Color operator*(Color const& a, float scalar)
  {
    auto tmp(a);
    tmp *= scalar;
    return tmp;
  }

  // NEU: Skalarmultiplikation (float * Color)
  friend Color operator*(float scalar, Color const& a)
  {
    return a * scalar;
  }

  float r = 0.0f;
  float g = 0.0f;
  float b = 0.0f;
};

#endif //#define BUW_COLOR_HPP