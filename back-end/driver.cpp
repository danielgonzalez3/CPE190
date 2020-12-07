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

#include "bcm2835.h"
#include "pca9685servo.h"

// Motor 1
#define M1P1 RPI_BPLUS_GPIO_J8_36
#define M1P2 RPI_BPLUS_GPIO_J8_32
// Motor 2
#define M2P1 RPI_BPLUS_GPIO_J8_37
#define M2P2 RPI_BPLUS_GPIO_J8_33
// Motor 3
#define M3P1 RPI_BPLUS_GPIO_J8_13
#define M3P2 RPI_BPLUS_GPIO_J8_15
// Motor 4
#define M4P1 RPI_BPLUS_GPIO_J8_11
#define M4P2 RPI_BPLUS_GPIO_J8_07

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
   if (getuid() != 0) {
      fprintf(stderr, "Program is not started as \'root\' (sudo)\n");
      return -1;
   }
   if (bcm2835_init() != 1) {
      fprintf(stderr, "bcm2835_init() failed\n");
      return -2;
   }
   cout << "Project Athena Ready and Running ..." << '\n' <<endl;
   sleep(2);
   PCA9685 pca9685;
   pca9685.Dump();
   PCA9685Servo servo;
   servo.Dump();

   // MG90S Micro Servo Settings
   servo.SetLeftUs(700);
   servo.SetRightUs(2400);

   // Set Motor Controls
   bcm2835_gpio_fsel(M1P1, BCM2835_GPIO_FSEL_OUTP);
   bcm2835_gpio_fsel(M1P2, BCM2835_GPIO_FSEL_OUTP);
   bcm2835_gpio_fsel(M2P1, BCM2835_GPIO_FSEL_OUTP);
   bcm2835_gpio_fsel(M2P2, BCM2835_GPIO_FSEL_OUTP);
   bcm2835_gpio_fsel(M3P1, BCM2835_GPIO_FSEL_OUTP);
   bcm2835_gpio_fsel(M3P2, BCM2835_GPIO_FSEL_OUTP);
   bcm2835_gpio_fsel(M4P1, BCM2835_GPIO_FSEL_OUTP);
   bcm2835_gpio_fsel(M4P2, BCM2835_GPIO_FSEL_OUTP);
   bcm2835_gpio_write(M1P1, 0);
   bcm2835_gpio_write(M1P2, 0);
   bcm2835_gpio_write(M2P1, 0);
   bcm2835_gpio_write(M2P2, 0);
   bcm2835_gpio_write(M3P1, 0);
   bcm2835_gpio_write(M3P2, 0);
   bcm2835_gpio_write(M4P1, 0);
   bcm2835_gpio_write(M4P2, 0);	
   
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
	      
         //State 1
         if (state == 1)
         {
            cout << "STATE: 1 " << "DELTA: " << t_delta << endl;
            bcm2835_gpio_write(M1P1, 0);
	    bcm2835_gpio_write(M1P2, 1);
	    pca9685.Write(CHANNEL(0), VALUE(baseFreq));
		 
	    bcm2835_gpio_write(M2P1, 0);
	    bcm2835_gpio_write(M2P2, 1);
	    pca9685.Write(CHANNEL(1), VALUE(baseFreq));	 
		 
	    bcm2835_gpio_write(M3P1, 0);
	    bcm2835_gpio_write(M3P2, 1);
	    pca9685.Write(CHANNEL(2), VALUE(baseFreq));	 
		 
	    bcm2835_gpio_write(M4P1, 0);
	    bcm2835_gpio_write(M4P2, 1);
	    pca9685.Write(CHANNEL(3), VALUE(baseFreq));	 
		 
         }
         //State 2
         if (state == 2)
         {
	    cout << "STATE: 2 " << "DELTA: " << t_delta << endl;
	    bcm2835_gpio_write(M1P2, 0);
	    bcm2835_gpio_write(M1P1, 0); 
	    bcm2835_gpio_write(M2P2, 0);
	    bcm2835_gpio_write(M2P1, 0); 
	    bcm2835_gpio_write(M3P2, 0);
	    bcm2835_gpio_write(M3P1, 0);
	    bcm2835_gpio_write(M4P2, 0);
	    bcm2835_gpio_write(M4P1, 0);
	    servo.SetAngle(CHANNEL(4), ANGLE(0));
	    servo.SetAngle(CHANNEL(5), ANGLE(0));
	    servo.SetAngle(CHANNEL(6), ANGLE(0));
	    servo.SetAngle(CHANNEL(7), ANGLE(0));
	    sleep(1);
	    servo.SetAngle(CHANNEL(4), ANGLE(90));
	    servo.SetAngle(CHANNEL(5), ANGLE(90));
	    servo.SetAngle(CHANNEL(6), ANGLE(90));
            servo.SetAngle(CHANNEL(7), ANGLE(90));
	 
         }
         //State 4
         if (state == 4)
         {
	    cout << "STATE: 4 " << "DELTA: " << t_delta << endl;
	    bcm2835_gpio_write(M1P2, 0);
	    bcm2835_gpio_write(M1P1, 0); 
	    bcm2835_gpio_write(M2P2, 0);
	    bcm2835_gpio_write(M2P1, 0); 
	    bcm2835_gpio_write(M3P2, 0);
	    bcm2835_gpio_write(M3P1, 0);
	    bcm2835_gpio_write(M4P2, 0);
	    bcm2835_gpio_write(M4P1, 0);
	    servo.SetAngle(CHANNEL(4), ANGLE(0));
	    servo.SetAngle(CHANNEL(5), ANGLE(0));
	    servo.SetAngle(CHANNEL(6), ANGLE(0));
	    servo.SetAngle(CHANNEL(7), ANGLE(0));
	    sleep(1);
	    servo.SetAngle(CHANNEL(4), ANGLE(90));
	    servo.SetAngle(CHANNEL(5), ANGLE(90));
	    servo.SetAngle(CHANNEL(6), ANGLE(90));
            servo.SetAngle(CHANNEL(7), ANGLE(90));
		 
         }
         //State 3
         if (state == 3)
         {
	    cout << "STATE: 3 " << "DELTA: " << t_delta << endl;
	    bcm2835_gpio_write(M1P1, 1);
	    bcm2835_gpio_write(M1P2, 0);
	    pca9685.Write(CHANNEL(0), VALUE(baseFreq));
		 
	    bcm2835_gpio_write(M2P1, 1);
	    bcm2835_gpio_write(M2P2, 0);
	    pca9685.Write(CHANNEL(1), VALUE(baseFreq));	 
		 
	    bcm2835_gpio_write(M3P1, 1);
	    bcm2835_gpio_write(M3P2, 0);
	    pca9685.Write(CHANNEL(2), VALUE(baseFreq));	 
		 
	    bcm2835_gpio_write(M4P1, 1);
	    bcm2835_gpio_write(M4P2, 0);
	    pca9685.Write(CHANNEL(3), VALUE(baseFreq));	 
         }
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
	    pca9685.Write(CHANNEL(0), VALUE(newFreq));
	    pca9685.Write(CHANNEL(1), VALUE(newFreq));
	    pca9685.Write(CHANNEL(2), VALUE(newFreq));
	    pca9685.Write(CHANNEL(3), VALUE(newFreq));
         }
         if (state == 3)
	 {
            pca9685.Write(CHANNEL(0), VALUE(newFreq));
	    pca9685.Write(CHANNEL(1), VALUE(newFreq));
	    pca9685.Write(CHANNEL(2), VALUE(newFreq));
	    pca9685.Write(CHANNEL(3), VALUE(newFreq));	 
	 }
      }
   }
}

string getFile(string filename)
{
   string buffer;
   char c;

   ifstream in( filename );   if ( !in ) { cout << filename << " not found";   exit( 1 ); }
   while ( in.get( c ) ) buffer += c;
   in.close();

   return buffer;
}

vector<string> getData(const string &text, string tag)
{ 
   vector<string> collection;
   unsigned int pos = 0, start;

   while (true)
   {
      start = text.find( "<" + tag, pos );   if ( start == string::npos ) return collection;
      start = text.find( ">" , start );
      start++;

      pos = text.find( "</" + tag, start );   if ( pos == string::npos ) return collection;
      collection.push_back( text.substr( start, pos - start ) );
   }
}

void stripTags(string &text)
{
   unsigned int start = 0, pos;

   while (start < text.size())
   {
      start = text.find("<", start);    if (start == string::npos) break;
      pos   = text.find(">", start);    if (pos   == string::npos) break;
      text.erase( start, pos - start + 1 );
   }
}



