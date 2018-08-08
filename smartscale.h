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

// clear screen macro definition
#define clear() printf("\033[H\033[J")


// External variables from hx711.c
extern long tareValue;
extern long calibrationValue;
extern int setup_result;
extern int gainSelect;
extern int unitSelect;

long hx711_averageRead(long tare, long calibration);
size_t strlen(const char *str);
void set_sleep_amount(long nanoseconds);
int setPinsForHx711(void);
int setdownPins(void);
int hx711_isReady(void);
long hx711_correct_twos_complement(long longValue);
long hx711_read(long tare, long calibration, long gain);
void powerDown(void);
void powerUp(void);
void hx711_reset(void);
void hx711_tare(void);
void hx711_Set_Calibration(void);
void chipui_wifiSetup(void);
void chipui_unitSelect(int unitSelect);
int readNumber(void);