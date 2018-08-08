#include "initialize.h"

int initialize(int minutes)
{
    //routine to test the entire system
    if(!test_all()){
        printf(KYEL "\nOne or more tests failed!\n");
        printf(KYEL "Refer to generated log error file 'log.json'\n");
        //printf(KNRM "");
        return 0;
    }
    upload_log_to_aws();

    return 1;
}

// void ensure_connection_to_aws(void * periodic_minutes)
// {
//     char * data;
//     int * tmp_var = (int *)periodic_minutes;
//     int sleep_time = (int)tmp_var * 5;

//     printf(KCYN "In new thread for AWS connectivity check.");
//     printf(KNRM "");

//     // For curl
//     curl_global_init(CURL_GLOBAL_DEFAULT);

//     while(1)
//     {
//         printf("\nPinging AWS...\n");
        
//         // ping AWS Lambda 'check_connection' function
//         data = get_check_connection_resource();
//         if(data != 0)
//         {
//             // printf(KCYN "Server response is: ");
//             // printf(KYEL "%s\n", data);
//             // printf(KNRM "");

//             // parse JSON response
//             json_object * jobj = json_tokener_parse(data);
            
//             struct_ensure_conn * server_response = 
//                 json_parse_check_connection(jobj);
            
//             printf("\nServer response --> datetime: %s, timezone: %s, success : %d\n",
//                 server_response->datetime,
//                 server_response->timezone,
//                 server_response->success
//             );

//             //exit this while loop
//             if(!server_response->success) break;
//         }
//         else
//         {
//             printf(KRED "A network error has occured locally!\n");
//             printf(KNRM "");

//             // Turn on led to show error
//             controlCSID2(1);

//             //exit this while loop
//             break;
//         }

//         // sleep until time period passes.
//         printf("Sleeping for %d minute(s)...\n", sleep_time);
//         sleep(sleep_time);
//     }

//     /* we're done with libcurl, so clean it up */ 
//     curl_global_cleanup();
// }

int upload_log_to_aws(void)
{
    //create logger struct
    //struct_logger_data * log_struct = load_from_log_file();
    char * the_log = str_load_from_log_file();

    /* send to aws. */
    // int network_success = post_aws_logger_data(
    //     create_json_logger_data_object_string(log_struct)
    // );
    int network_success = post_aws_logger_data(the_log);

    //printf("Was it successful to send to aws? %d\n", network_success);
    if(!network_success) {
        printf(KRED "Could not send log to AWS.\n");
        printf(KNRM "");
        str_save_to_log("Could not send log to AWS.", 1);
        return 0;
    }

    return 1;
}