#pragma once

#include <cstdint>

namespace MotionGraphics {
    struct Frame {
        unsigned char* data;
        std::uint32_t height;
        std::uint32_t width;
        std::uint32_t line;
    };
}