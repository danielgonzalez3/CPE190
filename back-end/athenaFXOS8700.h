//PWM ATHENA
extern "C" {
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>	
}

#ifndef __ATHENAFXOS8700_H__
#define __ATHENAFXOS8700_H__

#include <cstddef>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <cstdlib>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>



#define FXOS8700_ADDRESS                  0x1F  // 0011111
#define FXOS8700_ID                       0xC7  // 1100 0111

#define FXOS8700_REGISTER_STATUS           0x00
#define FXOS8700_REGISTER_OUT_X_MSB        0x01
#define FXOS8700_REGISTER_OUT_X_LSB        0x02
#define FXOS8700_REGISTER_OUT_Y_MSB        0x03
#define FXOS8700_REGISTER_OUT_Y_LSB        0x04
#define FXOS8700_REGISTER_OUT_Z_MSB        0x05
#define FXOS8700_REGISTER_OUT_Z_LSB        0x06
#define FXOS8700_REGISTER_WHO_AM_I         0x0D  // 11000111   r
#define FXOS8700_REGISTER_XYZ_DATA_CFG     0x0E
#define FXOS8700_REGISTER_CTRL_REG1        0x2A  // 00000000   r/w
#define FXOS8700_REGISTER_CTRL_REG2        0x2B  // 00000000   r/w
#define FXOS8700_REGISTER_CTRL_REG3        0x2C  // 00000000   r/w
#define FXOS8700_REGISTER_CTRL_REG4        0x2D  // 00000000   r/w
#define FXOS8700_REGISTER_CTRL_REG5        0x2E  // 00000000   r/w
#define FXOS8700_REGISTER_MSTATUS          0x32
#define FXOS8700_REGISTER_MOUT_X_MSB       0x33
#define FXOS8700_REGISTER_MOUT_X_LSB       0x34
#define FXOS8700_REGISTER_MOUT_Y_MSB       0x35
#define FXOS8700_REGISTER_MOUT_Y_LSB       0x36
#define FXOS8700_REGISTER_MOUT_Z_MSB       0x37
#define FXOS8700_REGISTER_MOUT_Z_LSB       0x38
#define FXOS8700_REGISTER_TEMPERATURE      0x51
#define FXOS8700_REGISTER_MCTRL_REG1       0x5B   // 00000000   r/w
#define FXOS8700_REGISTER_MCTRL_REG2       0x5C   // 00000000   r/w
#define FXOS8700_REGISTER_MCTRL_REG3       0x5D   // 00000000   r/w

#define ACCEL_RANGE_2G                     0x00
#define ACCEL_RANGE_4G                     0x01
#define ACCEL_RANGE_8G                     0x02

#define ACCEL_MG_LSB_2G                    0.000244
#define ACCEL_MG_LSB_4G                    0.000488
#define ACCEL_MG_LSB_8G                    0.000976
#define MAG_UT_LSB                         0.1



class FXOS8700
{
public:
    unsigned char kI2CBus;         // I2C bus
    int kI2CFileDescriptor;        // File Descriptor
    int kI2CAddress;               // Address
    int error;
    int rng;

    FXOS8700(int address=FXOS8700_ADDRESS);
    ~FXOS8700();
    bool openFXOS8700();
    void closeFXOS8700();
    void reset();
    
    // Write the the given value to the given register
    int writeValue(int writeValue);
    int writeByte(u_int8_t writeRegister, u_int16_t writeValue);
    int readWord(u_int16_t readRegister);
    int readByte(u_int8_t readRegister);
    float FXOS8700::readX();
    int getError();
};


#endif
