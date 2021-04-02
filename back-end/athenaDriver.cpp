/*Project Athena Driver
  Daniel Gonzalez */

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
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

// Motor Variables ["01" -> Foward | "10" -> Backwards] 
extern const jetsonGPIONumber M1_0 = gpio7;
extern const jetsonGPIONumber M1_1 = gpio15;

extern const jetsonGPIONumber M2_0 = gpio29;
extern const jetsonGPIONumber M2_1 = gpio31;

extern const jetsonGPIONumber M3_0 = gpio32;
extern const jetsonGPIONumber M3_1 = gpio33;

extern const jetsonGPIONumber M4_0 = gpio19;
extern const jetsonGPIONumber M4_1 = gpio21;

using namespace std;
string getFile(string filename);                         
string filename  = "/var/www/html/CPE190/front-end/data.xml";
int state        = 0;
int nextState    = 0;
int oldtime      = 0;
int newtime      = 0;
int t_delta      = 0;
int t_pivot      = 0;
int baseFreq     = 1000;
int servoMin_20  = 700;
int servoMax_20  = 1200;
int servoMin_60  = 700;
int servoMax_60  = 1200;
int temp_mcp     = -1;
PCA9685 *pca9685 = new PCA9685();
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
        mcp9808->closemcp9808();
	gpioSetValue(M1_0, 0);
	gpioSetValue(M1_1, 0);
	gpioSetValue(M2_0, 0);
	gpioSetValue(M2_1, 0);
	gpioSetValue(M3_0, 0);
	gpioSetValue(M3_1, 0);
	gpioSetValue(M4_0, 0);
	gpioSetValue(M4_1, 0);
	gpioUnexport(M1_0);
	gpioUnexport(M1_1);
	gpioUnexport(M2_0);
	gpioUnexport(M2_1);
	gpioUnexport(M3_0);
	gpioUnexport(M3_1);
	gpioUnexport(M4_0);
	gpioUnexport(M4_1);
	exit(signum);
}
int map ( int x, int in_min, int in_max, int out_min, int out_max)
{
	return  (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min ;
}

int main(int argc, char **argv) 
{
	gpioExport(M1_0);
	gpioExport(M1_1);
	gpioExport(M2_0);
	gpioExport(M2_1);
	gpioExport(M3_0);
	gpioExport(M3_1);
	gpioExport(M4_0);
	gpioExport(M4_1);
	gpioSetDirection(M1_0, outputPin);
	gpioSetDirection(M1_1, outputPin);
	gpioSetDirection(M2_0, outputPin);
	gpioSetDirection(M2_1, outputPin);
	gpioSetDirection(M3_0, outputPin);
	gpioSetDirection(M3_1, outputPin);
	gpioSetDirection(M4_0, outputPin);
	gpioSetDirection(M4_1, outputPin);
	signal(SIGINT, signal_clean_up);
	if (getuid() != 0) 
	{
		fprintf(stderr, "Program is not started as \'root\' (sudo)\n");
		return -1;
	}

	int err = pca9685->openPCA9685();
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
		if(text.substr(0,2) == "-1")
		{
			std::cout << "Waiting for Input..." << std::endl;
		}else{
			break;
		}
		sleep(2);	
	}
        std::cout << "\nProject Athena Ready and Running...\n" << '\n' << std::endl;
	//pca9685->setPWMFrequency(60);
	while(1) 
	{
		usleep(10000); // Minor Delay
		string text = getFile(filename);
		int tmp = std::stoi (text.substr(89, 10));	
		//test here
		//vector<string> all = getData(text, tag);
		//end here
		//cout << text.at(66) << endl;
		//int tmp = std::stoi (text.substr(0,3),nullptr,0);	      
		//std::cout << tmp << '\n';
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
			

			// TO-DO Add Servo Movements, Set Servos to Stationary when Foward and Backward. +/-45 Degress for L/R
			// State 1 - FOWARD
			if (nextState == 1)
			{
				std::cout << "STATE: 1 " << "STATE SWITCH: " << t_delta << " SEC " << temp_mcp << " F DEGREES" <<std::endl;
				gpioSetValue(M1_0, 0);
				gpioSetValue(M1_1, 1);
				pca9685->setPWM(0, 0, baseFreq);
				gpioSetValue(M2_0, 0);
				gpioSetValue(M2_1, 1);
				pca9685->setPWM(1, 0, baseFreq);
				gpioSetValue(M3_0, 0);
				gpioSetValue(M3_1, 1);
				pca9685->setPWM(2, 0, baseFreq);
				gpioSetValue(M4_0, 0);
				gpioSetValue(M4_1, 1);
				pca9685->setPWM(3, 0, baseFreq);
                                pca9685->setPWM(4, 0, 0);
                                pca9685->setPWM(5, 0, 0);
				pca9685->setPWM(6, 0, 0);
                                pca9685->setPWM(7, 0, 0);
                                pca9685->setPWM(8, 0, 0);
                                pca9685->setPWM(9, 0, 0);
                                pca9685->setPWM(10, 0, 0);
                                pca9685->setPWM(11, 0, 0);
				pca9685->setPWM(12, 0, 0);
                                pca9685->setPWM(13, 0, 0);
                                pca9685->setPWM(14, 0, 0);
                                pca9685->setPWM(15, 0, 0);

			}
			// State 2 - RIGHT
			if (nextState == 2)
			{
				std::cout << "STATE: 2 " << "STATE SWITCH: " << t_delta << " SEC " << temp_mcp << " F DEGREES" <<std::endl;
				//std::cout << state << std::endl;
				if(state == 0 || 4) 
				{
					gpioSetValue(M1_0, 0);
					gpioSetValue(M1_1, 1);
					pca9685->setPWM(0, 0, baseFreq);
					gpioSetValue(M2_0, 0);
					gpioSetValue(M2_1, 1);
					pca9685->setPWM(1, 0, baseFreq);
					gpioSetValue(M3_0, 0);
					gpioSetValue(M3_1, 1);
					pca9685->setPWM(2, 0, baseFreq);
					gpioSetValue(M4_0, 0);
					gpioSetValue(M4_1, 1);
					pca9685->setPWM(3, 0, baseFreq);
					//pca9685->setPWMFrequency(60);
					sleep(4);
					//pca9685->setPWM(11, 0, 1200);
					pca9685->setPWM(11,0, 140);
					pca9685->setPWM(10,0, 492);
					
					//pca9685->setPWMFrequency(0);
                                        /*pca9685->setPWM(5, 0, 1500);
                                        pca9685->setPWM(6, 0, 1500);
                                        pca9685->setPWM(7, 0, 1500);
                                        pca9685->setPWM(8, 0, 1500);					
					pca9685->setPWM(9, 0, 1500);
					pca9685->setPWM(10, 0, 1500);
					pca9685->setPWM(11, 0, 1500);
					pca9685->setPWM(12, 0, 1500);
					pca9685->setPWM(13, 0, 1500);
				        pca9685->setPWM(10, 0, 300);
					pca9685->setPWM(11, 0, 300);
                                        pca9685->setPWM(12, 0, 300);
	                                pca9685->setPWM(13, 0, 300);
					pca9685->setPWM(14, 0, 300);
					pca9685->setPWM(15, 0, 300);*/                                    
				}
			}
			// State 4 - BACK
			if (nextState == 4)
			{
				std::cout << "STATE: 4 " << "STATE SWITCH: " << t_delta << " SEC " << temp_mcp << " F DEGREES" <<std::endl;
				gpioSetValue(M1_0, 1);
				gpioSetValue(M1_1, 0);
				pca9685->setPWM(0, 0, baseFreq);
				gpioSetValue(M2_0, 1);
				gpioSetValue(M2_1, 0);
				pca9685->setPWM(1, 0, baseFreq);
				gpioSetValue(M3_0, 1);
				gpioSetValue(M3_1, 0);
				pca9685->setPWM(2, 0, baseFreq);
				gpioSetValue(M4_0, 1);
				gpioSetValue(M4_1, 0);
				pca9685->setPWM(3, 0, baseFreq);

				pca9685->setPWM(4, 0, 0);
                                pca9685->setPWM(5, 0, 0);
				pca9685->setPWM(6, 0, 0);
                                pca9685->setPWM(7, 0, 0);
                                pca9685->setPWM(8, 0, 0);  
                                pca9685->setPWM(9, 0, 0);                                                              
                                pca9685->setPWM(10, 0, 0); 
                                pca9685->setPWM(11, 0, 0); 
                                pca9685->setPWM(12, 0, 0);
                                pca9685->setPWM(13, 0, 0);
				pca9685->setPWM(14, 0, 0);
                                pca9685->setPWM(15, 0, 0);				
			}
			// State 3 - LEFT
			if (nextState == 3)
			{
				std::cout << "STATE: 3 " << "STATE SWITCH: " << t_delta << " SEC "<< temp_mcp << " F DEGREES" <<std::endl;
				if (state == 0 || 4)
				{
					gpioSetValue(M1_0, 0);
					gpioSetValue(M1_1, 1);
					pca9685->setPWM(0, 0, baseFreq);
					gpioSetValue(M2_0, 0);
					gpioSetValue(M2_1, 1);
					pca9685->setPWM(1, 0, baseFreq);
					gpioSetValue(M3_0, 0);
					gpioSetValue(M3_1, 1);
					pca9685->setPWM(2, 0, baseFreq);
					gpioSetValue(M4_0, 0);
					gpioSetValue(M4_1, 1);
					pca9685->setPWM(3, 0, baseFreq);
				}
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
			int newFreq = baseFreq + (t_delta*180);
			//newFreq = 0;
			if (state == 1 || 2 || 3 || 4)
			{
				pca9685->setPWM(0, 0, newFreq);
				pca9685->setPWM(1, 0, newFreq);
				pca9685->setPWM(2, 0, newFreq);
				pca9685->setPWM(3, 0, newFreq);
			}

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
