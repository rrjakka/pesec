#ifndef PESEC_THROW_H
#define PESEC_THROW_H

#include <stdio.h>
#include <stdlib.h>

#define THROW(...) \
    do { \
        fprintf(stderr, __VA_ARGS__); \
        exit(EXIT_FAILURE);\
    } while (0)


#endif // PESEC_THROW_H
