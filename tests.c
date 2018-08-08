#include "tests.h"


int test_all(void)
{
    int count;
    int success = 1;
    int results_array[5] = { 0, 0, 0, 0, 0 };
    char * messages[5] = { 
        "   Log creation test...",
        "   LED GPIO pins test...",
        "   H711 GPIO pins test...",
        "   AWS connection test ...",
        "   Wifi test..."
    };

    printf(KCYN "\nRunning all tests for the system...\n");
    printf(KNRM "");

    /* The tests */
    results_array[0] = test_error_file();
    results_array[1] = test_LED_pins();
    results_array[2] = test_hx711_pins();
    results_array[3] = test_aws_connection();
    results_array[4] = test_wifi();

    /* Print the results */
    printf("\r\n");
    printf("|-------- Test Results ---------|\n");
    printf("|-------------------------------|\n");
    for(count = 0; count < 5; count++)
    {
        printf(messages[count]);

        if(results_array[count]){
            printf(KGRN "passed");
            printf(KNRM " \n");
        }
        else {
            printf(KRED "failed");
            printf(KNRM " \n");
            success = 0;
        }
    }
    printf("|-------------------------------|\n");
    printf("|----- End of test results -----|\n");
    printf("\r\n");

    return success;
}

int test_error_file(void)
{
    return create_log_file();
}

int test_hx711_pins(void)
{
    return setPinsForHx711();
}

int test_LED_pins(void)
{
    return toggleAll();
}

int test_aws_connection(void)
{
    char * data;

    // For curl
    curl_global_init(CURL_GLOBAL_DEFAULT);

    printf("\nPinging AWS...\n");
    
    // ping AWS Lambda 'check_connection' function
    data = get_check_connection_resource();
    if(data != 0)
    {
        // parse JSON response
        json_object * jobj = json_tokener_parse(data);
        
        struct_ensure_conn * server_response = 
            json_parse_check_connection(jobj);
        
        // printf("\nServer response --> datetime: %s, timezone: %s, success : %d\n",
        //     server_response->datetime,
        //     server_response->timezone,
        //     server_response->success
        // );
        if(!server_response->success) return 0;
    }
    else
    {
        printf(KRED "A network error has occured locally!\n");
        printf(KNRM "");

        // Turn on led to show error
        controlCSID2(1);

        //exit this while loop
        return 0;
    }

    /* we're done with libcurl, so clean it up */ 
    curl_global_cleanup();

    return 1;
}

int test_wifi(void)
{
    if(parsePing()){
        return 1;
    }
    return 0;
}