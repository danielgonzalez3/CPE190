extern "C" {
#include <i2c/smbus.h>
}

#ifndef _ATHENAMCP9808_H
#define _ATHENAMCP9808_H

#include <cstddef>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <cstdlib>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define MCP9808_I2CADDR_DEFAULT 0x18
#define MCP9808_REG_CONFIG 0x01

// shutdown config
#define MCP9808_REG_CONFIG_SHUTDOWN 0x0100
// critical trip lock
#define MCP9808_REG_CONFIG_CRITLOCKED 0x0080
// alarm window lock
#define MCP9808_REG_CONFIG_WINLOCKED 0x0040
// interrupt clear
#define MCP9808_REG_CONFIG_INTCLR 0x0020
// alert output status
#define MCP9808_REG_CONFIG_ALERTSTAT 0x0010
// alert output control
#define MCP9808_REG_CONFIG_ALERTCTRL 0x0008
// alert output select
#define MCP9808_REG_CONFIG_ALERTSEL 0x0004
// alert output polarity
#define MCP9808_REG_CONFIG_ALERTPOL 0x0002
// alert output mode
#define MCP9808_REG_CONFIG_ALERTMODE 0x0001

// upper alert boundary
#define MCP9808_REG_UPPER_TEMP 0x02
// lower alert boundery
#define MCP9808_REG_LOWER_TEMP 0x03
// critical temperature
#define MCP9808_REG_CRIT_TEMP 0x04
// ambient temperature
#define MCP9808_REG_AMBIENT_TEMP 0x05
// manufacture ID
#define MCP9808_REG_MANUF_ID 0x06
// device ID
#define MCP9808_REG_DEVICE_ID 0x07
// resolutin
#define MCP9808_REG_RESOLUTION 0x08

class MCP9808 {
public:
    unsigned char kI2CBus;         
    int kI2CFileDescriptor;        
    int kI2CAddress;              
    int error;

    MCP9808(int address=0x1f);
    ~MCP9808();
    bool openMCP9808();
    void closeMCP9808();
    void reset();


    // Read the given register
    int readByte(int readRegister);

    // Write the the given value to the given register
    int writeByte(int writeRegister, int writeValue);

};

#endif
