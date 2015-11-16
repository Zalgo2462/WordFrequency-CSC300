#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include <cstring>
#include <algorithm>
#include <ctime>
#include "tokenize.h"
#include "hashmap.cpp"
using namespace std;

void printFiles(HashMap<string, int>::MapEntry * arr, int size, string f1, string f2, int total);
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
    clock_t time = clock();
    //create hashmap initial size 1000
    //constructor auto increases the value to a prime number
    HashMap<string, int> map( 1000, "", stringHash );
    string in, fileName = argv[1], wrd, csv;
    vector<string> token;
    int totalWords = 0, shrink = 0;
    HashMap<string, int>::MapEntry *arr;
    
    cout << "Creating Hashmap of size 1009." << endl; 
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

    wrd = fileName.replace(fileName.end()-3, fileName.end(), "wrd" );
    csv = fileName.replace(fileName.end()-3, fileName.end(), "csv" );
    printFiles( arr, shrink, wrd, csv, totalWords );

    file.close();
    time = clock() - time;
    cout.precision(1);
    cout << fixed << "Runtime: " << double(time)/CLOCKS_PER_SEC * 1000 << " msec" << endl;
        
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

void printFiles(HashMap<string, int>::MapEntry * arr, int size, string f1, string f2, int total)
{
    ofstream wrd(f1);
    ofstream csv(f2);
    int i = 0, j, t, x, start, end, num;
    float avg = 0.0;
    if( !wrd )
    {
        cout << "Error writing .wrd file" << endl;
        exit (1);
    }
    if( !csv )
    {
        cout << "Error writing .csv file" << endl;
        exit (1);
    }
    
    f1.replace(f1.end()-3, f1.end(), "txt" );
    
    wrd << "\nZipf's Law"<< endl
         << "----------" << endl
         << "File: " << f1 << endl
         << "Total Words: " << total << endl
         << "Total Unique Words: " << size << endl << endl
         << "Word Frequencies" << setw(20) << "Ranks" << setw(10) << "Avg Rank" << endl
         << "----------------" << setw(20) << "-----" << setw(10) << "--------";
             
    
    csv << "\n    Zipf's Law"<< endl
         << "    ----------" << endl
         << "    File: " << f1 << endl
         << "    Total Words: " << total << endl
         << "    Total Unique Words: " << size << endl << endl
         << "    Rank    Freq    Rank*Freq" << endl
         << "    ----    ----    ---------";

    wrd.precision(1);
    wrd << fixed;
    csv.precision(1);
    csv << fixed;
while ( arr[i].value != 0 )
   {
        wrd << endl << endl << endl;
        csv << endl;
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
        
        num = start;
        while( end - num > 0 )
        {
            start = i + 1 + (start + end - num );
            num++;
        }
        num = start;
        start = i + 1;
        avg = float(num) / ( end - start + 1 );
        
        csv << "    " << avg << ",    " << x << ",    " << avg*x;
        if( x != 1 )
        {
            if( start == end )  //only show start position for rank
                wrd << "Words occurring "<< x << " times:"<< setw(8) << start << setw(10) << avg;
            else        // start position -> end position for rank
                wrd << "Words occurring "<< x << " times:" << setw(8) << start << '-' << end << setw(10) << avg;
        }
        if( x == 1 )
        {
            if( start == end )
                wrd << "Words occurring once:" << setw(8) << start << setw(10) << avg;
            else
                wrd << "Words occurring once:" << setw(8) << start << '-' << end << setw(10) << avg;
        }
        t = 0;
        wrd << endl << setfill('-') << setw(60) << "" << endl;
        j = j + (i - 1);
        while( i <= j )
        {   // 20 spaces between words for more readability
            wrd << arr[i].key << setw( 19 - arr[i].key.size() ) << setfill(' ') << ' '; 
            t++;
            // 4 words per line so long as there are more words to output
            if( t % 4 == 0 && i != j )
                wrd << endl << setw(0);
            i++;
        }
        j = j - (i - 1);
        i = i + j;
    }
    wrd.close();
    csv.close(); 
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
