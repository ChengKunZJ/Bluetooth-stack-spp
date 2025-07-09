#ifndef TRACE_H
#define TRACE_H

#ifdef ENABLE_TRACE

#include <stdio.h>

// ANSI color escape sequences
#define COLOR_RED     "\033[31m"  // 红色：ERROR
#define COLOR_YELLOW  "\033[33m"  // 黄色：WARN
#define COLOR_GREEN   "\033[32m"  // 绿色：INFO
#define COLOR_BLUE    "\033[34m"  // 蓝色：DEBUG
#define COLOR_RESET   "\033[0m"   // 重置颜色

// Optional: extensions like bold text, background colors, etc.
#define COLOR_BOLD    "\033[1m"
#define COLOR_BG_RED  "\033[41m"

#ifdef ENABLE_TRACE_COLOR
    #define COLOR_ERROR  COLOR_RED
    #define COLOR_WARN   COLOR_YELLOW
    #define COLOR_INFO   COLOR_GREEN
    #define COLOR_DEBUG  COLOR_BLUE
    #define COLOR_END    COLOR_RESET
#else
    #define COLOR_ERROR  ""
    #define COLOR_WARN   ""
    #define COLOR_INFO   ""
    #define COLOR_DEBUG  ""
    #define COLOR_END    ""
#endif


#define TRACE(fmt, ...) \
    printf("[USER] %s:%d: " fmt "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)


// Set the default log level to 4 (print all) if not defined.
#ifndef TRACE_LEVEL
#define TRACE_LEVEL 4
#endif

// Definition of log level values
#define TRACE_LEVEL_ERROR   1
#define TRACE_LEVEL_WARN    2
#define TRACE_LEVEL_INFO    3
#define TRACE_LEVEL_DEBUG   4

// ERROR level logging (lowest level, commonly used for printing errors)
#if TRACE_LEVEL >= TRACE_LEVEL_ERROR
#define TRACE_ERROR(fmt, ...) \
    printf(COLOR_ERROR "[ERROR] %s:%d: " fmt COLOR_END "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define TRACE_ERROR(fmt, ...)
#endif

// WARNING level logging
#if TRACE_LEVEL >= TRACE_LEVEL_WARN
#define TRACE_WARN(fmt, ...) \
    printf(COLOR_WARN "[WARN] %s:%d: " fmt COLOR_END "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define TRACE_WARN(fmt, ...)
#endif

// INFO level logging
#if TRACE_LEVEL >= TRACE_LEVEL_INFO
#define TRACE_INFO(fmt, ...) \
    printf("[INFO] %s:%d: " fmt "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
    // printf(COLOR_INFO "[INFO] %s:%d: " fmt COLOR_END "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define TRACE_INFO(fmt, ...)
#endif

// DEBUG level logging
#if TRACE_LEVEL >= TRACE_LEVEL_DEBUG
#define TRACE_DEBUG(fmt, ...) \
    printf(COLOR_INFO "[INFO] %s:%d: " fmt COLOR_END "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
    // printf(COLOR_DEBUG "[DEBUG] %s:%d: " fmt COLOR_END "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define TRACE_DEBUG(fmt, ...)
#endif

#else

#define TRACE(fmt, ...)

// When TRACE is disabled, all macros are empty.
#define TRACE_ERROR(fmt, ...)
#define TRACE_WARN(fmt, ...)
#define TRACE_INFO(fmt, ...)
#define TRACE_DEBUG(fmt, ...)

#endif

#endif
