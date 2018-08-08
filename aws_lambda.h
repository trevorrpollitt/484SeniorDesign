#ifndef AWS_LAMBDA_H_
#define AWS_LAMBDA_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h> //for timespec struct
#include <errno.h>

#include <curl/curl.h>
#include "console_colors.h"
#include "the_structs.h"
#include "logfile.h"

/* static functions should NOT be defined in .h files */

char * get_check_connection_resource(void);

int aws_send_to_eagleio(long raw, long real);

char * get_aws_logger_data(void);

int post_aws_logger_data(char * contents);

#endif