#include <athenaFXAS21002C.h>
#include <math.h>

FXAS21002C::FXAS21002C(int address) {
    kI2CBus = 1;        // Default I2C
    kI2CAddress = 0x1F; // Defaults to 0x1F
    error = 0;
    rng = GYRO_RANGE_2000DPS; // DPS
}

FXAS21002C::~FXAS21002C() {
    closeFXAS21002C() ;
}

void FXAS21002C::getSensor(sensor_t *sensor) {
    /* Clear the sensor_t object */
    memset(sensor, 0, sizeof(sensor_t));
    /* Insert the sensor name in the fixed length char array */
    strncpy(sensor->name, "FXAS21002C", sizeof(sensor->name) - 1);
    sensor->name[sizeof(sensor->name) - 1] = 0;
    sensor->version = 1;
    sensor->type = SENSOR_TYPE_GYROSCOPE;
    sensor->min_delay = 0;
    sensor->max_value = (float)this->rng * SENSORS_DPS_TO_RADS;
    sensor->min_value = (this->rng * -1.0) * SENSORS_DPS_TO_RADS;
    sensor->resolution = 0.0F; // TBD
}
void   FXAS21002C::getEvent(sensors_event_t *event) {
    bool readingValid = false;
    memset(event, 0, sizeof(sensors_event_t));
    raw.x = 0;
    raw.y = 0;
    raw.z = 0;
    event->version = sizeof(sensors_event_t);
    //event->sensor_id = _sensorID
    event->type = SENSOR_TYPE_GYROSCOPE;
    //event->timestamp = millis();
    
    /* Read 7 bytes from the sensor */ 
    u8 values[7] = {0, 0, 0, 0, 0, 0, 0};
    
    // replace with master receiver !! int sensor_value = i2c_smbus_read_block_data(kI2CFileDescriptor, FXAS21002C_ADDRESS, values);
    int res = i2c_smbus_read_block_data(kI2CFileDescriptor, 0x1F, values);	
    
    // i2c smbus read block data
    writeValue(GYRO_REGISTER_STATUS|0x80);
    int res = i2c_smbus_read_block_data(kI2CFileDescriptor, 0x1F, values);        

}


bool FXAS21002C::openFXAS21002C()
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
    u_int8_t id = readByte(GYRO_REGISTER_WHO_AM_I);
    if (id != FXAS21002C_ID) {
	return false;
    }
    u_int8_t ctrlReg0 = 0x00;
    switch (rng) {
    case GYRO_RANGE_250DPS:
        ctrlReg0 = 0x03;
	break;
    case GYRO_RANGE_500DPS:
	ctrlReg0 = 0x02;
        break;
    case GYRO_RANGE_1000DPS:
	ctrlReg0 = 0x01;
	break;
    case GYRO_RANGE_2000DPS:
        ctrlReg0 = 0x00;
        break;
    }
    writeByte(GYRO_REGISTER_CTRL_REG1, 0x00);     // Standby
    writeByte(GYRO_REGISTER_CTRL_REG1, (1 << 6)); // Reset
    writeByte(GYRO_REGISTER_CTRL_REG0, ctrlReg0); // Set sensitivity
    writeByte(GYRO_REGISTER_CTRL_REG1, 0x0E);     // Active   
    return true;
}

void FXAS21002C::closeFXAS21002C()
{
    if (kI2CFileDescriptor > 0) {
        close(kI2CFileDescriptor);
        // WARNING - This is not quite right, need to check for error first
        kI2CFileDescriptor = -1 ;
    }
}

void FXAS21002C::reset () {
    // TO:DO
    usleep(5000) ;
}


int FXAS21002C::readByte(int readRegister)
{
    int toReturn = i2c_smbus_read_byte_data(kI2CFileDescriptor, readRegister);
    if (toReturn < 0) {
        printf("FXAS21002C Read Byte error: %d",errno) ;
        error = errno ;
        toReturn = -1 ;
    }
    return toReturn ;
}


int FXAS21002C::readWord(int readRegister)
{
    int toReturn = i2c_smbus_read_word_data(kI2CFileDescriptor, readRegister);
    if (toReturn < 0) {
        printf("FXAS21002C Read Byte error: %d",errno) ;
	error = errno ;
	toReturn = -1 ;
    }
    return toReturn;
}


/*
int FXAS21002C::readBlock(int readRegister)
{
    int toReturn = i2c_smbus_read_block_data(kI2CFileDescriptor, readRegister);
    if (toReturn < 0) {
        printf("FXAS21002C Read Byte error: %d",errno) ;
        error = errno ;
        toReturn = -1 ;
    }
    return toReturn ;
}*/


int FXAS21002C::writeValue(int writeValue)
{   
    int toReturn = i2c_smbus_write_byte(kI2CFileDescriptor, writeValue);
    if (toReturn < 0) {
        printf("FXAS21002C_ADDRESS Write Byte error: %d",errno) ;
        error = errno ;
        toReturn = -1 ;
    }
    return toReturn ;
}


int FXAS21002C::writeByte(int writeRegister, int writeValue)
{   
	int toReturn = i2c_smbus_write_byte_data(kI2CFileDescriptor, writeRegister, writeValue);
	if (toReturn < 0) {
		printf("FXAS21002C_ADDRESS Write Byte error: %d",errno) ;
		error = errno ;
		toReturn = -1 ;
	}
	return toReturn ;
}







