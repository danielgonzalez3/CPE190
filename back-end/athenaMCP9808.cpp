#include <athenaMCP9808.h>
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

MCP9808::MCP9808(int address) {
    kI2CBus = 1;           
    kI2CAddress = address; 
    error = 0;
}

MCP9808::~MCP9808() {
    closeMCP9808() ;
}

bool MCP9808::openMCP9808()
{
    char fileNameBuffer[32];
    sprintf(fileNameBuffer,"/dev/i2c-%d", kI2CBus);
    kI2CFileDescriptor = open(fileNameBuffer, O_RDWR);
    if (kI2CFileDescriptor < 0) {
       error = errno ;
       return false ;
    }
    if (ioctl(kI2CFileDescriptor, I2C_SLAVE, kI2CAddress) < 0) {
        error = errno ;
        return false ;
    }
    i2c_smbus_write_word_data(kI2CFileDescriptor, MCP9808_REG_CONFIG, 0x0);
    return true ;
}

void MCP9808::closeMCP9808()
{
    if (kI2CFileDescriptor > 0) {
        close(kI2CFileDescriptor);
        kI2CFileDescriptor = -1 ;
    }
}

void MCP9808::reset () {
    // TO:DO
    usleep(5000) ;
}


// Read the given register
int MCP9808::readByte(int readRegister)
{
    int toReturn = i2c_smbus_read_byte_data(kI2CFileDescriptor, readRegister);
    if (toReturn < 0) {
        printf("MCP9808 Read Byte error: %d",errno) ;
        error = errno ;
        toReturn = -1 ;
    }
    return toReturn ;
}

// Write the the given value to the given register
int MCP9808::writeByte(int writeRegister, int writeValue)
{   
    int toReturn = i2c_smbus_write_byte_data(kI2CFileDescriptor, writeRegister, writeValue);
    if (toReturn < 0) {
        printf("MCP9808 Write Byte error: %d",errno) ;
        error = errno ;
        toReturn = -1 ;
    }
    return toReturn ;
}

float MCP9808::readTempF() {
    float temp = NAN;
    int file;
    const char *bus = "/dev/i2c-1";
    if ((file = open(bus, O_RDWR)) < 0) {
    	printf("Temp Sensor Broken. \n");
	exit(1);
    }
    ioctl(file, I2C_SLAVE, 0x18);
    char config[3] = {0};
    config[0] = 0x01;
    config[1] = 0x00;
    config[2] = 0x00;
    (void) write(file, config, 3);
    config[0] = 0x08;
    config[1] = 0x03;
    (void) write(file, config, 2);
    usleep(5000);
    char reg[1] = {0x05};
    (void) write(file, reg, 1);
    char data[2] = {0};
    if(read(file, data, 2) != 2)
    {
        printf("Error : Input/Output error \n");
    }else{
    	temp = ((data[0] & 0x1F) * 256 + data[1]);
	temp = temp * 1.8 + 32;
    }


    return (temp/10);
}
