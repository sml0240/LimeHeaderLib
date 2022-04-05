#pragma once
#include <stdint.h>
#include <stdbool.h>

enum ResultVal {OK, ERROR};

typedef enum ErrorType {ERROR_CAST, ERROR_IO, ERROR_FILE, ERROR_ALLOC, ERROR_DEALLOC, ERROR_REALLOC }ErrorType;

typedef struct Error
{
    int error_type;
}Error;

// ------- OPTIONAL PTR -------
typedef struct
{
    union
    {
        void* value;
        int32_t error;
    };
    bool has_value;

} OptionalPtr;

void opt_set_value(OptionalPtr* opt, void* val)
{
    opt->value = val;
    opt->has_value = true;
}

void opt_set_empty(OptionalPtr* opt)
{
    opt->has_value = false;
}

typedef struct Result
{
    union
    {
        void* value;
        ErrorType error;
    };
    int result;
    
} Result;


void result_as_error(Result* result, ErrorType err_type)
{
    result->error = err_type;
    result->result = ERROR;
}