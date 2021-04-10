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

// Calibrated for a Robot Geek RGS-13 Servo
// Make sure these are appropriate for the servo being used!

int servoMin_20 = 1564;
int servoMax_20 = 6756;

int servoMin_60 = 1780;
int servoMax_60 = 7250;

// M1: 7 - 15
// M2: 29 - 31
// M3: 32 - 33
// m4: 19 - 21
   
int main() 
{
    PCA9685 *pca9685 = new PCA9685();

    int err = pca9685->openPCA9685();
    
    if (err == 0)
    {
        printf("Error: %d", pca9685->error);
	printf("/n");
	std::cout<<"wtf"<<std::endl;
	
    }else{
	//pca9685->reset();
        sleep(2);	
	pca9685->setAllPWM(0,0);

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



	//pca9685->setPWM(0, 2048, 0);
	/*
        pca9685->setAllPWM(0,0);
	jetsonGPIONumber M1_0 = gpio7;
	jetsonGPIONumber M1_1 = gpio15;
	gpioExport(M1_0);
	gpioExport(M1_1);
	gpioSetDirection(M1_0, outputPin);
	gpioSetDirection(M1_1, outputPin);
        pca9685->reset();	
	pca9685->setPWM(0,0,servoMin);
	gpioSetValue(M1_0, 1);
	sleep(7);
	gpioSetValue(M1_0, 0);
	gpioSetValue(M1_1, 0);
	pca9685->setPWM(0,0,0);
	gpioUnexport(M1_0);
	gpioUnexport(M1_1);*/

    }
    std::cout << "turning All PWM to Zero\n" << std::endl;
    pca9685->setAllPWM(0,0);    
    sleep(2);
    //pca9685->reset();
    std::cout << "closing..." << std::endl;
    pca9685->closePCA9685();
}
