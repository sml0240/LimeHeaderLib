#pragma once
#include<stdio.h>

#define FOREACH(type, start, end, fn) \ 
    for (type _foreach_var = start; _foreach_var != end; _foreach_var++) \
    { \ 
        fn(_foreach_var); \ 
                 }

#define PRINTS(n) printf("%s\n")
#define PRINTD(n) printf("%d\n")

#define ARR_SIZE(arr) printf("sizeof arr: %d", (sizeof(arr) / sizeof(arr[0])))