#pragma once

#include <cstdint>
#include <fstream>
#include <Marble/String.h>

#define FileInput(fileName) \
Marble::BasicString<char> input; \
{ \
    std::ifstream inputStream(fileName, std::ios::binary | std::ios::ate); \
    size_t len = inputStream.tellg(); \
    std::cout << fileName " text buffer size is " << len << "B." << std::endl; \
    input.resize(len); \
    inputStream.seekg(0, std::ios::beg); \
    inputStream.read(input.data(), len); \
    input.replace("\r\n", "\n"); \
    input.replace("\r", "\n"); /* In case some dickhead tries using CR files. */ \
    std::cout << fileName " text buffer contains: \n[\n" << input.insert(0, "\t").replace("\n", "\n\t") << "\n]." << std::endl; \
}

#define FileOutput(fileName, str) \
{ \
    std::ofstream outputStream(fileName, std::ios::beg); \
    std::cout << fileName " should contain \"" << str << "\"." << std::endl; \
    outputStream.write(str.data(), str.length()); \
}

using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;
using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;