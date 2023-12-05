#include <iostream>

#include "FFmpegReader.hpp"
#include "Exception.hpp"

namespace MotionGraphics
{
  FFmpegReader::FFmpegReader(std::string path)
  {
    info.path = path;
  }

  void FFmpegReader::Open()
  {
    format_context_ = avformat_alloc_context();
    if (!format_context_) {
        throw OutOfMemory("could not allocate memory for Format Context.");
    }

    if (avformat_open_input(&format_context_, info.path.c_str(), nullptr, nullptr) != 0) {
        throw InvalidFile("could not open the file.", info.path);
    }

    if (avformat_find_stream_info(format_context_, nullptr) < 0) {
        throw InvalidFormat("could not get the stream info", info.path);
    }

    codec_ = nullptr;
    codec_parameters_ = nullptr;
    video_stream_index_ = -1;
    for (int i = 0; i < format_context_->nb_streams; i++)
    {
      AVCodecParameters* local_codec_parameters = nullptr;
      local_codec_parameters = format_context_->streams[i]->codecpar;
      AVCodec* local_codec = nullptr;
      local_codec = avcodec_find_decoder(local_codec_parameters->codec_id);
      if (local_codec == nullptr)
      {
        continue;
      }
      if (local_codec_parameters->codec_type == AVMEDIA_TYPE_VIDEO)
      {
        if (video_stream_index_ == -1)
        {
          video_stream_index_ = i;
          codec_ = local_codec;
          codec_parameters_ = local_codec_parameters;
        }
      }
      else if (local_codec_parameters->codec_type == AVMEDIA_TYPE_AUDIO)
      {
      }
    }

    if (video_stream_index_ == -1) {
      throw NoStreamFound("does not contain a video stream.", info.path);
    }

    codec_context_ = avcodec_alloc_context3(codec_);
    if (!codec_context_) {
      throw OutOfMemory("failed to allocate memory for AVCodecContext.");
    }

    if (avcodec_parameters_to_context(codec_context_, codec_parameters_) < 0) {
      throw InvalidCodec("failed to copy codec params to codec context", info.path);
    }

    if (avcodec_open2(codec_context_, codec_, nullptr) < 0) {
      throw InvalidCodec("failed to open codec through avcodec_open2", info.path);
    }
  }

  Frame FFmpegReader::GetFrame() {
    AVFrame* frame = av_frame_alloc();
    if (!frame) {
      throw OutOfMemory("failed to allocate memory for AVFrame");
    }

    AVPacket* packet = av_packet_alloc();
    if (!packet) {
      throw OutOfMemory("failed to allocate memory for AVPacket");
    }

    Frame response;
    av_read_frame(format_context_, packet);
    while (av_read_frame(format_context_, packet) >= 0) {
      if (packet->stream_index == video_stream_index_) {
        response = DecodePacket(packet, codec_context_, frame);
        break;
      }
      av_packet_unref(packet);
    }
    av_packet_free(&packet);
    av_frame_free(&frame);
    return response;
  }

  void FFmpegReader::Close() {
    avformat_close_input(&format_context_);
    avcodec_free_context(&codec_context_);
  }

  Frame FFmpegReader::DecodePacket(AVPacket *pPacket, AVCodecContext *pCodecContext, AVFrame *pFrame) {
    int response = avcodec_send_packet(pCodecContext, pPacket);
    Frame f;
    f.data = nullptr;

    if (response < 0) {
      throw ErrorDecodingFrame("error while sending a packet to the decoder.", info.path);
    }

    while (response >= 0)
    {
      response = avcodec_receive_frame(pCodecContext, pFrame);
      if (response == AVERROR(EAGAIN)) {
        continue;
      }
      if (response == AVERROR_EOF) {
        throw ErrorDecodingFrame("error while sending a packet to the decoder2." + std::to_string(response), info.path);
      } else if (response < 0) {
        throw ErrorDecodingFrame("error while sending a packet to the decoder3.", info.path);
      }

      if (response >= 0) {
        Frame f;
        f.data = reinterpret_cast<unsigned char*>(pFrame->data[0]); 
        f.width = pFrame->width;
        f.height = pFrame->height;
        f.line = pFrame->linesize[0];
        return f;
      }
    }
    return f;
  }
}