#include <athenaTCA9548A.h>
#include <math.h>

TCA9548A::TCA9548A(int address) {
    kI2CBus = 1;           // Default I2C
    kI2CAddress = address; // Defaults to 0x70 for TCA9548A ; jumper settable
    error = 0;
}

TCA9548A::~TCA9548A() {
    closeTCA9548A() ;
}

bool TCA9548A::openTCA9548A()
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

void TCA9548A::closeTCA9548A()
{
    if (kI2CFileDescriptor > 0) {
        close(kI2CFileDescriptor);
        // WARNING - This is not quite right, need to check for error first
        kI2CFileDescriptor = -1 ;
    }
}

void TCA9548A::reset () {
    // TO:DO
    usleep(5000) ;
}


// Read the given register
int TCA9548A::readByte(int readRegister)
{
    int toReturn = i2c_smbus_read_byte_data(kI2CFileDescriptor, readRegister);
    if (toReturn < 0) {
        printf("TCA9548A Read Byte error: %d",errno) ;
        error = errno ;
        toReturn = -1 ;
    }
    return toReturn ;
}

// Write the the given value to the given register
int TCA9548A::writeByte(int writeRegister, int writeValue)
{   
    int toReturn = i2c_smbus_write_byte_data(kI2CFileDescriptor, writeRegister, writeValue);
    if (toReturn < 0) {
        printf("TCA9548A Write Byte error: %d",errno) ;
        error = errno ;
        toReturn = -1 ;
    }
    return toReturn ;
}
