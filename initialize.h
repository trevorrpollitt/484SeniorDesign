#ifndef INITIALIZE_H_
#define INITIALIZE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aws_lambda.h"
#include "json_stuff.h"
#include "lights.h"
#include "tests.h"
#include "the_structs.h"
#include "logfile.h"

int initialize(int minutes);

//void ensure_connection_to_aws(void *  periodic_minutes);

int upload_log_to_aws(void);

#endif