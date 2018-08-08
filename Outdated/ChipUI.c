#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <time.h> //for timespec struct
#include <errno.h>
#include <hx711.h>


int choice1, choice2, choice3, gain, loopAmount, weight;

void main(void){


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

                printf("Calibrating...\n");
                break;

            case 2: //Code for Raw input data loop (infinite)

                printf("Initializing Raw Data Loop...\n");
                break;

            case 3: //Code for real weight conversion loop (infinite)

                printf("Initializing Real Data Loop...\n");
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
                            break;
                        case 2: //Run five Real Weight Loops   

                            printf("Running Five Data Loops...\n");
                            break;
                        case 3: //Run ten Real Weight Loops
                        
                            printf("Running Ten Data Loops...\n");
                            break;
                        case 4: //Receive user input for number of loops to run

                            printf("Insert Desired Amount: \n");
                            scanf("%d",&loopAmount);
                            printf("Running %d Data Loops\n",loopAmount);
                            break;                        
                        case 5: //Return to previous menu.
                        default: 
                            break;
                    }
                break;

            case 5: //Settings for gain, weight conversion, etc.

                printf("+---------//---------+\n");
                printf("| 1. Set Gain        |\n");
                printf("| 2. Choose Units    |\n");
                printf("| 3.                 |\n");
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

                        weight = 0;
                        printf("Please Input Desired Weight Conversion (lbs, ounces, kg, g): ");
                        scanf("%s",&weight[0]);
                            
                        if((strcmp(weight,"lbs")==0) 
                        || (strcmp(weight,"Lbs")==0)
                        || (strcmp(weight,"LBs")==0)
                        || (strcmp(weight,"LBS")==0) 
                        || (strcmp(weight,"pounds")==0)
                        || (strcmp(weight,"Pounds")==0)
                        || (strcmp(weight,"POUNDS")==0)){
                            
                            printf("Weight Conversion Set to Pounds.");
                            break;
                        }
                        
                        else if((strcmp(weight,"oz")==0) 
                        || (strcmp(weight,"Oz")==0)
                        || (strcmp(weight,"OZ")==0)
                        || (strcmp(weight,"ounces")==0) 
                        || (strcmp(weight,"Ounces")==0)
                        || (strcmp(weight,"OUNCES")==0)
                        || (strcmp(weight,"ounce")==0) 
                        || (strcmp(weight,"Ounce")==0)
                        || (strcmp(weight,"OUNCE")==0)){
                        
                            printf("Weight Conversion Set to Ounces.");
                            break;
                        }
                        
                        else if((strcmp(weight,"kg")==0) 
                        || (strcmp(weight,"Kg")==0)
                        || (strcmp(weight,"KG")==0)
                        || (strcmp(weight,"kilograms")==0) 
                        || (strcmp(weight,"Kilograms")==0)
                        || (strcmp(weight,"KILOGRAMS")==0)
                        || (strcmp(weight,"kilogram")==0) 
                        || (strcmp(weight,"Kilogram")==0)
                        || (strcmp(weight,"KILOGRAM")==0)){
                        
                            printf("Weight Conversion Set to Kilograms.");
                            break;
                        }
                            
                        else if((strcmp(weight,"g")==0)
                        || (strcmp(weight,"G")==0) 
                        || (strcmp(weight,"grams")==0)
                        || (strcmp(weight,"Grams")==0)
                        || (strcmp(weight,"GRAMS")==0) 
                        || (strcmp(weight,"gram")==0)
                        || (strcmp(weight,"Gram")==0)
                        || (strcmp(weight,"GRAM")==0)){
                    
                            printf("Weight Conversion Set to Grams.");
                            break;
                        }
                        
                        else{
                        
                            printf("Invalid Input!");
                            break;
                        }
                        break;
                }
                break;

            default: // If incorrect value.
                printf("Not a selection! Please try again:\n\n");
                break;
        }
    }
}