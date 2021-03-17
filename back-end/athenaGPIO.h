#ifndef ATHENAGPIO_H_
#define ATHENAGPIO_H_
#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define POLL_TIMEOUT (2 * 1000) // 2 Second Timeout 
#define MAX_BUF 64

typedef unsigned int jetsonGPIO ;
typedef unsigned int pinDirection ;
typedef unsigned int pinValue ;

enum pinDirections {
	inputPin  = 0,
	outputPin = 1
} ;

enum pinValues {
    low = 0,
    high = 1,
    off = 0,  // synonym for things like lights
    on = 1
}  ;


enum jetsonGPIONumber {
       gpio7  = 436, //Motor1 
       gpio15 = 268,
       gpio29 = 421, //Motor2
       gpio31 = 422,
       gpio32 = 424, //Motor3
       gpio33 = 393,
       gpio19 = 493, //Motor4
       gpio21 = 492,  
} ;


int gpioExport (jetsonGPIO gpio);
int gpioUnexport (jetsonGPIO gpio );
int gpioSetDirection (jetsonGPIO, pinDirection out_flag);
int gpioSetValue (jetsonGPIO gpio, pinValue value );
int gpioGetValue (jetsonGPIO gpio, unsigned int *value );
int gpioSetEdge (jetsonGPIO gpio, char *edge );
int gpioOpen (jetsonGPIO gpio);
int gpioClose (int fileDescriptor);
int gpioActiveLow (jetsonGPIO gpio, unsigned int value);



#endif 
