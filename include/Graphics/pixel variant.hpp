//
//  pixel variant.hpp
//  Graphics
//
//  Created by Indiana Kernick on 14/6/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef graphics_pixel_variant_hpp
#define graphics_pixel_variant_hpp

#include <type_traits>

namespace gfx {

namespace detail {

template <typename... Ts>
struct largest;

template <typename T>
struct largest<T> {
  using type = T;
};

template <typename A, typename B, typename... Rest>
struct largest<A, B, Rest...> {
  using type = typename largest<
    std::conditional_t<sizeof(A) < sizeof(B), B, A>, Rest...
  >::type;
};

}

/// An untagged variant type for pixels
template <typename... Pixels>
class PixelVariant {
public:
  template <typename Pixel>
  static constexpr bool valid = (std::is_same_v<Pixel, Pixels> || ...);

  using Underlying = typename detail::largest<Pixels...>::type;

  constexpr PixelVariant() noexcept
    : pixel{} {}

  template <typename Pixel, typename = std::enable_if_t<valid<Pixel>>>
  constexpr explicit PixelVariant(const Pixel pixel) noexcept
    : pixel{pixel} {}
  
  template <typename Pixel, typename = std::enable_if_t<valid<Pixel>>>
  constexpr explicit operator Pixel() const noexcept {
    return static_cast<Pixel>(pixel);
  }
  
  bool zero() const noexcept {
    return pixel == 0;
  }
  
  Underlying &underlying() noexcept {
    return pixel;
  }
  const Underlying &underlying() const noexcept {
    return pixel;
  }
  
  bool operator==(const PixelVariant other) const noexcept {
    return pixel == other.pixel;
  }
  bool operator!=(const PixelVariant other) const noexcept {
    return pixel != other.pixel;
  }

private:
  Underlying pixel;
};

}

#endif
