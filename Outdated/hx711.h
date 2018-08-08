#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <time.h> //for timespec struct
#include <errno.h>

int valuefd132, valuefd133;
char value132[1] = {"0"}; char value133[1] = {"0"};
struct timespec ts;
unsigned char data[24];

size_t strlen(const char *str);
size_t strlen(const char *str)
{
    const char *s;
    for (s = str; *s; ++s);
    return(s - str);
}

void set_sleep_amount(long nanoseconds);
void set_sleep_amount(long nanoseconds){
    ts.tv_sec  = 0;
    ts.tv_nsec = nanoseconds;
}

int setupPins(void);
int setupPins(void)
{
    printf("This will export pins D0 and D1..."
        "and set D0 direction = in, D1 direction = out\r\n");

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
    printf("GPIO pins 132 and 133 exported successfully\n");            


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
    printf("GPIO pin 132 direction set as input successfully\n");

    // D1 = output since D1 = CLK
    directionfd133 = open("/sys/class/gpio/gpio133/direction", O_RDWR);            
    if (directionfd133 < 0)
    {
        printf("Cannot open GPIO direction for pin 133\n");                     
        return -3;      
    }
    write(directionfd133, "out", 3);
    close(directionfd133);
    printf("GPIO pin 133 direction set as output successfully\n");

    // Get the GPIO value ready.
    // Not closing the fds here since they will be needed continuously

    valuefd132 = open("/sys/class/gpio/gpio132/value", O_RDWR);
    if (valuefd132 < 0)
    {
        printf("Cannot open GPIO pin 132 value\n");
        return -4;
    }
    printf("GPIO pin 132 value opened.\n");

    valuefd133 = open("/sys/class/gpio/gpio133/value", O_RDWR);
    if (valuefd133 < 0)
    {
        printf("Cannot open GPIO pin 133 value\n");
        return -5;
    }
    printf("GPIO pin 133 value opened.\n");


    // According to hx711 docs,
    // initially CLK = D1 needs to be low
    write(valuefd133,"0", 1);


    // Write "0" value to the pins
    // Then close the fds opened right above

    // write(valuefd133,"0", 1);
    // lseek(valuefd133, 0, SEEK_SET);
    // if(read(valuefd133, value133, 1) < 0)
    // { printf("Error reading pin 133 value:\n"); printf(strerror(errno)); printf("\n"); }
    // printf("GPIO pin 133 value is %c\n", value133[0]);
    // close(valuefd133);

}

int hx711_isReady(void);
int hx711_isReady(void)
{
    // Read value of D0 = DAT 
    // If value = 1, hx711 is NOT ready
    // If value = 0, hx711 is ready

    // valuefd132 and valuefd133 already have been set by setupPins();
    lseek(valuefd132, 0, SEEK_SET);
    if(read(valuefd132, value132, 1) < 0)
    {
        printf("Error reading pin 132 value:\n"); printf(strerror(errno)); printf("\n");
        return -1;
    }
    //printf("GPIO pin 132 value is %c\n", value132[0]);
    if (value132[0] == '0') return 1;
    else return 0;
}

int hx711_read(void);
int hx711_read(void)
{
    // In C true is represented by any numeric value not equal to 0
    // and false is represented by 0

    // Check if ADC is ready
    for(;;)
    {
        int res = hx711_isReady();

        if(res < 0) return -1; //check for error
        if(res == 1) break; // the ready condition: exit for-loop

        // no error occured reading D0, but D0 isn't 0 yet, so sleep
        nanosleep(&ts, NULL); 
    }

    int data_index = 23; // data[] size is 24, but index is 0-23

    while(data_index != -1)
    {
        // digitalWrite(_pin_slk, HIGH);
        // bitWrite(data[j], i, digitalRead(_pin_dout));
        // digitalWrite(_pin_slk, LOW);

        // write '1' to D1 = CLK
        write(valuefd133,"1", 1);

        // read D0 = DAT value into variable value132
        lseek(valuefd132, 0, SEEK_SET);
        read(valuefd132, value132, 1);

        // write value133 into data[] array
        data[data_index] = value132[0];

        // write '0' to D1 = CLK
        write(valuefd133,"0", 1);
        
        // decrement data_index
        data_index--;
    }

	// digitalWrite(_pin_slk, HIGH);
	// digitalWrite(_pin_slk, LOW);

    // toggle D1 = CLK pin from high to low
    write(valuefd133,"1", 1);
    write(valuefd133,"0", 1);

	// data[2] ^= 0x80;
	// return ((uint32_t) data[2] << 16) | ((uint32_t) data[1] << 8)| (uint32_t) data[0];

    int a = 23;
    printf("\r\n");
    for(a; a > -1; a--){
        printf("%c ", data[a]);
    }
    printf("\r\n");
}