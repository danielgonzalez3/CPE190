#include <athenaFXOS8700.h>
#include <math.h>

FXOS8700::FXOS8700(int address) {
    kI2CBus = 1;        // Default I2C
    kI2CAddress = 0x1F; // Defaults to 0x1F
    error = 0;
}

FXOS8700::~FXOS8700() {
    closeFXOS8700() ;
}


bool FXOS8700::openFXOS8700()
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
    u_int8_t id = readByte(FXOS8700_REGISTER_WHO_AM_I);
    if (id != FXOS8700_ID) {
    	printf("ADDRESS NOT FOUND: %u", id);	
	error = errno;	
	return false;
    }
    // Set to standby mode
    writeByte(FXOS8700_REGISTER_CTRL_REG1, 0x00);
    
    // Set for 4G
    writeByte(FXOS8700_REGISTER_XYZ_DATA_CFG, ACCEL_RANGE_4G);

    // High resolution
    writeByte(FXOS8700_REGISTER_CTRL_REG2, 0x02);

    // Active, Normal Mode, Low Noise, 100Hz in Hybrid Mode
    writeByte(FXOS8700_REGISTER_CTRL_REG1, 0x15);

    // Hybrid Mode, Over Sampling Rate = 16
    writeByte(FXOS8700_REGISTER_MCTRL_REG1, 0x1F);

    // Jump to reg 0x33 after reading 0x06
    writeByte(FXOS8700_REGISTER_MCTRL_REG2, 0x20);

    usleep(10000);
    return true;
}

void FXOS8700::closeFXOS8700()
{
    if (kI2CFileDescriptor > 0) {
        close(kI2CFileDescriptor);
        kI2CFileDescriptor = -1 ;
    }
}

void FXOS8700::reset () {
    // TO:DO
    usleep(5000) ;
}


int FXOS8700::readByte(u_int8_t readRegister)
{
    u_int8_t toReturn = i2c_smbus_read_byte_data(kI2CFileDescriptor, readRegister);
    if (toReturn < 0) {
        printf("FXOS8700 Read Byte error: %d",errno) ;
        error = errno ;
        toReturn = -1 ;
    }
    return toReturn ;
}
float FXOS8700::readX()
{
    u8 values[7] = {0, 0, 0, 0, 0, 0, 0};
    int len;
    len = i2c_smbus_read_i2c_block_data_or_emulated(kI2CFileDescriptor, FXOS8700_M_DR_STATUS, 7, values);
    float factor = 1 / 10.0;
    int16_t x = (int16_t)((temp[1] << 8) | temp[2]);
    return (float)x * factor;
}


int FXOS8700::readWord(u_int16_t readRegister)
{
    u_int16_t toReturn = i2c_smbus_read_word_data(kI2CFileDescriptor, readRegister);
    if (toReturn < 0) {
        printf("FXOS8700 Read Byte error: %d",errno) ;
	error = errno ;
	toReturn = -1 ;
    }
    return toReturn;
}

int FXOS8700::writeValue(int writeValue)
{   
    int toReturn = i2c_smbus_write_byte(kI2CFileDescriptor, writeValue);
    if (toReturn < 0) {
        printf("FXOS8700 Write Byte error: %d",errno) ;
        error = errno ;
        toReturn = -1 ;
    }
    return toReturn ;
}


int FXOS8700::writeByte(u_int8_t writeRegister, u_int16_t writeValue)
{   
	u_int8_t toReturn = i2c_smbus_write_byte_data(kI2CFileDescriptor, writeRegister, writeValue);
	if (toReturn < 0) {
		printf("FXOS8700 Write Byte error: %d",errno) ;
		error = errno ;
		toReturn = -1 ;
	}
	return toReturn ;
}


