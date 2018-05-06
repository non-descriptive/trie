#include "Trie.hpp"



usize strlen_(const char* str)
{
    usize ret = 0;
    while (str and (*str != '\0'))
    {
        ++ret;
        ++str;
    }
    return ret;
}

