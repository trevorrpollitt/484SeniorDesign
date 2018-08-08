#ifndef CHIPUI_H_
#define CHIPUI_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hx711.h"
#include "logfile.h"

int chipui_wifiAdd(void);

void chipui_unitSelect(int unitSelect);

int chipui_wifiStatus(void);

int chipui_wifiRemove(void);

int chipui_wifiTest(void);

int chipui_wifiCheck(char* wifissid, int index);

char* getstring(void);

int parsePing(void);

#endif