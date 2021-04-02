#include <athenaPCA9685.h>
#include <math.h>
#include <cassert>
#include <iostream>
PCA9685::PCA9685(int address) {
    kI2CBus = 1;           // Default I2C
    kI2CAddress = address; // Defaults to 0x40 for PCA9685 ; jumper settable
    error = 0;
}

PCA9685::~PCA9685() {
    closePCA9685() ;
}

bool PCA9685::openPCA9685()
{
    char fileNameBuffer[32];
    sprintf(fileNameBuffer,"/dev/i2c-%d", kI2CBus);
    kI2CFileDescriptor = open(fileNameBuffer, O_RDWR);
    if (kI2CFileDescriptor < 0) {
        // Could not open the file
       error = errno ;
       return false ;
    }
    if (ioctl(kI2CFileDescriptor, I2C_SLAVE, kI2CAddress) < 0) {
        // Could not open the device on the bus
        error = errno ;
        return false ;
    }
    return true ;
}

void PCA9685::closePCA9685()
{
    if (kI2CFileDescriptor > 0) {
        close(kI2CFileDescriptor);
        // WARNING - This is not quite right, need to check for error first
        kI2CFileDescriptor = -1 ;
    }
}

void PCA9685::reset () {
    writeByte(PCA9685_MODE1, PCA9685_ALLCALL );
    writeByte(PCA9685_MODE2, PCA9685_OUTDRV) ;
    // Wait for oscillator to stabilize
    usleep(5000) ;
}

// Sets the frequency of the PWM signal
// Frequency is ranged between 40 and 1000 Hertz
void PCA9685::setPWMFrequency ( float frequency ) {
    printf("Setting PCA9685 PWM frequency to %f Hz\n",frequency) ;
    float rangedFrequency = fmin(fmax(frequency,40),1000) ;
    int prescale = (int)(25000000.0f / (4096 * rangedFrequency) - 0.5f) ;
    // For debugging
    // printf("PCA9685 Prescale: 0x%02X\n",prescale) ;
    int oldMode = readByte(PCA9685_MODE1) ;
     int newMode = ( oldMode & 0x7F ) | PCA9685_SLEEP ;
    writeByte(PCA9685_MODE1, newMode) ;
    writeByte(PCA9685_PRE_SCALE, prescale) ;
    writeByte(PCA9685_MODE1, oldMode) ;
    // Wait for oscillator to stabilize
    usleep(5000) ;
    writeByte(PCA9685_MODE1, oldMode | PCA9685_RESTART) ;
}

// Channels 0-15
// Channels are in sets of 4 bytes
void PCA9685::setPWM ( int channel, int onValue, int offValue) {
    printf("PCA9685 CHANNEL: 0x%02X\n",channel); 
    printf("on PWM: %d\n",onValue);
    printf("off PWM: %d\n\n\n",offValue);   
    writeByte(PCA9685_LED0_ON_L+4*channel, onValue & 0xFF) ;
    writeByte(PCA9685_LED0_ON_H+4*channel, onValue >> 8) ;
    writeByte(PCA9685_LED0_OFF_L+4*channel, offValue & 0xFF) ;
    writeByte(PCA9685_LED0_OFF_H+4*channel, offValue >> 8) ;
}
void PCA9685::setPWM_ANGLE (int angle, int L, int R){
    // Left_us = 700;
    // Right_us = 2400;
    // L = (.5 + ((204.8 * L) / 1000));
    // R = (.5 + ((204.8 * R) / 1000));
    
    if (angle == 0) {
        L = (.5 + ((204.8 * L) / 1000));	    
        setPWM(0, 0, L); 
    } else if (angle == 180) {
        R = (.5 + ((204.8 * R) / 1000));	    
        setPWM(0, 0, R);
    } else if (angle == 90) {
        float Center = (L + R)/2;
        float M = (.5 + ((204.8 * Center) / 1000));
        setPWM(0, 0, M);
    } else {
        float Center = (L + R)/2;
	float M = (.5 + ((204.8 * Center) / 1000));
	float PWM = (2 * M) - R + (.5 + (static_cast<float>((R - M)) / 90) * angle);
        setPWM(0, 0, PWM);
    }

}
void PCA9685::setPWM_60KG (int channel, int start, int end){
}
void PCA9685::setAllPWM (int onValue, int offValue) {
    writeByte(PCA9685_ALL_LED_ON_L, onValue & 0xFF) ;
    writeByte(PCA9685_ALL_LED_ON_H, onValue >> 8) ;
    writeByte(PCA9685_ALL_LED_OFF_L, offValue & 0xFF) ;
    writeByte(PCA9685_ALL_LED_OFF_H, offValue >> 8) ;
}


// Read the given register
int PCA9685::readByte(int readRegister)
{
    int toReturn = i2c_smbus_read_byte_data(kI2CFileDescriptor, readRegister);
    if (toReturn < 0) {
        printf("PCA9685 Read Byte error: %d",errno) ;
        error = errno ;
        toReturn = -1 ;
    }
    // For debugging
    // printf("Device 0x%02X returned 0x%02X from register 0x%02X\n", kI2CAddress, toReturn, readRegister);
    return toReturn ;
}

// Write the the given value to the given register
int PCA9685::writeByte(int writeRegister, int writeValue)
{   // For debugging:
    // printf("Wrote: 0x%02X to register 0x%02X \n",writeValue, writeRegister) ;
    int toReturn = i2c_smbus_write_byte_data(kI2CFileDescriptor, writeRegister, writeValue);
    if (toReturn < 0) {
        printf("PCA9685 Write Byte error: %d",errno);
	printf("\n");
        error = errno ;
        toReturn = -1 ;
    }
    return toReturn ;
}
/*
int PCA9685::writeBlock(int writeRegister, int writeValue)
{
	int toReturn = i2c_smbus_write_byte_data(kI2CFileDescriptor, writeRegister, writeValue);
	if (toReturn < 0) {
		printf("PCA9685 Write Block error: %d",errno) ;
		error = errno ;
		toReturn = -1 ;
	}
	return toReturn ;
}









// Servo Functions - May Not need
void PCA9685::SetLeftUs(u_int16_t nLeftUs) {
    assert(nLeftUs < m_nRightUs);
    assert(nLeftUs < m_nCenterUs);

    m_nLeftUs = nLeftUs;
    CalcLeftCount();
}
u_int16_t PCA9685::GetLeftUs() const {
    return m_nLeftUs;
}

void PCA9685::SetRightUs(u_int16_t nRightUs) {
    assert(m_nLeftUs < nRightUs);
    assert(m_nCenterUs < nRightUs);

    m_nRightUs = nRightUs;
    CalcRightCount();
}

u_int16_t PCA9685::GetRightUs() const {
    return m_nRightUs;
}
u_int16_t PCA9685Servo::GetCenterUs() const {
    return m_nCenterUs;
}

void PCA9685Servo::CalcLeftCount() {
    m_nLeftCount = (.5 + ((204.8 * m_nLeftUs) / 1000));
}

void PCA9685Servo::CalcRightCount() {
    m_nRightCount = (.5 + ((204.8 * m_nRightUs) / 1000));
}

void PCA9685Servo::CalcCenterCount() {
    m_nCenterCount = (.5 + ((204.8 * m_nCenterUs) / 1000));
}
void PCA9685Servo::SetAngle(u_int8_t nChannel, u_int8_t nAngle) 
{
    if (nAngle == 0) {
        Write(nChannel, m_nLeftCount);
    } else if (nAngle == 90) {
        Write(nChannel, m_nCenterCount);
    } else if (nAngle >= 180) {
        Write(nChannel, m_nRightCount);
	   } else if (nAngle < 90) {
        const u_int16_t nCount = m_nLeftCount + (.5 + (static_cast<float>((m_nCenterCount - m_nLeftCount)) / 90) * nAngle);
        Write(nChannel, nCount);
	   } else {
        const u_int16_t nCount = (2 * m_nCenterCount) - m_nRightCount + (.5 + (static_cast<float>((m_nRightCount - m_nCenterCount)) / 90) * nAngle);
        Write(nChannel, nCount);
	    }
}

void PCA9685::Write(u_int8_t nChannel, u_int16_t nValue) {
    Write(nChannel, static_cast<u_int16_t>(0), nValue);
}

void PCA9685::Write(u_int16_t nOn, u_int16_t nOff) {
    Write(static_cast<u_int8_t>(16), nOn, nOff);
}

void PCA9685::Write(u_int16_t nValue) {
    Write(static_cast<u_int8_t>(16), nValue);
}
void PCA9685::Write(u_int8_t nChannel, u_int16_t nOn, u_int16_t nOff) {
    u_int8_t reg;

    if (nChannel <= 15)
    {
        reg = PCA9685_ALL_LED_ON_L + (nChannel << 2);
    } else {
        reg = PCA9685_ALL_LED_ON_L;
    }

    I2cWriteReg(reg, nOn, nOff);
}
void PCA9685::I2cWriteReg(u_int8_t reg, u_int16_t data, u_int16_t data2) {
	char buffer[5];
	buffer[0] = reg;
	buffer[1] = (data & 0xFF);
	buffer[2] = (data >> 8);
	buffer[3] = (data2 & 0xFF);
	buffer[4] = (data2 >> 8);

	I2cSetup();

	FUNC_PREFIX(i2c_write(buffer, 5));
}

void PCA9685::I2cSetup() {
    FUNC_PREFIX(i2c_set_address(m_nAddress));
    FUNC_PREFIX(i2c_set_baudrate(hal::i2c::FULL_SPEED));
}

void PCA9685::I2cWriteReg(u_int8_t reg, u_int8_t data) {
    char buffer[2];

    buffer[0] = reg;
    buffer[1] = data;

    I2cSetup();

    FUNC_PREFIX(i2c_write(buffer, 2));
}

u_int8_t PCA9685::I2cReadReg(u_int8_t reg) {
    char data = reg;

    I2cSetup();

    FUNC_PREFIX(i2c_write(&data, 1));
    FUNC_PREFIX(i2c_read(&data, 1));

    return data;
}

void PCA9685::I2cWriteReg(u_int8_t reg, u_int16_t data) {
    char buffer[3];

    buffer[0] = reg;
    buffer[1] = (data & 0xFF);
    buffer[2] = (data >> 8);

    I2cSetup();

    FUNC_PREFIX(i2c_write(buffer, 3));
}

u_int16_t PCA9685::I2cReadReg16(u_int8_t reg) {
    char data = reg;
    char buffer[2] = { 0, 0 };

    I2cSetup();

    FUNC_PREFIX(i2c_write(&data, 1));
    FUNC_PREFIX(i2c_read(reinterpret_cast<char *>(&buffer), 2));

    return (buffer[1] << 8) | buffer[0];
}

void PCA9685::I2cWriteReg(u_int8_t reg, u_int16_t data, u_int16_t data2) {
    char buffer[5];

    buffer[0] = reg;
    buffer[1] = (data & 0xFF);
    buffer[2] = (data >> 8);
    buffer[3] = (data2 & 0xFF);
    buffer[4] = (data2 >> 8);

    I2cSetup();

    FUNC_PREFIX(i2c_write(buffer, 5));
}
*/
