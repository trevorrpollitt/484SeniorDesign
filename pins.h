#ifndef SET_PINS_H_
#define SET_PINS_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <inttypes.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include "console_colors.h"
#include "hx711.h"
#include "logfile.h"

int checkIfPinExported(char sysfs_pin[]);

//int readPinValue(char pin[]);

#endif