//
//  format.hpp
//  Graphics
//
//  Created by Indiana Kernick on 2/9/19.
//  Copyright © 2019 Indiana Kernick. All rights reserved.
//

#ifndef graphics_format_hpp
#define graphics_format_hpp

#include "color.hpp"

namespace gfx {

template <auto Ch0, auto Ch1, auto Ch2, auto Ch3>
struct GenericRGBA {
  using Pixel = std::uint32_t;
  
  static_assert(Ch0 != Ch1 && Ch0 != Ch2 && Ch0 != Ch3);
  static_assert(Ch1 != Ch2 && Ch1 != Ch3);
  static_assert(Ch2 != Ch3);

  static constexpr Color color(const Pixel pixel) noexcept {
    Color color{0, 0, 0, 0};
    (color.*Ch0) = pixel >> 24;
    (color.*Ch1) = (pixel >> 16) & 0xFF;
    (color.*Ch2) = (pixel >> 8) & 0xFF;
    (color.*Ch3) = pixel & 0xFF;
    return color;
  }
  static constexpr Pixel pixel(const Color color) noexcept {
    return (Pixel{color.*Ch0} << 24) |
           (Pixel{color.*Ch1} << 16) |
           (Pixel{color.*Ch2} <<  8) |
            Pixel{color.*Ch3};
  }
  
  static constexpr std::uint8_t red(const Pixel pixel) noexcept {
    return color(pixel).r;
  }
  static constexpr std::uint8_t green(const Pixel pixel) noexcept {
    return color(pixel).g;
  }
  static constexpr std::uint8_t blue(const Pixel pixel) noexcept {
    return color(pixel).b;
  }
  static constexpr std::uint8_t alpha(const Pixel pixel) noexcept {
    return color(pixel).a;
  }
  
  static constexpr Pixel pixel(const int r, const int g, const int b, const int a = 255) noexcept {
    return pixel({
      static_cast<std::uint8_t>(r),
      static_cast<std::uint8_t>(g),
      static_cast<std::uint8_t>(b),
      static_cast<std::uint8_t>(a)
    });
  }
};

#define GFX_GENERIC_RGBA(CH0, CH1, CH2, CH3)                                    \
  ::gfx::GenericRGBA<                                                           \
    &::gfx::Color::CH0,                                                         \
    &::gfx::Color::CH1,                                                         \
    &::gfx::Color::CH2,                                                         \
    &::gfx::Color::CH3                                                          \
  >

using ARGB = GFX_GENERIC_RGBA(a, r, g, b);
using RGBA_LE = GFX_GENERIC_RGBA(a, b, g, r);
using RGBA_BE = GFX_GENERIC_RGBA(r, g, b, a);

template <typename Target = ARGB>
struct I {
  const typename Target::Pixel *data;
  
  using Pixel = std::uint8_t;
  
  Color color(const Pixel pixel) const noexcept {
    assert(data);
    return Target::color(data[pixel]);
  }
};

template <auto Ch0, auto Ch1>
struct GenericYA {
  using Pixel = std::uint16_t;
  
  static_assert(Ch0 != Ch1);
  static_assert(Ch0 == &Color::r || Ch0 == &Color::a);
  static_assert(Ch1 == &Color::r || Ch1 == &Color::a);
  
  static constexpr Color color(const Pixel pixel) noexcept {
    Color color{0, 0, 0, 0};
    (color.*Ch0) = pixel >> 8;
    (color.*Ch1) = pixel & 0xFF;
    color.g = color.r;
    color.b = color.r;
    return color;
  }
  static constexpr Pixel pixel(const Color color) noexcept {
    return (Pixel{color.*Ch0} << 8) | Pixel{color.*Ch1};
  }
  
  static constexpr std::uint8_t gray(const Pixel pixel) noexcept {
    return color(pixel).r;
  }
  static constexpr std::uint8_t alpha(const Pixel pixel) noexcept {
    return color(pixel).a;
  }
  
  static constexpr Pixel pixel(const int gray, const int alpha = 255) noexcept {
    return pixel({
      static_cast<std::uint8_t>(gray), 0, 0, static_cast<std::uint8_t>(alpha)
    });
  }
};

using AY = GenericYA<&Color::a, &Color::r>;
using YA = GenericYA<&Color::r, &Color::a>;
using YA_LE = AY;
using YA_BE = YA;

struct Y {
  using Pixel = std::uint8_t;
  
  static constexpr Color color(const Pixel pixel) noexcept {
    return {pixel, pixel, pixel, 255};
  }
  static constexpr Pixel pixel(const Color color) noexcept {
    return color.r;
  }
};

}

#endif
