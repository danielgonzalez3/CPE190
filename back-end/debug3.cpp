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
#include <athenaMCP9808.h>
#include "athenaGPIO.h"

// Calibrated for a Robot Geek RGS-13 Servo
// Make sure these are appropriate for the servo being used!
// #define TCA_CHANNEL_0 0x1
// #define TCA_CHANNEL_1 0x2
// #define TCA_CHANNEL_2 0x4
int main() 
{
    MCP9808 *mcp9808 = new MCP9808();
    int err = mcp9808->openMCP9808();
    //int test_addr = 0x4;
    if (err < 0)
    {
	    printf("Error: %d", mcp9808->error);
	    printf("/n");			
    }else{
            //std::cout << "CHANNEL 1: " << (tca9548A->writeByte(0x2, )) << std::endl;
    }
    mcp9808->closeMCP9808();
}
