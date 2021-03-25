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
#include <athenaTCA9548A.h>
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
    TCA9548A *tca9548A = new TCA9548A();
    int err = tca9548A->openTCA9548A();
    if (err < 0)
    {
	    printf("Error: %d", tca9548A->error);
	    printf("/n");			
    }else{
	    
    }
    tca9548A->closeTCA9548A();
}
