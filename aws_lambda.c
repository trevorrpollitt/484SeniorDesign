#include "aws_lambda.h"

struct DataStruct {
  char *data;
  size_t size;
  int status;
};

/**
 * Ex Host: 4d5hwp29ll.execute-api.us-east-1.amazonaws.com
 * Ex Full URL = https://4d5hwp29ll.execute-api.us-east-1.amazonaws.com/development/check_connection
*/
char aws_check_connection_url[] = "https://4d5hwp29ll.execute-api.us-east-1.amazonaws.com/development/check_connection";
char aws_eagleio_url[] = "https://4d5hwp29ll.execute-api.us-east-1.amazonaws.com/development/eagleio";
char aws_logger_url[] = "https://4d5hwp29ll.execute-api.us-east-1.amazonaws.com/development/logger";

static size_t get_check_connection_resource_callback(void *contents, size_t size,
    size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct DataStruct *mem = (struct DataStruct *)userp;
 
  mem->data = realloc(mem->data, mem->size + realsize + 1);
  if(mem->data == NULL) {
    /* out of memory! */ 
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }
 
  memcpy(&(mem->data[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->data[mem->size] = 0;
 
  return realsize;
}

static size_t eagleio_read_callback(void *contents, size_t size, 
    size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct DataStruct *mem = (struct DataStruct *)userp;
 
  mem->data = realloc(mem->data, mem->size + realsize + 1);
  if(mem->data == NULL) {
    /* out of memory! */ 
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }
 
  memcpy(&(mem->data[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->data[mem->size] = 0;
 
  return realsize;
}

static size_t logger_read_callback(void *contents, size_t size, 
    size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct DataStruct *mem = (struct DataStruct *)userp;
 
  mem->data = realloc(mem->data, mem->size + realsize + 1);
  if(mem->data == NULL) {
    /* out of memory! */ 
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }
 
  memcpy(&(mem->data[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->data[mem->size] = 0;
 
  return realsize;
}

char * get_check_connection_resource(void)
{
    // need to implement api key...
    const char * x_api_key = "x-api-key: LxU6jiAU8z3IzMG5GpPKZ2tQJR6X7Rp17iX9oWy8";
    //int return_value;
    long http_status_code;

    /* CURL stuff */
    CURL *curl_handle;
    CURLcode res;

    /* For received data */
    struct DataStruct server_response;
    server_response.data = malloc(1);  /* will be grown as needed by the realloc above */ 
    server_response.size = 0;    /* no data at this point */
 
    /* For request headers */
    struct curl_slist *headers = NULL;

    //curl_global_init(CURL_GLOBAL_DEFAULT);

    /* init the curl session */ 
    curl_handle = curl_easy_init();

    if(curl_handle)
    {
        // used to store size of error
        char errbuf[CURL_ERROR_SIZE];

        /* Specify Accept header as JSON */
        headers = curl_slist_append(headers, "Accept: application/json");

        /* Add AWS Authentication Key to request header */
        headers = curl_slist_append(headers, x_api_key);

        /* provide a buffer to store errors in */
        curl_easy_setopt(curl_handle, CURLOPT_ERRORBUFFER, errbuf);

        /* Add headers to CURL */
        curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headers);

        /* specify url to GET */
        curl_easy_setopt(curl_handle, CURLOPT_URL, aws_check_connection_url);

        /* send all received data to this function */
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, get_check_connection_resource_callback);
        
        /* we pass our 'server_response' struct to the callback function */ 
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&server_response);

        /* some servers don't like requests that are made without a user-agent
            field, so we provide one */ 
        curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

        /* get verbose debug output please */ 
        //curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);
 
        /* set the error buffer as empty before performing a request */
        errbuf[0] = 0;

        /* Perform the request, res will get the return code */ 
        res = curl_easy_perform(curl_handle);

        /* Check for errors */ 
        if(res != CURLE_OK)
        {
            //get length of error.
            size_t len = strlen(errbuf);
            // Combine partial paths to get full message
            char message[200];

            // print out either detailed message or generic.
            if(len)
            {
                //print to console
                printf(KRED "%s%s", errbuf,
                    ((errbuf[len - 1] != '\n') ? "\n" : "")
                );
                printf(KNRM "");

                //create message to save to log.json
                sprintf(message, "curl_easy_perform() failed when attempting to check connection to AWS."
                    "Error: %s%s",
                    errbuf,
                    ((errbuf[len - 1] != '\n') ? "\n" : "")
                );

                // error code is 3 (curl error)
                str_save_to_log(message, 3);
            }
            else
            {
                printf(KRED "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res)
                );
                printf(KNRM "");

                //create message to save to log.json
                sprintf(message, "curl_easy_perform() failed when attempting to check connection to AWS."
                    "Error: %s", strdup(curl_easy_strerror(res)));

                // error code is 3 (curl error)
                str_save_to_log(message, 3);
            }

            //returns char * data (empty)
            return 0;
        }

        /*
        * Now, our server_response.data 
        * points to a memory block that is server_response.size
        * bytes big and contains the server's response.
        */

        /* get HTTP Status code */
        curl_easy_getinfo (curl_handle, CURLINFO_RESPONSE_CODE, &http_status_code);

        /* always cleanup */ 
        curl_easy_cleanup(curl_handle);

        /* check for failure */
        if(server_response.size == 0) //fail
        {
            server_response.data = 0;
            str_save_to_log("Server response is empty.", 1);
            return server_response.data;
        }
        if(http_status_code != 200) //fail
        {
            server_response.data = 0;
            str_save_to_log("Status code is not 200", 1);
            return server_response.data;
        }

        //free(server_response.data);
    }
    else
    {
        printf("Could not make request to AWS for some reason.\n");
        printf(KNRM "");

        // Combine partial paths to get full message
        char message[200];
        sprintf(message, "curl_easy_perform() failed when attempting to check connection to AWS.");

        // error code is 3 (curl error)
        str_save_to_log(message, 3);

        return 0;
    }
    /* we're done with libcurl, so clean it up */ 
    //curl_global_cleanup();

    return server_response.data;
}

int aws_send_to_eagleio(long raw, long real)
{
    // need to implement api key...
    const char * x_api_key = "x-api-key: LxU6jiAU8z3IzMG5GpPKZ2tQJR6X7Rp17iX9oWy8";
    char json_data[256];
    long http_status_code;
    sprintf(json_data,
        "{"
        "\"value\":[%ld,%ld]"
        "}",raw,real
    );

    /* CURL stuff */
    CURL *curl_handle;
    CURLcode res;

    /* For request headers */
    struct curl_slist *headers = NULL;

    /* For received data */
    struct DataStruct server_response;
    server_response.data = malloc(1);  /* will be grown as needed by the realloc above */ 
    server_response.size = 0;    /* no data at this point */
 
    curl_global_init(CURL_GLOBAL_DEFAULT);

    /* init the curl session */ 
    curl_handle = curl_easy_init();

    if(curl_handle)
    {
        // used to store size of error
        char errbuf[CURL_ERROR_SIZE];

        /* Specify Accept header as JSON */
        headers = curl_slist_append(headers, "Accept: application/json");

        /* Add AWS Authentication Key to request header */
        headers = curl_slist_append(headers, x_api_key);

        /* Disable HTTP 1.1 "Expect: 100-continue" header */
        headers = curl_slist_append(headers, "Expect:");

        /* provide a buffer to store errors in */
        curl_easy_setopt(curl_handle, CURLOPT_ERRORBUFFER, errbuf);
        
        /* Add headers to CURL */
        curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headers);

        /* Set the URL that is about to receive our POST. */ 
        curl_easy_setopt(curl_handle, CURLOPT_URL, aws_eagleio_url);
    
        /* Now specify the POST data */ 
        curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, json_data);

        /* send all received data to this function */
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, eagleio_read_callback);
        
        /* we pass our 'server_response' struct to the callback function */ 
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&server_response);

        /* some servers don't like requests that are made without a user-agent
            field, so we provide one */ 
        curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

        /* get verbose debug output please */ 
        //curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);
 
        /* set the error buffer as empty before performing a request */
        errbuf[0] = 0;

        /* Perform the request, res will get the return code */ 
        res = curl_easy_perform(curl_handle);

        /* Check for errors */ 
        if(res != CURLE_OK)
        {
            //get length of error.
            size_t len = strlen(errbuf);
            // Combine partial paths to get full message
            char message[200];

            // print out either detailed message or generic.
            if(len)
            {
                //print to console
                printf(KRED "%s%s", errbuf,
                    ((errbuf[len - 1] != '\n') ? "\n" : "")
                );
                printf(KNRM "");

                //create message to save to log.json
                sprintf(message, "curl_easy_perform() failed when attempting to check connection to AWS."
                    "Error: %s%s",
                    errbuf,
                    ((errbuf[len - 1] != '\n') ? "\n" : "")
                );

                // error code is 3 (curl error)
                str_save_to_log(message, 3);
            }
            else
            {
                printf(KRED "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res)
                );
                printf(KNRM "");

                //create message to save to log.json
                sprintf(message, "curl_easy_perform() failed when attempting to check connection to AWS."
                    "Error: %s", strdup(curl_easy_strerror(res)));

                // error code is 3 (curl error)
                str_save_to_log(message, 3);
            }

            return 0;
        }

        /* get HTTP Status code */
        curl_easy_getinfo (curl_handle, CURLINFO_RESPONSE_CODE, &http_status_code);

        /* check success code */
        if(http_status_code != 200)
        {
            printf(KRED "HTTP status code: (%ld) is NOT 200",
                http_status_code
            );
            printf(KNRM "");

            // error code is 2
            str_save_to_log("HTTP status code is NOT 200", 2);
            return 0;
        }
        printf("\nThe actual value from aws was: %s\n", server_response.data);

        /* always cleanup */ 
        curl_easy_cleanup(curl_handle);

        //free(server_response.data);
    }
    else
    {
        printf(KRED "Could not make network request to AWS for some reason.\n");
        printf(KNRM "");

        server_response.data = 0;

        // error code is 1
        str_save_to_log("Could not make network request to AWS for some reason", 1);

        return 0;
    }
    /* we're done with libcurl, so clean it up */ 
    //curl_global_cleanup();

    return 1;
}

char * get_aws_logger_data(void)
{
    // need to implement api key...
    const char * x_api_key = "x-api-key: LxU6jiAU8z3IzMG5GpPKZ2tQJR6X7Rp17iX9oWy8";
    //int return_value;
    long http_status_code;

    /* CURL stuff */
    CURL *curl_handle;
    CURLcode res;

    /* For received data */
    struct DataStruct server_response;
    server_response.data = malloc(1);  /* will be grown as needed by the realloc above */ 
    server_response.size = 0;    /* no data at this point */
 
    /* For request headers */
    struct curl_slist *headers = NULL;

    //curl_global_init(CURL_GLOBAL_DEFAULT);

    /* init the curl session */ 
    curl_handle = curl_easy_init();

    if(curl_handle)
    {
        // used to store size of error
        char errbuf[CURL_ERROR_SIZE];

        /* Specify Accept header as JSON */
        headers = curl_slist_append(headers, "Accept: application/json");

        /* Add AWS Authentication Key to request header */
        headers = curl_slist_append(headers, x_api_key);

        /* provide a buffer to store errors in */
        curl_easy_setopt(curl_handle, CURLOPT_ERRORBUFFER, errbuf);
        
        /* Add headers to CURL */
        curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headers);

        /* specify url to GET */
        curl_easy_setopt(curl_handle, CURLOPT_URL, aws_logger_url);

        /* send all received data to this function */
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, get_check_connection_resource_callback);
        
        /* we pass our 'server_response' struct to the callback function */ 
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&server_response);

        /* some servers don't like requests that are made without a user-agent
            field, so we provide one */ 
        curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

        /* get verbose debug output please */ 
        //curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);
 
        /* set the error buffer as empty before performing a request */
        errbuf[0] = 0;

        /* Perform the request, res will get the return code */ 
        res = curl_easy_perform(curl_handle);

        /* Check for errors */ 
        if(res != CURLE_OK)
        {
            //get length of error.
            size_t len = strlen(errbuf);
            // Combine partial paths to get full message
            char message[200];

            // print out either detailed message or generic.
            if(len)
            {
                //print to console
                printf(KRED "%s%s", errbuf,
                    ((errbuf[len - 1] != '\n') ? "\n" : "")
                );
                printf(KNRM "");

                //create message to save to log.json
                sprintf(message, "curl_easy_perform() failed when attempting to check connection to AWS."
                    "Error: %s%s",
                    errbuf,
                    ((errbuf[len - 1] != '\n') ? "\n" : "")
                );

                // error code is 3 (curl error)
                str_save_to_log(message, 3);
            }
            else
            {
                printf(KRED "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res)
                );
                printf(KNRM "");

                //create message to save to log.json
                sprintf(message, "curl_easy_perform() failed when attempting to check connection to AWS."
                    "Error: %s", strdup(curl_easy_strerror(res)));

                // error code is 3 (curl error)
                str_save_to_log(message, 3);
            }

            //returns char * data (empty)
            return 0;
        }

        /*
        * Now, our server_response.data 
        * points to a memory block that is server_response.size
        * bytes big and contains the server's response.
        */

        /* get HTTP Status code */
        curl_easy_getinfo (curl_handle, CURLINFO_RESPONSE_CODE, &http_status_code);

        /* always cleanup */ 
        curl_easy_cleanup(curl_handle);

        /* check for failure */
        if(server_response.size == 0 || http_status_code != 200) //fail
        {
            // error code is 1
            str_save_to_log("Could not make network request to AWS for some reason", 1);
            return 0;
        }

        //free(server_response.data);
    }
    else {
        printf("Could not make request to AWS for some reason.\n");
        printf(KNRM "");

        // error code is 1
        str_save_to_log("Could not make request to AWS for some reason.", 1);
        return 0;
    }
    /* we're done with libcurl, so clean it up */ 
    //curl_global_cleanup();

    return server_response.data;
}

int post_aws_logger_data(char * contents)
{
    // need to implement api key...
    const char * x_api_key = "x-api-key: LxU6jiAU8z3IzMG5GpPKZ2tQJR6X7Rp17iX9oWy8";
    long http_status_code;

    /* CURL stuff */
    CURL *curl_handle;
    CURLcode res;

    /* For request headers */
    struct curl_slist *headers = NULL;

    /* For received data */
    struct DataStruct server_response;
    server_response.data = malloc(1);  /* will be grown as needed by the realloc above */ 
    server_response.size = 0;    /* no data at this point */
 
    curl_global_init(CURL_GLOBAL_DEFAULT);

    /* init the curl session */ 
    curl_handle = curl_easy_init();

    if(curl_handle)
    {
        // used to store size of error
        char errbuf[CURL_ERROR_SIZE];

        /* Specify Accept header as JSON */
        headers = curl_slist_append(headers, "Accept: application/json");

        /* Add AWS Authentication Key to request header */
        headers = curl_slist_append(headers, x_api_key);

        /* Disable HTTP 1.1 "Expect: 100-continue" header */
        headers = curl_slist_append(headers, "Expect:");

        /* Add headers to CURL */
        curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headers);

        /* Set the URL that is about to receive our POST. */ 
        curl_easy_setopt(curl_handle, CURLOPT_URL, aws_logger_url);
    
        /* Now specify the POST data */ 
        curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, contents);

        /* send all received data to this function */
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, logger_read_callback);
        
        /* we pass our 'server_response' struct to the callback function */ 
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&server_response);

        /* some servers don't like requests that are made without a user-agent
            field, so we provide one */ 
        curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

        /* get verbose debug output please */ 
        //curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);
 
        /* Perform the request, res will get the return code */ 
        res = curl_easy_perform(curl_handle);

        /* Check for errors */ 
        if(res != CURLE_OK)
        {
            //get length of error.
            size_t len = strlen(errbuf);
            // Combine partial paths to get full message
            char message[200];

            // print out either detailed message or generic.
            if(len)
            {
                //print to console
                printf(KRED "%s%s", errbuf,
                    ((errbuf[len - 1] != '\n') ? "\n" : "")
                );
                printf(KNRM "");

                //create message to save to log.json
                sprintf(message, "curl_easy_perform() failed when attempting to check connection to AWS."
                    "Error: %s%s",
                    errbuf,
                    ((errbuf[len - 1] != '\n') ? "\n" : "")
                );

                // error code is 3 (curl error)
                str_save_to_log(message, 3);
            }
            else
            {
                printf(KRED "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res)
                );
                printf(KNRM "");

                //create message to save to log.json
                sprintf(message, "curl_easy_perform() failed when attempting to check connection to AWS."
                    "Error: %s", strdup(curl_easy_strerror(res)));

                // error code is 3 (curl error)
                str_save_to_log(message, 3);
            }
            
            return 0;
        }

        /* get HTTP Status code */
        curl_easy_getinfo (curl_handle, CURLINFO_RESPONSE_CODE, &http_status_code);

        /* check success code */
        if(http_status_code != 200)
        {
            printf(KRED "HTTP status code: (%ld) is NOT 200",
                http_status_code
            );
            printf(KNRM "");

            // error code is 1
            str_save_to_log("HTTP status code is NOT 200", 1);

            return 0;
        }

        //printf("\nThe actual value from aws was: %s\n", server_response.data);

        /* always cleanup */ 
        curl_easy_cleanup(curl_handle);

        //free(server_response.data);
    }
    else
    {
        printf(KRED "Could not make network request to AWS for some reason.\n");
        printf(KNRM "");

        str_save_to_log("Could not make network request to AWS for some reason.", 3);

        return 0;
    }
    /* we're done with libcurl, so clean it up */ 
    //curl_global_cleanup();

    return 1;
}