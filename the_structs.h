#ifndef THE_STRUCTS_H
#define THE_STRUCTS_H

typedef struct 
{
    char * datetime;
    char * timezone;
    int success;
} struct_ensure_conn;

// typedef struct 
// {
//     char * string_data;
//     int int_data;
//     double double_data;
// } struct_error_data;

typedef struct 
{
    char * datetime;
    char * data;
    int error_code;
} struct_logger_data;

#endif