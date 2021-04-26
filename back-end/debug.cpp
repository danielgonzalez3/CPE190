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

extern const jetsonGPIONumber M0_F = gpio33;
extern const jetsonGPIONumber M0_B = gpio32;

extern const jetsonGPIONumber M1_F = gpio29;
extern const jetsonGPIONumber M1_B = gpio31;

extern const jetsonGPIONumber M2_F = gpio7;
extern const jetsonGPIONumber M2_B = gpio15;

extern const jetsonGPIONumber M3_F = gpio21;
extern const jetsonGPIONumber M3_B = gpio19;

int servoMin_20 = 1564;
int servoMax_20 = 6756;

int servoMin_60 = 1780;
int servoMax_60 = 7250;

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
    int err = pca9685->openPCA9685();
    if (err == 0 || getkey() == 27)
    {
        printf("Error: %d", pca9685->error);
	printf("\n");
	
    }else{
	printf("\nPCA9685 Device Address: 0x%02X\n",pca9685->kI2CAddress);
	std::cout << "\nRESET BOTH PCA\n" << std::endl;
        
        pca9685->setAllPWM(0,0);
	pca9685->reset();
	
	sleep(2);

	//Full Test Here
	
	// BACK LEFT
        pca9685->setPWM_ANGLE(9, 135, servoMin_20, servoMax_20);
	pca9685->setPWM_ANGLE(10, 110, servoMin_60, servoMax_60);
        pca9685->setPWM_ANGLE(11, 135, servoMin_20, servoMax_20);	
	
	// BACK RIGHT
	pca9685->setPWM_ANGLE(2, 135, servoMin_20, servoMax_20);
	pca9685->setPWM_ANGLE(1, 110, servoMin_60, servoMax_60);       
	pca9685->setPWM_ANGLE(0, 135, servoMin_20, servoMax_20);
	
	// FRONT LEFT
	pca9685->setPWM_ANGLE(8, 135, servoMin_20, servoMax_20);       
	pca9685->setPWM_ANGLE(7, 140, servoMin_60, servoMax_60);	
	pca9685->setPWM_ANGLE(6, 135, servoMin_20, servoMax_20);
        
	// FRONT RIGHT
	pca9685->setPWM_ANGLE(3, 135, servoMin_20, servoMax_20);
	pca9685->setPWM_ANGLE(4, 115, servoMin_60, servoMax_60);
	pca9685->setPWM_ANGLE(5, 135, servoMin_20, servoMax_20);
		
	/* Individual Test
	pca9685->setPWM_ANGLE(9, 135, servoMin_20, servoMax_20);	
        pca9685->setPWM_ANGLE(10, 135, servoMin_60, servoMax_60);
        pca9685->setPWM_ANGLE(11, 135, servoMin_20, servoMax_20);	
	sleep(2);
        pca9685->setPWM_ANGLE(9, 110, servoMin_20, servoMax_20);        
        pca9685->setPWM_ANGLE(10, 110, servoMin_60, servoMax_60);
        pca9685->setPWM_ANGLE(11, 110, servoMin_20, servoMax_20);
	sleep(2);
	pca9685->setPWM_ANGLE(9, 135, servoMin_20, servoMax_20);
        pca9685->setPWM_ANGLE(10, 135, servoMin_60, servoMax_60);
        pca9685->setPWM_ANGLE(11, 135, servoMin_20, servoMax_20);
 	sleep(3);		
	*/

	/*
        std::cout<<"135 DEGREES"<<std::endl;	
        pca9685->setPWM_ANGLE(7, 135, servoMin_60, servoMax_60);	
	sleep(4);

        std::cout<<"265 DEGREES"<<std::endl;	
	pca9685->setPWM_ANGLE(7, 265, servoMin_60, servoMax_60);
	sleep(3);

        std::cout<<"0 DEGREES"<<std::endl;
        pca9685->setPWM_ANGLE(7, 0, servoMin_60, servoMax_60);	
	sleep(2);
        
	std::cout<<"135 DEGREES"<<std::endl;
        pca9685->setPWM_ANGLE(7, 135, servoMin_60, servoMax_60);        
	sleep(2);

        std::cout<<"265 DEGREES"<<std::endl;
        pca9685->setPWM_ANGLE(7, 265, servoMin_60, servoMax_60);	
	sleep(3);

        std::cout<<"0 DEGREES"<<std::endl;	        
        pca9685->setPWM_ANGLE(7, 0, servoMin_60, servoMax_60);	
	sleep(3);

        std::cout<<"135 DEGREES"<<std::endl;	
        pca9685->setPWM_ANGLE(7, 135, servoMin_60, servoMax_60);	
	sleep(2);
	*/

	/* MOTOR STUFF 
	
	gpioExport(M3_F);
	gpioExport(M3_B);
	gpioSetDirection(M3_F, outputPin);
	gpioSetDirection(M3_B, outputPin);
        gpioSetValue(M3_F, 0);
        gpioSetValue(M3_B, 0);        
   	sleep(2);	
	pca9685->setPWM(12, 0, 1520);
        sleep(2);
	gpioSetValue(M3_F, 1);

	sleep(12);
	gpioSetValue(M3_F, 0);
	sleep(3);
	gpioUnexport(M3_F);
	gpioUnexport(M3_B);	
	
	sleep(2);
 	std::cout << "turning All  PWM to Zero\n" << std::endl;
	pca9685->setAllPWM(0,0);
        sleep(2);
    	std::cout << "closing..." << std::endl;
    	pca9685->closePCA9685();
        sleep(2);*/       
    }

}
