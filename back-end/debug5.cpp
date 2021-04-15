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
#include <athenaFXOS8700.h>
#include "athenaGPIO.h"

int main() 
{
    FXOS8700 *test = new FXOS8700();
    int err = test->openFXOS8700();
    if (err <= 0)
    {
        printf("\nError: %d", test->error);
    }else{
	unsigned char temp[7]; 
    }
    
    test->closeFXOS8700();
}
