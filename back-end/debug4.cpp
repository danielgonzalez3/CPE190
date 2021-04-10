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
	    printf("Error: %d", test->error);
	    printf("/n");			
    }else{
            test->writeByte(GYRO_REGISTER_CTRL_REG1, 0x00);
            test->writeByte(GYRO_REGISTER_CTRL_REG1, (1 << 6));
	    test->writeByte(GYRO_REGISTER_CTRL_REG0, 0x03); // GYRO_RANGE_250DPS
	    test->writeByte(GYRO_REGISTER_CTRL_REG0, 0x0E);
	    test->activate(true);
	    u_int8_t whoami;
            whoami = test->readByte(GYRO_REGISTER_WHO_AM_I);
	    std::cout << "whoami: " << unsigned(whoami) << std::endl;

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
		//test->writeByte(GYRO_REGISTER_CTRL_REG1, 0x00);
		//test->writeByte(GYRO_REGISTER_CTRL_REG1, (1 << 6));
		//test->writeByte(GYRO_REGISTER_CTRL_REG0, 0x01); // GYRO_RANGE_250DPS 
		//test->writeByte(GYRO_REGISTER_CTRL_REG0, 0x0E);
	    }
    }
    
    test->closeFXAS21002C();
}
