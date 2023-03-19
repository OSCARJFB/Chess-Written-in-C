/*
    Writen by: Oscar Bergström
    https://github.com/OSCARJFB
*/

#ifndef CHESS_MACROS_H
#define CHESS_MACROS_H

#define ENTER 10

#ifdef __linux__
#define SYSTEM "clear"
#elif _WIN32
#define SYSTEM "cls"
#elif __APPLE__
#define SYSTEM "clear"
#else 
#error "Unknown platform"
#endif

#endif // CHESS_MACROS_H