#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include "tokenize.h"
#include "hashmap.cpp"
using namespace std;

void printWrd(HashMap<string, int>::MapEntry * arr, int size);
void printCsv(HashMap<string, int>::MapEntry * arr, int size);
int moveNulls(HashMap<string, int>::MapEntry * arr, int size);
void sort(HashMap<string, int>::MapEntry *& arr, int start, int end);
int qsorter(HashMap<string, int>::MapEntry *& arr, int start, int end);
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
    string in;
    vector<string> token;
    int totalWords = 0, shrink = 0, i;
    HashMap<string, int>::MapEntry *arr;
 
    while ( getline( file, in ) )
    {
        tokenize( in, token, VALID );
        
        for ( int i = 0; i < token.size(); i++ )
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
    //set equal to table entries
    arr = map.getEntries();
    //move empty spots to back and get good size value
    shrink = moveNulls(arr, map.capacity() );
    //sort that bish
    sort( arr, 0, shrink );
    cout << " ... Done." << endl;

    //write output to .wrd file
    //write output to .csv file
    
    //output to see how well it actually sorted
    //prints word frequency and array position
    i = 0;
    while ( arr[i].value != 0 )
    {
        cout << arr[i].value << "\tarr[" << i << ']' << endl;
        i++;
    }
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

void printWrd(HashMap<string, int>::MapEntry * arr, int size)
{
    //traverse sorted
}


void printCsv(HashMap<string, int>::MapEntry * arr, int size)
{
    //traverse sorted
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
    i = 0; 
    while ( arr[i].value != 0 )
        i++;
    //minus one for off by one array size
    return i - 1;
}

void sort(HashMap<string, int>::MapEntry *& arr, int start, int end)
{
    int middle;
    if ( start < end )
    {   //find middle point of current array section
        middle = qsorter(arr, start, end);
        //sort the front part
        sort( arr, start, middle );
        //sort the back part
        sort( arr, middle + 1, end );
    }
    //return
    return;
}

int qsorter(HashMap<string, int>::MapEntry *& arr, int start, int end)
{
    int x = arr[end].value;
    int i = end - 1, j = start + 1;

    //and something about a pivot point, that's probably why it doesn't work    
    do
    {
        do{
            j--;
        }while ( x > arr[j].value );
        do{
            i++;
        }while ( x < arr[i].value );
        if ( i < j )
           swap( arr[i], arr[j] );
    }while (i <= j );

    return j;
    //return i - 1 gives same output
}

