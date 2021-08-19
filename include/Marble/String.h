#pragma once

#include <cctype>
#include <cstdarg>
#include <cstdint>
#include <cstring>
#pragma warning(push)
#pragma warning(disable: 26495 26451 26812 26498)
#define FMT_HEADER_ONLY
#include <fmt/format.h>
#pragma warning(pop)
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

		#pragma warning(push)
		#pragma warning(disable:4244)
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
		#pragma warning(pop)

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

		template <typename T> T identity(T);
	}

	template <typename T>
	class BasicString final
	{
		static constexpr T whitespace[] { T(' '), T('\t'), T('\n'), T('\r'), T('\f'), T('\v') };

		std::basic_string<T> buffer;
	public:
		constexpr BasicString()
		{
		}
		constexpr BasicString(size_t length) noexcept(std::is_nothrow_constructible<std::basic_string<T>, size_t>::value) : buffer(length + 1)
		{
		}
		constexpr BasicString(const T* cstring) noexcept(std::is_nothrow_constructible<std::basic_string<T>, const T*>::value) : buffer(cstring)
		{
		}
		constexpr BasicString(const BasicString<T>& other) noexcept(std::is_nothrow_copy_constructible<std::basic_string<T>>::value) : buffer(other.buffer)
		{
		}
		constexpr BasicString(BasicString<T>&& other) noexcept(std::is_nothrow_move_constructible<std::basic_string<T>>::value) : buffer(std::move(other.buffer))
		{
		}

		constexpr BasicString& operator=(const T* cstring) noexcept(std::is_nothrow_assignable<std::basic_string<T>, const T*>::value)
		{
			this->buffer = cstring;
			return *this;
		}
		constexpr BasicString& operator=(const BasicString<T>& other) noexcept(std::is_nothrow_copy_assignable<std::basic_string<T>>::value)
		{
			this->buffer = other.buffer;
			return *this;
		}
		constexpr BasicString& operator=(BasicString<T>&& other) noexcept(std::is_nothrow_move_assignable<std::basic_string<T>>::value)
		{
			this->buffer = std::move(other.buffer);
			return *this;
		}

		template <typename... Args>
		static constexpr BasicString<T> format(const T* format, Args... args)
		{
			try
			{
				fmt::basic_memory_buffer<T> ret;
				fmt::format_to(ret.begin(), format, args...);
				return ret.begin();
			}
			catch (fmt::format_error)
			{
				T err[] { 'F', 'a', 'i', 'l', 'e', 'd', ' ', 't', 'o', ' ', 'f', 'o', 'r', 'm', 'a', 't', '!' };
				return err;
			}
		}
		constexpr std::vector<BasicString<T>> split(const BasicString<T>& delimiter)
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
		static constexpr BasicString<T> join(std::vector<BasicString<T>> strings, const BasicString<T>& delimiter)
		{
			if (!strings.empty()) [[likely]]
			{
				BasicString<T> ret(*strings.begin());
				for (auto it = ++strings.begin(); it != strings.end(); ++it)
				{
					ret.buffer.append(delimiter.buffer.data());
					ret.buffer.append(it->buffer);
				}
				return ret;
			}
			else return BasicString<T>();
		}

		constexpr const T* cStr() const noexcept
		{
			return this->buffer.c_str();
		}
		constexpr operator const T* () const noexcept
		{
			return this->buffer.c_str();
		}
		constexpr T* data() const noexcept//(i give up on this one)
		{
			return const_cast<T*>(buffer.data()); // I am going to hell.
		}

		constexpr void shrinkToFit()
		{
			this->buffer.shrink_to_fit();
		}
		constexpr void resize(size_t len)
		{
			this->buffer.resize(len);
		}
		constexpr void clear()
		{
			this->buffer.clear();
		}
		constexpr bool empty()
		{
			return this->empty();
		}

		constexpr std::basic_string<T>::iterator begin()
		{
			return this->buffer.begin();
		}
		constexpr std::basic_string<T>::iterator end()
		{
			return this->buffer.end();
		}
		constexpr std::basic_string<T>::reverse_iterator rbegin()
		{
			return this->buffer.rbegin();
		}
		constexpr std::basic_string<T>::reverse_iterator rend()
		{
			return this->buffer.rend();
		}
		constexpr std::basic_string<T>::const_iterator cbegin()
		{
			return this->buffer.cbegin();
		}
		constexpr std::basic_string<T>::const_iterator cend()
		{
			return this->buffer.cend();
		}
		constexpr std::basic_string<T>::const_reverse_iterator crbegin()
		{
			return this->buffer.crbegin();
		}
		constexpr std::basic_string<T>::const_reverse_iterator crend()
		{
			return this->buffer.crend();
		}
		constexpr T& front()
		{
			return this->buffer.front();
		}
		constexpr T& back()
		{
			return this->buffer.back();
		}
		constexpr T& operator[](const size_t& index) const
		{
			return this->buffer[index];
		}
		constexpr size_t length()
		{
			return this->buffer.size();
		}
		constexpr size_t capacity()
		{
			return this->buffer.capacity();
		}

		constexpr BasicString<T>& operator+=(const BasicString<T>& str)
		{
			this->buffer.reserve(this->buffer.size() + str.buffer.size());
			this->buffer.append(str.buffer);
			return *this;
		}
		constexpr BasicString<T> operator+(const BasicString<T>& str)
		{
			BasicString<T> ret(this->buffer.data());
			ret.buffer.append(str.buffer.data());
			return ret;
		}

		constexpr bool operator==(const BasicString<T>& other) const
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
		constexpr bool operator!=(const BasicString<T>& other) const
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

		
		template <typename IntegralType>
		constexpr IntegralType toIntegralType()
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
		/*template <typename FloatingPointType>
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
		}*/

		template <typename FloatingPointType>
		constexpr FloatingPointType toFloatingPointType()
		{
			FloatingPointType ret;
			T* endPtr;
			ret = Internal::toFloatingPointType<T, FloatingPointType>(this->buffer.data(), &endPtr);
			if (endPtr != this->buffer.data() + Internal::cstrLen(this->buffer.data()))
				return std::numeric_limits<FloatingPointType>::max();
			else return ret;
		}
		/*template <typename IntegralType>
		IntegralType toIntegralType()
		{
			IntegralType ret;
			T* endPtr;
			ret = Internal::toIntegralType<T, IntegralType>(this->buffer.data(), &endPtr);
			if (endPtr != this->buffer.data() + Internal::cstrLen(this->buffer.data()))
				return std::numeric_limits<IntegralType>::max();
			else return ret;
		}*/

		bool contains(const BasicString<T>& str)
		{
			size_t len = this->buffer.size();

			if (len < str.buffer.size())
				return false;

			for (size_t i = 0; i <= len - str.buffer.size(); i++)
				if (Internal::eqIgnoreLen(str.buffer.data(), this->buffer.data()))
					return true;
			return false;
		}
		constexpr BasicString<T>& insert(size_t at, const BasicString<T>& str)
		{
			this->buffer.insert(at, str.buffer);
			return *this;
		}

		constexpr BasicString<T>& trim()
		{
			this->buffer.erase(this->buffer.find_last_not_of(BasicString<T>::whitespace) + 1);
			this->buffer.erase(0, this->buffer.find_first_not_of(BasicString<T>::whitespace));
			return *this;
		}
		constexpr BasicString<T>& trimStart()
		{
			this->buffer.erase(0, this->buffer.find_first_not_of(BasicString<T>::whitespace));
			return *this;
		}
		constexpr BasicString<T>& trimEnd()
		{
			this->buffer.erase(this->buffer.find_last_not_of(BasicString<T>::whitespace) + 1);
			return *this;
		}

		constexpr bool startsWith(const BasicString<T>& str)
		{
			if (Internal::cstrLen(this->buffer.data()) < Internal::cstrLen(str.buffer.data()))
				return false;
			return Internal::eqIgnoreLen(str.buffer.data(), this->buffer.data());
		}
		constexpr bool endsWith(const BasicString<T>& str)
		{
			size_t thisLen = Internal::cstrLen(this->buffer.data());
			size_t strLen = Internal::cstrLen(str.buffer.data());
			if (thisLen < strLen)
				return false;
			return Internal::eqIgnoreLen(str.buffer.data(), this->buffer.data() + thisLen - strLen);
		}

		constexpr BasicString<T>& toLowerCase()
		{
			size_t i = 0;
			while (this->buffer[i] != 0)
			{
				this->buffer[i] = Internal::toLowerCase(this->buffer[i]);
				i++;
			}
			return *this;
		}
		constexpr BasicString<T>& toUpperCase()
		{
			size_t i = 0;
			while (this->buffer[i] != 0)
			{
				this->buffer[i] = Internal::toUpperCase(this->buffer[i]);
				i++;
			}
			return *this;
		}

		constexpr BasicString<T> substring(size_t fromInclusive, size_t toExclusive)
		{
			return { this->buffer.substr(fromInclusive, toExclusive) };
		}
		constexpr BasicString<T>& remove(size_t fromInclusive, size_t toExclusive)
		{
			this->buffer.erase(fromInclusive, toExclusive - fromInclusive);
			return *this;
		}
		constexpr BasicString<T>& replace(const BasicString<T>& matchStr, const BasicString<T>& replaceStr)
		{
			size_t mlen = matchStr.buffer.size();
			size_t rlen = replaceStr.buffer.size();
			size_t index = 0;
			while ((index = this->buffer.find(matchStr, index)) != std::basic_string<T>::npos)
			{
				this->buffer.replace(index, mlen, replaceStr.buffer);
				index += rlen;
			}
			return *this;
		}
	};

	using string = BasicString<char>;
	using wstring = BasicString<wchar_t>;
}