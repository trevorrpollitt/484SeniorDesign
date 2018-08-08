#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h> //for timespec struct
#include <pthread.h>

// void *blink_D0(void *t);
// void *blink_D1(void *t);

void blink_D0();
void blink_D1();

int main(int argc, char** argv)
{
    /*
    int ret1, ret2;
    pthread_t thread1, thread2;
    char *message1 = "Thread 1";
    char *message2 = "Thread 2";

    ret1 = pthread_create(&thread1, NULL, blink_D0, (void*)message1);
    ret2 = pthread_create(&thread2, NULL, blink_D1, (void*)message2);
    
    pthread_join( thread1, NULL);
    pthread_join( thread2, NULL); 

    printf("Thread 1 returns: %d\n",ret1);
    printf("Thread 2 returns: %d\n",ret2);
    */

    blink_D0();
    //blink_D1();

    return 0;
}

void blink_D0()
{
    printf("D0 blinking has started\r\n");
    
    int f_temp; int fd;

    struct timespec ts = {0, 60000L }; //60us = 60000ns
    char d0_value_location[] = "/sys/class/gpio/gpio132/value";
    char on = '1';
    char off = '0';

    //tell system to start listening to D0
    f_temp = open("/sys/class/gpio/export", O_WRONLY);
    write(f_temp, "132", 1);
    close(f_temp);

    //set direction to out...be able to write values
    f_temp = open("/sys/class/gpio/gpio132/direction", O_WRONLY);
    write(f_temp, "out", 1);
    close(f_temp);

    /*
    fastest way with write() & close()
    instead of fwrite or fprintf
    */
    for(;;)
    {
        fd = open(d0_value_location, O_WRONLY);
        write(fd, &on, 1);
        close(fd);
        //nanosleep (&ts, NULL);

        fd = open(d0_value_location, O_WRONLY);
        write(fd, &off, 1);
        close(fd);
        //nanosleep (&ts, NULL);
    }
}

void blink_D1()
{
    printf("D1 blinking has started\r\n");
    
    int f_temp; int fd;

    struct timespec ts = {0, 60000L }; //60us = 60000ns
    char d1_value_location[] = "/sys/class/gpio/gpio133/value";
    char on = '1';
    char off = '0';

    //tell system to start listening to D1
    f_temp = open("/sys/class/gpio/export", O_WRONLY);
    write(f_temp, "133", 1);
    close(f_temp);

    //set direction to out...be able to write values
    f_temp = open("/sys/class/gpio/gpio133/direction", O_WRONLY);
    write(f_temp, "out", 1);
    close(f_temp);

    /*
    fastest way with write() & close()
    instead of fwrite or fprintf
    */
    for(;;)
    {
        fd = open(d1_value_location, O_WRONLY);
        write(fd, &on, 1);
        close(fd);
        //nanosleep (&ts, NULL);

        fd = open(d1_value_location, O_WRONLY);
        write(fd, &off, 1);
        close(fd);
        //nanosleep (&ts, NULL);
    }
}