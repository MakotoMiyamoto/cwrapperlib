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

#define str_(expr) #expr
#define str(expr) str_(expr)

void exit_from_raise_() {
    printf("Exiting with errno %d", errno);
    exit(errno);
}

void raise_(const char *filename, const char *trace, int line, const char *reason) {
    fprintf(stderr, "Error raised in %s:%s at line %d: %s\n\terrno = %d\n", 
    filename, 
    trace, 
    line, 
    reason,
    errno);
    exit_from_raise_();
}

#define READ_INT_READLEN__ 1
#define READ_DOUBLE_READLEN__ 1
#define READ_CHAR_READLEN__ 1

inline int read_int_(FILE *p, int line_, const char *filename_) {
    int d;
    int res = fscanf(p, "%d", &d);
    if (res != READ_INT_READLEN__) {
        raise_(filename_, "read_int(FILE *)", line_, "scanf_s read failure.");
    }
    return d;
}

inline double read_double_(FILE *p, int line_, const char *filename_) {
    double lf;
    int res = fscanf(p, "%lf", &lf);
    if (res != READ_DOUBLE_READLEN__) {
        raise_(filename_, "read_double(FILE *)", line_, "fscanf read failure.");
    }
    return lf;
}

inline char _read_char(FILE *p, int line_, const char *filename_) {
    char c;
    int res = fscanf(p, "%c", &c);
    if (res != READ_CHAR_READLEN__) {
        raise_(filename_, "read_char(FILE *)", line_, "fscanf read failure.");
    }
    return c;
}

/// Attempts to read a 32-bit integer from a stream.
///
/// Raises:
/// Function will raise an error if a 32-bit integer cannot be read
/// from the stream.
#define read_int_f(filep) read_int_(filep, __LINE__, __FILE__)
/// Attempts to read a double from a stream.
///
/// Raises:
/// Function will raise an error if a double cannot be read
/// from the stream.
#define read_double_f(filep) read_double_(filep, __LINE__, __FILE__)
/// Attempts to read a char from a stream.
///
/// Raises:
/// Function will raise an error if a char cannot be read
/// from the stream.
///
/// Note: This is not to be confused with a byte.
#define read_char_f(filep) read_char_(filep, __LINE__, __FILE__)

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
    raise_(__FILE__, "main(void)", __LINE__, "Arbitrary error call.");
}
