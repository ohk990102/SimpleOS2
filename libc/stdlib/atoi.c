#include <stdlib.h>

int atoi(const char * c) {
    int value = 0;
    int sign = 1;
    if( *c == '+' || *c == '-' )
    {
        if( *c == '-' ) sign = -1;
        c++;
    }
    while ('0' <= *c && *c <= '9')
    {
        value *= 10;
        value += (int) (*c-'0');
        c++;
    }
    return (value * sign);
}