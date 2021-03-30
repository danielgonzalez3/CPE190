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
	    //std::cout << "writing" << (tca9548A->writeByte(0x70, TCA_CHANNEL_7)) << std::endl;
	    std::cout << "Select Channel 2" << (tca9548A->writeValue(TCA_CHANNEL_2)) << std::endl;
	    sleep(3);
	    std::cout << "Set Sensor: " << (tca9548A->writeByte(0x2D, 8)) << std::endl;
	    sleep(2);
	    std::cout << "Sanity Check, should be 8 " << (tca9548A->readByte(0x2D)) << std::endl;
	    
	    std::cout << "Read X-Axis: " << (tca9548A->readWord(0x32)) << std::endl;

            sleep(2);
	    std::cout << "Close" << (tca9548A->writeValue(0x1)) << std::endl;

	    /*std::cout << "PRIOR:     " << (tca9548A->readByte(0x32)) << std::endl; 
            std::cout << "PRIOR:     " << (tca9548A->readByte(0x33)) << std::endl;
            std::cout << "PRIOR:     " << (tca9548A->readByte(0x34)) << std::endl;
            std::cout << "PRIOR:     " << (tca9548A->readByte(0x35)) << std::endl;
            std::cout << "PRIOR:     " << (tca9548A->readByte(0x36)) << std::endl;

	    std::cout << "writing" << (tca9548A->writeByte(0x70, 0x4)) << std::endl;
	    std::cout << "writing" << (tca9548A->writeByte(0x2D, 8)) << std::endl;
	    
	    std::cout << "AFTER:     " << (tca9548A->readByte(0x32)) << std::endl;
	    std::cout << "AFTER:     " << (tca9548A->readByte(0x33)) << std::endl;
	    std::cout << "AFTER:     " << (tca9548A->readByte(0x34)) << std::endl;
	    std::cout << "AFTER:     " << (tca9548A->readByte(0x35)) << std::endl;
	    std::cout << "AFTER:     " << (tca9548A->readByte(0x36)) << std::endl;
	    std::cout << "AFTER:     " << (tca9548A->readByte(0x37)) << std::endl;
		
            //std::cout << "writing" << (tca9548A->writeByte(0x70, 0x0)) << std::endl;
	    //std::cout << "CHANNEL 1: " << (tca9548A->writeByte(0x2, )) << std::endl;*/
    }
    tca9548A->closeTCA9548A();
}
