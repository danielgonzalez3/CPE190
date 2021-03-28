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
    
    if (err < 0)
    {
        printf("Error: %d", pca9685->error);
	printf("/n");
	std::cout<<"wtf"<<std::endl;
	
    }else{
	//pca9685->setAllPWM(0,1600);
	pca9685->setAllPWM(0,0);
	sleep(3);
	/*
	for(int i = 0; i < 10; i++) {
		pca9685->setPWM_20KG(9, 0, 2048-(i*64));
                pca9685->setPWM_20KG(8, 0, 2048-(i*64));
		pca9685->setPWM_20KG(7, 0, 2048-(i*64));
		pca9685->setPWM_20KG(6, 0, 2048-(i*64));
		sleep(3);
	}*/
	pca9685->setPWM_20KG(9, 0, 2048-(0*64));
	//pca9685->setPWM_20KG(9, 1024, 3072);

	sleep(10);
	/*
	for (int i=0; i<1000; i+=10) {
		pca9685->setPWM(12,0, i);
		std::cout << i << std::endl;
		sleep(2);
	}*/
       
       	//_20KG	
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
