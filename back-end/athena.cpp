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
#include <JHPWMPCA9685.h>
#include "jetsonGPIO.h"

// Calibrated for a Robot Geek RGS-13 Servo
// Make sure these are appropriate for the servo being used!

int servoMin = 2000;
int servoMax = 2600;
jetsonGPIONumber M1_0 = gpio33;
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
        pca9685->reset();	
	//pca9685->setPWM(0,0,servoMin);
	gpioSetValue(M1_0, 1);
	sleep(5);
	gpioSetValue(M1_0, 0);
    }
    pca9685->closePCA9685();
}
