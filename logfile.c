#include "logfile.h"

char * get_the_current_datetime(void)
{
    char time_string[100];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    strftime(time_string,
        sizeof(time_string)-1, 
        "%m/%d/%Y %H:%M:%S %Z", 
        t
    );

    return (char *) strdup(time_string);
}

int is_file_empty(FILE * fp)
{
    if (fp != NULL)
    {
        fseek (fp, 0, SEEK_END);
        int size = ftell(fp);

        if (size == 0) {
            return 1;
        }
        else return 0;
    }
    else return 1;
}

char * read_all_from_file(FILE * fp)
{
    if(fp == NULL) return 0;

    /* read ALL contents of file */
    char * buffer;
    long file_size;
    
    //get total file size
    fseek(fp, 0L, SEEK_END);
    file_size = ftell(fp);

    //reset file cursor
    fseek(fp, 0L, SEEK_SET);

    //allocate space
    buffer = (char *) calloc(file_size, sizeof(char));
    if(buffer == NULL)
    {
        printf(KRED "Could not allocate buffer space.\n");
        printf(KNRM "");

        // release control
        fclose(fp);
        return 0;
    }

    //copy file contents into buffer
    size_t bytes_read = fread(buffer, sizeof(char), file_size, fp);
    printf("File size is: %ld\n", file_size);
    printf("Bytes read using fread(): %zu\n", bytes_read);
    if(bytes_read == 0)
    {
        printf(KYEL "Data size read from file is 0!\n");
        printf(KNRM "");

        // release control
        fclose(fp);
        return 0;
    }

    return (char *) buffer;
}

int delete_log_data(void)
{
    // w+ will delete data first
    FILE * log_fp = fopen("log.json", "w+");
    if(log_fp == NULL)
    {
        // release control
        fclose(log_fp);
        return 0;
    }
    // release control
    fclose(log_fp);
    return 1;
}

int create_log_file(void)
{
    FILE * log_fp;
    int res;

// create/open log.json in current working dr
    // w+ will erase file first before writing if it exists
    log_fp = fopen("log.json", "a+");

    if(log_fp == NULL)
    {
        // release control
        fclose(log_fp);
        return 0;
    }

    //create logger struct
    struct_logger_data * log_struct = 
        malloc(sizeof(struct struct_logger_data *));
    log_struct->error_code = 0;
    log_struct->datetime = strdup(get_the_current_datetime());
    log_struct->data = strdup("Testing 'log.json'");

    if(is_file_empty(log_fp))
    {
        /* write date as first entry in file
        * fprintf params:
        * string of format specifiers
        * followed by the data according to the format
        */
        res = fprintf(log_fp,
            "%s",
            create_new_json_logger_array(log_struct)
        );
    }
    else
    {
        //get all data from file
        char * buffer = read_all_from_file(log_fp);
        if(buffer == 0)
        {
            printf(KRED "Could not read data from log file.\n");
            printf(KNRM "");

            // release control
            fclose(log_fp);
            return 0;
        }

        // delete data
        if(!delete_log_data())
        {
            printf(KRED "Could not empty log file.\n");
            printf(KNRM "");
            // release control
            fclose(log_fp);
            return 0;
        }

        //write existing & new data to file
        res = fprintf(log_fp,
            "%s",
            add_to_json_logger_data_array(buffer, log_struct)
        );
    }

    // check for successful write
    if(res <= 0)
    {
        printf("\n\nError occured writing to log.json\n\n");
        printf(KRED "Error is: %s\n", strerror(errno));
        printf(KNRM "");
        
        // release control
        fclose(log_fp);
        return 0;
    }

    // release control
    fclose(log_fp);
    return 1;
}

int str_save_to_log(const char * error_message, int error_code)
{
    FILE * log_fp;
    int res;

    /* open log.json in current working dr
     * mode is append
    */
    log_fp = fopen("log.json", "a+");

    if(log_fp == NULL)
    {
        printf(KRED "Could not open or create log.json file\n");
        printf(KNRM "");

        // release control
        fclose(log_fp);
        return 0;
    }

    if(error_message == NULL)
    {
        printf(KRED "The error_message string cannot be null.\n");
        printf(KNRM "");

        // release control
        fclose(log_fp);
        return 0;
    }

    //create logger struct
    struct_logger_data * error_data = 
        malloc(sizeof(struct struct_logger_data *));

    // set error code type
    error_data->error_code = error_code;

    // set datetime
    error_data->datetime = strdup(get_the_current_datetime());

    // set error message
    error_data->data = strdup(error_message);

    /* fprintf params:
    * string of format specifiers
    * followed by the data according to the format
    */
    
    //get all existing data from file
    char * buffer = read_all_from_file(log_fp);
    if(buffer == 0)
    {
        printf(KRED "Could not read data from log file.\n");
        printf(KNRM "");

        free(error_data);
        // release control
        fclose(log_fp);
        return 0;
    }

    // delete data
    if(!delete_log_data())
    {
        printf(KRED "Could not empty log file.\n");
        printf(KNRM "");

        free(error_data);
        // release control
        fclose(log_fp);
        return 0;
    }

    //write existing & new data to file
    res = fprintf(log_fp,
        "%s",
        add_to_json_logger_data_array(buffer, error_data)
    );

    // check for successful write
    if(res <= 0)
    {
        printf("\nError occured writing to log.json\n");
        printf(KRED "Error is: %s\n", strerror(errno));
        printf(KNRM "");

        free(error_data);
        // release control
        fclose(log_fp);
        return 0;
    }

    free(error_data);
    // release control
    fclose(log_fp);

    return 1;
}

int save_to_log(struct_logger_data * error_data)
{
    FILE * log_fp;
    int res;

    /* open log.json in current working dr
     * mode is append
    */
    log_fp = fopen("log.json", "a+");

    if(log_fp == NULL)
    {
        printf(KRED "Could not open or create log.json file\n");
        printf(KNRM "");

        // release control
        fclose(log_fp);
        return 0;
    }
    if(error_data->data == NULL)
    {
        printf(KRED "In the error_data struct, data cannot be null.\n");
        printf(KNRM "");

        // release control
        fclose(log_fp);
        return 0;
    }

    // set datetime
    error_data->datetime = strdup(get_the_current_datetime());

    /* fprintf params:
    * string of format specifiers
    * followed by the data according to the format
    */
    
    //get all data from file
    char * buffer = read_all_from_file(log_fp);
    if(buffer == 0)
    {
        printf(KRED "Could not read data from log file.\n");
        printf(KNRM "");

        // release control
        fclose(log_fp);
        return 0;
    }

    // delete data
    if(!delete_log_data())
    {
        printf(KRED "Could not empty log file.\n");
        printf(KNRM "");
        // release control
        fclose(log_fp);
        return 0;
    }

    //write existing & new data to file
    res = fprintf(log_fp,
        "%s",
        add_to_json_logger_data_array(buffer, error_data)
    );

    // check for successful write
    if(res <= 0)
    {
        printf("\nError occured writing to log.json\n");
        printf(KRED "Error is: %s\n", strerror(errno));
        printf(KNRM "");

        // release control
        fclose(log_fp);
        return 0;
    }

    // release control
    fclose(log_fp);

    return 1;
}

struct_logger_data * load_from_log_file(void)
{
    FILE * log_fp;
    //long file_size;
    char * buffer;

    //open file
    log_fp = fopen("log.json", "r");

    if(log_fp == NULL)
    {
        printf(KRED "Could not open or create log.json file\n");
        printf(KNRM "");

        // release control
        fclose(log_fp);
        return 0;
    }

    // read ALL contents of file . No need to pre-alloc
    buffer = strdup(read_all_from_file(log_fp));
    
    //create logger struct
    struct_logger_data * log_struct = malloc(sizeof(struct struct_logger_data *));
    log_struct->error_code = 0;
    log_struct->datetime = strdup(get_the_current_datetime());
    log_struct->data = strdup(buffer);

    // release control
    fclose(log_fp);
    free(buffer);

    return log_struct;
}

char * str_load_from_log_file(void)
{
    FILE * log_fp;
    //long file_size;
    //char * buffer;

    //open file
    log_fp = fopen("log.json", "r");

    if(log_fp == NULL)
    {
        printf(KRED "Could not open or create log.json file\n");
        printf(KNRM "");

        // release control
        fclose(log_fp);
        return 0;
    }

    // read ALL contents of file . No need to pre-alloc
    return strdup(read_all_from_file(log_fp));
}