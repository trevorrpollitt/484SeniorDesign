#ifndef LIGHTS_H_
#define LIGHTS_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <inttypes.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include "pins.h"
#include "console_colors.h"
#include "logfile.h"

int setPinsForLEDs(void);

int controlCSID2(int turnOn);

int controlCSID3(int turnOn);

int controlCSID4(int turnOn);

int controlCSID5(int turnOn);

int controlCSID6(int turnOn);

int controlCSID7(int turnOn);

int toggleAll(void);

#endif