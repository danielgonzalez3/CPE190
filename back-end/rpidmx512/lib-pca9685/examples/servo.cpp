/**
 * @file servo.cpp
 *
 */
/* Copyright (C) 2017-2018 by Arjan van Vught mailto:info@orangepi-dmx.nl
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#include "bcm2835.h"
#include "pca9685servo.h"

// Motor 1
#define M1P1 RPI_BPLUS_GPIO_J8_36
#define M1P2 RPI_BPLUS_GPIO_J8_32

// Motor 2
#define M2P1 RPI_BPLUS_GPIO_J8_37
#define M2P2 RPI_BPLUS_GPIO_J8_33

// Motor 3
#define M3P1 RPI_BPLUS_GPIO_J8_13
#define M3P2 RPI_BPLUS_GPIO_J8_15

// Motor 4
#define M4P1 RPI_BPLUS_GPIO_J8_11
#define M4P2 RPI_BPLUS_GPIO_J8_07

int main(int argc, char **argv) {	
	if (getuid() != 0) {
		fprintf(stderr, "Program is not started as \'root\' (sudo)\n");
		return -1;
	}

	if (bcm2835_init() != 1) {
		fprintf(stderr, "bcm2835_init() failed\n");
		return -2;
	}
	PCA9685 pca9685;
	pca9685.Dump();
	PCA9685Servo servo;
	// MG90S Micro Servo
	servo.SetLeftUs(700);
	servo.SetRightUs(2400);
	
	//Motor Controls
	bcm2835_gpio_fsel(M1P1, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(M1P2, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(M2P1, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(M2P2, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(M3P1, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(M3P2, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(M4P1, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(M4P2, BCM2835_GPIO_FSEL_OUTP);
	
	servo.Dump();
	
	puts("testing...");
	
        servo.SetAngle(CHANNEL(4), ANGLE(90));
	servo.SetAngle(CHANNEL(5), ANGLE(90));
	servo.SetAngle(CHANNEL(6), ANGLE(90));
	servo.SetAngle(CHANNEL(7), ANGLE(90));

	sleep(1);
	
	puts("Turning on motor");
	bcm2835_gpio_write(M2P1, 1);
        bcm2835_gpio_write(M2P2, 0);
	pca9685.Write(CHANNEL(0), VALUE(1819));
	sleep(5); 	
	pca9685.Write(CHANNEL(0), VALUE(3638));
	sleep(5);
	bcm2835_gpio_write(M2P1, 0);
	
	puts("Testing servos");
	for (;;) {
		puts("set to zero");
	        servo.SetAngle(CHANNEL(4), ANGLE(0));
		servo.SetAngle(CHANNEL(5), ANGLE(0));
		servo.SetAngle(CHANNEL(6), ANGLE(0));
		servo.SetAngle(CHANNEL(7), ANGLE(0));
		sleep(2);
		puts("set to 90");
		servo.SetAngle(CHANNEL(4), ANGLE(90));
		servo.SetAngle(CHANNEL(5), ANGLE(90));
		servo.SetAngle(CHANNEL(6), ANGLE(90));
		servo.SetAngle(CHANNEL(7), ANGLE(90));
		sleep(2);
	}

	return 0;
}
