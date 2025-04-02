#ifndef LOG_H
#define LOG_H

#include <stdio.h>

typedef enum {
    INFO,
    WARNING,
    ERROR
} LogLevel;

void logger(LogLevel level, const char *msg);

#endif
