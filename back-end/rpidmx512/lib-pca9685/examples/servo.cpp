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

int M1_PIN1 = 12;
int M1_PIN2 = 16;

#define M1P1 RPI_GPIO_P1_32
#define M1P2 RPI_GPIO_P1_36

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
	
	sleep(2);
	PCA9685Servo servo;

	// MG90S Micro Servo
	servo.SetLeftUs(700);
	servo.SetRightUs(2400);
	
	//Motor Controls
	bcm2835_gpio_fsel(M1P1, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(M1P2, BCM2835_GPIO_FSEL_OUTP);
	
	//pca9685 set freq
	pca9685.SetFrequency(100);
	
	servo.Dump();

	puts("testing...");
	
        servo.SetAngle(CHANNEL(4), ANGLE(90));
	servo.SetAngle(CHANNEL(5), ANGLE(90));
	servo.SetAngle(CHANNEL(6), ANGLE(90));
	servo.SetAngle(CHANNEL(7), ANGLE(90));

	sleep(1);
	
	puts("Turning on motor");
	bcm2835_gpio_write(M1P1, HIGH);
        bcm2835_gpio_write(M1P2, LOW);
	pca9685.SetFrequency(100);
	pca9685.Write(CHANNEL(2), VALUE(819)); 	
	
	sleep(15);
	bcm2835_gpio_write(M1P1, LOW);
	pca9685.SetFrequency(0);
	
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
