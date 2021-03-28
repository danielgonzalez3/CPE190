#ifndef PCA9685SERVO_H_
#define PCA9685SERVO_H_

#include <stdint.h>

#include "pca9685.h"

#define ANGLE(x)	(static_cast<uint8_t>(x))

#define SERVO_LEFT_DEFAULT_US	1000
#define SERVO_CENTER_DEFAULT_US	1500
#define SERVO_RIGHT_DEFAULT_US 	2000

class PCA9685Servo: public PCA9685 {
public:
	PCA9685Servo(uint8_t nAddress = 0x40);
	~PCA9685Servo();

	void SetLeftUs(uint16_t);
	uint16_t GetLeftUs() const;

	void SetRightUs(uint16_t);
	uint16_t GetRightUs() const;

	void SetCenterUs(uint16_t);
	uint16_t GetCenterUs() const;

	void Set(uint8_t nChannel, uint16_t nData);
	void Set(uint8_t nChannel, uint8_t nData);

	void SetAngle(uint8_t nChannel, uint8_t nAngle);

private:
	void CalcLeftCount();
	void CalcRightCount();
	void CalcCenterCount();

private:
	uint16_t m_nLeftUs{SERVO_LEFT_DEFAULT_US};
	uint16_t m_nRightUs{SERVO_RIGHT_DEFAULT_US};
	uint16_t m_nCenterUs{SERVO_CENTER_DEFAULT_US};
	uint16_t m_nLeftCount;
	uint16_t m_nRightCount;
	uint16_t m_nCenterCount;
};

#endif /* PCA9685SERVO_H_ */
