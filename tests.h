#ifndef TESTS_H_
#define TESTS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "aws_lambda.h"
#include "chipui.h"
#include "console_colors.h"
#include "json_stuff.h"
#include "lights.h"
#include "hx711.h"
#include "the_structs.h"
#include "logfile.h"

int test_all(void);

int test_error_file(void);

int test_hx711_pins(void);

int test_LED_pins(void);

/* This unlike ensure_connection_to_aws()
 * does NOT have a while loop
 * Use to test 1 time only NOT repeatedly
*/
int test_aws_connection(void);

int test_wifi(void);

#endif