#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <inttypes.h>
#include <fcntl.h>
#include <string.h>
#include <time.h> //for timespec struct
#include <errno.h>

// External variables from hx711.c
extern long tareValue;
extern long calibrationValue;
extern int setup_result;

long hx711_averageRead(long tareValue, long calibrationValue);
size_t strlen(const char *str);
void set_sleep_amount(long nanoseconds);
int setupPins(void);
int setdownPins(void);
int hx711_isReady(void);
long hx711_correct_twos_complement(long longValue);
long hx711_read(long tareValue, long calibrationValue);
void powerDown(void);
void powerUp(void);
void hx711_reset(void);
void hx711_tare(void);
void hx711_Set_Calibration(void);