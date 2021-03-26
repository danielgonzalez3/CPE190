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

int servoMin = 2000;
int servoMax = 2600;

// M1: 7 - 15
// M2: 29 - 31
// M3: 32 - 33
// m4: 19 - 21
   
int main() 
{
    PCA9685 *pca9685 = new PCA9685();

    int err = pca9685->openPCA9685();
    //pca9685->setAllPWM(0,0);
    if (err < 0)
    {
        printf("Error: %d", pca9685->error);
	printf("/n");
	
    }else{
	pca9685->setAllPWM(0,0);
	pca9685->setPWMFrequency(60);
	pca9685->setPWM(11,0, 2000);
	pca9685->setPWM(12,0, 2000);
	sleep(2) 
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
    pca9685->setAllPWM(0,0);
    pca9685->closePCA9685();
}
