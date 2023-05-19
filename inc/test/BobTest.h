#ifndef __BOB_TEST_H__
#define __BOB_TEST_H__

#ifdef __DEBUG_BOB__

#include <stdio.h>

#define ASSERT(cond, ...) \
        do                \
        {                 \
            if(cond)      \
            {             \
                printf(__VA_ARGS__); \
            }             \
        } while(0)

#define EXCEPT(cond, ...) \
        do                \
        {                 \
            if(cond)      \
            {             \
                printf(__VA_ARGS__); \
                _exit(-1);  \
            }             \
        } while(0)
#else

#define ASSERT(cond, ...)

#define EXCEPT(cond, ...)

#endif

#endif