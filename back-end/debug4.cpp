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
	    test->activate(true);
	    std::cout << "test" << " " << std::endl;
	    for (int i = 0; i < 5; i++) 
	    {
	    	sleep(1);
	    	u_int8_t status = test->getStatus();
	    	std::cout << unsigned(status)<< " test status" << std::endl;

            
	    	u_int16_t x = test->getX();
	    	std::cout << unsigned(x)<< " reg value" << std::endl;
            
	   	u_int16_t y = test->getY();
		std::cout << unsigned(y)<< " reg value" << std::endl;
        	    
		u_int16_t z = test->getZ();
		std::cout << unsigned(z)<< " reg value" << std::endl;

		status = test->getStatus();
		std::cout << unsigned(status)<< " reg value" << std::endl;
	    	sleep(1);
	    }
    }
    
    test->closeFXAS21002C();
}
