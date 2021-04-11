#include <athenaFXAS21002C.h>
#include <math.h>

FXAS21002C::FXAS21002C(int address) {
    kI2CBus = 1;        // Default I2C
    kI2CAddress = 0x21; // Defaults to 0x1F
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
/*
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
    
    // Read 7 bytes from the sensor  
    u8 values[7] = {0, 0, 0, 0, 0, 0, 0};
    
    // replace with master receiver !! int sensor_value = i2c_smbus_read_block_data(kI2CFileDescriptor, FXAS21002C_ADDRESS, values);
    int res = i2c_smbus_read_block_data(kI2CFileDescriptor, 0x1F, values);	
    
    // i2c smbus read block data
    writeValue(GYRO_REGISTER_STATUS|0x80);
    int res = i2c_smbus_read_block_data(kI2CFileDescriptor, 0x1F, values);        

}
*/

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
        printf("ADDRESS NOT FOUND. GYRO_REG: %u", id);	
    	error = errno;	
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
    //writeByte(GYRO_REGISTER_CTRL_REG0, ctrlReg0); // Set sensitivity
    writeByte(GYRO_REGISTER_CTRL_REG0, 0x00); // Set sensitivity
    writeByte(GYRO_REGISTER_CTRL_REG1, 0x0E);     // Active */ 
    //writeByte(GYRO_REGISTER_CTRL_REG1, (3 << 2) | 2);
    usleep(100000);
    return true;
}

void FXAS21002C::closeFXAS21002C()
{
    if (kI2CFileDescriptor > 0) {
        close(kI2CFileDescriptor);
        kI2CFileDescriptor = -1 ;
    }
}

void FXAS21002C::reset () {
    // TO:DO
    usleep(5000) ;
}


int FXAS21002C::readByte(u_int8_t readRegister)
{
    u_int8_t toReturn = i2c_smbus_read_byte_data(kI2CFileDescriptor, readRegister);
    if (toReturn < 0) {
        printf("FXAS21002C Read Byte error: %d",errno) ;
        error = errno ;
        toReturn = -1 ;
    }
    return toReturn ;
}


int FXAS21002C::readWord(u_int16_t readRegister)
{
    u_int16_t toReturn = i2c_smbus_read_word_data(kI2CFileDescriptor, readRegister);
    if (toReturn < 0) {
        printf("FXAS21002C Read Byte error: %d",errno) ;
	error = errno ;
	toReturn = -1 ;
    }
    return toReturn;
}

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


int FXAS21002C::writeByte(u_int8_t writeRegister, u_int16_t writeValue)
{   
	u_int8_t toReturn = i2c_smbus_write_byte_data(kI2CFileDescriptor, writeRegister, writeValue);
	if (toReturn < 0) {
		printf("FXAS21002C_ADDRESS Write Byte error: %d",errno) ;
		error = errno ;
		toReturn = -1 ;
	}
	return toReturn ;
}

u_int8_t FXAS21002C::getStatus(void) 
{
	u_int8_t status ;
        status = i2c_smbus_read_byte_data(kI2CFileDescriptor, REG_STATUS);	
        return(status) ;
}

int16_t FXAS21002C::getX(void)
{
	u_int8_t data[2];
	int16_t value;
        data[0] = i2c_smbus_read_byte_data(kI2CFileDescriptor, REG_OUT_X_MSB);	
        while(data[0] == 255)
	{
		data[0] = i2c_smbus_read_byte_data(kI2CFileDescriptor, REG_OUT_Y_MSB);
	}	
	data[1] = i2c_smbus_read_byte_data(kI2CFileDescriptor, REG_OUT_X_LSB);
	value = ((int16_t)(data[0] << 8)) | data[1];
	return(value);
}

int16_t FXAS21002C::getY(void)
{
        u_int8_t data[2];
        int16_t value;
        data[0] = i2c_smbus_read_byte_data(kI2CFileDescriptor, REG_OUT_Y_MSB);
	while(data[0] == 255)
	{
		data[0] = i2c_smbus_read_byte_data(kI2CFileDescriptor, REG_OUT_Y_MSB);
	}
        data[1] = i2c_smbus_read_byte_data(kI2CFileDescriptor, REG_OUT_Y_LSB);	
	value = ((int16_t)(data[0] << 8)) | data[1];
        return(value);
}

int16_t FXAS21002C::getZ(void)
{
        u_int8_t data[2];
        int16_t value;
        data[0] = i2c_smbus_read_byte_data(kI2CFileDescriptor, REG_OUT_Z_MSB);        
        while(data[0] == 255)
	{
		data[0] = i2c_smbus_read_byte_data(kI2CFileDescriptor, REG_OUT_Y_MSB);
	}	
	data[1] = i2c_smbus_read_byte_data(kI2CFileDescriptor, REG_OUT_Z_LSB);
        value = ((int16_t)(data[0] << 8)) | data[1];
	return(value);
}

void FXAS21002C::setCTRL1(u_int8_t value) 
{
    writeByte(REG_CTRL_REG1, value);
}

void FXAS21002C::setCTRL2(u_int8_t value)
{
    writeByte(REG_CTRL_REG2, value);
}

void FXAS21002C::setCTRL3(u_int8_t value)
{
    writeByte(REG_CTRL_REG3, value);
}

u_int8_t FXAS21002C::getCTRL1(void) 
{
	return readByte(REG_CTRL_REG1);
}

u_int8_t FXAS21002C::getCTRL2(void)
{
	return readByte(REG_CTRL_REG2);
}

u_int8_t FXAS21002C::getCTRL3(void)
{
	return readByte(REG_CTRL_REG3);
}

void FXAS21002C::selftest(bool mode)
{
	u_int8_t value = getCTRL1();
	if (mode)
	{
	    value |= 0x10;
	} else {
	    value ^= 0x10;
	}
	setCTRL1(value);
}
void FXAS21002C::activate(bool mode)
{
        u_int8_t value = getCTRL1();
        if (mode)
        {
            value |= 0x02;
        } else {
            value ^= 0x02;
        }
        setCTRL1(value);
}
void FXAS21002C::ready(bool mode)
{
        u_int8_t value = getCTRL1();
        if (mode)
        {
            value |= 0x01;
        } else {
            value ^= 0x01;
        }
        setCTRL1(value);
}


