#include "hx711.h"

// Source file specific globals
int valuefd132, valuefd133;
char value132; char value133;
struct timespec ts, ts_1;
unsigned char data[24];
long _offset;
float _scale;

// Definition of external variables
long tareValue = 0;
long calibrationValue = 1;
int setup_result = 0;
int gainSelect = 1;
int unitSelect = 0;
long timeValue = 0;

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

int setPinsForHx711(void)
{
    set_sleep_amount(100000000L);
    ts_1.tv_nsec = 2400000000LL;
    ts_1.tv_sec = 0;
    printf("\nSetting up pins CSID0 & CSID1 for HX711...\n");

    char *sysfs_pins[] = { "132", "133"};
    char partial_direction_path[] = "/sys/class/gpio/gpio%s/direction";

    int index, exportfd, directionfd, res;

    /*
     * this for-loop is to
     * set export mode on pins
    */
    printf("\nSetting pin mode...\n");
    for(index = 0; index < 2; index++)
    {
        /* RED ALERT:
         * This will guarantee fail if pin is already exported!
        */
        if(checkIfPinExported(sysfs_pins[index]))
        {
            //yellow color to console
            printf(KYEL "GPIO Pin %s has already been exported. Skipping it.\n",
                sysfs_pins[index]
            );
            printf(KNRM "");
            continue;
        }

        //The GPIO has to be exported to be able to see it in sysfs
        exportfd = open("/sys/class/gpio/export", O_WRONLY);
        if (exportfd < 0)
        {
            printf(KRED "Cannot open SYSFS GPIO pins %s to export it.\n", sysfs_pins[index]);
            printf(KRED "Error is: %s\n", strerror(errno));
            printf(KNRM "");
            close(exportfd);

            // Combine partial paths to get full message
            char message[200];
            sprintf(message, "Cannot open SYSFS GPIO pins %s to export it. Error: %s",
                sysfs_pins[index], strerror(errno));

            // error code is 4 (hx711)
            str_save_to_log(message, 4);
            return 0;
        }
        
        //Set CSID0 & CSID1 to export
        res = write(exportfd, sysfs_pins[index], 3);

        // return if write error
        if(res <= 0){
            printf(KRED "Oh dear, something went wrong with write()! %s\n",
                strerror(errno)
            );
            printf(KNRM "");
            close(exportfd);
            nanosleep(&ts_1,NULL);
            // error code is 4
            str_save_to_log(strerror(errno), 4);
            return 0;
        }
        else {
            //allow new setting to marinate...
            nanosleep(&ts,NULL);
        }
        
        close(exportfd);
    }

    printf("\nSetting pin direction...\n");
      
    /* 
     * this second for-loop
     * set direction to out
    */
    for(index = 0; index < 2; index++)
    {
        //create a full sysfs direction path
        char path[34] = {0};
        sprintf(path, partial_direction_path, sysfs_pins[index]);
        
        directionfd = open(path, O_WRONLY);            
        if (directionfd < 0)
        {
            printf(KRED "Cannot open GPIO direction for pin %s\n", sysfs_pins[index]);  
            printf(KRED "Message: %s\n", strerror(errno));
            printf(KNRM "");    
            nanosleep(&ts_1,NULL); 
            close(exportfd);
            
            // Combine partial paths to get full message
            char message[200];
            sprintf(message, "Cannot open SYSFS GPIO pin %s to set direction. Error: %s",
                sysfs_pins[index], strerror(errno));

            // error code is 4 (hx711)
            str_save_to_log(message, 4);             
            return 0;       
        }
        
        // pin 132 = D0 has direction = in
        // pin 133 = D1 has direction = out
        if(strcmp(sysfs_pins[index], "132") == 0)
        {
            res = write(directionfd, "in", 2);
        }
        else{
            res = write(directionfd, "out", 3);
        }

        // return if write error
        if(res <= 0){
            printf(KRED "Oh dear, something went wrong with write()! %s\n",
                strerror(errno)
            );
            printf(KNRM ""); 
            close(exportfd);
            nanosleep(&ts_1,NULL);
            
            // error code is 4
            str_save_to_log(strerror(errno), 4);
            return 0;
        }
        else {
            //allow new setting to marinate...
            nanosleep(&ts,NULL);
        }

        close(directionfd);
    }
    printf("GPIO pins D0 and D1 direction set successfully\n");

    /*
     * Get the GPIO value ready.
     * Not closing the fds here since they will be needed continuously
    */
    valuefd132 = open("/sys/class/gpio/gpio132/value", O_RDWR);
    if (valuefd132 < 0)
    {
        printf(KRED "Cannot open GPIO pin 132 value\n");    
        printf(KRED "Error is: %s\n", strerror(errno));  
        printf(KNRM ""); 
        close(valuefd132);   
        nanosleep(&ts_1,NULL);

        // error code is 4 (hx711)
        str_save_to_log("Cannot open SYSFS GPIO pin 132 for hx711 to read/write value.", 4);           
        return 0;
    }
    printf("GPIO pin 132 value opened.\n");

    valuefd133 = open("/sys/class/gpio/gpio133/value", O_RDWR);
    if (valuefd133 < 0)
    {
        printf(KRED "Cannot open GPIO pin 133 value\n");    
        printf(KRED "Error is: %s\n", strerror(errno));  
        printf(KNRM ""); 
        close(valuefd133); 
        nanosleep(&ts_1,NULL);

        // error code is 4 (hx711)
        str_save_to_log("Cannot open SYSFS GPIO pin 133 for hx711 to read/write value.", 4);   
        return 0;
    }
    printf("GPIO pin 133 value opened.\n");


    // According to hx711 docs,
    // initially CLK = D1 needs to be low
    res = write(valuefd133,"0", 1);

    if(res <= 0)
    {
        printf(KRED "Oh dear, something went wrong with write()! %s\n",
            strerror(errno)
        );  
        printf(KRED "Error is: %s\n", strerror(errno));  
        printf(KNRM "");   
        close(valuefd133);
        nanosleep(&ts_1,NULL);
        
        // error code is 4
        str_save_to_log(strerror(errno), 4);
        return 0;
    }

    /* Set console colors! */
    printf(KGRN "Done! ");
    printf(KNRM " Successfully set Pins For HX711.\n");
    
    nanosleep(&ts,NULL);

    return 1;
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
        printf("Error reading pin 132 value:\n");
        printf(strerror(errno));

        // Combine partial paths to get full message
        char message[200];
        sprintf(message, "Error reading from pin 132. Error: %s", strerror(errno));

        // error code is 4 (hx711)
        str_save_to_log(message, 4);   
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

long hx711_read(long tare, long calibration, long gain)
{
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

    if(gain == 2){
        write(valuefd133,"1", 1);
        write(valuefd133,"0", 1);
        write(valuefd133,"1", 1);
        write(valuefd133,"0", 1);
    }

    long res = hx711_correct_twos_complement(longValue);
	res = res - tare;
	res = res / calibration;
	return round(res);
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
		longValue = hx711_read(tare,calibration,gainSelect);
		
		//printf("Value = %ld | averageValue = %ld | totalValue = %ld | i = %ld | calibration = %ld\n",longValue,averageValue,totalValue,i,calibration);
		// if read value has >10% deviance from the average of the current read cycle, and i > 3 so you already have a decent average
		if(i > 3 && ((longValue >= 300 && calibration == 1) || (calibration != 1))){
			if((longValue > ((totalValue / i) * 1.10)) || (longValue < ((totalValue / i) * .90))){
				//printf("Bad value = %ld | averageValue = %ld | totalValue = %ld | i = %ld\n",longValue,averageValue,totalValue,i);
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
        else if(i > 3 && (longValue < 300 && calibrationValue == 1)){ //handle if raw value is less than 300
            if(averageValue > 400){
                i = -1; // reset loop counter
                totalValue = 0; // reset running total
                badValueCount = 0; // reset bad value counter since you're restarting the loop
                //printf("Got bad values!\n");
            }
        }
		else if(i > 3 && ((longValue < -300 && calibration == 1) || (calibration != 1))){
			if((longValue < ((totalValue / i) * 1.10)) || (longValue > ((totalValue / i) * .90))){
				//printf("Bad value = %ld | averageValue = %ld | totalValue = %ld | i = %ld\n",longValue,averageValue,totalValue,i);
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
        else if(i > 3 && (longValue > -300 && calibrationValue == 1)){ //handle if raw value is less than 300
            if(averageValue < -400){
                i = -1; // reset loop counter
                totalValue = 0; // reset running total
                badValueCount = 0; // reset bad value counter since you're restarting the loop
                //printf("Got bad values!\n");
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
		if(badValueCount>2){
			i = -1; // reset loop counter
			totalValue = 0; // reset running total
			badValueCount = 0; // reset bad value counter since you're restarting the loop
			//printf("Got bad values!\n");
		}
	}
	
	longValue = averageValue;
	return round(longValue);
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

void hx711_tare(void)
{	
	tareValue = hx711_averageRead(0,1);
}

void hx711_Set_Calibration(void)
{
    // Custom Calibration	
	calibrationValue = hx711_averageRead(tareValue,1);
}