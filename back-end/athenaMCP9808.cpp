#include <athenaMCP9808.h>
#include <math.h>

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
