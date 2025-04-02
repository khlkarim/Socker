#include "log.h"

void logger(LogLevel level, const char *msg) {
    const char *level_strings[] = {"INFO", "WARNING", "ERROR"};
    printf("[%s]: %s\n", level_strings[level], msg);
}
