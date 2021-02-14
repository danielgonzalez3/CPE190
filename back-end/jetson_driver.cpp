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
//#define M4P1 RPI_BPLUS_GPIO_J8_07
//#define M4P2 RPI_BPLUS_GPIO_J8_11

using namespace std;
string getFile(string filename);                         // Reads whole file into a string buffer
vector<string> getData(const string &text, string tag);  // Gets collection of items between given tags
void stripTags(string &text); 
int state        = 0;
int nextState    = 0;
int oldtime = 0;
int newtime = 0;
int t_delta = 0;
int baseFreq = 1000;

int main(int argc, char **argv) 
{
	if (getuid() != 0) 
	{
		fprintf(stderr, "Program is not started as \'root\' (sudo)\n");
		return -1;
	}
    cout << "\nProject Athena Ready and Running ...\n" << '\n' <<endl;
    sleep(2);

    while(1) 
    {
        string filename = "/var/www/html/CPE190/front-end/data.xml";
        string tag = "controlState";
        bool stripOtherTags = true;
        string text = getFile(filename);
        vector<string> all = getData(text, tag);
        for (string &s : all) 
        {
        	if (stripOtherTags) stripTags(s);
	 		int tmp = std::stoi (s.substr(3,12),nullptr,0);	      
         	//cout << s << '\n';
         	if (state == 0)
	 		{
	    		cout << "States Set" << endl;
	    	    state = s.at(2) - '0';
	    		nextState = s.at(2) - '0';
            	oldtime = tmp;
         	}else{
	    		nextState = s.at(2) - '0';
         	}
         	newtime = tmp;
      	}
      	if(state != nextState)
      	{
	 		t_delta = newtime - oldtime;
         	state = nextState;
         	oldtime = newtime;
	 		FILE *tempFile;
         	double T;
         	tempFile = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
	 		fscanf (tempFile, "%lf", &T);
	 		T /= 1000;
         	printf ("TEMPERATURE: %6.3f C\n", T);
         	fclose(tempFile);     
         	// State 1
         	if (state == 1)
         	{
 	    		cout << "STATE: 1 " << "DELTA: " << t_delta << " SEC" << endl;        
	 		// State 2
         	if (state == 2)
         	{
	    		cout << "STATE: 2 " << "DELTA: " << t_delta << " SEC" << endl;
         	}
         	// State 4
         	if (state == 4)
         	{
	    		cout << "STATE: 4 " << "DELTA: " << t_delta << " SEC" << endl;
			// State 3
         	if (state == 3)
         	{
	    		cout << "STATE: 3 " << "DELTA: " << t_delta << " SEC" << endl;
	 		}
      		if(state == nextState)
      		{
				time_t currentTime;
         		long int osTime;
         		currentTime = time(0);
         		osTime = static_cast<int> (currentTime);
			
		        t_delta = osTime - oldtime;
	 			//cout << t_delta << endl;
         		t_delta = (t_delta < 0) ? 0 : t_delta;
	 			t_delta = (t_delta > 10) ? 10 : t_delta;
	 			int newFreq = baseFreq + (t_delta * 180);
         		//cout << newFreq << endl;
         		if (state == 1)
			 	{
			  	    cout << "state 1" <<endl;
			 	}
				if (state == 3)
	            {
	            	cout << "state 3" <<endl;
	 			}
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



