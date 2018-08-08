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
#include "hx711.h"

// Source file specific globals
int valuefd132, valuefd133;
char value132; char value133;
struct timespec ts;
unsigned char data[24];
long _offset;
float _scale;

// Definition of external variables
long tareValue = 0;
long calibrationValue = 1;
int setup_result = 0;

size_t strlen(const char *str)
{
    const char *s;
    for (s = str; *s; ++s);
    return(s - str);
}

void set_sleep_amount(long nanoseconds){
    ts.tv_sec  = 0;
    ts.tv_nsec = nanoseconds;
}

int setupPins(void)
{
    int exportfd, unexportfd, directionfd132,directionfd133;
    
    //The GPIO has to be exported to be able to see it in sysfs
 
    exportfd = open("/sys/class/gpio/export", O_WRONLY);
    if (exportfd < 0)
    {
        printf("Cannot open GPIO to export it\n");
        return -1;
    }
    write(exportfd, "132", 3); //D0
    write(exportfd, "133", 3); //D1
    close(exportfd);         


    // Update the direction of the GPIO pins.
    // D0 = sysfs # 132 = DAT
    // D1 = sysfs # 133 = CLK

    // D0 = input since D0 = DAT
    directionfd132 = open("/sys/class/gpio/gpio132/direction", O_RDWR);            
    if (directionfd132 < 0)
    {
        printf("Cannot open GPIO direction for pin 132\n");                     
        return -2;       
    }
    write(directionfd132, "in", 3);
    close(directionfd132);

    // D1 = output since D1 = CLK
    directionfd133 = open("/sys/class/gpio/gpio133/direction", O_RDWR);            
    if (directionfd133 < 0)
    {
        printf("Cannot open GPIO direction for pin 133\n");                     
        return -3;      
    }
    write(directionfd133, "out", 3);
    close(directionfd133);

    // Get the GPIO value ready.
    // Not closing the fds here since they will be needed continuously

    valuefd132 = open("/sys/class/gpio/gpio132/value", O_RDWR);
    if (valuefd132 < 0)
    {
        printf("Cannot open GPIO pin 132 value\n");
        return -4;
    }

    valuefd133 = open("/sys/class/gpio/gpio133/value", O_RDWR);
    if (valuefd133 < 0)
    {
        printf("Cannot open GPIO pin 133 value\n");
        return -5;
    }


    // According to hx711 docs,
    // initially CLK = D1 needs to be low
    write(valuefd133,"0", 1);
}

int hx711_isReady(void)
{
    // Read value of D0 = DAT 
    // If value = 1, hx711 is NOT ready
    // If value = 0, hx711 is ready

    // valuefd132 and valuefd133 already have been set by setupPins();
    lseek(valuefd132, 0, SEEK_SET);
    if(read(valuefd132, &value132, 1) < 0)
    {
        printf("Error reading pin 132 value:\n"); printf(strerror(errno)); printf("\n");
        return -1;
    }
    //printf("GPIO pin 132 value is %c\n", &value132);
    if (value132 == '0') return 1;
    else return 0;
}

long hx711_correct_twos_complement(long longValue)
{
    long bitsToRead = 24;
    long twosComplementThreshold = 1 << (bitsToRead-1);
    long twosComplementOffset = -(1 << (bitsToRead));

    if(longValue >= twosComplementThreshold){
        return longValue + twosComplementOffset;
    }
    else{
        return longValue;
    }
}

long hx711_read(long tareValue, long calibrationValue)
{
	int i;
    // In C true is represented by any numeric value not equal to 0
    // and false is represented by 0

    for(;;)
    {
        int res = hx711_isReady();
        if(res < 0) return -1; //check for error
        if(res == 1) break; // the ready condition: exit for-loop
    }
    
    long longValue = 0; unsigned char index;

	for(index = 0; index < 24; index++)
	{
		// write '1' to D1 = CLK
		write(valuefd133,"1", 1);

		// read D0 = DAT value into variable value132
		lseek(valuefd132, 0, SEEK_SET);
		read(valuefd132, &value132, 1);

		// convert char value to long
		long bitValue = (long) (value132 - '0');

		// write '0' to D1 = CLK
		write(valuefd133,"0", 1);

		longValue = longValue << 1;
		longValue = longValue | bitValue;
		
	}
	// set channel and gain factor to 128
	write(valuefd133,"1", 1);
	write(valuefd133,"0", 1);

    long res = hx711_correct_twos_complement(longValue);
	res = res - tareValue;
	res = res / calibrationValue;
	return res;
}

long hx711_averageRead(long tare, long calibration)
{
	long totalValue = 0;
	long averageValue = 0;
	long longValue = 0;
	int badValueCount = 0;
	long i;
	
	for(i=0;i<=10;i++)
	{
		longValue = hx711_read(tare,calibration);
		
		printf("Value = %ld | averageValue = %ld | totalValue = %ld | i = %ld\n",longValue,averageValue,totalValue,i);
		// if read value has >10% deviance from the average of the current read cycle, and i > 3 so you already have a decent average
		if(i > 3 && longValue >= 0){
			if((longValue > ((totalValue / i) * 1.25)) || (longValue < ((totalValue / i) * .75))){
				printf("Bad value = %ld | averageValue = %ld | totalValue = %ld | i = %ld\n",longValue,averageValue,totalValue,i);
				i--; // discard value and redo the read.
				badValueCount++;
			}
			
			// if all error checking is okay, add to total.
			else{
				if(i>0){
					averageValue = totalValue / i;
				}
				totalValue += longValue; // keep running total for average later
				badValueCount = 0; // reset bad value counter when you get a good value.
			}
		}
		else if(i > 3 && longValue < 0){
			if((longValue < ((totalValue / i) * 1.25)) || (longValue > ((totalValue / i) * .75))){
				printf("Bad value = %ld | averageValue = %ld | totalValue = %ld | i = %ld\n",longValue,averageValue,totalValue,i);
				i--; // discard value and redo the read.
				badValueCount++;
			}
			
			// if all error checking is okay, add to total.
			else{
				if(i>0){
					averageValue = totalValue / i;
				}
				totalValue += longValue; // keep running total for average later
				badValueCount = 0; // reset bad value counter when you get a good value.
			}
		}
		else{
			if(i>0){
				averageValue = totalValue / i;
			}
			totalValue += longValue;
			badValueCount = 0;
		}
		
		// If you get multiple bad values in a row, assume bad average and redo the entire read cycle.
		if(badValueCount > 3){
			i = -1; // reset loop counter
			totalValue = 0; // reset running total
			badValueCount = 0; // reset bad value counter since you're restarting the loop
			printf("Got bad values!\n");
		}
	}
	
	longValue = totalValue / i;
	return longValue;
}

void powerDown(void)
{
    // set sleep amount of 100us
    set_sleep_amount(100000L);

    // write '0' to D1 = CLK
    write(valuefd133,"0", 1);
    // write '1' to D1 = CLK
    write(valuefd133,"1", 1);

    //sleep
    nanosleep(&ts, NULL);
}

void powerUp(void)
{
    // GPIO.output(self.PD_SCK, False)
    // time.sleep(0.0001)

    // set sleep amount of 100us
    set_sleep_amount(100000L);

    // write '0' to D1 = CLK
    write(valuefd133,"0", 1);

    //sleep
    nanosleep(&ts, NULL);
}

void hx711_reset(void)
{
    powerDown();
    powerUp();
}

void hx711_tare(void){
	
	tareValue = hx711_averageRead(0,1);
}

void hx711_Set_Calibration(void){
		
	calibrationValue = hx711_averageRead(tareValue,1);
}