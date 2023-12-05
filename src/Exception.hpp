#pragma once

#include <exception>
#include <string>

namespace MotionGraphics {
    class ExceptionBase: public std::exception {
    protected:
        std::string message_;
    public:
        ExceptionBase(std::string message): message_(message) {}
        virtual ~ExceptionBase() noexcept {}
        virtual const char* what() const noexcept {
            return message_.c_str();
        }
    };

    class InvalidFile: public ExceptionBase {
    public:
        std::string file_path;
        InvalidFile(std::string message, std::string file_path): ExceptionBase(message), file_path(file_path) {}
        virtual ~InvalidFile() noexcept {}
    };

    class OutOfMemory : public ExceptionBase
    {
    public:
        OutOfMemory(std::string message): ExceptionBase(message) {}
        virtual ~OutOfMemory() noexcept {}
    };

    class InvalidCodec: public ExceptionBase {
    public:
        std::string file_path;
        InvalidCodec(std::string message, std::string file_path): ExceptionBase(message), file_path(file_path) {}
        virtual ~InvalidCodec() noexcept {}
    };

    class InvalidFormat: public ExceptionBase {
    public:
        std::string file_path;
        InvalidFormat(std::string message, std::string file_path): ExceptionBase(message), file_path(file_path) {}
        virtual ~InvalidFormat() noexcept {}
    };

    class ErrorDecodingFrame: public ExceptionBase {
    public:
        std::string file_path;
        ErrorDecodingFrame(std::string message, std::string file_path): ExceptionBase(message), file_path(file_path) {}
        virtual ~ErrorDecodingFrame() noexcept {}
    };

    class NoStreamFound: public ExceptionBase {
    public:
        std::string file_path;
        NoStreamFound(std::string message, std::string file_path): ExceptionBase(message), file_path(file_path) {}
        virtual ~NoStreamFound() noexcept {}
    };
}
