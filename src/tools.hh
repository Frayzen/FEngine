#pragma once

#include <iostream>
#include <ostream>

// Macros
#define FAIL_ON(Condition, Message)                                            \
    if (Condition) {                                                           \
        std::cerr << Message << std::endl;                                     \
        clean_exit(-1);                                                        \
    }

void clean_exit(int code);
void checkGLError(const char *message);
