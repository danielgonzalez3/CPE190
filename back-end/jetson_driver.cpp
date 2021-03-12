/*Project Athena Driver
  Daniel Gonzalez */
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <csignal>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <ctime>

// old
#define M1P1 gpio7
#define M1P2 gpio15
#define M2P1 gpio29
#define M2P2 gpio31
#define M3P1 gpio32
#define M3P2 gpio33
#define M4P1 gpio19
#define M4P2 gpio21

using namespace std;
string getFile(string filename);                         // Reads whole file into a string buffer
string filename = "/var/www/html/CPE190/front-end/data.xml";
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
		sleep(1); //--> Temp Fix
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
				std::cout << "STATE: 3 " << "STATE SWITCH:: " << t_delta << " SEC" << std::endl;
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
			if (nextState == 1)
			{
				std::cout << "STATE 1" << std::endl;
			}
                        if (nextState == 2)
		        {
				std::cout << "STATE 2" << std::endl;
			}
			if (nextState == 4)
			{
				std::cout << "STATE 4" << std::endl;
			}
			if (nextState == 3)
			{
				std::cout << "STATE 3" << std::endl;
			}
		}
	}
}
string getFile(string filename)
{
	string buffer;
	char c;
	ifstream in(filename);   if (!in) { cout << filename << " not found";   exit( 1 ); }
	while (in.get(c)) buffer += c;
	in.close();
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

