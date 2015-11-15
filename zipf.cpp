#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include "tokenize.h"
#include "hashmap.cpp"
using namespace std;

void printWrd(HashMap<string, int>::MapEntry * arr, int size, string name, int total);
void printCsv(HashMap<string, int>::MapEntry * arr, int size, string name, int total);
int moveNulls(HashMap<string, int>::MapEntry * arr, int size);
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
    ifstream file( argv[1], ios::in );
    if(!file)
    {
        cerr << "ERROR: Could not open " << argv[1] << endl;
        exit(1);
    }

    //create hashmap initial size 1000
    //constructor auto increases the value to a prime number
    HashMap<string, int> map( 1000, "", stringHash );
    string in, fileName = argv[1];
    vector<string> token;
    int totalWords = 0, shrink = 0;
    HashMap<string, int>::MapEntry *arr;
    
    
    while ( getline( file, in ) )
    {
        tokenize( in, token, VALID );
        //move to lower and quote handling here
        processTokens(token);

        for ( size_t i = 0; i < token.size(); i++ )
        {
            if( map.contains(token[i]) )
                map[token[i]] += 1;
            else
                map.insert( token[i], 1 );
            totalWords += 1;
        }

        token.clear();
        //insert function handles when near full and auto resizes
    }

    //say words
    cout << "Final table size is " << map.capacity() << '.' << endl
    << "Read a total of " << totalWords << " from " << argv[1] << '.' << endl
    << "Inserted " << map.size() << " unique words into the table." << endl
    << "Sorting Table ...";

    //allocate array
    arr = new  HashMap<string, int>::MapEntry [map.capacity()];

    //copy values into new array
    for (int i = 0; i < map.capacity(); i++)
    {
	       arr[i] = map.getEntries()[i];
    }

    //move empty spots to back and get good size value
    shrink = moveNulls(arr, map.capacity() );

    //sort that bish
    qsort( arr, shrink, sizeof(HashMap<string, int>::MapEntry), qsorter);
    cout << " ... Done." << endl;

    fileName.replace(fileName.end()-3, fileName.end(), "wrd" );
    printWrd( arr, shrink, fileName, totalWords );
    fileName.replace(fileName.end()-3, fileName.end(), "csv" );
    printCsv( arr, shrink, fileName, totalWords );

    //possibly use <chronos> for clocking?
    file.close();
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

void printWrd(HashMap<string, int>::MapEntry * arr, int size, string name, int total)
{
    ofstream file(name);
    int i = 0, j, t, x, start, end;
    float avg = 0.0;
    if( !file )
    {
        cout << "Error writing .wrd file" << endl;
        exit (1);
    }
    name.replace(name.end()-3, name.end(), "txt" );
    
    file << "\nZipf's Law"<< endl
         << "----------" << endl
         << "File: " << name << endl
         << "Total Words: " << total << endl
         << "Total Unique Words: " << size << endl << endl
         << "Word Frequencies\t\t\t\t\t\tRanks\tAvg Rank" << endl
         << "----------------\t\t\t\t\t\t-----\t--------";
         

    while ( arr[i].value != 0 )
   {
        file << endl << endl;
        x = arr[i].value;
        j = 0;
        do
        {
            i++;
            j++;
        }while( x == arr[i].value );
        i = i - j;
        start = i + 1;
        end = start + j - 1;
        avg = (start + end) / (j + 1);
        if( x != 1 )
            file << "Words occurring "<< x << " times:\t\t\t\t\t" << start << "\t\t" << avg;
        if( x == 1 )
            file << "Words occurring once:\t\t\t\t\t\t" << i + 1 << "\t\t" << (i + j) / j;
        
        t = 0;
        file << endl;
        j = j + (i - 1);
        while( i <= j )
        {
            file << arr[i].key << "\t";
            t++;
            if( t % 4 == 0 )
                file << endl;
            i++;
        }
        j = j - (i - 1);
        i = i + j;
    }
    file.close();
}


void printCsv(HashMap<string, int>::MapEntry * arr, int size, string name, int total)
{
    ofstream file(name);
    int i = 0;
    
    if( !file )
    {
        cout << "Error writing .csv file" << endl;
        exit (1);
    }

    while ( arr[i].value != 0 )
    {
        //cout << arr[i].value << "\tarr[" << i << ']' << "\t" << arr[i].key << endl;
        i++;
    }
    file.close();
}

int moveNulls(HashMap<string, int>::MapEntry * arr, int size)
{
    int begin = 0, i;
    //move all filled values to the front of the array
    //to ignore empty values for easier sorting
    for ( i = 0; i < size; i++ )
    {
        if ( arr[i].value != 0 )
        {   //move good items to beginning portion of array
            swap(arr[i], arr[begin]);
            //incriment beginning portion so items aren't just
            //swapped back and forth
            begin++;
        }
    }
    //traverse valid list items for good size
    //get size of contiguous good values
    i = 0;
    while ( arr[i].value != 0 )
        i++;

    //undo last addition to i so it is the number of elements
    return i;
}

int qsorter(const void * a, const void * b)
{
    HashMap<string, int>::MapEntry first = *(HashMap<string, int>::MapEntry *) a;
    HashMap<string, int>::MapEntry second = *(HashMap<string, int>::MapEntry *) b;

    //return positive if out order
    int value = second.value - first.value;
    if (value == 0)
    {
	       return strcmp(first.key.c_str(), second.key.c_str());
    }
    return value;
}
