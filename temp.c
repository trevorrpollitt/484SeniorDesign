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
#include "smartscale.h"

// Loop variable for signal handling in infinite loops
volatile int loop = 0;

void sig_handler(int signo)
{
	if(loop == 1){
		printf("\n");
		loop = 0;
	}
	else{
		printf("\n");
		exit(0);
	}
}

void sig_handler2(int signo)
{
	printf("\nTaring...\n");
	hx711_tare();
}

int main(int argc, char *argv[ ])
{
		
	int choice1, choice2, choice3, gain, loopAmount, weight, i, j;
	char pass = 0;
	char c = 0;
	long longValue2 = 0;
	

	signal(SIGINT, sig_handler);
	signal(SIGTSTP, sig_handler2);
    // set the struct ts value = 10us (in nanoseconds) for sleeping
    set_sleep_amount(10000L);
	
	setup_result = setupPins(); //if result < 0, error has occured
	if(setup_result < 0) {
		printf("Error occured setting up the GPIO pins 132 and 133\n");
		return -1;
	}
	
	while(1)
    {

        printf("+---------//---------+\n");
        printf("| 1. Calibration     |\n");
        printf("| 2. Raw Loop        |\n");
        printf("| 3. Real Loop       |\n");
        printf("| 4. Variable Loop   |\n");
        printf("| 5. Settings        |\n");
        printf("+---------//---------+\n");
        printf("Your choice: ");
        scanf("%d", &choice1);

        switch(choice1){

            case 1: //Code for Taring

				printf("Please take all weight off of scale.\n");
				printf("Press CTRL + C when ready.\n");
				loop = 1;
				while(loop == 1){
					sleep(1);
				}
				hx711_reset();
				hx711_tare();
				printf("Please set calibration weight on scale.\n");
				printf("Press CTRL + C when ready.\n");
				loop = 1;
				while(loop == 1){
					sleep(1);
				}
				printf("\n");
				hx711_Set_Calibration();
				printf("Calibrated!\n");
                break;

            case 2: //Code for Raw input data loop (infinite)
	
				
				printf("Initializing Raw Data Loop...\n");
				loop = 1;
				while(loop == 1){
					longValue2 = hx711_averageRead(tareValue, 1); //Read in value
					printf("%ld\n",longValue2);
				}
				printf("\n");
				break;

            case 3: //Code for real weight conversion loop (infinite)

                printf("Initializing Real Data Loop...\n");
				loop = 1;
				while(loop == 1){
					longValue2 = hx711_averageRead(tareValue,calibrationValue);
					printf("%ld\n",longValue2);
				}
				printf("\n");
                break;

            case 4: //Variable Loop (New menu)

                printf("+---------//---------+\n");
                printf("| 1. One Loop        |\n");
                printf("| 2. Five Loops      |\n");
                printf("| 3. Ten Loops       |\n");
                printf("| 4. n Loops         |\n");
                printf("| 5. Go Back         |\n");
                printf("+---------//---------+\n");
                printf("Your choice: ");
                scanf("%d", &choice2);

                    switch(choice2){

                        case 1: //Run one Real Weight Loop
                            
                            printf("Running One Data Loop...\n");
							longValue2 = hx711_averageRead(tareValue,calibrationValue);
							printf("%ld",longValue2);
                            break;
                        case 2: //Run five Real Weight Loops   

                            printf("Running Five Data Loops...\n");
							for(i=0;i<5;i++){
								longValue2 = hx711_averageRead(tareValue,calibrationValue);
								printf("%ld",longValue2);
							}
                            break;
                        case 3: //Run ten Real Weight Loops
                        
                            printf("Running Ten Data Loops...\n");
							for(i=0;i<10;i++){
								longValue2 = hx711_averageRead(tareValue,calibrationValue);
								printf("%ld",longValue2);
							}
                            break;
                        case 4: //Receive user input for number of loops to run

                            printf("Insert Desired Amount: \n");
                            scanf("%d",&loopAmount);
                            printf("Running %d Data Loops\n",loopAmount);
							for(i=0;i<loopAmount;i++){
								longValue2 = hx711_averageRead(tareValue,calibrationValue);
								printf("%ld",longValue2);
							}
                            break;                        
                        case 5: //Return to previous menu.
                        default: 
                            break;
                    }
                break;

            case 5: //Settings for gain, weight conversion, etc.

                printf("+---------//---------+\n");
                printf("| 1. Set Gain        |\n");
                printf("| 2.                 |\n");
                printf("| 3. Setup WiFi      |\n");
                printf("| 4.                 |\n");
                printf("| 5.                 |\n");
                printf("+---------//---------+\n");
                printf("Your choice: ");
                scanf("%d",&choice3);

                switch(choice3){

                    case 1: //Allow for user to set gain
                        
                        printf("Please Input Desired Gain(32, 64, 128): ");
                        scanf("%d",&gain);
                            switch(gain){
                                case 32:
                                    
                                    printf("Gain set to 32");
                                    break;
                                case 64:
                                
                                    printf("Gain set to 64");
                                    break;
                                case 128:
                                
                                    printf("Gain set to 128");
                                    break;
                                default:
                                
                                    printf("Invalid Input!");
                                    break;
                            }
                        break;
                    case 2: //Allow for user to change unit conversion (lbs, kg, g, ounces, etc)

                    {
                       
                        
                            printf("Invalid Input!");
                            
                        break;
                    }
                    case 3:
                    {
						j = 0;
						while(j == 0){	
							printf("Is there a wifi password (Y/N)?");
							scanf(" %c",&pass);
							if(pass == 'Y' || pass == 'y'){
								
								printf("SSID: ");
								scanf("%s", wifissid);
								printf("Wireless Password: ");
								scanf("%s", wifipass);
								
								//Print SSID
								printf("SSID: ");
								for(i=0;i<strlen(wifissid);i++){
									
									printf("%c",wifissid[i]);
									
								}
								printf("\nPassword: ");
								// Print password
								for(i=0;i<strlen(wifipass);i++){
									
								   printf("%c",wifipass[i]);
								}
								
								sprintf(command1,"sudo nmcli device wifi connect \"%s\" password \"%s\" ifname wlan0", wifissid,wifipass);
								
								//print system command
								for(i=0;i<strlen(command1);i++){
									
									printf("%c",command1[i]);
								}
								printf("\n");
								system(command1);
								while ((c = getchar()) != '\n' && c != EOF) { }
								j = 1;
							}
							else if(pass == 'N' || pass == 'n'){
								
								printf("SSID: ");
								scanf("%s", wifissid);
								
								//Print SSID
								printf("SSID: ");
								for(i=0;i<strlen(wifissid);i++){
									
									printf("%c",wifissid[i]);
									
								}
								
								sprintf(command1,"sudo nmcli device wifi connect \"%s\" ifname wlan0", wifissid);
								
								//print system command
								for(i=0;i<strlen(command1);i++){
									
									printf("%c",command1[i]);
								}
								printf("\n");
								system(command1);
								while((c = getchar()) != '\n' && c != EOF){}
								j = 1;
							}
							else{
								printf("Invalid input! Please try again.\n");
							}
						}
                        break;
                    }
					default: // If incorrect value.
					printf("Not a selection! Please try again:\n\n");
					break;
                }
                break;

            default: // If incorrect value.
                printf("Not a selection! Please try again:\n\n");
                break;
        }
    }
	return 0;
}