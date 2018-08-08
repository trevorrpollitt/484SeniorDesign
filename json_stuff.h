#ifndef JSON_STUFF_H_
#define JSON_STUFF_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json/json.h>

#include "the_structs.h"

/* INTERNAL: This function only called inside json_stuff.c. Nowhere else! */
json_object * internal_use_get_log_array(json_object * log_obj);

/* These functions below are ok to call in other c files */
struct_ensure_conn * json_parse_check_connection(json_object * jobj);

json_object * create_json_logger_data_object(struct_logger_data* log_data);

//char * create_json_logger_data_object_string(struct_logger_data* log_data);

char * create_new_json_logger_array(struct_logger_data* log_data);

char * add_to_json_logger_data_array(char * buffer, struct_logger_data* log_data);


#endif