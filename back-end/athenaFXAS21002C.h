//PWM ATHENA
extern "C" {
#include <i2c/smbus.h>
}

#ifndef __ATHENAFXAS21002C_H__
#define __ATHENAFXAS21002C_H__

#include <cstddef>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <cstdlib>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define REG_STATUS               0x00
#define REG_OUT_X_MSB            0x01
#define REG_OUT_X_LSB            0x02
#define REG_OUT_Y_MSB            0x03
#define REG_OUT_Y_LSB            0x04
#define REG_OUT_Z_MSB            0x05
#define REG_OUT_Z_LSB            0x06
#define REG_DR_STATUS            0x07
#define REG_F_STATUS             0x08
#define REG_F_SETUP              0x09
#define REG_F_EVENT              0x0A
#define REG_INT_SRC_FLAG         0x0B
#define REG_WHO_AM_I             0x0C
#define REG_CTRL_REG0            0x0D
#define REG_RT_CFG               0x0E
#define REG_RT_SRC               0x0F
#define REG_RT_THS               0x10
#define REG_RT_COUNT             0x11
#define REG_TEMP                 0x12
#define REG_CTRL_REG1            0x13
#define REG_CTRL_REG2            0x14
#define REG_CTRL_REG3            0x15

#define FXAS21002C_ADDRESS       (0x1F) // 0100001
#define FXAS21002C_ID            (0xD7) // 1101 0111
#define GYRO_SENSITIVITY_250DPS  (0.0078125F) // Table 35 of datasheet
#define GYRO_SENSITIVITY_500DPS  (0.015625F)
#define GYRO_SENSITIVITY_1000DPS (0.03125F)
#define GYRO_SENSITIVITY_2000DPS (0.0625F)
#define SENSORS_DPS_TO_RADS      (0.017453293F) 

typedef struct
{
    char     name[12];                        /**< sensor name */
    int32_t  version;                         /**< version of the hardware + driver */
    int32_t  sensor_id;                       /**< unique sensor identifier */
    int32_t  type;                            /**< this sensor's type (ex. SENSOR_TYPE_LIGHT) */
    float    max_value;                       /**< maximum value of this sensor's value in SI units */
    float    min_value;                       /**< minimum value of this sensor's value in SI units */
    float    resolution;                      /**< smallest difference between two values reported by this sensor */
    int32_t  min_delay;                       /**< min delay in microseconds between events. zero = not a constant rate */
} sensor_t;

typedef enum
{
    SENSOR_TYPE_ACCELEROMETER         = (1),   /**< Gravity + linear acceleration */
    SENSOR_TYPE_MAGNETIC_FIELD        = (2),
    SENSOR_TYPE_ORIENTATION           = (3),
    SENSOR_TYPE_GYROSCOPE             = (4),
    SENSOR_TYPE_LIGHT                 = (5),
    SENSOR_TYPE_PRESSURE              = (6),
    SENSOR_TYPE_PROXIMITY             = (8),
    SENSOR_TYPE_GRAVITY               = (9),
    SENSOR_TYPE_LINEAR_ACCELERATION   = (10),  /**< Acceleration not including gravity */
    SENSOR_TYPE_ROTATION_VECTOR       = (11),
    SENSOR_TYPE_RELATIVE_HUMIDITY     = (12),
} sensors_type_t;
 

typedef struct {
    union {
        float v[3];
        struct {
            float x;
            float y;
            float z;
        };
        /* Orientation sensors */
        struct {
            float roll;    /**< Rotation around the longitudinal axis (the plane body, 'X axis'). Roll is positive and increasing when moving downward. -90?<=roll<=90? */
            float pitch;   /**< Rotation around the lateral axis (the wing span, 'Y axis'). Pitch is positive and increasing when moving upwards. -180?<=pitch<=180?) */
            float heading; /**< Angle between the longitudinal axis (the plane body) and magnetic north, measured clockwise when viewing from the top of the device. 0-359? */
        };
    };
    int8_t status;
    u_int8_t reserved[3];
} sensors_vec_t;




typedef struct
{
    int32_t version;                          /**< must be sizeof(struct sensors_event_t) */
    int32_t sensor_id;                        /**< unique sensor identifier */
    int32_t type;                             /**< sensor type */
    int32_t reserved0;                        /**< reserved */
    int32_t timestamp;                        /**< time is in milliseconds */
    union
    {
        float           data[4];
        sensors_vec_t   acceleration;         /**< acceleration values are in meter per second per second (m/s^2) */
        sensors_vec_t   magnetic;             /**< magnetic vector values are in micro-Tesla (uT) */
        sensors_vec_t   orientation;          /**< orientation values are in degrees */
        sensors_vec_t   gyro;                 /**< gyroscope values are in rad/s */
        float           temperature;          /**< temperature is in degrees centigrade (Celsius) */
        float           distance;             /**< distance in centimeters */
        float           light;                /**< light in SI lux units */
        float           pressure;             /**< pressure in hectopascal (hPa) */
        float           relative_humidity;    /**< relative humidity in percent */
        float           current;              /**< current in milliamps (mA) */
        float           voltage;              /**< voltage in volts (V) */
    };
} sensors_event_t;


typedef enum {
    GYRO_RANGE_250DPS  = 250,   /**< 250dps */
    GYRO_RANGE_500DPS  = 500,   /**< 500dps */
    GYRO_RANGE_1000DPS = 1000, /**< 1000dps */
    GYRO_RANGE_2000DPS = 2000  /**< 2000dps */
} gyroRange_t;

typedef enum {
    GYRO_REGISTER_STATUS    = 0x00, /**< 0x00 */
    GYRO_REGISTER_OUT_X_MSB = 0x01, /**< 0x01 */
    GYRO_REGISTER_OUT_X_LSB = 0x02, /**< 0x02 */
    GYRO_REGISTER_OUT_Y_MSB = 0x03, /**< 0x03 */
    GYRO_REGISTER_OUT_Y_LSB = 0x04, /**< 0x04 */
    GYRO_REGISTER_OUT_Z_MSB = 0x05, /**< 0x05 */
    GYRO_REGISTER_OUT_Z_LSB = 0x06, /**< 0x06 */
    GYRO_REGISTER_WHO_AM_I  = 0x0C, /**< 0x0C (default value = 0b11010111, read only) */
    GYRO_REGISTER_CTRL_REG0 = 0x0D, /**< 0x0D (default value = 0b00000000, read/write) */
    GYRO_REGISTER_CTRL_REG1 = 0x13, /**< 0x13 (default value = 0b00000000, read/write) */
    GYRO_REGISTER_CTRL_REG2 = 0x14, /**< 0x14 (default value = 0b00000000, read/write) */
} gyroRegisters_t;

typedef struct gyroRawData_s {
    int16_t x; /**< Raw int16_t value for the x axis */
    int16_t y; /**< Raw int16_t value for the y axis */
    int16_t z; /**< Raw int16_t value for the z axis */
} gyroRawData_t;


class FXAS21002C
{
public:
    unsigned char kI2CBus;         // I2C bus
    int kI2CFileDescriptor;        // File Descriptor
    int kI2CAddress;               // Address
    int error;
    int rng;

    FXAS21002C(int address=FXAS21002C_ADDRESS);
    ~FXAS21002C();
    bool openFXAS21002C();
    void closeFXAS21002C();
    void reset();
    
    void getSensor(sensor_t* sensor);
    void getEvent(sensors_event_t *event);
    // Read the given register
    int readByte(u_int8_t readRegister);
    int readWord(u_int16_t readRegister);
    int readBlock(int readRegister);

    // Write the the given value to the given register
    int writeValue(int writeValue);
    int writeByte(u_int8_t writeRegister, u_int16_t writeValue);
    int getError();
    gyroRawData_t raw;

    // New Implementation
    u_int8_t getStatus(void);
    int16_t getX(void);
    int16_t getY(void);
    int16_t getZ(void);
    void activate(bool mode);
    void selftest(bool mode);
    void ready(bool mode);
    u_int8_t getCTRL1(void);
    void setCTRL1(u_int8_t value);
    u_int8_t getCTRL2(void);
    void setCTRL2(u_int8_t value);
    u_int8_t getCTRL3(void);
    void setCTRL3(u_int8_t value);
};


#endif
