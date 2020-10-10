/*Project Athena Driver
Daniel Gonzalez
10/10/2020 */
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
using namespace std;
string getFile( string filename );                         // Reads whole file into a string buffer
vector<string> getData( const string &text, string tag );  // Gets collection of items between given tags
void stripTags( string &text ); 

int main() 
{
   string filename = "/var/www/html/CPE190/front-end/data.xml";
   string tag = "controlState";
   bool stripOtherTags = true;
   string text = getFile( filename );
   vector<string> all = getData( text, tag );
   for ( string &s : all ) 
   {
      if ( stripOtherTags ) stripTags( s );
      cout << s << '\n';
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

void stripTags( string &text )
{
   unsigned int start = 0, pos;

   while ( start < text.size() )
   {
      start = text.find( "<", start );    if ( start == string::npos ) break;
      pos   = text.find( ">", start );    if ( pos   == string::npos ) break;
      text.erase( start, pos - start + 1 );
   }
}



