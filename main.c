#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#ifdef _WIN32
    #define __scanf_indpt(...) scanf_s(__VA_ARGS__)
    #define __fscanf_indpt(...) fscanf_s(__VA_ARGS__)
    #define __sscanf_indpt(...) sscanf_s(__VA_ARGS__)
#else
    #define __scanf_indpt(...) scanf(__VA_ARGS__)
    #define __fscanf_indpt(...) fscanf(__VA_ARGS__)
    #define __sscanf_indpt(...) sscanf_s(__VA_ARGS__)
#endif

#define scanf(...) __scanf_indpt(__VA_ARGS__)
#define fscanf(...) __fscanf_indpt(__VA_ARGS__)
#define sscanf(...) __sscanf_indpt(__VA_ARGS__)

#define nameof(FUNC) #FUNC

void _exit_from_raise() {
    printf("Exiting with errno %d", errno);
    exit(errno);
}

void _raise(const char *filename, const char *function, int line, const char *reason) {
    fprintf(stderr, "Error raised in %s:%s at line %d: %s\n\terrno = %d\n", 
    filename, 
    function, 
    line, 
    reason,
    errno);
    _exit_from_raise();
}

#define __READ_INT_READLEN 1
#define __READ_DOUBLE_READLEN 1
#define __READ_CHAR_READLEN 1

int _read_int(FILE *p, int _line, const char *_filename) {
    int d;
    int res = __fscanf_indpt(p, "%d", &d);
    if (res != __READ_INT_READLEN) {
        _raise(_filename, "read_int(FILE *)", _line, "scanf_s read failure.");
    }
    return d;
}

double _read_double(FILE *p, int _line, const char *_filename) {
    double lf;
    int res = __fscanf_indpt(p, "%lf", &lf);
    if (res != __READ_DOUBLE_READLEN) {
        _raise(_filename, "read_double(FILE *)", _line, "fscanf read failure.");
    }
    return lf;
}

char _read_char(FILE *p, int _line, const char *_filename) {
    char c;
    int res = __fscanf_indpt(p, "%c", &c);
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

// #define unnamed(type, err) \
// enum type##_try { \
//     Ok(type), \
//     Err(err) \
// };

// unnamed(int, const char *)

union ResultU {
    int ok;
    char err[256];
};

bool is_err(union ResultU *res) {
    return res->err[255] == '\1';
}

typedef struct _Result {
    bool err_flag;
    union ResultU __res;
} Result;

int Ok(Result *res) {
    if (res->err_flag == 1) {
        _raise(__FILE__, "none()", __LINE__, "Result not Ok.");
    }

    return res->__res.ok;
}

const char *Err(Result *res) {
    if (res->err_flag == 0) {
        _raise(__FILE__, "none()", __LINE__, "Result not Err.");
    }

    return res->__res.err;
}

Result get_result() {
    union ResultU r = { .ok = 10 };
    Result res = { .__res = r, .err_flag = 0 };
    return res;
}

// struct Result {
//     // bool (*is_err)(void);
//     union _Result res;
// };

// union Result {
//     int ok;
//     char err[256];
// };



int main(void) {
    printf("Hello, world...\n");
    Result res = get_result();
    printf("%d\n", Ok(&res));
    printf("%s", Err(&res));
}
