#include <array>
#include <cassert>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

template <typename CharT>
struct CSTraits {
};

template <>
struct CSTraits<char> {
    static std::size_t strlen(const char* v) { return std::strlen(v); }
    static char* strcpy(char* d, const char* s) { return std::strcpy(d, s); }
};

template <>
struct CSTraits<wchar_t> {
    static std::size_t strlen(const wchar_t* v) { return std::wcslen(v); }
    static wchar_t* strcpy(wchar_t* d, const wchar_t* s) { return std::wcscpy(d, s); }
};

/**
* N is without null terminator
* Unlike fixed_capacity_string it doesn't support embedded nulls,
* but it is optimized for space. It doesn't store anything except the array of characters.
**/
template <typename CharT, std::size_t N>
class basic_cstring {
    std::array<CharT, N + 1> data{};

    template <typename, std::size_t>
    friend class basic_cstring;

public /*typedefs*/:
    using value_type = CharT;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = value_type*;
    using const_iterator = const value_type*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
    using Traits = CSTraits<CharT>;

public /*special members*/:
    basic_cstring() = default;
    basic_cstring(const basic_cstring&) = default;
    basic_cstring(basic_cstring&&) = delete;

    basic_cstring& operator=(const basic_cstring&) = default;
    basic_cstring& operator=(basic_cstring&&) = delete;

    ~basic_cstring() = default;

    basic_cstring(const value_type* val)
    {
        assert(Traits::strlen(val) <= N);
        Traits::strcpy(data.data(), val);
    }

    basic_cstring& operator=(const value_type* val)
    {
        *this = basic_cstring(val);
    }

    basic_cstring(size_type count, CharT ch)
    {
        assert(count <= N);
        for (int i = 0; i < count; ++i)
            data[i]
                = ch;
    }

    template <std::size_t ON>
    basic_cstring(const basic_cstring<CharT, ON>& other, size_type pos)
    {
        assert(Traits::strlen(other.data.data() + pos) <= N);
        Traits::strcpy(data.data(), other.data.data() + pos);
    }

    template <std::size_t ON>
    basic_cstring(const basic_cstring<CharT, ON>& other, size_type pos, size_type count)
    {
        assert(count <= N);
        Traits::strncpy(data.data(), other.data.data() + pos, count);
    }

    basic_cstring(const CharT* s, size_type count)
    {
        assert(count <= N);
        Traits::strncpy(data.data(), s, count);
    }
};

template <std::size_t N>
using cstring = basic_cstring<char, N>;

template <std::size_t N>
using wcstring = basic_cstring<wchar_t, N>;

int main()
{
    cstring<255> a;
    wcstring<63> b;
    return 0;
}
