#include <array>
#include <cassert>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

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
    using Traits = std::char_traits<CharT>;
    static constexpr auto NullChar = static_cast<CharT>(0);

public /*constructors & destructor*/:
    basic_cstring() noexcept = default; //(1)

    basic_cstring(size_type count, CharT ch) //(2)
    {
        assert(count <= N);
        for (int i = 0; i < count; ++i)
            array_[i]
                = ch;
        data[count] = NullChar;
    }

    template <std::size_t ON>
    basic_cstring(const basic_cstring<CharT, ON>& other, size_type pos) //(3a)
        : basic_cstring(other, pos, Traits::length(other.array_.data() + pos))
    {
    }

    template <std::size_t ON>
    basic_cstring(const basic_cstring<CharT, ON>& other, size_type pos, size_type count) //(3b)
    {
        assert(count <= N);
        Traits::copy(array_.data(), other.array_.data() + pos, count);
        data[count] = NullChar;
    }

    basic_cstring(const CharT* s, size_type count) //(4)
    {
        assert(count <= N);
        Traits::copy(array_.data(), s, count);
        data[count] = NullChar;
    }

    basic_cstring(const CharT* val) //(5)
    {
        assert(Traits::length(val) <= N);
        Traits::copy(array_.data(), val, Traits::length(val) + 1);
    }

    template <typename InputIt>
    basic_cstring(InputIt first, InputIt last) //(6)
    {
        assert(std::distance(first, last) <= N);
        CharT* p = array_.data();
        for (InputIt i = first; i != last; ++i)
            *(p++) = *i;
        *p = NullChar;
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
    iterator begin() noexcept
    {
        return array_.begin();
    }

    const_iterator begin() const noexcept
    {
        return array_.begin();
    }

    const_iterator cbegin() const noexcept
    {
        return array_.cbegin();
    }

    iterator end() noexcept //TODO fix all end
    {
        return array_.end();
    }

    const_iterator end() const noexcept
    {
        return array_.end();
    }

    const_iterator cend() const noexcept
    {
        return array_.cend();
    }

    reverse_iterator rbegin() noexcept
    {
        return array_.rbegin();
    }

    const_reverse_iterator rbegin() const noexcept
    {
        return array_.rbegin();
    }

    const_reverse_iterator crbegin() const noexcept
    {
        return array_.crbegin();
    }

    reverse_iterator rend() noexcept
    {
        return array_.rend();
    }

    const_reverse_iterator rend() const noexcept
    {
        return array_.rend();
    }

    const_reverse_iterator crend() const noexcept
    {
        return array_.crend();
    }

public /*Capacity*/:
    [[nodiscard]] bool empty() const noexcept
    {
        return array_[0] == NullChar;
    }

    size_type size() const noexcept
    {
        return Traits::length(array_.data());
    }

    size_type length() const noexcept
    {
        return Traits::length(array_.data());
    }

    constexpr size_type max_size() const noexcept
    {
        return N;
    }

    constexpr void reserve(size_type new_cap) const noexcept
    {
        //keep that function for compatibility with std::string
        assert(new_cap <= N);
    }

    constexpr size_type capacity() const noexcept
    {
        return N;
    }

    constexpr void shrink_to_fit() const noexcept
    {
        //keep that function for compatibility with std::string
    }
public /*Operations*/:
public /*Search*/:
};

template <std::size_t N>
using cstring = basic_cstring<char, N>;

template <std::size_t N>
using wcstring = basic_cstring<wchar_t, N>;

int main()
{
    cstring<255> a{ "abc" };
    wcstring<63> b;

    a.at(0);
    a[0];
    a.front();
    a.back();
    a.data();
    a.c_str();

    a.begin();
    a.cbegin();
    a.end();
    a.cend();
    a.rbegin();
    a.crbegin();
    a.rend();
    a.crend();

    bool empty = a.empty();
    (void)empty;
    a.size();
    a.length();
    a.max_size();
    a.reserve(10);
    a.capacity();
    a.shrink_to_fit();

    return 0;
}
