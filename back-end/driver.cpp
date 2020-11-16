/*Project Athena Driver
Daniel Gonzalez
10/10/2020 */
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <wiringPi.h>
#include <csignal>

using namespace std;
string getFile(string filename);                         // Reads whole file into a string buffer
vector<string> getData(const string &text, string tag);  // Gets collection of items between given tags
void stripTags(string &text); 
int state        = 0;
int nextState    = 0;
int S1_PIN = 3;
int S2_PIN = 2;
int S3_PIN = 0;
int S4_PIN = 1;

int main()
{
   wiringPiSetup();
   cout << "Project Athena Ready and Running ..." << '\n' <<endl;
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
         //cout << s << '\n';
         if (state == 0)
	 {
	    state = s.at(2) - '0';
	    nextState = s.at(2) - '0';
         }else{
	    nextState = s.at(2) - '0';
         }
      }
      if(state != nextState)
      {
         state = nextState;
         //State 1
         if (state == 1)
         {
            cout << "STATE 1 PIN:" << S1_PIN  << endl;
       	    pinMode(S1_PIN, OUTPUT); //Phyiscal pin 15
            digitalWrite(S1_PIN, 1); //Turn on to 3.3v
            delay(1000);        //ms
	    digitalWrite(S1_PIN, 0);
	    delay(3000);
         }
         //State 2
         if (state == 2)
         {
	    cout << "STATE 2 PIN:" << S2_PIN << endl;
	    pinMode(S2_PIN, OUTPUT); //Physical pin 13
	    digitalWrite(S2_PIN, 1); //Turn on to 3.3v
	    delay(1000);	    //ms
	    digitalWrite(S2_PIN, 0);
	    delay(3000);
         }

         //State 3
         if (state == 3)
         {
	    cout << "STATE 3 PIN:" << S3_PIN << endl;
            pinMode(S3_PIN, OUTPUT); //Phyiscal pin 11
            digitalWrite(S3_PIN, 1); //Turn on to 3.3v
            delay(1000);        //ms 
            digitalWrite(S3_PIN, 0);
            delay(3000);
         }
         //State 4
         if (state == 4)
         {
	    cout << "STATE 4 PIN:" << S4_PIN <<endl;
	    pinMode(S4_PIN, OUTPUT); //Phyiscal pin 12
	    digitalWrite(S4_PIN, 1); //Turn on to 3.3v
	    delay(1000);        //ms
            digitalWrite(S4_PIN, 0);
 	    delay(3000);
         }
      }
   }
}

string getFile( string filename )
{
   string buffer;
   char c;

   ifstream in( filename );   if ( !in ) { cout << filename << " not found";   exit( 1 ); }
   while ( in.get( c ) ) buffer += c;
   in.close();

   return buffer;
}

vector<string> getData( const string &text, string tag )
{ 
   vector<string> collection;
   unsigned int pos = 0, start;

   while ( true )
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



