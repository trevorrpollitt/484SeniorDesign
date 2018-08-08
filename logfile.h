#ifndef LOG_FILES_H_
#define LOG_FILES_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> //for timespec struct
#include <json/json.h>
#include <errno.h>

#include "console_colors.h"
#include "the_structs.h"
#include "json_stuff.h"

char * get_the_current_datetime(void);

int is_file_empty(FILE * fp);

char * read_all_from_file(FILE * fp);

int delete_log_data(void);

int create_log_file(void);

int str_save_to_log(const char * error_message, int error_code);

int save_to_log(struct_logger_data * error_data);

//remove dependencies to aws here. just get the file
//int upload_log_to_aws(void);

struct_logger_data * load_from_log_file(void);

char * str_load_from_log_file(void);

#endif