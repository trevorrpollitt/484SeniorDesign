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

#include "chipui.h"
#include "hx711.h"
#include "lights.h"
#include "initialize.h"
#include "json_stuff.h"

// Loop variable for signal handling in infinite loops
volatile int loop = 0, loop2 = 0, techloop = 0, setup = 1;

void sig_handler(int signo)
{
	if(loop2 == 1){ //if you are in the automation loop
		loop2 = 0;
		setup = 1;
		printf("\n");
	}
	else if(techloop == 1 && loop != 1){ //If you are in the technician console
		printf("\n");
		techloop = 0;
		sleep(1);
	}
	else if(loop == 1){ //If you are in an infinite while loop
		printf("\n");
		loop = 0;
		sleep(1);
	}
	else{ //Else, exit the program
		printf(KNRM "");
		printf("\n");
		exit(0);
	}
}

void sig_handler2(int signo) //CTRL+Z, currently tares the scale
{
	printf("\nTaring...\n");
	hx711_tare();
}

void sig_handler3(int signo){ //watchdog

	if(techloop == 1){ //if you're in the tech console this way it doesn't restart it if you step away while you're in the console
		techloop = 0;
		printf("\n");
	}
	else{
		system("reboot");
	}
}

int main(int argc, char *argv[ ])
{	
	// Handles invalid text popup	
	int invalid1 = 0; int invalid2 = 0; int invalid3 = 0; int invalid4 = 0, invalid5 = 0;
	
	//Status handler
	int status = 0;

	//char pinValue = 0;
	char choice1, choice2, choice3, choice4, choice5;
	int loopAmount, i, j, reads = 0;
	char c = 0, cont = 0;
	long longReal = 0;
	long longRaw = 0;	
	int minutes = 1;

	set_sleep_amount(10000L);

	time_t t;
	struct tm c_time;
	
	signal(SIGALRM, sig_handler3); //watchdog initialization
	signal(SIGINT, sig_handler); //infinite loop handler
	signal(SIGTSTP, sig_handler2); //tare shortcut
	
    
	/* POWER ON TEST
	* This tests the Log file, GPIO
	* pins, LEDs, AWS connectivity
	* and Wifi Connectivity on startup
	**********************/
	status = initialize(minutes);
	if(!status)
	{
		while(cont != 'Y' && cont != 'y'){
			printf(KRED "Would you like to continue? (Y/N): ");
			printf(KNRM "");
			cont = getchar();
			while((c = getchar()) != '\n' && c != EOF){}
			if(cont == 'N' || cont == 'n'){
				return -1;
			}
		}
	}
	//If test is passed, but values are not initialized (system restarted)
		

	printf("Moving on in ");
	printf("3.. ");
	fflush(stdout);
	sleep(1);
	printf("2.. ");
	fflush(stdout);
	sleep(1);
	printf("1.. ");
	fflush(stdout);
	sleep(1);

	/* AUTOMATION
	* This will run automatically when the program
	* begins, you can leave the automated process to 
	* get to the technician console by pressing
	* CTRL + C
	*************************/

	//check if first time setup...
	FILE *file = fopen("setup.txt","r");
	fscanf(file,"%d",&setup);
	fclose(file);

	if(status && setup == 0){
		chipui_loadData();
	}

	while(1){
		if(setup == 0){
			loop2 = 1;
			while(loop2 == 1){
				clear();
				alarm(30); //turn on watchdog so we don't freeze longer than 30 seconds through this process

				//Run system check before reading data
				printf("Running Periodic System Check...\n");
				if(!test_LED_pins()){ //if LED pins don't work correctly
					controlCSID5(1); //turn on 6th LED for LED issue
				}
				else{
					printf("LEDs are working correctly!\n...\n");
				}
				if(!test_hx711_pins()){ //If hx711 pins cannot be set.
					controlCSID4(1); //turn on 5th LED for GPIO issue
				}
				else{
					printf("Hx711 pins have been set successfully!\n...\n");
				}
				if(!test_aws_connection()){ //If aws connection cannot be made
					controlCSID3(1); //turn on 4th LED for aws connectivity issues
				}
				else{
					printf("You are connected to AWS!\n...\n");
				}
				if(!test_wifi()){ //If internet cannot be reached via wifi
					controlCSID2(1); //turn on 3rd LED for wifi issues
				}
				else{
					printf("Wifi Test is successful!\n...\n");
				}

				//Get values and send to eagle.io
				printf("Sending Values to Eagle.io...\n...\n");
				longReal = hx711_averageRead(tareValue,calibrationValue);
				printf("Real = %ld\n...\n",longReal);
				longRaw = hx711_averageRead(tareValue,1);
				printf("Raw = %ld\n",longRaw);
				aws_send_to_eagleio(longRaw,longReal);

				
				alarm(10); //set watchdog to make sure we don't sleep longer than 5 minutes.
				timeValue++; // increment timeValue for use in creep algorithm

				//save timeValue and current time

				//get current time
				t = time(NULL);
				c_time = *localtime(&t);

				if(!chipui_saveData(tareValue,calibrationValue,timeValue,c_time)){
					controlCSID6(1); //turn on 7th LED for file IO issues
				}
				//sleep for 5 minutes.
				for(i = 0; i < 5; i++){
					printf(".\n");
					sleep(1);
				}
				alarm(0); //set watchdog to sleep since we're done with this iteration...
			}
		}


			//ensure_connection_to_aws(1);

			/*int a = 0;
			while(1){
				printf("Count is: %d\n", a);
				a++;
				sleep(2);
			}*/



			// set the struct ts value = 10us (in nanoseconds) for sleeping
			

			/* TECHNICIAN CONSOLE *
			* This interface will allow a 
			* technician to use built-in commands
			* to help fix problems that might
			* occur with the hardware or program
			***********************/
		else{
			techloop = 1;
			while(techloop == 1)
			{
				if(setup == 0){
					alarm(300); //turn on watchdog for 5 minutes in case tech console was accidentally activated somehow
				}
				clear();
				choice1 = 0;
				printf("+---------//---------+\n");
				printf("|     FUNCTIONS      |\n");
				printf("+--------------------+\n");
				printf("| 1. Calibration     |\n");
				printf("| 2. Raw Loop        |\n");
				printf("| 3. Real Loop       |\n");
				printf("| 4. Upload Data     |\n");
				//printf("| 5. Toggle One LED  |\n");
				printf("| 5. LED Test        |\n");
				printf("|---------//---------|\n");
				printf("|      SETTINGS      |\n");
				printf("+--------------------+\n");
				printf("| 6. Setup Wifi      |\n");
				printf("| 7. Change Gain     |\n");
				printf("+---------//---------+\n");
				printf("| 8. Confirm Setup   |\n");
				printf("+---------//---------+\n");
				printf("Press CTRL + C at any time \nto exit Technician Console\n");
				if(invalid1 == 1) printf("\nInvalid Input! Please Try Again.\n");
				printf("\nYour choice: ");
				choice1 = getchar();
				while((c = getchar()) != '\n' && c != EOF){}

				switch(choice1){

					case '1': //Code for Taring

						invalid1 = 0;

						printf("Please take all weight off of scale.\n");
						printf("Press CTRL + C when ready.\n");
						loop = 1;
						while(loop == 1){
							sleep(1);
						}
						hx711_reset();
						hx711_tare();

						//reset timeValue and current time since we just tared it again
						timeValue = 0;
						t = time(NULL);
						c_time = *localtime(&t);

						printf("Press CTRL + C when ready.\n");
						loop = 1;
						while(loop == 1){
							sleep(1);
						}
						printf("\n");
						j = 0;
						invalid2 = 0;
						while(j == 0){
							invalid2 = 0;
							choice4 = 0;
							clear();
							alarm(0); //kick the dog...
							printf("+---------//---------+\n");
							printf("|   Unit Selection   |\n");
							printf("|--------------------|\n");
							printf("| 1. Pounds (lbs)    |\n");
							printf("| 2. Kilograms (kg)  |\n");
							printf("| 3. Ounces (oz)     |\n");
							printf("| 4. Grams (g)       |\n");
							printf("| 5. Custom          |\n");
							printf("| 6. Go Back         |\n");
							printf("+---------//---------+\n");
							if(invalid2 == 1) printf("\nInvalid Input! Please Try Again.\n\n");
							printf("Your choice: ");
							choice4 = getchar();
							while((c = getchar()) != '\n' && c != EOF){}
							if(choice4 > '0' && choice4 <= '4'){
								choice4 = choice4 - '0';
								chipui_unitSelect(choice4);
								j = 1;
								invalid2 = 0;
							}
							if(choice4 == '5'){
								printf("Please set calibration weight on scale\n");
								loop = 1;
								alarm(30); //kick the dog...
								printf("Press CTRL + C when ready.\n");
								loop = 1;
								while(loop == 1){
									sleep(1);
								}
								hx711_Set_Calibration();
								j = 1;
								invalid2 = 0;
							}
							if(choice4 == '6'){
								j = 1;
								invalid2 = 0;
							}
							else{
								invalid2 = 1;
							}
						}
						printf("Calibrated!\n");
						chipui_saveData(tareValue,calibrationValue,timeValue,c_time);
						invalid1 = 0;
						break;

					case '2': //Code for Raw input data loop (infinite)

						invalid1 = 0;
						printf("Initializing Raw Data Loop...\n");
						loop = 1;
						alarm(300); //kick the dog...
						while(loop == 1){
							longRaw = hx711_averageRead(tareValue, 1); //Read in value
							printf("%ld\n",longRaw);
						}
						printf("\n");
						invalid1 = 0;
						break;

					case '3': //Code for real weight conversion loop (infinite)

						invalid1 = 0;
						printf("Initializing Real Data Loop...\n");
						loop = 1;
						alarm(300); //kick the dog...
						while(loop == 1){
							longReal = hx711_averageRead(tareValue,calibrationValue);
							printf("%ld\n",longReal);
						}
						printf("\n");
						invalid1 = 0;
						break;

					case '4': //Variable Loop (New menu)

						invalid1 = 0;
						invalid3 = 0;
						j = 0;
						while(j == 0){
							alarm(0); //turn off watchdog
							clear();
							choice2 = 0;
							printf("+---------//---------+\n");
							printf("|   Variable Loop    |\n");
							printf("|--------------------|\n");
							printf("| 1. Fast Upload     |\n");
							printf("| 2. Five Uploads    |\n");
							printf("| 3. Ten Uploads     |\n");
							printf("| 4. Variable Upload |\n");
							printf("| 5. Go Back         |\n");
							printf("+---------//---------+\n");
							if(invalid3 == 1) printf("\nInvalid Input! Please Try Again.\n\n");
							printf("Your choice: ");
							choice2 = getchar();
							while((c = getchar()) != '\n' && c != EOF){}

								switch(choice2){

									case '1': //Run one Real Weight Loop
										
										printf("Running One Data Loop...\n");
										longReal = hx711_averageRead(tareValue,calibrationValue);
										printf("%ld",longReal);
										longRaw = hx711_averageRead(tareValue,1);
										printf("%ld",longRaw);
										aws_send_to_eagleio(longRaw,longReal);
										invalid3 = 0;
										sleep(3);
										break;

									case '2': //Run five Real Weight Loops   

										printf("Running Five Data Loops...\n");
										for(i=0;i<5;i++){
											longReal = hx711_averageRead(tareValue,calibrationValue);
											printf("%ld",longReal);
											longRaw = hx711_averageRead(tareValue,1);
											printf("%ld",longRaw);
											aws_send_to_eagleio(longRaw,longReal);
										}
										invalid3 = 0;
										sleep(3);
										break;

									case '3': //Run ten Real Weight Loops
									
										printf("Running Ten Data Loops...\n");
										for(i=0;i<10;i++){
											longReal = hx711_averageRead(tareValue,calibrationValue);
											printf("%ld",longReal);
											longRaw = hx711_averageRead(tareValue,1);
											printf("%ld",longRaw);
											aws_send_to_eagleio(longRaw,longReal);
										}
										invalid3 = 0;
										sleep(3);
										break;

									case '4': //Receive user input for number of loops to run

										printf("Insert Desired Amount: \n");
										reads = scanf("%d",&loopAmount);
										if(reads != 1){
											invalid3 = 1;
											break;
										}
										printf("Running %d Data Loops\n",loopAmount);
										for(i=0;i<loopAmount;i++){
											longReal = hx711_averageRead(tareValue,calibrationValue);
											printf("%ld",longReal);
											longRaw = hx711_averageRead(tareValue,1);
											printf("%ld",longRaw);
											aws_send_to_eagleio(longRaw,longReal);
										}
										invalid3 = 0;
										sleep(3);
										break;  

									case '5': //Return to previous menu.

										invalid3 = 0;
										invalid1 = 0;
										j = 1;
										break;

									default: 
										invalid3 = 1;
										break;
								}
						}
						invalid1 = 0;
						break;

					/*case '5':
						
						printf("\n");
						printf("Please Enter Pin Value (1 - 8) \n");
						pinValue = getchar();
						while((pinValue > '8') || (pinValue < '1')){
							
							printf("\nIncorrect Value! Please Try Again: ");

							pinValue = getchar();
						}
						toggle(pinValue);
						break;*/

					case '5':

						alarm(0); //turn off watchdog
						invalid1 = 0;
						clear();
						toggleAll();
						invalid1 = 0;
						break;
					
					case '6': // Setup Wifi

						invalid1 = 0;
						invalid5 = 0;
						j = 0;
						while(j==0){
							alarm(0); //turn off watchdog
							clear();
							printf("+---------//---------+\n");
							printf("|     Wifi Setup     |\n");
							printf("|--------------------|\n");
							printf("| 1. WiFi Status     |\n");
							printf("| 2. Add Network     |\n");
							printf("| 3. Remove Network  |\n");
							printf("| 4. WiFi Test       |\n");
							printf("| 5. Go Back         |\n");
							printf("+---------//---------+\n");
							if(invalid5 == 1) printf("\nInvalid Choice! Please Try Again.\n\n");
							printf("Your Choice: ");
							choice5 = getchar();
							while((c = getchar()) != '\n' && c != EOF){}

							switch(choice5){
								case '1':
									alarm(60);
									//run command, push to status variable
									status = chipui_wifiStatus();

									//check status variable, print relevant information
									invalid5 = 0;
									break;

								case '2':
									alarm(60);

									//run command, push to status variable
									status = chipui_wifiAdd();
									invalid5 = 0;
									break;

									//check status variable, print relevant information
								case '3':
									alarm(60);

									//run command, push to status variable
									status = chipui_wifiRemove();
									invalid5 = 0;
									break;

									//check status variable, print relevant information
								case '4':
									alarm(60);

									//run command, push to status variable
									status = chipui_wifiTest();
									
									invalid5 = 0;
									break;

									//check status variable, print relevant information
								case '5':
									j = 1;
									invalid5 = 0;				

								default:
									invalid5 = 1;
									break;
							}

						}
						invalid1 = 0;
						break;
					
					case '7': // Set Gain

						invalid1 = 0;
						j = 0;
						while(j == 0){
							alarm(0); //turn off watchdog
							clear();
							choice3 = 0;
							printf("+---------//---------+\n");
							printf("|   Gain Selection   |\n");
							printf("|--------------------|\n");
							printf("| 1. 128             |\n");
							printf("| 2. 64              |\n");
							printf("| 3. Go Back         |\n");
							printf("+---------//---------+\n");
							if(invalid4 == 1) printf("\nInvalid Choice! Please Try Again.\n\n");
							printf("Your Choice ");
							choice3 = getchar();
							while((c = getchar()) != '\n' && c != EOF){}
							switch(choice3){
								case '1':
									gainSelect = 1;
									j = 1;
									invalid4 = 0;
									break;
								
								case '2':
									gainSelect = 2;
									j = 1;
									invalid4 = 0;
									break;
								
								case '3':
									j = 1;
									invalid4 = 0;
									break;

								default:

									invalid4 = 1;
									break;
							}
						}
						break;

					case '8': //confirm setup

						file = fopen("setup.txt","w");
						fprintf(file,"%d\n",0); //turn off setup mode
						setup = 0;
						invalid1 = 0;
						techloop = 0;
						break;
					
					default:
						invalid1 = 1;
						break;
				}
			}
		}
	}
	return 0;
}