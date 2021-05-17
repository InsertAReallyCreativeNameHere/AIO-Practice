#pragma once

#include <cctype>
#include <cstdarg>
#include <cstdint>
#include <cstring>
#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <list>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
#include <Marble/ManagedArray.h>

#include <iostream>

namespace Marble
{
	namespace Internal
	{
		template <typename CharType>
		bool isSpace(CharType c)
		{
			static_assert(false, "trim cannot be used for a non char/wchar_t BasicString. Create a template specialisation for [template <typename CharType> bool Marble::Internal::isSpace(CharType c)].");
		}
		template <>
		bool isSpace<char>(char c)
		{
			return isspace(c);
		}
		template <>
		bool isSpace<wchar_t>(wchar_t c)
		{
			return iswspace(c);
		}

		template <typename CharType>
		bool isSign(CharType c)
		{
			static_assert(false, "isSign cannot be used for a non char/wchar_t BasicString. Create a template specialisation for [template <typename CharType> bool Marble::Internal::isSign(CharType c)].");
		}
		template <>
		bool isSign<char>(char c)
		{
			return c == '+' || c == '-';
		}
		template <>
		bool isSign<wchar_t>(wchar_t c)
		{
			return c == L'+' || c == L'-';
		}

		template <typename CharType>
		bool isBase10Digit(CharType c)
		{
			static_assert(false, "isBase10Digit cannot be used for a non char/wchar_t BasicString. Create a template specialisation for [template <typename CharType> bool Marble::Internal::isBase10Digit(CharType c)].");
		}
		template <>
		bool isBase10Digit<char>(char c)
		{
			return isdigit(c);
		}
		template <>
		bool isBase10Digit<wchar_t>(wchar_t c)
		{
			return iswdigit(c);
		}

		template <typename CharType>
		bool isBase16Digit(CharType c)
		{
			static_assert(false, "isBase16Digit cannot be used for a non char/wchar_t BasicString. Create a template specialisation for [template <typename CharType> bool Marble::Internal::isBase16Digit(CharType c)].");
		}
		template <>
		bool isBase16Digit<char>(char c)
		{
			return isxdigit(c);
		}
		template <>
		bool isBase16Digit<wchar_t>(wchar_t c)
		{
			return iswxdigit(c);
		}

		template <typename CharType>
		CharType toLowerCase(CharType c)
		{
			static_assert(false, "toLowerCase cannot be used for a non char/wchar_t BasicString. Create a template specialisation for [template <typename CharType> CharType Marble::Internal::toLowerCase(CharType c)].");
		}
		template <>
		char toLowerCase<char>(char c)
		{
			return tolower(c);
		}
		template <>
		wchar_t toLowerCase<wchar_t>(wchar_t c)
		{
			return towlower(c);
		}

		template <typename CharType>
		CharType toUpperCase(CharType c)
		{
			static_assert(false, "toUpperCase cannot be used for a non char/wchar_t BasicString. Create a template specialisation for [template <typename CharType> CharType Marble::Internal::toUpperCase(CharType c)].");
		}
		template <>
		char toUpperCase<char>(char c)
		{
			return toupper(c);
		}
		template <>
		wchar_t toUpperCase<wchar_t>(wchar_t c)
		{
			return towupper(c);
		}

		template <typename CharType, typename Type>
		struct toString final
		{
			toString(const Type& type)
			{
				static_assert(false, "BasicString<T>::format cannot be used for a non char/wchar_t BasicString. Create a partial template specialisation for the class [template <typename CharType, typename Type> struct toString final].");
			}
		};
		template <typename Type>
		struct toString<char, Type> final
		{
			toString(const Type& type)
			{
				this->str = std::move(fmt::to_string(type));
			}

			std::basic_string<char>* operator->()
			{
				return &str;
			}
		private:
			std::basic_string<char> str;
		};
		template <typename Type>
		struct toString<wchar_t, Type> final
		{
			toString(const Type& type)
			{
				this->str = std::move(fmt::to_wstring(type));
			}

			std::basic_string<wchar_t>* operator->()
			{
				return &str;
			}
		private:
			std::basic_string<wchar_t> str;
		};

		template <typename CharType, typename FloatingPointType>
		FloatingPointType toFloatingPointType(const CharType* str, CharType** endPtr)
		{
			static_assert(false, "Unknown FloatingPointType for toFloatingPointType. Create a template specialisation for [template <typename CharType, typename FloatingPointType> FloatingPointType Marble::Internal::toFloatingPointType(const CharType* str)].");
		}
		template <>
		float toFloatingPointType(const char* str, char** endPtr)
		{
			return strtof(str, endPtr);
		}
		template <>
		double toFloatingPointType(const char* str, char** endPtr)
		{
			return strtod(str, endPtr);
		}
		template <>
		long double toFloatingPointType(const char* str, char** endPtr)
		{
			return strtold(str, endPtr);
		}
		template <>
		float toFloatingPointType(const wchar_t* str, wchar_t** endPtr)
		{
			return wcstof(str, endPtr);
		}
		template <>
		double toFloatingPointType(const wchar_t* str, wchar_t** endPtr)
		{
			return wcstod(str, endPtr);
		}
		template <>
		long double toFloatingPointType(const wchar_t* str, wchar_t** endPtr)
		{
			return wcstold(str, endPtr);
		}

		template <typename CharType, typename IntegralType>
		IntegralType toIntegralType(const CharType* str, CharType** endPtr)
		{
			static_assert(false, "Unknown IntegralType for toIntegralType. Create a template specialisation for [template <typename CharType, typename IntegralType> IntegralType Marble::Internal::toIntegralType(const CharType* str)].");
		}
		template <>
		char toIntegralType(const char* str, char** endPtr)
		{
			return strtol(str, endPtr, 0);
		}
		template <>
		unsigned char toIntegralType(const char* str, char** endPtr)
		{
			return strtoul(str, endPtr, 0);
		}
		template <>
		short toIntegralType(const char* str, char** endPtr)
		{
			return strtol(str, endPtr, 0);
		}
		template <>
		unsigned short toIntegralType(const char* str, char** endPtr)
		{
			return strtoul(str, endPtr, 0);
		}
		template <>
		int toIntegralType(const char* str, char** endPtr)
		{
			return strtol(str, endPtr, 0);
		}
		template <>
		unsigned int toIntegralType(const char* str, char** endPtr)
		{
			return strtoul(str, endPtr, 0);
		}
		template <>
		long toIntegralType(const char* str, char** endPtr)
		{
			return strtol(str, endPtr, 0);
		}
		template <>
		long long toIntegralType(const char* str, char** endPtr)
		{
			return strtoll(str, endPtr, 0);
		}
		template <>
		unsigned long toIntegralType(const char* str, char** endPtr)
		{
			return strtoul(str, endPtr, 0);
		}
		template <>
		unsigned long long toIntegralType(const char* str, char** endPtr)
		{
			return strtoull(str, endPtr, 0);
		}
		template <>
		char toIntegralType(const wchar_t* str, wchar_t** endPtr)
		{
			return wcstol(str, endPtr, 0);
		}
		template <>
		unsigned char toIntegralType(const wchar_t* str, wchar_t** endPtr)
		{
			return wcstoul(str, endPtr, 0);
		}
		template <>
		short toIntegralType(const wchar_t* str, wchar_t** endPtr)
		{
			return wcstol(str, endPtr, 0);
		}
		template <>
		unsigned short toIntegralType(const wchar_t* str, wchar_t** endPtr)
		{
			return wcstoul(str, endPtr, 0);
		}
		template <>
		int toIntegralType(const wchar_t* str, wchar_t** endPtr)
		{
			return wcstol(str, endPtr, 0);
		}
		template <>
		unsigned int toIntegralType(const wchar_t* str, wchar_t** endPtr)
		{
			return wcstoul(str, endPtr, 0);
		}
		template <>
		long toIntegralType(const wchar_t* str, wchar_t** endPtr)
		{
			return wcstol(str, endPtr, 0);
		}
		template <>
		long long toIntegralType(const wchar_t* str, wchar_t** endPtr)
		{
			return wcstoll(str, endPtr, 0);
		}
		template <>
		unsigned long toIntegralType(const wchar_t* str, wchar_t** endPtr)
		{
			return wcstoul(str, endPtr, 0);
		}
		template <>
		unsigned long long toIntegralType(const wchar_t* str, wchar_t** endPtr)
		{
			return wcstoull(str, endPtr, 0);
		}

		template <typename CharType>
		size_t cstrLen(const CharType* cstring)
		{
			size_t i = 0;
			while (cstring[i] != 0)
				i++;
			return i;
		}

		template <typename CharType>
		bool eqIgnoreLen(const CharType* lhs, const CharType* rhs)
		{
			size_t i = 0;
			while (lhs[i] != 0 && rhs[i] != 0)
			{
				if (lhs[i] != rhs[i])
					return false;
				i++;
			}
			return true;
		}

		template <typename CharType>
		void mkStr(CharType* buffer, CharType* begin, CharType* end)
		{
			size_t len = end - begin;
			for (size_t i = 0; i < len; i++)
				buffer[i] = begin[i];
		}
	}

	#pragma region BasicString Forward
	template <typename T>
	class BasicString;
	#pragma endregion

	#pragma region BasicStringBuilder Declaration
	template <typename T>
	class BasicStringBuilder
	{
		std::list<ManagedArray<T>> internalBuffer;
	public:
		void append(const T* buffer);
		void append(const T* buffer, size_t len);
		void append(const T* begin, const T* end);
		BasicString<T> construct();
	};
	#pragma endregion

	#pragma region BasicString Definition
	template <typename T>
	class BasicString final
	{
		ManagedArray<T> buffer;
	public:
		BasicString() : buffer(1, 0)
		{
		}
		BasicString(size_t length) : buffer(length + 1)
		{
			this->buffer[length] = 0;
		}
		BasicString(std::vector<BasicString<T>> strings, const BasicString<T>& delimiter) : buffer(1, 0)
		{
			if (!strings.empty())
			{
				BasicStringBuilder<T> builder;
				for (auto it = strings.begin(); it != --strings.end(); ++it)
				{
					builder.append(*it);
					builder.append(delimiter.buffer.data());
				}
				builder.append(*(--strings.end()));
				this->buffer = std::move(builder.construct().buffer);
			}
		}
		BasicString(const T* cstring) : buffer(Internal::cstrLen(cstring) + 1)
		{
			T* data = this->buffer.data();
			for (size_t i = 0; i < this->buffer.length(); i++)
				data[i] = cstring[i];
			this->buffer[this->buffer.length() - 1] = 0;
		}
		BasicString(const BasicString<T>& other) : buffer(other.buffer)
		{	
		}
		BasicString(BasicString<T>&& other) noexcept
		{
			this->buffer = other.buffer;
			other.buffer = ManagedArray<T>();
		}

		BasicString& operator=(const T* cstring)
		{
			this->buffer = ManagedArray<T>(Internal::cstrLen(cstring) + 1);
			T* data = buffer.data();
			for (size_t i = 0; i < buffer.length(); i++)
				data[i] = cstring[i];
			this->buffer[this->buffer.length() - 1] = 0;
			return *this;
		}
		BasicString& operator=(const BasicString<T>& other)
		{
			this->buffer = other.buffer;
			return *this;
		}
		BasicString& operator=(BasicString<T>&& other) noexcept
		{
			if (this != &other)
			{
				this->buffer = other.buffer;
				other.buffer = ManagedArray<T>();
			}
			return *this;
		}

		template <typename... Args>
		static BasicString<T> format(const BasicString<T>& format, Args... args)
		{
			try
			{
				fmt::basic_memory_buffer<T> ret;
				fmt::format_to(ret.begin(), format.cStr(), args...);
				return ret.begin();
			}
			catch (fmt::format_error)
			{
				T err[] { 'F', 'a', 'i', 'l', 'e', 'd', ' ', 't', 'o', ' ', 'f', 'o', 'r', 'm', 'a', 't', '!' };
				return err;
			}
		}

		const T* cStr() const
		{
			return this->buffer.data();
		}
		operator const T* () const
		{
			return this->buffer.data();
		}
		T* data() const
		{
			return buffer.data();
		}

		void shrinkToFit()
		{
			BasicStringBuilder<T> builder;
			builder.append(this->buffer, Internal::cstrLen(this->buffer));
			this->buffer = std::move(builder.construct().buffer);
		}
		void resize(size_t len)
		{
			this->buffer.reset(len + 1);
			this->buffer[this->buffer.length() - 1] = 0;
		}
		void clear()
		{
			this->buffer.reset(1);
			this->buffer[0] = 0;
		}
		bool empty()
		{
			size_t i = 0;
			while (this->buffer[i] != 0)
				i++;
			return !static_cast<bool>(i);
		}

		struct iterator
		{
			using iterator_category = std::bidirectional_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;

			iterator(pointer ptr) : ptr(ptr)
			{
			}

			reference operator*()
			{
				return *ptr;
			}
			pointer operator->()
			{
				return ptr;
			}

			iterator& operator++()
			{
				++ptr;
				return *this;
			}
			iterator operator++(int)
			{
				iterator temp = *this;
				++ptr;
				return temp;
			}
			iterator& operator--()
			{
				--ptr;
				return *this;
			}
			iterator operator--(int)
			{
				iterator temp = *this;
				--ptr;
				return temp;
			}

			operator pointer ()
			{
				return ptr;
			}

			friend bool operator==(const iterator& lhs, const iterator& rhs)
			{
				return lhs.ptr == rhs.ptr;
			}
			friend bool operator!=(const iterator& lhs, const iterator& rhs)
			{
				return lhs.ptr != rhs.ptr;
			}

			friend difference_type operator-(const iterator& lhs, const iterator& rhs)
			{
				return lhs.ptr - rhs.ptr;
			}
		private:
			pointer ptr;
		};
		struct reverse_iterator
		{
			using iterator_category = std::bidirectional_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;

			reverse_iterator(pointer ptr) : ptr(ptr)
			{
			}

			reference operator*()
			{
				return *ptr;
			}
			pointer operator->()
			{
				return ptr;
			}

			reverse_iterator& operator++()
			{
				--ptr;
				return *this;
			}
			reverse_iterator operator++(int)
			{
				iterator temp = *this;
				--ptr;
				return temp;
			}
			reverse_iterator& operator--()
			{
				++ptr;
				return *this;
			}
			reverse_iterator operator--(int)
			{
				iterator temp = *this;
				++ptr;
				return temp;
			}

			operator pointer ()
			{
				return ptr;
			}

			friend bool operator==(const reverse_iterator& lhs, const reverse_iterator& rhs)
			{
				return lhs.ptr == rhs.ptr;
			}
			friend bool operator!=(const reverse_iterator& lhs, const reverse_iterator& rhs)
			{
				return lhs.ptr != rhs.ptr;
			}

			friend difference_type operator-(const reverse_iterator& lhs, const reverse_iterator& rhs)
			{
				return lhs.ptr + rhs.ptr;
			}
		private:
			pointer ptr;
		};

		iterator begin()
		{
			return this->buffer.data();
		}
		iterator end()
		{
			return this->buffer.data() + Internal::cstrLen(this->buffer.data());
		}
		reverse_iterator rbegin()
		{
			return this->buffer.data() + Internal::cstrLen(this->buffer.data()) - 1;
		}
		reverse_iterator rend()
		{
			return this->buffer.data() - 1;
		}
		T& front()
		{
			return this->buffer[0];
		}
		T& back()
		{
			return this->buffer[Internal::cstrLen(this->buffer.data()) - 1];
		}
		T& operator[](size_t index) const
		{
			return this->buffer[index];
		}
		size_t length()
		{
			return Internal::cstrLen(this->buffer.data());
		}
		size_t capacity()
		{
			return this->buffer.length();
		}

		BasicString<T>& operator+=(const BasicString<T>& str)
		{
			BasicStringBuilder<T> builder;
			builder.append(this->buffer.data());
			builder.append(str.buffer.data());
			this->buffer = std::move(builder.construct().buffer);

			return *this;
		}
		BasicString<T> operator+(const BasicString<T>& str)
		{
			BasicStringBuilder<T> builder;
			builder.append(this->buffer.data());
			builder.append(str.buffer.data());
			return std::move(builder.construct());
		}

		bool operator==(const BasicString<T>& other) const
		{
			if (Internal::cstrLen(this->buffer.data()) != Internal::cstrLen(other.buffer.data()))
				return false;

			size_t i = 0;
			while (this->buffer[i] != 0)
			{
				if (this->buffer[i] != other.buffer[i])
					return false;
				i++;
			}

			return true;
		}
		bool operator!=(const BasicString<T>& other) const
		{
			if (Internal::cstrLen<T>(this->buffer.data()) != Internal::cstrLen<T>(other.buffer.data()))
				return true;

			size_t i = 0;
			while (this->buffer[i] != 0)
			{
				if (this->buffer[i] != other.buffer[i])
					return true;
				i++;
			}

			return false;
		}

		/*
		template <typename IntegralType>
		IntegralType toIntegralType()
		{
			static_assert(std::is_integral<IntegralType>::value, "toIntegralType requires an integral template parameter.");
			static_assert(std::is_same<T, char>::value || std::is_same<T, wchar_t>::value, "toIntegralType requires a char/wchar_t BasicString.");

			size_t len = Internal::cstrLen(this->buffer.data());

			IntegralType ret(0);
			size_t currentPlaceValue = 1;
			if constexpr (std::is_unsigned<IntegralType>::value)
			{
				if (len < 3)
					goto UnsignedDecimal;
				switch (this->buffer[0] + this->buffer[1])
				{
				case T('0') + T('x'):
					for (size_t i = len - 1; i != 1; i--)
					{
						switch (this->buffer[i])
						{
						case T('0'):
							break;
						case T('1'):
							ret += static_cast<IntegralType>(0x1) * currentPlaceValue;
							break;
						case T('2'):
							ret += static_cast<IntegralType>(0x2) * currentPlaceValue;
							break;
						case T('3'):
							ret += static_cast<IntegralType>(0x3) * currentPlaceValue;
							break;
						case T('4'):
							ret += static_cast<IntegralType>(0x4) * currentPlaceValue;
							break;
						case T('5'):
							ret += static_cast<IntegralType>(0x5) * currentPlaceValue;
							break;
						case T('6'):
							ret += static_cast<IntegralType>(0x6) * currentPlaceValue;
							break;
						case T('7'):
							ret += static_cast<IntegralType>(0x7) * currentPlaceValue;
							break;
						case T('8'):
							ret += static_cast<IntegralType>(0x8) * currentPlaceValue;
							break;
						case T('9'):
							ret += static_cast<IntegralType>(0x9) * currentPlaceValue;
							break;
						case T('a'):
							ret += static_cast<IntegralType>(0xa) * currentPlaceValue;
							break;
						case T('b'):
							ret += static_cast<IntegralType>(0xb) * currentPlaceValue;
							break;
						case T('c'):
							ret += static_cast<IntegralType>(0xc) * currentPlaceValue;
							break;
						case T('d'):
							ret += static_cast<IntegralType>(0xd) * currentPlaceValue;
							break;
						case T('e'):
							ret += static_cast<IntegralType>(0xe) * currentPlaceValue;
							break;
						case T('f'):
							ret += static_cast<IntegralType>(0xf) * currentPlaceValue;
							break;
						default:
							return std::numeric_limits<IntegralType>::max();
						}
						currentPlaceValue *= 0x10;
					}
					break;
				case T('0') + T('b'):
					for (size_t i = len - 1; i != 1; i--)
					{
						switch (this->buffer[i])
						{
						case T('0'):
							break;
						case T('1'):
							ret += static_cast<IntegralType>(0b1) * currentPlaceValue;
							break;
						default:
							return std::numeric_limits<IntegralType>::max();
						}
						currentPlaceValue *= 0b10;
					}
					break;
				default:
					UnsignedDecimal:
					for (size_t i = len - 1; i != std::numeric_limits<size_t>::max(); i--)
					{
						switch (this->buffer[i])
						{
						case T('0'):
							break;
						case T('1'):
							ret += static_cast<IntegralType>(1) * currentPlaceValue;
							break;
						case T('2'):
							ret += static_cast<IntegralType>(2) * currentPlaceValue;
							break;
						case T('3'):
							ret += static_cast<IntegralType>(3) * currentPlaceValue;
							break;
						case T('4'):
							ret += static_cast<IntegralType>(4) * currentPlaceValue;
							break;
						case T('5'):
							ret += static_cast<IntegralType>(5) * currentPlaceValue;
							break;
						case T('6'):
							ret += static_cast<IntegralType>(6) * currentPlaceValue;
							break;
						case T('7'):
							ret += static_cast<IntegralType>(7) * currentPlaceValue;
							break;
						case T('8'):
							ret += static_cast<IntegralType>(8) * currentPlaceValue;
							break;
						case T('9'):
							ret += static_cast<IntegralType>(9) * currentPlaceValue;
							break;
						default:
							return std::numeric_limits<IntegralType>::max();
						}
						currentPlaceValue *= 10;
					}
				}
			}
			else
			{
				size_t i = 0;
				size_t negativeCount = 0;
				while (this->buffer[i] == T('+') || [&, this]() -> bool { if (this->buffer[i] == T('-')) { negativeCount++; return true; } else return false; } ())
					i++;

				size_t digitBufferBegin = i;
				if (len < i + 3)
					goto SignedDecimal;
				switch (this->buffer[digitBufferBegin] + this->buffer[digitBufferBegin + 1])
				{
				case T('0') + T('x'):
					for (i = len - 1; i != digitBufferBegin + 1; i--)
					{
						switch (this->buffer[i])
						{
						case T('0'):
							break;
						case T('1'):
							ret += static_cast<IntegralType>(0x1) * currentPlaceValue;
							break;
						case T('2'):
							ret += static_cast<IntegralType>(0x2) * currentPlaceValue;
							break;
						case T('3'):
							ret += static_cast<IntegralType>(0x3) * currentPlaceValue;
							break;
						case T('4'):
							ret += static_cast<IntegralType>(0x4) * currentPlaceValue;
							break;
						case T('5'):
							ret += static_cast<IntegralType>(0x5) * currentPlaceValue;
							break;
						case T('6'):
							ret += static_cast<IntegralType>(0x6) * currentPlaceValue;
							break;
						case T('7'):
							ret += static_cast<IntegralType>(0x7) * currentPlaceValue;
							break;
						case T('8'):
							ret += static_cast<IntegralType>(0x8) * currentPlaceValue;
							break;
						case T('9'):
							ret += static_cast<IntegralType>(0x9) * currentPlaceValue;
							break;
						case T('a'):
							ret += static_cast<IntegralType>(0xa) * currentPlaceValue;
							break;
						case T('b'):
							ret += static_cast<IntegralType>(0xb) * currentPlaceValue;
							break;
						case T('c'):
							ret += static_cast<IntegralType>(0xc) * currentPlaceValue;
							break;
						case T('d'):
							ret += static_cast<IntegralType>(0xd) * currentPlaceValue;
							break;
						case T('e'):
							ret += static_cast<IntegralType>(0xe) * currentPlaceValue;
							break;
						case T('f'):
							ret += static_cast<IntegralType>(0xf) * currentPlaceValue;
							break;
						default:
							return std::numeric_limits<IntegralType>::max();
						}
						currentPlaceValue *= 0x10;
					}
					break;
				case T('0') + T('b'):
					for (i = len - 1; i != digitBufferBegin + 1; i--)
					{
						switch (this->buffer[i])
						{
						case T('0'):
							break;
						case T('1'):
							ret += static_cast<IntegralType>(0b1) * currentPlaceValue;
							break;
						default:
							return std::numeric_limits<IntegralType>::max();
						}
						currentPlaceValue *= 0b10;
					}
					break;
				default:
					SignedDecimal:
					for (i; i < len; i++)
					{
						if (!Internal::isBase10Digit(this->buffer[i]))
							return std::numeric_limits<IntegralType>::max();
					}
					for (i = len - 1; i != SIZE_MAX; i--)
					{
						switch (this->buffer[i])
						{
						case T('-'):
							ret = -ret;
							break;
						case T('1'):
							ret += static_cast<IntegralType>(1) * currentPlaceValue;
							break;
						case T('2'):
							ret += static_cast<IntegralType>(2) * currentPlaceValue;
							break;
						case T('3'):
							ret += static_cast<IntegralType>(3) * currentPlaceValue;
							break;
						case T('4'):
							ret += static_cast<IntegralType>(4) * currentPlaceValue;
							break;
						case T('5'):
							ret += static_cast<IntegralType>(5) * currentPlaceValue;
							break;
						case T('6'):
							ret += static_cast<IntegralType>(6) * currentPlaceValue;
							break;
						case T('7'):
							ret += static_cast<IntegralType>(7) * currentPlaceValue;
							break;
						case T('8'):
							ret += static_cast<IntegralType>(8) * currentPlaceValue;
							break;
						case T('9'):
							ret += static_cast<IntegralType>(9) * currentPlaceValue;
							break;
						}
						currentPlaceValue *= 10;
					}
				}

				if (negativeCount - (2 * (negativeCount / 2)) != 0)
					ret = -ret;
			}
			return ret;
		}
		template <typename FloatingPointType>
		FloatingPointType toFloatingPointType()
		{
			if constexpr (std::is_same<FloatingPointType, float>::value)
			{
				if constexpr (std::is_same<T, char>::value)
				{
					char* err;
					return strtof(this->buffer.data(), &err);
				}
				else if constexpr (std::is_same<T, wchar_t>::value)
				{
					wchar_t* err;
					return wcstof(this->buffer.data(), &err);
				}
				else static_assert(false, "toFloatingPointType requires a char/wchar_t BasicString.");
			}
			else if constexpr (std::is_same<FloatingPointType, double>::value)
			{
				if constexpr (std::is_same<T, char>::value)
				{
					char* err;
					return strtod(this->buffer.data(), &err);
				}
				else if constexpr (std::is_same<T, wchar_t>::value)
				{
					wchar_t* err;
					return wcstod(this->buffer.data(), &err);
				}
				else static_assert(false, "toFloatingPointType requires a char/wchar_t BasicString.");
			}
			else static_assert(false, "toFloatingPointType requires a floating-point type template parameter.");
		}
		*/

		template <typename FloatingPointType>
		FloatingPointType toFloatingPointType()
		{
			FloatingPointType ret;
			T* endPtr;
			ret = Internal::toFloatingPointType<T, FloatingPointType>(this->buffer.data(), &endPtr);
			if (endPtr != this->buffer.data() + Internal::cstrLen(this->buffer.data()))
				return std::numeric_limits<FloatingPointType>::max();
			else return ret;
		}
		template <typename IntegralType>
		IntegralType toIntegralType()
		{
			IntegralType ret;
			T* endPtr;
			ret = Internal::toIntegralType<T, IntegralType>(this->buffer.data(), &endPtr);
			if (endPtr != this->buffer.data() + Internal::cstrLen(this->buffer.data()))
				return std::numeric_limits<IntegralType>::max();
			else return ret;
		}

		bool contains(const BasicString<T>& str)
		{
			size_t len = Internal::cstrLen(this->buffer.data());

			if (len < Internal::cstrLen(str.buffer.data()))
				return false;

			for (size_t i = 0; i <= len - Internal::cstrLen(str.buffer.data()); i++)
				if (Internal::eqIgnoreLen(str.buffer.data(), this->buffer.data()))
					return true;
			return false;
		}
		BasicString<T>& insert(size_t at, const BasicString<T>& str)
		{
			BasicStringBuilder<T> builder;
			builder.append(this->buffer.data(), at);
			builder.append(str.buffer.data());
			builder.append(this->buffer.data() + at);
			this->buffer = std::move(builder.construct().buffer);

			return *this;
		}

		BasicString<T>& trim()
		{
			size_t len = Internal::cstrLen(this->buffer.data());

			size_t beginOffset = 0;
			while (Internal::isSpace(this->buffer[beginOffset]))
			{
				if (beginOffset == len - 1) // Check for all whitespace string.
				{
					this->buffer.reset(1);
					this->buffer[0] = 0;
					return;
				}
				beginOffset++;
			}

			size_t endOffset = len - 1;
			while (Internal::isSpace(this->buffer[endOffset]))
				endOffset--;

			BasicStringBuilder<T> builder;
			builder.append(this->buffer.data() + beginOffset, this->buffer.data() + endOffset + 1);
			this->buffer = std::move(builder.construct().buffer);

			return *this;
		}
		BasicString<T>& trimStart()
		{
			size_t len = Internal::cstrLen(this->buffer.data());

			size_t beginOffset = 0;
			while (Internal::isSpace(this->buffer[beginOffset]))
			{
				if (beginOffset == len - 1) // Check for all whitespace string.
				{
					this->buffer.reset(1);
					this->buffer[0] = 0;
					return;
				}
				beginOffset++;
			}

			BasicStringBuilder<T> builder;
			builder.append(this->buffer.data() + beginOffset, this->buffer.data() + len);
			this->buffer = std::move(builder.construct().buffer);

			return *this;
		}
		BasicString<T>& trimEnd()
		{
			size_t len = Internal::cstrLen(this->buffer.data());

			size_t endOffset = len - 1;
			while (Internal::isSpace(this->buffer[endOffset]))
			{
				if (endOffset == 0)
				{
					this->buffer.reset(1);
					this->buffer[0] = 0;
					return;
				}
				endOffset--;
			}

			BasicStringBuilder<T> builder;
			builder.append(this->buffer.data(), this->buffer.data() + endOffset + 1);
			this->buffer = std::move(builder.construct().buffer);

			return *this;
		}
		std::vector<BasicString<T>> split(const BasicString<T>& delimiter)
		{
			std::vector<BasicString<T>> splitStrings;

			size_t thisLen = Internal::cstrLen(this->buffer.data());

			size_t splitBegin = 0;
			for (size_t i = 0; i < thisLen - Internal::cstrLen(delimiter.buffer.data()) + 1;)
			{
				if (Internal::eqIgnoreLen(delimiter.buffer.data(), this->buffer.data() + i))
				{
					BasicString<T> str(i - splitBegin);
					Internal::mkStr(str.buffer.begin(), this->buffer.data() + splitBegin, this->buffer.data() + i);
					splitStrings.push_back(std::move(str));

					i += Internal::cstrLen(delimiter.buffer.data());
					splitBegin = i;
				}
				else i++;
			}
			BasicString<T> str(thisLen - splitBegin);
			Internal::mkStr(str.buffer.begin(), this->buffer.data() + splitBegin, this->buffer.data() + thisLen);
			splitStrings.push_back(std::move(str));

			return std::move(splitStrings);
		}

		bool startsWith(const BasicString<T>& str)
		{
			if (Internal::cstrLen(this->buffer.data()) < Internal::cstrLen(str.buffer.data()))
				return false;
			return Internal::eqIgnoreLen(str.buffer.data(), this->buffer.data());
		}
		bool endsWith(const BasicString<T>& str)
		{
			size_t thisLen = Internal::cstrLen(this->buffer.data());
			size_t strLen = Internal::cstrLen(str.buffer.data());
			if (thisLen < strLen)
				return false;
			return Internal::eqIgnoreLen(str.buffer.data(), this->buffer.data() + thisLen - strLen);
		}

		BasicString<T>& toLowerCase()
		{
			size_t i = 0;
			while (this->buffer[i] != 0)
			{
				this->buffer[i] = Internal::toLowerCase(this->buffer[i]);
				i++;
			}
			return *this;
		}
		BasicString<T>& toUpperCase()
		{
			size_t i = 0;
			while (this->buffer[i] != 0)
			{
				this->buffer[i] = Internal::toUpperCase(this->buffer[i]);
				i++;
			}
			return *this;
		}

		BasicString<T> substring(size_t fromInclusive, size_t toExclusive)
		{
			BasicStringBuilder<T> builder;
			builder.append(this->buffer.data() + fromInclusive, this->buffer.data() + toExclusive);
			return std::move(builder.construct());
		}
		BasicString<T>& remove(size_t fromInclusive, size_t toExclusive)
		{
			BasicStringBuilder<T> builder;
			builder.append(this->buffer.data(), this->buffer.data() + fromInclusive);
			builder.append(this->buffer.data() + toExclusive, this->buffer.data() + Internal::cstrLen(this->buffer.data()));
			this->buffer = std::move(builder.construct().buffer);

			return *this;
		}
		BasicString<T>& replace(const BasicString<T>& matchStr, const BasicString<T>& replaceStr)
		{
			BasicStringBuilder<T> builder;

			size_t thisLen = Internal::cstrLen(this->buffer.data());

			size_t splitBegin = 0;
			for (size_t i = 0; i < thisLen - Internal::cstrLen(matchStr.buffer.data()) + 1;)
			{
				if (Internal::eqIgnoreLen(matchStr.buffer.data(), this->buffer.data() + i))
				{
					builder.append(this->buffer.data() + splitBegin, this->buffer.data() + i);
					builder.append(replaceStr.buffer.data());
					i += Internal::cstrLen(matchStr.buffer.data());
					splitBegin = i;
				}
				else i++;
			}
			builder.append(this->buffer.data() + splitBegin, this->buffer.data() + thisLen);

			this->buffer = std::move(builder.construct().buffer);

			return *this;
		}
	};
	#pragma endregion

	#pragma region BasicStringBuilder Definition
	template <typename T>
	void BasicStringBuilder<T>::append(const T* buffer)
	{
		size_t len = Internal::cstrLen(buffer);
		ManagedArray<T> sec(len);
		for (size_t i = 0; i < len; i++)
			sec[i] = buffer[i];
		this->internalBuffer.push_back(std::move(sec));
	}
	template <typename T>
	void BasicStringBuilder<T>::append(const T* buffer, size_t len)
	{
		ManagedArray<T> sec(len);
		for (size_t i = 0; i < len; i++)
			sec[i] = buffer[i];
		this->internalBuffer.push_back(std::move(sec));
	}
	template <typename T>
	void BasicStringBuilder<T>::append(const T* begin, const T* end)
	{
		size_t len = end - begin;
		ManagedArray<T> sec(len);
		for (size_t  i = 0; i < len; i++)
			sec[i] = *(begin + i);
		this->internalBuffer.push_back(std::move(sec));
	}
	template <typename T>
	BasicString<T> BasicStringBuilder<T>::construct()
	{
		size_t strLen = 0;
		for (auto it = this->internalBuffer.begin(); it != this->internalBuffer.end(); ++it)
			strLen += it->length();

		BasicString<T> ret(strLen + 1);
		size_t i = 0;
		for (auto it = this->internalBuffer.begin(); it != this->internalBuffer.end(); ++it)
		{
			for (size_t j = 0; j < it->length(); j++)
			{
				ret[i] = (*it)[j];
				i++;
			}
		}
		ret[strLen] = 0;

		return std::move(ret);
	}
	#pragma endregion

	using string = BasicString<char>;
	using wstring = BasicString<wchar_t>;

	using StringBuilder = BasicStringBuilder<char>;
	using WideStringBuilder = BasicStringBuilder<wchar_t>;
}