#ifndef HX711_H_
#define HX711_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <fcntl.h>
#include <string.h>
#include <time.h> //for timespec struct
#include <errno.h>
#include <math.h>

#include "pins.h"
#include "chipui.h"
#include "console_colors.h"
#include "logfile.h"

// clear screen macro definition

#define clear() printf("\033[H\033[J")


// External variables from hx711.c
extern volatile int loop;
extern long tareValue;
extern long calibrationValue;
extern int setup_result;
extern int gainSelect;
extern int unitSelect;
extern long timeValue;

void set_sleep_amount(long nanoseconds);

//int setupPins(void); ---> USE: setPinsForHx711() instead. Found in setupPins.h
int setPinsForHx711(void);

void toggleLEDs(void);

void toggle(int pinNumber);

int hx711_isReady(void);

long hx711_correct_twos_complement(long longValue);

long hx711_read(long tare, long calibration, long gain);

long hx711_averageRead(long tare, long calibration);

void powerDown(void);

void powerUp(void);

void hx711_reset(void);

void hx711_tare(void);

void hx711_Set_Calibration(void);

int chipui_wifiAdd(void);

int chipui_wifiStatus(void);

int chipui_wifiRemove(void);

int chipui_wifiTest(void);

void chipui_unitSelect(int unitSelect);

int readNumber(void);

size_t strlen(const char *str);

void set_sleep_amount(long nanoseconds);

int chipui_wifiCheck(char* wifissid, int index);

char* getstring(void);

int parsePing(void);

int chipui_saveData(long tarevalue, long calibrationvalue, int timevalue, struct tm c_time);

int chipui_loadData(void);

int diff(struct tm l_time, struct tm c_time);

#endif