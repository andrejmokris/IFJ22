#include "error.h"

void errorExit(int errorCode, const char *fmt, ...) {
    va_list arg_ptr;
    va_start(arg_ptr, fmt);

    fprintf(stderr, "Error %d: ", errorCode);
    vfprintf(stderr, fmt, arg_ptr);

    va_end(arg_ptr);
    exit(errorCode);
}