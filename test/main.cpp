#include <cstring/cstring_wrapper.hpp>

#include <iostream>

int main()
{
    cstring<255> a{ "abc" };
    wcstring<63> b;

    static_assert(sizeof(a) == 256);

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

    std::cout << a.c_str();

    return 0;
}
