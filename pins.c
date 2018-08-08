#include "pins.h"


int checkIfPinExported(char sysfs_pin[])
{
    int fd;
    char partial_path[] = "/sys/class/gpio/gpio%s";

    // Combine partial paths to get full sysfs path
    char path[24] = {0};
    sprintf(path, partial_path, sysfs_pin);

    /* Try to open folder.
     * If able to, then pin is already exported.
     * If not, pin is UNexported
    */
    fd = open(path, O_RDONLY);
    if (fd < 0)
    {
        //pin has NOT been exported
        close(fd);
        return 0;
    }
    else {
        //pin HAS been exported
        close(fd);
        return 1;
    }
    
}

/*void toggle(char pinValue){
	
	//create full sysfs path (Direction) set to output
	int pinNumber = pinValue - 48;
	char path[30] = {};
	char partial_direction_path[] = "echo %s > /sys/class/gpio/gpio%d/direction";
	sprintf(path, partial_direction_path, "out", pinNumber);
	system(path);
	
	//create full sysfs path (Value = 1) turn LEDs on
	char partial_value_path[] = "echo %d > /sys/class/gpio/gpio%d/value";
	sprintf(path, partial_value_path, 1, pinNumber);
	system(path);
	
	sleep(1);
	
	//create full sysfs path (Value = 0) turn LEDs off
	sprintf(path, partial_value_path, 0, pinNumber);
	system(path);
	
	if(pinNumber == 132){
		//create full sysfs path (Direction) set direction for pin 133 (CLK)
		sprintf(path, partial_direction_path, "in", pinNumber);
		system(path);
	}
}*/

/*void toggleLEDs(void){
	
    ts1.tv_nsec = 100000000L;
	ts1.tv_sec = 0;
	char path[255] = {};
	char partial_value_path[] = "echo %d > /sys/class/gpio/gpio%s/value";
	char partial_direction_path[] = "echo %s > /sys/class/gpio/gpio%s/direction";

	//Set all pins to output
	sprintf(path,partial_direction_path,"out","132");
	system(path);
	sprintf(path,partial_direction_path,"out","133");
	system(path);
	sprintf(path,partial_direction_path,"out","134");
	system(path);
	sprintf(path,partial_direction_path,"out","135");
	system(path);
	sprintf(path,partial_direction_path,"out","136");
	system(path);
	sprintf(path,partial_direction_path,"out","137");
	system(path);
	sprintf(path,partial_direction_path,"out","138");
	system(path);
	sprintf(path,partial_direction_path,"out","139");
	system(path);
	
	//Turn on all pins
	sprintf(path,partial_value_path,1,"132");
	system(path);
	nanosleep(&ts1,NULL);
	sprintf(path,partial_value_path,1,"133");
	system(path);
	nanosleep(&ts1,NULL);
	sprintf(path,partial_value_path,1,"134");
	system(path);
	nanosleep(&ts1,NULL);
	sprintf(path,partial_value_path,1,"135");
	system(path);
	nanosleep(&ts1,NULL);
	sprintf(path,partial_value_path,1,"136");
	system(path);
	nanosleep(&ts1,NULL);
	sprintf(path,partial_value_path,1,"137");
	system(path);
	nanosleep(&ts1,NULL);
	sprintf(path,partial_value_path,1,"138");
	system(path);
	nanosleep(&ts1,NULL);
	sprintf(path,partial_value_path,1,"139");
	system(path);
	nanosleep(&ts1,NULL);
	
	//Turn off all pins
	sprintf(path,partial_value_path,0,"132");
	system(path);
	nanosleep(&ts1,NULL);
	sprintf(path,partial_value_path,0,"133");
	system(path);
	nanosleep(&ts1,NULL);
	sprintf(path,partial_value_path,0,"134");
	system(path);
	nanosleep(&ts1,NULL);
	sprintf(path,partial_value_path,0,"135");
	system(path);
	nanosleep(&ts1,NULL);
	sprintf(path,partial_value_path,0,"136");
	system(path);
	nanosleep(&ts1,NULL);
	sprintf(path,partial_value_path,0,"137");
	system(path);
	nanosleep(&ts1,NULL);
	sprintf(path,partial_value_path,0,"138");
	system(path);
	nanosleep(&ts1,NULL);
	sprintf(path,partial_value_path,0,"139");
	system(path);
	nanosleep(&ts1,NULL);

	//Set GPIO132 to input (DOUT)
    char partial133_direction_path[] = "echo in > /sys/class/gpio/gpio132/direction";
	system(partial133_direction_path);
}*/
	
// pin is the SYSFS pin #
// int readPinValue(char pin[])
// {
//     int valuefd; char value;

//     //create full sysfs path
//     char path[30] = {0};
//     char partial_direction_path[] = "/sys/class/gpio/gpio%s/value";
//     sprintf(path, partial_direction_path, pin);

//     //open
//     valuefd = open(path, O_RDONLY);
//     if (valuefd < 0)
//     {
//         printf(KRED "Cannot open SYFS GPIO pin %s.\n", path);    
//         printf(KRED "Error is: %s\n", strerror(errno));  
//         printf(KNRM ""); 
//         close(valuefd);              
//         return 0;
//     }

//     //read
//     lseek(valuefd, 0, SEEK_SET);
//     if(read(valuefd, &value, 1) < 0)
//     {
//         printf(KRED "Error reading pin %s value.\n", pin);
//         printf(KRED "Message is: %s\n", strerror(errno));
//         printf(KNRM "\n");
//         return 0;
//     }
    
//     return (int) (value - '0');
// }
