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
#include <athenaFXAS21002C.h>
#include "athenaGPIO.h"

// Calibrated for a Robot Geek RGS-13 Servo
// Make sure these are appropriate for the servo being used!
// #define TCA_CHANNEL_0 0x1
// #define TCA_CHANNEL_1 0x2
// #define TCA_CHANNEL_2 0x4
int main() 
{
    FXAS21002C *test = new FXAS21002C();
    int err = test->openFXAS21002C();
    //int test_addr = 0x4;
    if (err < 0)
    {
	    printf("Error: %d", test->error);
	    printf("/n");			
    }else{
            std::cout << "test" << " " << std::endl;
    }
    test->closeFXAS21002C();
}
