/*
    Writen by: Oscar Bergström
    https://github.com/OSCARJFB
*/

#ifndef CHESS_MACROS_H
#define CHESS_MACROS_H

#define SIZE_EIGHT 8
#define ENTER 10

#ifdef linux
#define SYSTEM "clear"
#elif _WIN32
#define SYSTEM "cls"
#elif __APPLE__
#define SYSTEM "clear"
#endif

#endif // CHESS_MACROS_H