#pragma once

#include <cstdint>
#include <fstream>
#include <Marble/String.h>

namespace std
{
    namespace chrono
    {
    }
    namespace filesystem
    {
    }
    namespace ranges
    {
    }
    namespace literals
    {
        namespace chrono_literals
        {
        }
    }
}

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
    input.replace("\r", "\n"); /* In case some *nice person* tries using CR files. */ \
    std::cout << fileName " text buffer contains: \n[\n" << string(input).insert(0, "\t").replace("\n", "\n\t") << "\n]." << std::endl; \
} ""

#define FileOutput(fileName, str) \
{ \
    std::ofstream outputStream(fileName, std::ios::beg); \
    std::cout << fileName " should contain \n[\n" << string(str).insert(0, "\t").replace("\n", "\n\t") << "\n]." << std::endl; \
    outputStream.write(str.data(), str.length()); \
} ""

#define __sbyte int8_t
#define __short int16_t
#define __int int32_t
#define __long int64_t
#define __byte uint8_t
#define __ushort uint16_t
#define __uint uint32_t
#define __ulong uint64_t

#ifdef __GNUC__
#define __noinline __attribute__((noinline))
#else
#define __noinline __declspec(noinline)
#endif
#define __sec_func_attr __noinline

#pragma region Floating Point Literals
constexpr float operator"" f(unsigned long long val)
{
    return float(val);
}
constexpr float operator"" f(long double val)
{
    return float(val);
}
constexpr double operator"" d(unsigned long long val)
{
    return double(val);
}
constexpr double operator"" d(long double val)
{
    return double(val);
}
constexpr long double operator"" ld(unsigned long long val)
{
    return long double(val);
}
constexpr long double operator"" ld(long double val)
{
    return val;
}
#pragma endregion

#pragma region Integer Literals
constexpr __byte operator"" b(unsigned long long val)
{
    return __byte(val);
}
constexpr __sbyte operator"" sb(unsigned long long val)
{
    return __sbyte(val);
}
constexpr __short operator"" s(unsigned long long val)
{
    return __short(val);
}
constexpr __ushort operator"" us(unsigned long long val)
{
    return __ushort(val);
}
constexpr __int operator"" i(unsigned long long val)
{
    return __int(val);
}
constexpr __uint operator"" ui(unsigned long long val)
{
    return __uint(val);
}
constexpr __long operator"" l(unsigned long long val)
{
    return __long(val);
}
constexpr __ulong operator"" ul(unsigned long long val)
{
    return __ulong(val);
}
#pragma endregion

namespace chrono = std::chrono;
namespace fs = std::filesystem;
namespace ranges = std::ranges;
using namespace std::literals::chrono_literals;