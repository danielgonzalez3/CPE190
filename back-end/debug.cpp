//Driver
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <sys/time.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <athenaPCA9685.h>
#include "athenaGPIO.h"


int servoMin_20 = 1564;
int servoMax_20 = 6756;

int servoMin_60 = 1780;
int servoMax_60 = 7250;

// M0: 19 - 21
// M1: 32 - 33 
// M2: 7  - 15
// M3: 29 - 31
int getkey() {
    int character;
    struct termios orig_term_attr;
    struct termios new_term_attr;

    /* set the terminal to raw mode */
    tcgetattr(fileno(stdin), &orig_term_attr);
    memcpy(&new_term_attr, &orig_term_attr, sizeof(struct termios));
    new_term_attr.c_lflag &= ~(ECHO|ICANON);
    new_term_attr.c_cc[VTIME] = 0;
    new_term_attr.c_cc[VMIN] = 0;
    tcsetattr(fileno(stdin), TCSANOW, &new_term_attr);

    /* read a character from the stdin stream without blocking */
    /*   returns EOF (-1) if no character is available */
    character = fgetc(stdin);

    /* restore the original terminal attributes */
    tcsetattr(fileno(stdin), TCSANOW, &orig_term_attr);

    return character;
}
int main() 
{
    PCA9685 *pca9685  = new PCA9685((int)0x70);
//    PCA9685 *pca9685_2 = new PCA9685((int)0x41);

    int err = pca9685->openPCA9685();
//    int err2 = pca9685_2->openPCA9685();

    if (err == 0 || getkey() == 27)
    {
        printf("Error: %d", pca9685->error);
	//printf("Error: %d", pca9685_2->error);
	printf("\n");
	
    }else{
	printf("\nPCA9685 Device Address: 0x%02X\n",pca9685->kI2CAddress);
	//printf("\nPCA9685 Device Address: 0x%02X\n",pca9685_2->kI2CAddress);
	std::cout << "\nRESET BOTH PCA\n" << std::endl;
        
	pca9685->setAllPWM(0,0);
	sleep(2);
	pca9685->reset();
	
	//pca9685_2->setAllPWM(0,0);
	//sleep(2);
	//pca9685_2->reset();
	
	sleep(2);
	pca9685->setPWM_ANGLE(3, 135, servoMin_20, servoMax_20);
        sleep(3);
        pca9685->setPWM_ANGLE(3, 100, servoMin_20, servoMax_20);	
        sleep(3);
	pca9685->setPWM_ANGLE(3, 135, servoMin_20, servoMax_20);
        sleep(3);		

	/*
        pca9685->setPWM_ANGLE_60KG(135, servoMin_60, servoMax_60);
	sleep(3);
	pca9685->setPWM_ANGLE_60KG(70, servoMin_60, servoMax_60);
	sleep(3);
	pca9685->setPWM_ANGLE_60KG(135, servoMin_60, servoMax_60);
	sleep(3);
	*/

	/*
        std::cout<<"135 DEGREES"<<std::endl;	
        pca9685->setPWM_ANGLE(135, servoMin_20, servoMax_20);
        pca9685->setPWM_ANGLE_60KG(135, servoMin_60, servoMax_60);
	sleep(4);

        std::cout<<"265 DEGREES"<<std::endl;	
	pca9685->setPWM_ANGLE(265, servoMin_20, servoMax_20);
	pca9685->setPWM_ANGLE_60KG(265, servoMin_60, servoMax_60);	
	sleep(3);

        std::cout<<"0 DEGREES"<<std::endl;
        pca9685->setPWM_ANGLE(0, servoMin_20, servoMax_20);	
        pca9685->setPWM_ANGLE_60KG(0, servoMin_60, servoMax_60);
	//pca9685->setPWM(1, 0, 2044);
	sleep(2);
        
	std::cout<<"135 DEGREES"<<std::endl;
        pca9685->setPWM_ANGLE(135, servoMin_20, servoMax_20);
	pca9685->setPWM_ANGLE_60KG(135, servoMin_60, servoMax_60);
        sleep(2);

        std::cout<<"265 DEGREES"<<std::endl;
	pca9685->setPWM_ANGLE(265, servoMin_20, servoMax_20);
	pca9685->setPWM_ANGLE_60KG(265, servoMin_60, servoMax_60);
	sleep(3);

        std::cout<<"0 DEGREES"<<std::endl;	        
	pca9685->setPWM_ANGLE(0, servoMin_20, servoMax_20);	        
	pca9685->setPWM_ANGLE_60KG(0, servoMin_60, servoMax_60);
	sleep(3);

        std::cout<<"135 DEGREES"<<std::endl;	
        pca9685->setPWM_ANGLE(135, servoMin_20, servoMax_20);	
	pca9685->setPWM_ANGLE_60KG(135, servoMin_60, servoMax_60);
	sleep(2);
	*/

	/* MOTOR STUFF
	
	jetsonGPIONumber M1_0 = gpio29;
	jetsonGPIONumber M1_1 = gpio31;
	
	gpioExport(M1_0);
	gpioExport(M1_1);
	gpioSetDirection(M1_0, outputPin);
	gpioSetDirection(M1_1, outputPin);
        gpioSetValue(M1_0, 0);
        gpioSetValue(M1_1, 0);        
   	sleep(2);	
	pca9685->setPWM(14, 0, 1520);
        sleep(2);
	gpioSetValue(M1_0, 1);

	sleep(12);
	gpioSetValue(M1_0, 0);
	gpioUnexport(M1_0);
	gpioUnexport(M1_1);
	*/
	
	
 	std::cout << "turning All PCA 1 PWM to Zero\n" << std::endl;
	pca9685->setAllPWM(0,0);
        sleep(2);
        std::cout << "turning All PCA 2 PWM to Zero\n" << std::endl;	
	//pca9685_2->setAllPWM(0,0);
    	sleep(2);
    	std::cout << "closing..." << std::endl;
    	pca9685->closePCA9685();
        sleep(2);        
	//pca9685_2->closePCA9685();
//	sleep(2);
    }

}
