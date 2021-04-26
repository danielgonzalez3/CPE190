/*Project Athena Driver
  Daniel Gonzalez */

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include <cstring>
#include <sys/time.h> //Might not need
#include <iostream>
#include <string>
#include <unistd.h>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <csignal>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <ctime>
#include <athenaPCA9685.h>
#include <athenaMCP9808.h>
#include "athenaGPIO.h"

// Motor Variables ["10" -> Foward | "01" -> Backwards] 
extern const int servoMin_20 = 1564;
extern const int servoMax_20 = 6756;

extern const int servoMin_60 = 1780;
extern const int servoMax_60 = 7250;

int M0 = 15; // FRONT LEFT
int M1 = 14; // FRONT RIGHT
int M2 = 13; // BACK  LEFT
int M3 = 12; // BACK  RIGHT

extern const jetsonGPIONumber M0_F = gpio33;
extern const jetsonGPIONumber M0_B = gpio32;

extern const jetsonGPIONumber M1_F = gpio29;
extern const jetsonGPIONumber M1_B = gpio31;

extern const jetsonGPIONumber M2_F = gpio7;
extern const jetsonGPIONumber M2_B = gpio15;

extern const jetsonGPIONumber M3_F = gpio21;
extern const jetsonGPIONumber M3_B = gpio19;

extern const int L0_0 = 6; // BOT
extern const int L0_1 = 7; // MID
extern const int L0_2 = 8; // TOP

extern const int L1_0 = 5;
extern const int L1_1 = 4;
extern const int L1_2 = 3;

extern const int L2_0 = 11;
extern const int L2_1 = 10;
extern const int L2_2 = 9;

extern const int L3_0 = 0;
extern const int L3_1 = 1;
extern const int L3_2 = 2;


using namespace std;
string getFile(string filename);                         
string filename  = "/var/www/html/CPE190/front-end/data.xml";
int state        = 0;
int nextState    = 0;
int oldtime      = 0;
int newtime      = 0;
int t_delta      = 0;
int t_pivot      = 0;
int baseFreq     = 1100;
int temp_mcp     = -1;
PCA9685 *pca9685  = new PCA9685((int)0x70);
MCP9808 *mcp9808 = new MCP9808();

// Signal Clean Up Process
void signal_clean_up(int signum)
{
	printf("\n\nTerminating Athena\nINTERRUPT_SIGN_TYPE:%d\n",signum);
	state = 0;
	nextState = 0;
	pca9685->setAllPWM(0,0);
	pca9685->reset();
	pca9685->closePCA9685();
        mcp9808->closeMCP9808();
	gpioSetValue(M0_F, 0);
	gpioSetValue(M0_B, 0);
	gpioSetValue(M1_F, 0);
	gpioSetValue(M1_B, 0);
	gpioSetValue(M2_F, 0);
	gpioSetValue(M2_B, 0);
	gpioSetValue(M3_F, 0);
	gpioSetValue(M3_B, 0);
	gpioUnexport(M0_F);
	gpioUnexport(M0_B);
	gpioUnexport(M1_F);
	gpioUnexport(M1_B);
	gpioUnexport(M2_F);
	gpioUnexport(M2_B);
	gpioUnexport(M3_F);
	gpioUnexport(M3_B);
	exit(signum);
}

int getkey() {
    int character;
    struct termios orig_term_attr;
    struct termios new_term_attr;

    // Terminal raw mode
    tcgetattr(fileno(stdin), &orig_term_attr);
    std::memcpy(&new_term_attr, &orig_term_attr, sizeof(struct termios));
    new_term_attr.c_lflag &= ~(ECHO|ICANON);
    new_term_attr.c_cc[VTIME] = 0;
    new_term_attr.c_cc[VMIN] = 0;
    tcsetattr(fileno(stdin), TCSANOW, &new_term_attr);

    // read a character from the stdin stream without blocking */
    // returns EOF (-1) if no character is available */
    character = fgetc(stdin);

    tcsetattr(fileno(stdin), TCSANOW, &orig_term_attr);

    return character;
}

int main(int argc, char **argv) 
{
        int err = pca9685->openPCA9685();	
	gpioExport(M0_F);
	gpioExport(M0_B);
	gpioExport(M1_F);
	gpioExport(M1_B);
	gpioExport(M2_F);
	gpioExport(M2_B);
	gpioExport(M3_F);
	gpioExport(M3_B);
	gpioSetDirection(M0_F, outputPin);
	gpioSetDirection(M0_B, outputPin);
	gpioSetDirection(M1_F, outputPin);
	gpioSetDirection(M1_B, outputPin);
	gpioSetDirection(M2_F, outputPin);
	gpioSetDirection(M2_B, outputPin);
	gpioSetDirection(M3_F, outputPin);
	gpioSetDirection(M3_B, outputPin);
        gpioSetValue(M0_B, 0);
        gpioSetValue(M1_B, 0);
        gpioSetValue(M2_B, 0);
        gpioSetValue(M3_B, 0);
        gpioSetValue(M0_F, 0);
        gpioSetValue(M1_F, 0);
        gpioSetValue(M2_F, 0);
        gpioSetValue(M3_F, 0);
	signal(SIGINT, signal_clean_up);
	if (getuid() != 0) 
	{
		fprintf(stderr, "Program is not started as \'root\' (sudo)\n");
		return -1;
	}
	if (err < 0)
	{
		printf("Error: %d", pca9685->error); 
	}else{
		pca9685->setAllPWM(0,0);
	}

	ofstream xml;
	xml.open(filename, std::ofstream::trunc);
	xml << "-1";
	xml.close();
	while(1) 
	{	string text = getFile(filename);
		if (text.substr(0,2) == "-1")
		{
			std::cout << "Waiting for Initialization..." << std::endl;
		}else{	
			break;
		}
		sleep(2);	
	}
        std::cout << "\nProject Athena Ready and Running...\n" << '\n' << std::endl;
	pca9685->setAllPWM(0,0);
	pca9685->reset();
	sleep(2);
	while(1) 
	{
		usleep(10000); // Minor Delay
		string text = getFile(filename);
		int tmp = std::stoi (text.substr(89, 10));	
		if (state == 0)
		{
			cout << "States Set" << endl;
			nextState = text.at(66) - '0';
			oldtime = tmp;
			newtime = tmp;
		}else{
			nextState = text.at(66) - '0';
		}
		newtime = tmp;

		if(state != nextState)
		{

			t_delta = newtime - oldtime;
			oldtime = newtime;
			temp_mcp = mcp9808->readTempF();
			
			// State 1 - FORWARD
			if (nextState == 1)
			{
			        std::cout << "STATE: 1 " << "STATE SWITCH: " << t_delta << " LAST STATE " << state << " SEC "<< temp_mcp << " F DEGREES" <<std::endl;	
				gpioSetValue(M0_B, 0);
				gpioSetValue(M1_B, 0);
				gpioSetValue(M2_B, 0);
				gpioSetValue(M3_B, 0);
                                
				gpioSetValue(M0_F, 1);
				gpioSetValue(M1_F, 1);
                                gpioSetValue(M2_F, 0);
                                gpioSetValue(M3_F, 0);

				pca9685->setPWM(M0, 0, baseFreq);
                                pca9685->setPWM(M1, 0, baseFreq);
			}
			// State 2 - RIGHT
			if (nextState == 2)
			{
                                std::cout << "STATE: 2 " << "STATE SWITCH: " << t_delta << " LAST STATE " << state << " SEC "<< temp_mcp << " F DEGREES" <<std::endl;				
                                gpioSetValue(M0_B, 0);
                                gpioSetValue(M1_B, 0);
                                gpioSetValue(M2_B, 0);
                                gpioSetValue(M3_B, 0);

                                gpioSetValue(M0_F, 1);
                                gpioSetValue(M1_F, 0);
                                gpioSetValue(M2_F, 1);
                                gpioSetValue(M3_F, 0);

                                pca9685->setPWM(M0, 0, baseFreq);
                                pca9685->setPWM(M2, 0, baseFreq);			
			}
			// State 4 - LEFT
			if (nextState == 4)
			{
                                std::cout << "STATE: 4 " << "STATE SWITCH: " << t_delta << " LAST STATE " << state << " SEC "<< temp_mcp << " F DEGREES" <<std::endl;				
                                gpioSetValue(M0_B, 0);
                                gpioSetValue(M1_B, 0);
                                gpioSetValue(M2_B, 0);
                                gpioSetValue(M3_B, 0);

                                gpioSetValue(M0_F, 0);
                                gpioSetValue(M1_F, 1);
                                gpioSetValue(M2_F, 0);
                                gpioSetValue(M3_F, 1);

                                pca9685->setPWM(M1, 0, baseFreq);
                                pca9685->setPWM(M3, 0, baseFreq);			
			}
			// State 3 - BACK
			if (nextState == 3)
			{
				std::cout << "STATE: 3 " << "STATE SWITCH: " << t_delta << " LAST STATE" << state << " SEC "<< temp_mcp << " F DEGREES" <<std::endl;
                                gpioSetValue(M0_B, 0);
                                gpioSetValue(M1_B, 0);
                                gpioSetValue(M2_B, 1);
                                gpioSetValue(M3_B, 1);

                                gpioSetValue(M0_F, 0);
                                gpioSetValue(M1_F, 0);
                                gpioSetValue(M2_F, 0);
                                gpioSetValue(M3_F, 0);

                                pca9685->setPWM(M2, 0, baseFreq);
                                pca9685->setPWM(M3, 0, baseFreq);			
			}
			state = nextState;
		}
		if(state == nextState)
		{
			time_t currentTime;
			long int osTime;
			currentTime = time(0);
			osTime = static_cast<int> (currentTime);
			t_delta = osTime - oldtime;
			t_delta = (t_delta < 0) ? 0 : t_delta;
			t_delta = (t_delta > 10) ? 10 : t_delta; 
			int newFreq = baseFreq + (t_delta*60);
			
			/*if (state == 1)
			{
				pca9685->setPWM(M0, 0, newFreq);
                                pca9685->setPWM(M1, 0, newFreq);
                                pca9685->setPWM(M2, 0, newFreq);
                                pca9685->setPWM(M3, 0, newFreq);
			}
                        if (state == 2)
	                {
				pca9685->setPWM(M0, 0, newFreq);
		                pca9685->setPWM(M1, 0, 0);
                                pca9685->setPWM(M2, 0, newFreq);
                                pca9685->setPWM(M3, 0, 0);		
			}	
			if (state == 4)
                        {
                                pca9685->setPWM(M0, 0, 0);
                                pca9685->setPWM(M1, 0, newFreq);
                                pca9685->setPWM(M2, 0, 0);
				pca9685->setPWM(M3, 0, newFreq);
                        }
                        if (state == 3)
                        {
                                pca9685->setPWM(M0, 0, 0);
                                pca9685->setPWM(M1, 0, newFreq);
                                pca9685->setPWM(M2, 0, 0);
                                pca9685->setPWM(M3, 0, newFreq);
                        }	*/		
		}
	}

	return EXIT_SUCCESS;
}
string getFile(string filename)
{
	string buffer = "";
	while (buffer.size() == 0) 
	{
		char c;
		ifstream in(filename);   if (!in) { cout << filename << " not found";   exit( 1 ); }
		while (in.get(c)) buffer += c;
		in.close();
	}
	return buffer;
}

vector<string> getData(const string &text, string tag)
{ 
	vector<string> collection;
	unsigned int pos = 0, start;
	while (true)
	{
		start = text.find("<" + tag, pos);   if (start == string::npos) return collection;
		start = text.find(">" , start);
		start++;

		pos = text.find("</" + tag, start);   if (pos == string::npos) return collection;
		collection.push_back( text.substr(start, pos - start));
	}
}

void stripTags(string &text)
{
	unsigned int start = 0, pos;

	while (start < text.size())
	{
		start = text.find("<", start);    if (start == string::npos) break;
		pos   = text.find(">", start);    if (pos   == string::npos) break;
		text.erase(start, pos - start + 1);
	}
}
