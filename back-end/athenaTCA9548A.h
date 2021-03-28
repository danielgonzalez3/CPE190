//PWM ATHENA
extern "C" {
#include <i2c/smbus.h>
}

#ifndef _ATHENATCA9548A_H
#define _ATHENATCA9548A_H

#include <cstddef>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <cstdlib>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define TCA_CHANNEL_0 0x1
#define TCA_CHANNEL_1 0x2
#define TCA_CHANNEL_2 0x4
#define TCA_CHANNEL_3 0x8
#define TCA_CHANNEL_4 0x10
#define TCA_CHANNEL_5 0x20
#define TCA_CHANNEL_6 0x40
#define TCA_CHANNEL_7 0x80


class TCA9548A
{
public:
    unsigned char kI2CBus;         // I2C bus of the TCA9548A
    int kI2CFileDescriptor;        // File Descriptor to the TCA9548A
    int kI2CAddress;               // Address of TCA9548A; defaults to 0x70
    int error;

    TCA9548A(int address=0x70);
    ~TCA9548A();
    bool openTCA9548A();
    void closeTCA9548A();
    void reset();


    // Read the given register
    int readByte(int readRegister);

    // Write the the given value to the given register
    int writeByte(int writeRegister, int writeValue);

    int getError();

};


#endif
