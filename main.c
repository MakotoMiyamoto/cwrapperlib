#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #define scanf(...) scanf_s(__VA_ARGS__)
    #define fscanf(...) fscanf_s(__VA_ARGS__)
    #define sscanf(...) sscanf_s(__VA_ARGS__)
    #define strcpy(...) strcpy_s(__VA_ARGS__)
    #define strncpy(...) strncpy_s(__VA_ARGS__)
#endif

#define _str(expr) #expr
#define str(expr) _str(expr)

void _exit_from_raise() {
    printf("Exiting with errno %d", errno);
    exit(errno);
}

void _raise(const char *filename, const char *trace, int line, const char *reason) {
    fprintf(stderr, "Error raised in %s:%s at line %d: %s\n\terrno = %d\n", 
    filename, 
    trace, 
    line, 
    reason,
    errno);
    _exit_from_raise();
}

#define __READ_INT_READLEN 1
#define __READ_DOUBLE_READLEN 1
#define __READ_CHAR_READLEN 1

inline int _read_int(FILE *p, int _line, const char *_filename) {
    int d;
    int res = fscanf(p, "%d", &d);
    if (res != __READ_INT_READLEN) {
        _raise(_filename, "read_int(FILE *)", _line, "scanf_s read failure.");
    }
    return d;
}

inline double _read_double(FILE *p, int _line, const char *_filename) {
    double lf;
    int res = fscanf(p, "%lf", &lf);
    if (res != __READ_DOUBLE_READLEN) {
        _raise(_filename, "read_double(FILE *)", _line, "fscanf read failure.");
    }
    return lf;
}

inline char _read_char(FILE *p, int _line, const char *_filename) {
    char c;
    int res = fscanf(p, "%c", &c);
    if (res != __READ_CHAR_READLEN) {
        _raise(_filename, "read_char(FILE *)", _line, "fscanf read failure.");
    }
    return c;
}

/// Attempts to read a 32-bit integer from a stream.
///
/// Raises:
/// Function will raise an error if a 32-bit integer cannot be read
/// from the stream.
#define read_int_f(filep) _read_int(filep, __LINE__, __FILE__)
/// Attempts to read a double from a stream.
///
/// Raises:
/// Function will raise an error if a double cannot be read
/// from the stream.
#define read_double_f(filep) _read_double(filep, __LINE__, __FILE__)
/// Attempts to read a char from a stream.
///
/// Raises:
/// Function will raise an error if a char cannot be read
/// from the stream.
///
/// Note: This is not to be confused with a byte.
#define read_char_f(filep) _read_char(filep, __LINE__, __FILE__)

/// Attempts to read an int from stdin.
///
/// Raises:
/// Function will raise an error if an int cannot be read 
/// from stdin.
#define read_int() read_int_f(stdin)
/// Attempts to read a double from stdin.
///
/// Raises:
/// Function will raise an error if a double cannot be read 
/// from stdin.
#define read_double() read_double_f(stdin)
/// Attempts to read a char from a stdin.
///
/// Raises:
/// Function will raise an error if a char cannot be read
/// from stdin.
///
/// Note: This is not to be confused with a byte.
#define read_char() read_char_f(stdin)

int main(void) {
    errno = 5;
    _raise(__FILE__, "main(void)", __LINE__, "Arbitrary error call.");
}
