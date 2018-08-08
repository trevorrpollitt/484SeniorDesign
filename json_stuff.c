#include "json_stuff.h"

json_object * internal_use_get_log_array(json_object * log_obj)
{
    enum json_type type;

    json_object_object_foreach(log_obj, key, val)
    {
        type = json_object_get_type(val);

        //This will only look at string JSON values
        if(type == json_type_array)
        {
            if(strcmp(key, "Log") == 0)
            {
                //get the array obj
                return val;
            }
        }
    }

    return NULL;
}

struct_ensure_conn * json_parse_check_connection(json_object * jobj)
{
    enum json_type type;

    struct_ensure_conn * ret_struct = malloc(sizeof(struct struct_ensure_conn *));

    json_object_object_foreach(jobj, key, val)
    {
        type = json_object_get_type(val);

        //This will only look at string JSON values
        if(type == json_type_string)
        {
            if(strcmp(key, "dateTime") == 0) {
                //not sure if strdup is better than malloc then strcpy
                ret_struct->datetime = strdup(json_object_get_string(val));
            }
            if(strcmp(key, "timeZone") == 0) {
                ret_struct->timezone = strdup(json_object_get_string(val));
            }
            if(strcmp(key, "success") == 0) {
                if(strcmp(json_object_get_string(val), "yes") == 0)
                    ret_struct->success = 1;
                else
                    ret_struct->success = 0;
            }
        }
    }

    return ret_struct;
}

json_object * create_json_logger_data_object(struct_logger_data* log_data)
{
    // Creating a json object
    json_object * jobj = json_object_new_object();

    // Creating a json string
    json_object *error_code_jint = json_object_new_int(log_data->error_code);
    json_object *datetime_jstring = json_object_new_string(log_data->datetime);
    json_object *data_jstring = json_object_new_string(log_data->data);

    // Form the json object
    json_object_object_add(jobj,"errorCode", error_code_jint);
    json_object_object_add(jobj,"datetime", datetime_jstring);
    json_object_object_add(jobj,"data", data_jstring);

    // return json obj
    return jobj;
}

// char * create_json_logger_data_object_string(struct_logger_data* log_data)
// {
//     // Creating a json object
//     json_object * jobj = json_object_new_object();

//     // Creating a json string
//     json_object * error_code_jint = json_object_new_int(log_data->error_code);
//     json_object *datetime_jstring = json_object_new_string(log_data->datetime);
//     json_object *data_jstring = json_object_new_string(log_data->data);

//     // Form the json object
//     json_object_object_add(jobj,"errorCode", error_code_jint);
//     json_object_object_add(jobj,"datetime", datetime_jstring);
//     json_object_object_add(jobj,"data", data_jstring);

//     // return json obj as string
//     return (char *) json_object_to_json_string(jobj);
// }

char * create_new_json_logger_array(struct_logger_data* log_data)
{
    /* Structure:
     * { "Log": [{ "datetime": "...", "data": "..." }, ... ]}
    */

    // Creating a json object
    json_object * outer_jobj = json_object_new_object();
    json_object * inner_jobj = json_object_new_object();

    // Creating a json array
    json_object *jarray = json_object_new_array();

    // Creating json strings
    json_object * error_code_jint = json_object_new_int(log_data->error_code);
    json_object * datetime_jstring = json_object_new_string(log_data->datetime);
    json_object * data_jstring = json_object_new_string(log_data->data);

    // Form the INNER json object from the strings
    json_object_object_add(inner_jobj,"errorCode", error_code_jint);
    json_object_object_add(inner_jobj,"datetime", datetime_jstring);
    json_object_object_add(inner_jobj,"data", data_jstring);

    // Adding the above created INNER json object to the array
    json_object_array_add(jarray,inner_jobj);

    // Form the json object
    json_object_object_add(outer_jobj,"Log", jarray);

    // Now printing the json object
    //printf ("\r\nThe json object created: %s\n",json_object_to_json_string(outer_jobj));

    return (char *) json_object_to_json_string(outer_jobj);
}

char * add_to_json_logger_data_array(char * buffer, struct_logger_data* new_data)
{
    /* Structure:
     * { "Log": [{ "datetime": "...", "data": "..." }, ... ]}
    */
    
    json_object * outer_jobj = json_object_new_object();
    json_object * inner_jobj = json_object_new_object();
    //json_object *jarray = json_object_new_array();

    //parse json obj
    json_object * log_obj = json_tokener_parse(buffer);

    //get array
    json_object * jarray = internal_use_get_log_array(log_obj);

    // Creating json strings
    json_object * error_code_jint = json_object_new_int(new_data->error_code);
    json_object * datetime_jstring = json_object_new_string(new_data->datetime);
    json_object * data_jstring = json_object_new_string(new_data->data);

    // Form the INNER json object from the strings
    json_object_object_add(inner_jobj,"errorCode", error_code_jint);
    json_object_object_add(inner_jobj,"datetime", datetime_jstring);
    json_object_object_add(inner_jobj,"data", data_jstring);

    // Adding the above created INNER json object to the array
    json_object_array_add(jarray,inner_jobj);

    // Form the json object
    json_object_object_add(outer_jobj,"Log", jarray);

    // Now printing the json object
    //printf ("\r\nThe json object created: %s\n",json_object_to_json_string(outer_jobj));

    return (char *) json_object_to_json_string(outer_jobj);
}