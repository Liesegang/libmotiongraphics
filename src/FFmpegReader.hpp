#pragma once

#include <string>

#include "ReaderBase.hpp"

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavutil/timestamp.h>
    #include <libavutil/opt.h>
}

namespace MotionGraphics {
    class FFmpegReader: public ReaderBase {
    public:
        FFmpegReader(std::string path);
        virtual ~FFmpegReader() = default;

        void Open() override;
        void Close() override;
        Frame GetFrame() override;
    private:
        AVFormatContext* format_context_;
        AVCodec* codec_;
        AVCodecParameters* codec_parameters_;
        int video_stream_index_;
        AVCodecContext* codec_context_;
        Frame DecodePacket(AVPacket *pPacket, AVCodecContext *pCodecContext, AVFrame *pFrame);
    };
}