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

int main() 
{
    FXAS21002C *test = new FXAS21002C();
    int err = test->openFXAS21002C();
    if (err <= 0)
    {
	    printf("\nError: %d", test->error);
    }else{
	    u_int8_t whoami;
            whoami = test->readByte(GYRO_REGISTER_WHO_AM_I);
	    std::cout << "whoami: " << unsigned(whoami) << std::endl;
	    test->activate(true);
	    for (int i = 0; i < 50; i++) 
	    {
                //test->activate(true);		    
	    	sleep(1);
	    	//u_int8_t status = test->getStatus();
	    	//std::cout << unsigned(status)<< "status" << std::endl;

            
	    	u_int16_t x = test->getX();
		float x_value = x * GYRO_SENSITIVITY_2000DPS * SENSORS_DPS_TO_RADS;
	    	std::cout << x_value << " ->x" << std::endl;
            
	   	u_int16_t y = test->getY();
		float y_value = y * GYRO_SENSITIVITY_2000DPS * SENSORS_DPS_TO_RADS;
		std::cout << y_value << " ->y" << std::endl;
        	    
		u_int16_t z = test->getZ();
	        float z_value = z * GYRO_SENSITIVITY_2000DPS * SENSORS_DPS_TO_RADS;	
		std::cout << z_value << " ->z" << std::endl;
		
		std::cout << "------------" << std::endl;
	    	
		//sleep(1);
                test->activate(false);		
	    }
    }
    
    test->closeFXAS21002C();
}
