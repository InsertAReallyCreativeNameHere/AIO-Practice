#pragma once

#include <cstdint>
#include <random>

namespace Marble
{
    static struct RandomInitializer
    {
        RandomInitializer()
        {
            srand((unsigned)time(NULL) * 420);
        }
    } init;

    template <typename T = int32_t>
    class Random final
    {
        std::mt19937 engine;
    public:
        Random() : engine
        (
            std::random_device()() ^
            std::chrono::duration_cast<std::chrono::duration<T>>
            (
                std::chrono::system_clock::now().time_since_epoch() +
                std::chrono::high_resolution_clock::now().time_since_epoch()
            )
            .count()
        )
        {
        }

        T next(T fromInclusive, T toExclusive)
        {
            return std::uniform_int_distribution<T>(fromInclusive, toExclusive)(this->engine);
        }
    };
}