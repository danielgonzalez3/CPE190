/*Project Athena Driver
  Daniel Gonzalez */

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include <sys/time.h>
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
#include <ctime>
#include <athenaPCA9685.h>
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
int baseFreq     = 1000;

int main(int argc, char **argv) 
{
	if (getuid() != 0) 
	{
		fprintf(stderr, "Program is not started as \'root\' (sudo)\n");
		return -1;
	}

	PCA9685 *pca9685 = new PCA9685();
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
			state = text.at(66) - '0';
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
			state = nextState;
			oldtime = newtime;
			/*FILE *tempFile;
			double T;
			tempFile = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
			fscanf (tempFile, "%lf", &T);
			T /= 1000;
			printf ("TEMPERATURE: %6.3f C\n", T);
			fclose(tempFile); */	
			// State 1
			if (nextState == 1)
			{
				std::cout << "STATE: 1 " << "STATE SWITCH: " << t_delta << " SEC" << std::endl;	
			}
			// State 2
			if (nextState == 2)
			{
				std::cout << "STATE: 2 " << "STATE SWITCH: " << t_delta << " SEC" << std::endl;
			}
			// State 4
			if (nextState == 4)
			{
				std::cout << "STATE: 4 " << "STATE SWITCH: " << t_delta << " SEC" << std::endl;
			}
			// State 3
			if (nextState == 3)
			{
				std::cout << "STATE: 3 " << "STATE SWITCH: " << t_delta << " SEC" << std::endl;
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
			int newFreq = baseFreq + (t_delta * 180);
		}
	}
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

