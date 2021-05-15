#pragma once

#include <cstdint>
#include <algorithm>

namespace Marble
{
    template<typename T>
    struct ManagedArray final
    {
        ManagedArray(const size_t& length = 0) : array(new T[length]), arrlen(length)
        {
        }
        ManagedArray(const size_t& length, const T& fill) : array(new T[length]), arrlen(length)
        {
            std::fill(this->array, this->array + this->arrlen, fill);
        }
        ManagedArray(const ManagedArray<T>& other) : array(new T[other.arrlen]), arrlen(other.arrlen)
        {
            for (size_t i = 0; i < this->arrlen; i++)
                this->array[i] = other.array[i];
        }
        ManagedArray(ManagedArray<T>&& other) noexcept : array(nullptr), arrlen(SIZE_MAX)
        {
            this->array = other.array;
            this->arrlen = other.arrlen;

            other.array = nullptr;
            other.arrlen = SIZE_MAX;
        }
        ~ManagedArray()
        {
            delete[] this->array;
        }
        inline ManagedArray<T>& operator=(const ManagedArray<T>& other)
        {
            delete[] this->array;
            this->arrlen = other.arrlen;
            this->array = new T[this->arrlen];
            for (size_t i = 0; i < this->arrlen; i++)
                this->array[i] = other.array[i];
            return *this;
        }
        inline ManagedArray<T>& operator=(ManagedArray<T>&& other) noexcept
        {
            if (this != &other)
            {
                delete[] this->array;

                this->array = other.array;
                this->arrlen = other.arrlen;

                other.array = nullptr;
                other.arrlen = SIZE_MAX;
            }

            return *this;
        }

        inline T& operator[](const size_t& index) const
        {
            return this->array[index];
        }
        inline size_t length() const
        {
            return this->arrlen;
        }

        inline T* data() const
        {
            return this->array;
        }
        inline void reset(const size_t& length)
        {
            delete[] this->array;
            this->arrlen = length;
            this->array = new T[this->arrlen];
        }
        inline void reset(const size_t& length, const T& fill)
        {
            delete[] this->array;
            this->arrlen = length;
            this->array = new T[this->arrlen];

            std::fill(this->array, this->array + this->arrlen, fill);
        }

        inline T* begin()
        {
            return this->array;
        }
        inline T* end()
        {
            return this->array + this->arrlen;
        }
    private:
        T* array;
        size_t arrlen;
    };
}
