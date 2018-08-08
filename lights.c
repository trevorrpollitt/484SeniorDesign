#include "lights.h"
#include "hx711.h"


/**
 * Physical Pins are 35 - 30
 * Sysfs pins: 134 - 139
 * This sets export mode and direction = out
*/
int setPinsForLEDs(void)
{
    struct timespec ts1;
    ts1.tv_nsec = 100000000L;
    ts1.tv_sec = 0;

    printf("\nSetting up pins CSID2 - CSID7 for LED output...\n");

    char *sysfs_pins[] = { "132", "133", "134", "135", "136", "137", "138", "139" };
    char partial_direction_path[] = "/sys/class/gpio/gpio%s/direction";

    int index, exportfd, directionfd;

    /*
     * this for-loop is to
     * set export mode on pins
    */
    printf("\nSetting pin mode...\n");
    for(index = 0; index < 8; index++)
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
            printf(KRED "Cannot open SYSFS LED GPIO pins to export it.\n");
            printf(KRED "Error is: %s\n", strerror(errno));
            printf(KNRM "");
            close(exportfd);
            str_save_to_log("Cannot open SYSFS LED GPIO pins to export it.", 5);
            return 0;
        }
        
        //Btw, pin names are also CSID2 ... CSID7 in numbering format
        int res = write(exportfd, sysfs_pins[index], 3);

        // return if write error
        if(res <= 0){
            printf(KRED "Could not write to sysfs pins for the LEDs: %s\n",
                strerror(errno)
            );
            printf(KNRM "");
            close(exportfd);
            str_save_to_log("Could not write to sysfs pins for the LEDs", 5);
            return 0;
        }
        else {
            //allow new setting to marinate...
            nanosleep(&ts1,NULL);
        }
        
        close(exportfd);
    }

    printf("\nSetting pin direction...\n");
      
    /* 
     * this second for-loop
     * set direction to out
    */
    for(index = 0; index < 8; index++)
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
            close(exportfd);
            str_save_to_log("Cannot open GPIO direction for pin %s", 5);              
            return 0;       
        }

        int res = write(directionfd, "out", 3);
        // return if write error
        if(res <= 0){
            printf(KRED "Could not write to sysfs pins for the LEDs! %s\n",
                strerror(errno)
            );
            printf(KNRM ""); 
            close(exportfd);
            str_save_to_log("Could not write to sysfs pins for the LEDs", 5);
            return 0;
        }

        close(directionfd);
    }
    
    nanosleep(&ts1,NULL);
    printf("GPIO pins 132 - 139 direction set successfully\n");

    /* Set console colors! */
    printf(KGRN "Done! ");
    printf(KNRM "Successfully set Pins For LED lights\n");

    return 1;
}

//CSID0 = SYSFS # 134
int controlCSID0(int turnOn)
{
    int valuefd, res;

    //open
    valuefd = open("/sys/class/gpio/gpio132/value", O_WRONLY);
    if (valuefd < 0)
    {
        printf(KRED "Cannot open GPIO pin CSID2 (sysfs 132)\n");    
        printf(KRED "Error is: %s\n", strerror(errno));  
        printf(KNRM ""); 
        close(valuefd);
        str_save_to_log("Cannot open GPIO pin CSID2 (sysfs 132)", 5);              
        return 0;
    }

    //write
    if(turnOn){
        res = write(valuefd,"1", 1);
    }
    else{
        res = write(valuefd,"0", 1);
    }

    if(res <= 0)
    {
        printf(KRED "Could not write to sysfs pins for the LEDs! %s\n",
            strerror(errno)
        );  
        printf(KRED "Error is: %s\n", strerror(errno));  
        printf(KNRM "");   
        close(valuefd);
        str_save_to_log("Could not write to sysfs pins for the LEDs", 5);
        return 0;
    }

    return 1;
}

//CSID1 = SYSFS # 134
int controlCSID1(int turnOn)
{
    int valuefd, res;

    //open
    valuefd = open("/sys/class/gpio/gpio133/value", O_WRONLY);
    if (valuefd < 0)
    {
        printf(KRED "Cannot open GPIO pin CSID2 (sysfs 133)\n");    
        printf(KRED "Error is: %s\n", strerror(errno));  
        printf(KNRM ""); 
        close(valuefd);
        str_save_to_log("Cannot open GPIO pin CSID2 (sysfs 133)", 5);             
        return 0;
    }

    //write
    if(turnOn){
        res = write(valuefd,"1", 1);
    }
    else{
        res = write(valuefd,"0", 1);
    }

    if(res <= 0)
    {
        printf(KRED "Could not write to sysfs pins for the LEDs %s\n",
            strerror(errno)
        );  
        printf(KRED "Error is: %s\n", strerror(errno));  
        printf(KNRM "");   
        close(valuefd);
        str_save_to_log("Could not write to sysfs pins for the LEDs", 5);
        return 0;
    }

    return 1;
}

//CSID2 = SYSFS # 134
int controlCSID2(int turnOn)
{
    int valuefd, res;

    //open
    valuefd = open("/sys/class/gpio/gpio134/value", O_WRONLY);
    if (valuefd < 0)
    {
        printf(KRED "Cannot open GPIO pin CSID2 (sysfs 134)\n");    
        printf(KRED "Error is: %s\n", strerror(errno));  
        printf(KNRM ""); 
        close(valuefd);  
        str_save_to_log("Cannot open GPIO pin CSID2 (sysfs 134)", 5);            
        return 0;
    }

    //write
    if(turnOn){
        res = write(valuefd,"1", 1);
    }
    else{
        res = write(valuefd,"0", 1);
    }

    if(res <= 0)
    {
        printf(KRED "Could not write to sysfs pins for the LEDs %s\n",
            strerror(errno)
        );  
        printf(KRED "Error is: %s\n", strerror(errno));  
        printf(KNRM "");   
        close(valuefd);
        str_save_to_log("Could not write to sysfs pins for the LEDs", 5); 
        return 0;
    }

    return 1;
}

//CSID3 = SYSFS # 135
int controlCSID3(int turnOn)
{
    int valuefd, res;

    //open
    valuefd = open("/sys/class/gpio/gpio135/value", O_WRONLY);
    if (valuefd < 0)
    {
        printf(KRED "Cannot open GPIO pin CSID3 (sysfs 135)\n");    
        printf(KRED "Error is: %s\n", strerror(errno));  
        printf(KNRM ""); 
        close(valuefd);      
        str_save_to_log("Cannot open GPIO pin CSID3 (sysfs 135)", 5);        
        return 0;
    }

    //write
    if(turnOn){
        res = write(valuefd,"1", 1);
    }
    else{
        res = write(valuefd,"0", 1);
    }

    if(res <= 0)
    {
        printf(KRED "Could not write to sysfs pins for the LEDs %s\n",
            strerror(errno)
        );  
        printf(KRED "Error is: %s\n", strerror(errno));  
        printf(KNRM "");   
        close(valuefd);
        str_save_to_log("Could not write to sysfs pins for the LEDs", 5); 
        return 0;
    }

    return 1;
}


//CSID4 = SYSFS # 136
int controlCSID4(int turnOn)
{
    int valuefd, res;

    //open
    valuefd = open("/sys/class/gpio/gpio136/value", O_WRONLY);
    if (valuefd < 0)
    {
        printf(KRED "Cannot open GPIO pin CSID4 (sysfs 136)\n");    
        printf(KRED "Error is: %s\n", strerror(errno));  
        printf(KNRM ""); 
        close(valuefd);
        str_save_to_log("Cannot open GPIO pin CSID3 (sysfs 136)", 5);                
        return 0;
    }

    //write
    if(turnOn){
        res = write(valuefd,"1", 1);
    }
    else{
        res = write(valuefd,"0", 1);
    }

    if(res <= 0)
    {
        printf(KRED "Could not write to sysfs pins for the LEDs %s\n",
            strerror(errno)
        );  
        printf(KRED "Error is: %s\n", strerror(errno));  
        printf(KNRM "");   
        close(valuefd);
        str_save_to_log("Could not write to sysfs pins for the LEDs", 5);
        return 0;
    }

    return 1;
}


//CSID5 = SYSFS # 137
int controlCSID5(int turnOn)
{
    int valuefd, res;

    //open
    valuefd = open("/sys/class/gpio/gpio137/value", O_WRONLY);
    if (valuefd < 0)
    {
        printf(KRED "Cannot open GPIO pin CSID5 (sysfs 137)\n");    
        printf(KRED "Error is: %s\n", strerror(errno));  
        printf(KNRM ""); 
        close(valuefd);              
        return 0;
    }

    //write
    if(turnOn){
        res = write(valuefd,"1", 1);
    }
    else{
        res = write(valuefd,"0", 1);
    }

    if(res <= 0)
    {
        printf(KRED "Could not write to sysfs pins for the LEDs %s\n",
            strerror(errno)
        );  
        printf(KRED "Error is: %s\n", strerror(errno));  
        printf(KNRM "");   
        close(valuefd);
        str_save_to_log("Could not write to sysfs pins for the LEDs", 5);
        return 0;
    }

    return 1;
}


//CSID6 = SYSFS # 138
int controlCSID6(int turnOn)
{
    int valuefd, res;

    //open
    valuefd = open("/sys/class/gpio/gpio138/value", O_WRONLY);
    if (valuefd < 0)
    {
        printf(KRED "Cannot open GPIO pin CSID6 (sysfs 138)\n");    
        printf(KRED "Error is: %s\n", strerror(errno));  
        printf(KNRM ""); 
        close(valuefd);
        str_save_to_log("Cannot open GPIO pin CSID6 (sysfs 138)", 5);              
        return 0;
    }

    //write
    if(turnOn){
        res = write(valuefd,"1", 1);
    }
    else{
        res = write(valuefd,"0", 1);
    }

    if(res <= 0)
    {
        printf(KRED "Could not write to sysfs pins for the LEDs %s\n",
            strerror(errno)
        );  
        printf(KRED "Error is: %s\n", strerror(errno));  
        printf(KNRM "");   
        close(valuefd);
        str_save_to_log("Could not write to sysfs pins for the LEDs", 5); 
        return 0;
    }

    return 1;
}

//CSID7 = SYSFS # 139
int controlCSID7(int turnOn)
{
    int valuefd, res;

    //open
    valuefd = open("/sys/class/gpio/gpio139/value", O_WRONLY);
    if (valuefd < 0)
    {
        printf(KRED "Cannot open GPIO pin CSID7 (sysfs 139)\n");    
        printf(KRED "Error is: %s\n", strerror(errno));  
        printf(KNRM ""); 
        close(valuefd);       
        str_save_to_log("Cannot open GPIO pin CSID6 (sysfs 139)", 5);        
        return 0;
    }

    //write
    if(turnOn){
        res = write(valuefd,"1", 1);
    }
    else{
        res = write(valuefd,"0", 1);
    }

    if(res <= 0)
    {
        printf(KRED "Could not write to sysfs pins for the LEDs %s\n",
            strerror(errno)
        );  
        printf(KRED "Error is: %s\n", strerror(errno));  
        printf(KNRM "");   
        close(valuefd);
        str_save_to_log("Could not write to sysfs pins for the LEDs", 5);
        return 0;
    }

    return 1;
}

int toggleAll(void)
{
    //Set pins up for LED test
    if(!setPinsForLEDs()){
        return 0;
    }

    struct timespec ts1;
    ts1.tv_nsec = 100000000L;
    ts1.tv_sec = 0;
    //Turn all pin LEDs on.
    //If there is an error, fail the LED test.
    if(!controlCSID0(1)){
        return 0;
    }
    nanosleep(&ts1,NULL);
    if(!controlCSID1(1)){
        return 0;
    }
    nanosleep(&ts1,NULL);
    if(!controlCSID2(1)){
        return 0;
    }
    nanosleep(&ts1,NULL);
    if(!controlCSID3(1)){
        return 0;
    }
    nanosleep(&ts1,NULL);
    if(!controlCSID4(1)){
        return 0;
    }
    nanosleep(&ts1,NULL);
    if(!controlCSID5(1)){
        return 0;
    }
    nanosleep(&ts1,NULL);
    if(!controlCSID6(1)){
        return 0;
    }
    nanosleep(&ts1,NULL);
    if(!controlCSID7(1)){
        return 0;
    }
    nanosleep(&ts1,NULL);

    //Turn all pin LEDs off
    //If there is an error, fail LED test
    if(!controlCSID0(0)){
        return 0;
    }
    nanosleep(&ts1,NULL);
    if(!controlCSID1(0)){
        return 0;
    }
    nanosleep(&ts1,NULL);
    if(!controlCSID2(0)){
        return 0;
    }
    nanosleep(&ts1,NULL);
    if(!controlCSID3(0)){
        return 0;
    }
    nanosleep(&ts1,NULL);
    if(!controlCSID4(0)){
        return 0;
    }
    nanosleep(&ts1,NULL);
    if(!controlCSID5(0)){
        return 0;
    }
    nanosleep(&ts1,NULL);
    if(!controlCSID6(0)){
        return 0;
    }
    nanosleep(&ts1,NULL);
    if(!controlCSID7(0)){
        return 0;
    }
    nanosleep(&ts1,NULL);

    if(!setPinsForHx711()){
        return 0;
    }
    nanosleep(&ts1,NULL);

    return 1;
}