//
//  copy.hpp
//  Graphics
//
//  Created by Indiana Kernick on 2/9/19.
//  Copyright © 2019 Indiana Kernick. All rights reserved.
//

#ifndef graphics_copy_hpp
#define graphics_copy_hpp

#include <cstring>
#include "region.hpp"
#include "traits.hpp"
#include "iterator.hpp"

namespace gfx {

template <typename Pixel>
bool copyRegion(
  const Surface<Pixel> dst,
  const CSurface<identity_t<Pixel>> src,
  const Point srcPos
) noexcept {
  return region(dst, src, srcPos, [](auto dstView, auto srcView) {
    copy(dstView, srcView);
  });
}

template <typename Pixel>
void copy(
  const Surface<Pixel> dst,
  const CSurface<identity_t<Pixel>> src
) noexcept {
  assert(dst.size() == src.size());
  const std::size_t width = dst.byteWidth();
  auto srcRowIter = begin(src);
  for (auto row : range(dst)) {
    std::memcpy(row.begin(), srcRowIter.begin(), width);
    ++srcRowIter;
  }
}

template <typename Pixel>
void overCopy(
  const Surface<Pixel> dst,
  const CSurface<identity_t<Pixel>> src
) noexcept {
  assert(dst.size() == src.size());
  assert(dst.pitch() == src.pitch());
  std::memcpy(dst.data(), src.data(), dst.wholeByteSize());
}

template <typename Pixel>
void patternCopy(
  const gfx::Surface<Pixel> dst, 
  const gfx::CSurface<gfx::identity_t<Pixel>> pat,
  Point patPos = {0, 0}
) noexcept {
  patPos.x %= pat.width();
  patPos.y %= pat.height();
  if (patPos.x < 0) patPos.x += pat.width();
  if (patPos.y < 0) patPos.y += pat.height();

  const auto patRowBeg = gfx::begin(pat);
  const auto patRowEnd = gfx::end(pat);
  auto patRowIter = patRowBeg + patPos.y;
  
  for (auto dstRow : dst) {
    const Pixel *const patColBeg = patRowIter.begin();
    const Pixel *const patColEnd = patRowIter.end();
    const Pixel *patColIter = patColBeg + patPos.x;
    
    for (Pixel &dstPixel : dstRow) {
      dstPixel = *patColIter;
      
      ++patColIter;
      if (patColIter == patColEnd) patColIter = patColBeg;
    }
    
    ++patRowIter;
    if (patRowIter == patRowEnd) patRowIter = patRowBeg;
  }
}

}

#endif
