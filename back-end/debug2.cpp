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
// #define TCA_CHANNEL_0 0x1
// #define TCA_CHANNEL_1 0x2
// #define TCA_CHANNEL_2 0x4
int main() 
{
    TCA9548A *tca9548A = new TCA9548A();
    int err = tca9548A->openTCA9548A();
    //int test_addr = 0x4;
    if (err < 0)
    {
	    printf("Error: %d", tca9548A->error);
	    printf("/n");			
    }else{
	    std::cout << "CHANNEL 0: " << (tca9548A->readByte(0x1)) << std::endl;
	    std::cout << "CHANNEL 1: " << (tca9548A->readByte(0x2)) << std::endl;
            std::cout << "CHANNEL 2: " << (tca9548A->readByte(0x4)) << std::endl;
	    std::cout << "CHANNEL 3: " << (tca9548A->readByte(0x2D)) << std::endl;
	    
	    std::cout << "writing" << std::endl;
            //std::cout << "CHANNEL 1: " << (tca9548A->writeByte(0x2, )) << std::endl;
    }
    tca9548A->closeTCA9548A();
}
