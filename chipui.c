#include "chipui.h"

int chipui_wifiAdd(void){

    int i, k;
	char pass = 0;
    char c = 0, try = 0;
    char* wifissid;
    char* wifipass; 
    char command1[64] = {};

    k = 0;
    while(k == 0){	
        clear();
        printf("Is there a wifi password? (Y/N): ");
        pass = getchar();
        while((c = getchar()) != '\n' && c != EOF){}
        if(pass == 'Y' || pass == 'y'){
            
            printf("Please Enter SSID: ");
            wifissid = getstring();

            printf("Wireless Password: ");
            wifipass = getstring();
            
            //Print SSID
            printf("Entered SSID: ");
            for(i=0;i<strlen(wifissid);i++){
                
                printf("%c",wifissid[i]);
                
            }

            printf("\nEntered Password: ");
            // Print password
            for(i=0;i<strlen(wifipass);i++){
                
                printf("%c",wifipass[i]);
            }

            printf("\n");

            if(!chipui_wifiCheck(wifissid,strlen(wifissid))){
                sprintf(command1,"sudo nmcli device wifi connect \"%s\" password \"%s\" ifname wlan0", wifissid,wifipass);
                //printf("%s",command1);
                //fflush(stdout);
                //sleep(5);
            
            
                //print system command
                /*for(i=0;i<strlen(command1);i++){
                    
                    printf("%c",command1[i]);
                } debugging*/
                printf("\n");
                system(command1);
                k = 1;
                sleep(1);

            }
            else{
                printf("Error! That SSID is already added!\n");
                printf("Would you like to try again? (Y/N): ");
                try = 0;
                while(!((try == 'Y') || (try == 'y'))){
                    try = getchar();
                    while((c = getchar()) != '\n' && c != EOF){}
                    if(try == 'Y' || try == 'y'){
                        break;
                    }
                    if((try == 'n') || (try == 'N')){
                        k = 1; 
                        return 0;
                    }
                }
            }
        }
        else if(pass == 'N' || pass == 'n'){
            
            printf("Please Enter SSID: ");
            wifissid = getstring();
            
            //Print SSID
            printf("Your Entry: ");
            for(i=0;i<strlen(wifissid);i++){
                
                printf("%c",wifissid[i]);
                
            }
            printf("\n");
            
            if(!chipui_wifiCheck(wifissid,strlen(wifissid))){
                sprintf(command1,"sudo nmcli device wifi connect \"%s\" ifname wlan0", wifissid);
                printf("%s",command1);
                fflush(stdout);
            
            
                //print system command
                /*for(i=0;i<strlen(command1);i++){
                    
                    printf("%c",command1[i]);
                } debugging */
                printf("\n");
                system(command1);
                k=1;
                sleep(1);
            }
            else{
                printf("Error! That SSID is already added!\n");
                printf("Would you like to try again? (Y/N): ");
                try = 0;
                while(!((try == 'Y') || (try == 'y'))){
                    try = getchar();
                    while((c = getchar()) != '\n' && c != EOF){}
                    if((try == 'n') || (try == 'N')){
                        k = 1;
                        return 0;
                    }
                }
            }
        }
        else{
            printf("Invalid input! Please try again.\n");
            sleep(1);
        }
    }
    return 1;
}

void chipui_unitSelect(int unitSelect){
    if(unitSelect == 1){
        printf("Changing unit to pounds (lbs)...");
        if(gainSelect == 2) calibrationValue = 5000;
        else{
            calibrationValue = 10000;
        }
        sleep(1);
    }
    if(unitSelect == 2){
        printf("Changing unit to Kilograms (kg)...");
        if(gainSelect == 2) calibrationValue = 5000 / 2.20462;
        else{
            calibrationValue = 10000 * 2.20462;
        }
        sleep(1);
    }
    if(unitSelect == 3){
        printf("Changing unit to Ounces (oz)...");
        if(gainSelect == 2) calibrationValue = 5000 / 16;
        else{
            calibrationValue = 10000 / 16;
        }
        sleep(1);
    }
    if(unitSelect == 4){
        printf("Changing unit to grams (g)...");
        if(gainSelect == 2) calibrationValue = 5000 * 2.20462 / 1000;
        else{
            calibrationValue = 10000 * 2.20462 / 1000;
        }
        sleep(1);
    }

}

int chipui_wifiStatus(void){
    char status_check_string[] = "nmcli device status";
    printf("\n");
    system(status_check_string);

    printf("\nMoving on in 5.. ");
    fflush(stdout);
    sleep(1);
    printf("4.. ");
    fflush(stdout);
    sleep(1);
    printf("3.. ");
    fflush(stdout);
    sleep(1);
    printf("2.. ");
    fflush(stdout);
    sleep(1);
    printf("1.. ");
    fflush(stdout);
    sleep(1);

    return 1;
}

int chipui_wifiRemove(void){
    
    char command[64] = {};
    char* wifissid;
    int k = 0;
    char c, try;
    while(k == 0){
		clear();
        char device_delete_string[] = "nmcli connection delete \"%s\"";
        printf("Here are your current connections:\n\n");
        system("nmcli c");
        printf("\nPlease enter the SSID of the connection to delete: ");
        wifissid = getstring();
        sprintf(command,device_delete_string,wifissid);
        

        if(chipui_wifiCheck(wifissid,strlen(wifissid))){
            system(command);
            printf("Connection successfully deleted!");
            fflush(stdout);
            sleep(1);
            k = 1;
        }
        else{
            printf("There are no SSIDs that match your entry\n");
            printf("Would you like to try again? (Y/N): ");
            try = 0;
            fflush(stdin);
            while(!((try == 'Y') || (try == 'y'))){
                try = getchar();
                while((c = getchar()) != '\n' && c != EOF){}
                if(try == 'Y' || try == 'y'){
                    break;
                }
                if(try == 'N' || try == 'n'){
                    k = 1;
                    return 0;
                }
            }
        }
    }
    return 1;
}

int chipui_wifiTest(void){

    if(parsePing()){
        printf("Wifi Test has ");
        printf(KGRN "succeeded!");
        printf(KNRM " You are connected to the internet.\n");

        printf("\nMoving on in 3.. ");
        fflush(stdout);
        sleep(1);
        printf("2.. ");
        fflush(stdout);
        sleep(1);
        printf("1.. ");
        fflush(stdout);
        sleep(1);

        return 1;
    }
    else{
        printf("Wifi Test has ");
        printf(KRED "failed!");
        printf(KNRM " You are NOT connected to the internet.\n");
        
        printf("\nMoving on in 5.. ");
        fflush(stdout);
        sleep(1);
        printf("4.. ");
        fflush(stdout);
        sleep(1);
        printf("3.. ");
        fflush(stdout);
        sleep(1);
        printf("2.. ");
        fflush(stdout);
        sleep(1);
        printf("1.. ");
        fflush(stdout);
        sleep(1);

        return 0;
    }
    return -1;
}

//Parses through current connections and checks for the SSID passed in parameter char[] wifissid, int index is number of values in wifissid char array
//Return 1 if there is a match, return 0 otherwise.
int chipui_wifiCheck(char* wifissid, int index){

    char connections_check_string[] = "nmcli c";
    FILE *fp;
    int i, j, count = 0;
    char path[1500];

    //build timespec for nanosleepeeeeeee
    struct timespec ts1;
    ts1.tv_nsec = 100000000L;
    ts1.tv_sec = 0; 

    //Open command for reading
    printf("Opening Command...\n");
    nanosleep(&ts1,NULL);
    fp = popen(connections_check_string,"r");//Open connections to parse
    if(fp == NULL){
        printf("Failed to run command\n" );

        // error code is 6 (system error)
        str_save_to_log("Failed to run command popen('nmcli c')", 6);
        
        return -1;
    }
    while (fgets(path, sizeof(path)-1, fp) != NULL) {//Get line of code, exit iff no more lines
        i = 0;
        j = 0;
        //printf("loop 1!\n"); //debugging
        while(path[i] != '\n'){ //parse loop; terminate if newline is found (end of current line from fgets)
            if(path[i] == wifissid[j]){//test path char array for first character of ssid
                //printf("First1 letter found!\n"); //debugging
                count++; //increment match counter by 1 for initial match
                i++; //increment i for next char in path array for next comparison
                for(j=1;j<index;j++){ //continue through path to check for entire ssid string
                    if(path[i] == wifissid[j]){
                        count++;
                        //printf("%d\n",count); //debugging
                        i++;
                    }
                    else{
                        count = 0; //set count to zero because match not found
                        //printf("Oops\n"); //debugging
                        break; //break out of for loop
                    }
                    if((count == index) && (path[i] < '0')){//if number of matched chars equals number of chars in ssid array && no more values in path entry
                        //printf("Match Found!\n"); 
                        //fflush(stdout);
                        //sleep(20); //debugging
                        return 1; //match found
                    }
                }
            }
            i++;
        }
    }
    nanosleep(&ts1,NULL);
    pclose(fp);
    // sleep(20); debugging
    return 0; //No match found
}

//Gets string input from user with buffer overflow management. Made to be the exact same implementation as gets, with less error.
char* getstring(void){
    
    char* input = malloc(64);
    if (input == NULL) {
        printf("No memory\n");
        return "no memory";
    }
    
    /* Get the input, with size limit. */

    fgets(input, 64, stdin);

    /* Remove trailing newline, if there. */

    if ((strlen(input) > 0) && (input[strlen (input) - 1] == '\n')){
        input[strlen (input) - 1] = '\0';
    }

    return input;
}

//This function will first do the ping command to www.google.com to test DNS and Wireless connectivity, if that fails, it will ping 8.8.8.8 to test connectivity without DNS.
int parsePing(void){

    char connections_check_string[] = "ping www.google.com -c 5";
    //char dns_check_string[] = "ping 8.8.8.8 -c 5";
    char* parseCheck1 = "64 bytes";
    //char* parseCheck2 = "unknown host";
    //char* parseCheck3 = "unreachable";
    FILE *fp;
    int i, j, count = 0, packets = 0;
    double packetRcvd = 0;
    char path[1500];

    //build timespec for nanosleep
    struct timespec ts1;
    ts1.tv_nsec = 100000000L;
    ts1.tv_sec = 0; 

    //Open normal ping command with DNS.
    printf("\nRunning WiFi Test... This will take about 5 seconds...\n");
    nanosleep(&ts1,NULL);
    fp = popen(connections_check_string,"r");//Open connections to parse
    if(fp == NULL){
        printf("Failed to run command\n" );

        // error code is 6 (system error)
        str_save_to_log("Failed to run command popen('ping www.google.com -c 5')", 6);

        return -1;
    }
    while (fgets(path, sizeof(path)-1, fp) != NULL) {//Get line of code, exit iff no more lines
        i = 0;
        j = 0;
        //printf("loop 1!\n"); //debugging
        //printf("%s",path);
        while(path[i] != '\n'){ //parse loop; terminate if newline is found (end of current line from fgets)
            if(path[i] == parseCheck1[j]){//test path char array for first character of ssid
                //printf("First letter found!\n"); //debugging
                count++; //increment match counter by 1 for initial match
                i++; //increment i for next char in path array for next comparison
                for(j=1;j<strlen(parseCheck1);j++){ //continue through path to check for entire ssid string
                    if(path[i] == parseCheck1[j]){
                        count++;
                        //printf("%d\n",count); //debugging
                        i++;
                    }
                    else{
                        count = 0; //set count to zero because match not found
                        //printf("Oops\n"); //debugging
                        break; //break out of for loop
                    }
                    if((count == 8) && (path[i] < '0')){//if number of matched chars equals number of chars in ssid array && no more values in path entry
                        printf("Packet Received!\n"); 
                        fflush(stdout);
                        //sleep(20); //debugging
                        i++;
						packets++;
						count = 0;
                        /*packets = path[i];
                        packets = packets - '0';
                        packetRcvd = packets/5.0 * 100;

                        printf("%lf%%\n",packetRcvd);
                        if(packetRcvd == 100.0){
                            
                            pclose(fp);
                            return 1;
                        }*/
                    }
                }
            }
            //Test if not able to resolve host (DNS issue)
            /*else if(path[i] == parseCheck2[k]){ //Check for DNS issue
                printf("First letter found!\n"); //debugging
                count++; //increment match counter by 1 for initial match
                i++; //increment i for next char in path array for next comparison
                for(j=1;j<strlen(parseCheck1);j++){ //continue through path to check for entire ssid string
                    if(path[i] == parseCheck1[j]){
                        count++;
                        printf("%d\n",count); //debugging
                        i++;
                    }
                    else{
                        count = 0; //set count to zero because match not found
                        printf("Oops\n"); //debugging
                        break; //break out of for loop
                    }
                    if((count == strlen(parseCheck1)) && (path[i] < '0')){//if number of matched chars equals number of chars in ssid array && no more values in path entry
                        printf("Match Found!\n"); 
                        fflush(stdout);
                        sleep(20); //debugging
                        return 1; //match found
                    }
                }
            }*/ //Additional functionality, not implemented
            i++;
        }
    }
    pclose(fp);
    sleep(1);

    //Open ping command without DNS to test for DNS issue (Most common issue with home networks).
    /*printf("Opening Command... This will take about 5 seconds...\n");
    nanosleep(&ts1,NULL);
    fp = popen(dns_check_string,"r");//Open connections to parse
    if(fp == NULL){
        printf("Failed to run command\n" );
        return -1;
    }
    while (fgets(path, sizeof(path)-1, fp) != NULL) {//Get line of code, exit iff no more lines
        i = 0;
        j = 0;
        k = 0;
        printf("loop 1!\n"); //debugging
        while(path[i] != '\n'){ //parse loop; terminate if newline is found (end of current line from fgets)
            if(path[i] == parseCheck1[j]){//test path char array for first character of ssid
                printf("First letter found!\n"); //debugging
                count++; //increment match counter by 1 for initial match
                i++; //increment i for next char in path array for next comparison
                for(j=1;j<strlen(parseCheck1);j++){ //continue through path to check for entire ssid string
                    if(path[i] == parseCheck1[j]){
                        count++;
                        printf("%d\n",count); //debugging
                        i++;
                    }
                    else{
                        count = 0; //set count to zero because match not found
                        printf("Oops\n"); //debugging
                        break; //break out of for loop
                    }
                    if((count == strlen(parseCheck1)) && (path[i] < '0')){ //if number of matched chars equals number of chars in ssid array && no more values in path entry
                        printf("Match Found!\n"); 
                        fflush(stdout);
                        sleep(20); //debugging
                        return 1; //match found
                    }
                }
            }
            else if(path[i] == parseCheck3[k])
            i++;
        }
    }*/ //Additional functionality, not implemented
    //nanosleep(&ts1,NULL);
    packetRcvd = packets/5.0 * 100;

    printf("Packets Received: %lf%%\n",packetRcvd);
    if(packetRcvd == 100.0){
        
        return 1;
    }
    //pclose(fp);
    // sleep(20); debugging
    return 0; //No match found
}

//This function will save the data that you give it depending on what values are populated when it is called

int chipui_saveData(long tarevalue, long calibrationvalue, int timevalue, struct tm c_time){
    FILE *file;

    file = fopen("ScaleData.txt","w");
    if(file == NULL){
        return 0;
    }
    //save tareValue
    fprintf(file,"%ld\n",tarevalue);

    //save calibrationValue
    fprintf(file,"%ld\n",calibrationvalue);

    //save time data
    fprintf(file,"%d\n",c_time.tm_year);
    fprintf(file,"%d\n",c_time.tm_mon);
    fprintf(file,"%d\n",c_time.tm_mday);
    fprintf(file,"%d\n",c_time.tm_hour);
    fprintf(file,"%d\n",c_time.tm_min);
    fprintf(file,"%d\n",c_time.tm_sec);
    fprintf(file,"%d\n",timevalue);

    fclose(file);
    return 1;
}

int chipui_loadData(void){
    FILE *file;

    file = fopen("ScaleData.txt","r");
    if(file == NULL){
        return 0;
    }

    fscanf(file,"%ld",&tareValue);

    fscanf(file,"%ld",&calibrationValue);

    //local variables
    struct tm l_time; //old time struct
    int l_timevalue; //old timeValue
    int d_time = 0; //difference of time in minutes

    //get current time
    time_t t = time(NULL);
    struct tm c_time = *localtime(&t);

    //get last saved time stamp (when last timevalue was saved)
    fscanf(file,"%d\n",&l_time.tm_year);
    fscanf(file,"%d\n",&l_time.tm_mon);
    fscanf(file,"%d\n",&l_time.tm_mday);
    fscanf(file,"%d\n",&l_time.tm_hour);
    fscanf(file,"%d\n",&l_time.tm_min);
    fscanf(file,"%d\n",&l_time.tm_sec);

    //get last timevalue
    fscanf(file,"%d\n",&l_timevalue);

    //get the difference of the two timestamps
    d_time = diff(l_time,c_time);

    timeValue = l_timevalue + (d_time / 5);

    fclose(file);
    return 1;
}

//This function will take two timespec structs and determine the difference between the two in minutes.
int diff(struct tm l_time, struct tm c_time)
{
    int d_year = 0, d_month = 0, d_day = 0, d_hour = 0, d_min = 0;
    int changetime = 0;

    d_year = c_time.tm_year - l_time.tm_year;
    d_month = c_time.tm_mon - l_time.tm_mon;
    d_day = c_time.tm_mday - l_time.tm_mday;
    d_hour = c_time.tm_hour - l_time.tm_hour;
    d_min = c_time.tm_min - l_time.tm_min;

    printf("Last: %d %d %d %d %d\n",l_time.tm_year,l_time.tm_mon,l_time.tm_mday,l_time.tm_hour,l_time.tm_min);
    printf("Current: %d %d %d %d %d\n",c_time.tm_year,c_time.tm_mon,c_time.tm_mday,c_time.tm_hour,c_time.tm_min);

    changetime = (d_year * 525949) + (d_month * 43800) + (d_day * 1440) + (d_hour * 60) + d_min;
    printf("changetime = %d",changetime);
    return changetime;
}