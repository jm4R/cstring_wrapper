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
    std::array<CharT, N + 1> array_{};

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

public /*constructors & destructor*/:
    basic_cstring() noexcept = default; //(1)

    basic_cstring(size_type count, CharT ch) //(2)
    {
        assert(count <= N);
        for (int i = 0; i < count; ++i)
            array_[i]
                = ch;
        data[count] = static_cast<CharT>(0);
    }

    template <std::size_t ON>
    basic_cstring(const basic_cstring<CharT, ON>& other, size_type pos) //(3a)
    {
        assert(Traits::strlen(other.array_.data() + pos) <= N);
        Traits::strcpy(array_.data(), other.array_.data() + pos);
    }

    template <std::size_t ON>
    basic_cstring(const basic_cstring<CharT, ON>& other, size_type pos, size_type count) //(3b)
    {
        assert(count <= N);
        Traits::strncpy(array_.data(), other.array_.data() + pos, count);
    }

    basic_cstring(const CharT* s, size_type count) //(4)
    {
        assert(count <= N);
        Traits::strncpy(array_.data(), s, count);
    }

    basic_cstring(const CharT* val) //(5)
    {
        assert(Traits::strlen(val) <= N);
        Traits::strcpy(array_.data(), val);
    }

    template <typename InputIt>
    basic_cstring(InputIt first, InputIt last) //(6)
    {
        assert(std::distance(first, last) <= N);
        CharT* p = array_.data();
        for (InputIt i = first; i != last; ++i)
            *(p++) = *i;
        *p = static_cast<CharT>(0);
    }

    basic_cstring(const basic_cstring& other) noexcept = default; //(7)
    basic_cstring(basic_cstring&&) = delete; //(8)

    basic_cstring(std::initializer_list<CharT> ilist) //(9)
        : basic_cstring(ilist.begin(), ilist.end())
    {
    }

    //TODO (10) and (11)

    ~basic_cstring() = default;

public /*operator=*/:
    basic_cstring& operator=(const basic_cstring&) = default;
    basic_cstring& operator=(basic_cstring&&) = delete;

    template <typename... Args>
    basic_cstring& operator=(Args&&... args)
    {
        *this = basic_cstring(std::forward<Args>(args)...);
        return *this;
    }

    template <typename... Args>
    basic_cstring& assign(Args&&... args)
    {
        *this = basic_cstring(std::forward<Args>(args)...);
        return *this;
    }

public /*Element access*/:

    constexpr reference at(std::size_t pos)
    {
        return array_.at(pos);
    }

    constexpr const_reference at(std::size_t pos) const
    {
        return array_.at(pos);
    }

    constexpr reference operator[](size_type pos)
    {
        return array_[pos];
    }

    constexpr const_reference operator[](size_type pos) const
    {
        return array_[pos];
    }

    constexpr reference front()
    {
        return array_.front();
    }

    constexpr const_reference front() const
    {
        return array_.front();
    }

    constexpr reference back()
    {
        return array_.back();
    }

    constexpr const_reference back() const
    {
        return array_.back();
    }

    constexpr CharT* data() noexcept
    {
        return array_.data();
    }

    constexpr const CharT* data() const noexcept
    {
        return array_.data();
    }

    constexpr const CharT* c_str() const noexcept
    {
        return array_.data();
    }

    //TODO operator basic_string_view

    ///TODO:
public /*Iterators*/:
public /*Capacity*/:
public /*Operations*/:
public /*Search*/:
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
