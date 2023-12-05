#pragma once

#include <string>
#include "Frame.hpp"

namespace MotionGraphics {
    struct ReaderInfo {
        std::string path;
        int64_t width;
        int64_t height;
        int64_t duration;
        int64_t fps;
    };

    class ReaderBase {
    public:
        explicit ReaderBase() = default;
        virtual ~ReaderBase() = default;

        virtual void Open() = 0;
        virtual void Close() = 0;
        virtual Frame GetFrame() = 0;

        MotionGraphics::ReaderInfo info;
    };
}