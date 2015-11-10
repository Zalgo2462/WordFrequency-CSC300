#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

#include "tokenize.h"
#include "hashmap.cpp"
using namespace std;

void printWrd(HashMap<string, int>::MapEntry * sorted, int numEntries);
void printCsv(HashMap<string, int>::MapEntry * sorted, int numEntries);
void sort(HashMap<string, int>::MapEntry * unsorted);
int qsorter(const void * a, const void * b);
unsigned int stringHash(string s);

const char* VALID = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz'";

int main(int argc, char ** argv)
{
    if ( argc != 2 )
    {
        cerr << "Usage: ./zipf filename" << endl;
        exit(1);
    }
    ifstream input( argv[1] );
    if(!input)
    {
        cerr << "ERROR: Could not open " << argv[1] << endl;
        exit(1);
    }
    
    //create hashmap initial size 1000
    //constructor auto increases the value to a prime number
    HashMap<string, int> map( 1000, "", stringHash );
    string in;
    vector<string> token;
    int totalWords = 0;
 
    while ( !input.eof() )
    {
        getline( input, in );
        tokenize( in, token, VALID );
        for ( int i = 0; i < token.size(); i++ )
        {
            totalWords++;
            if( map.contains(token[i]) )
                map[token[i]] += 1;
            else
                map.insert( token[i], 1 );
        }
        //insert function handles when near full and auto resizes  
    }
    
    cout << "Final table size is " << map.capacity() << '.' << endl
    << "Read a total of " << totalWords << " from " << argv[1] << '.' << endl
    << "Inserted " << map.size() << " unique words into the table." << endl
    << "Sorting Table ...";
      
    //need to write qsort function
    
    cout << " ... Done." << endl;
    
    
        
    //write output to .wrd file
    //write output to .csv file
    
    //possibly use <chronos> for clocking?
    
    return 0;
}


//FNV-1a hash
//http://www.isthe.com/chongo/tech/comp/fnv/#FNV-1a
unsigned int stringHash(string s)
{
    unsigned int h = 2166136261;
    for (unsigned char c : s)
    {
        h = h ^ c;
        h = h * 16777619;
    }
    return h;
}

void printWrd(HashMap<string, int>::MapEntry * sorted, int numEntries)
{
    //need to find way to traverse
}


void printCsv(HashMap<string, int>::MapEntry * sorted, int numEntries)
{
    //traversal
}

void sort(HashMap<string, int>::MapEntry * unsorted)
{
    //traversal
}

int qsorter(const void * a, const void * b)
{


}
