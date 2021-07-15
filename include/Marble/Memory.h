#pragma once

#include <cstdint>
#include <string>
#include <type_traits>

namespace Marble
{
    class Memory final
    {
        inline static struct Initializer final
        {
            Initializer()
            {
                Memory::buffer = new uint8_t[4096] {0};
                Memory::bufferSize = 4096;
            }
            ~Initializer()
            {
                delete[] Memory::buffer;
            }
        } init;

        inline static uint8_t* buffer;
        inline static size_t bufferSize;
    public:
        Memory() = delete;

        inline static uint8_t* take(size_t byteCount)
        {
            for (auto it = Memory::buffer; it < Memory::buffer + Memory::bufferSize - byteCount - sizeof(size_t); ++it)
            {
                if (*it == 0 && *(it + byteCount + sizeof(size_t) - 1) == 0)
                {
                    *reinterpret_cast<size_t*>(it) = byteCount;
                    return it + sizeof(size_t);
                }
            }
            return nullptr;
        }
        inline static void free(uint8_t* bytes)
        {
            size_t len = *reinterpret_cast<size_t*>(bytes - sizeof(size_t));
            for (auto it = bytes - sizeof(size_t); it < bytes + len + sizeof(size_t); ++it)
                *it = 0;
        }
        inline static size_t getFragmentSize(uint8_t* bytes)
        {
            return *reinterpret_cast<size_t*>(bytes - sizeof(size_t));
        }

        inline static std::string dump()
        {
            std::string ret;
            ret.reserve(Memory::bufferSize * 4);

            size_t cc = 0;

            for (auto it = Memory::buffer; it < Memory::buffer + Memory::bufferSize; it += sizeof(uint32_t))
            {
                ++cc;

                ret.append((static_cast<std::ostringstream>(std::ostringstream()) << *reinterpret_cast<uint32_t*>(it)).str());

                if (cc == 32) [[unlikely]]
                {
                    ret.append(1, '\n');
                    cc = 0;
                }
                else ret.append(1, ' ');
            }
            *(--ret.end()) = 0;
            return ret;
        }
    };

    namespace Internal
    {
        template <typename T>
        inline void destroy(T* t)
        {
            t->~T();
        }
    }
}

#define __new(T) new(Marble::Memory::take(sizeof(T))) T
#define __delete(mem) if constexpr (!std::is_trivially_destructible<decltype(*mem)>::value) Marble::Internal::destroy(mem); Marble::Memory::free(reinterpret_cast<uint8_t*>(mem))