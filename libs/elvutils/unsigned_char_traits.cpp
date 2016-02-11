#ifdef __GNUC__
#if __GNUC__ < 4
#if __GNUC_MINOR__ < 4

#include <string.h>
#include <string>

unsigned char* std::char_traits<unsigned char>::assign(unsigned char* s, size_t n, unsigned char a)
{
    memset(s, a, n);
    return s;
}

void std::char_traits<unsigned char>::assign(unsigned char& c1, const unsigned char& c2)
{
    c1 = c2;
}

unsigned char* std::char_traits<unsigned char>::copy(unsigned char* s1, const unsigned char* s2, size_t n)
{
    memcpy(s1, s2, n);
    return s1;
}

unsigned char* std::char_traits<unsigned char>::move(unsigned char* s1, const unsigned char* s2, size_t n)
{
    memmove(s1, s2, n);
    return s1;
}

#endif
#endif
#endif
