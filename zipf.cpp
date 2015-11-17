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


//Author: Brady Shimp
//Prints the corresponding .wrd and .csv files
//arr is a shrunken array of hash table entries, size is the number of unique words
//f0 is the name of the input file,
//f1 is the name for the wrd file,
//f2 is the name for the csv file
//total is the total number of words found
void printFiles ( HashMap<string, int>::MapEntry * arr, int size, string f0, string f1, string f2, int total );

//Author: Brady Shimp
//Shrinks an array of hashtable entries by moving the empty and deleted entries to the back
int moveNulls ( HashMap<string, int>::MapEntry * arr, int size );

//Author: Logan Lembke
//Sorts the shrank array of hashtable entries in descending order by frequency and alphabetical order
int qsorter ( const void * a, const void * b );

//Author: Logan Lembke
//Original: http://www.isthe.com/chongo/tech/comp/fnv/#FNV-1a
//Implements the FNV-1a hash function for strings
unsigned int stringHash ( string s );

//Used for parsing the words from the input file
const char* VALID = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz'";

/* Author: Brady Shimp, Logan Lembke
 Program starting point, command line arguments are checked for validity, if
 an incorrect amount are given, a usage statement is displayed to the terminal.
 If the input filename is given incorrectly, an error is displayed.  ctime is used
 to measure the total runtime, getting times before the first instance of the hashmap
 is created and after printing out file information.  The input file is processed line
 by line with a vector of c++ strings that is passed into tokenizing functions to remove
 any unwanted punctuation or symbols that would obscure the end result of the word counts.
 The processed vector is then processed to see if each word it contains has been inserted
 in the table yet or not. If it has, the frequency count of that word is incrimented,
 otherwise, the word is inserted in the table and given a frequency count of 1. After 
 processing each line, the vector is emptied before the next line is read.  After the entire
 input file is read through, processed and inserted, statistics about the hash table are printed
 and the data gets sorted by frequency and alphabetically within each frequency before being
 output to a .wrd file that can be used to view all unique words within their frequency ranges
 and a .csv file that can be ran through libre math and made into a graph that shows the 
 logarithmic slope described by Zipf's law on word frequency and rank relations. */  
int main ( int argc, char ** argv )
{
    // error check command args
    if ( argc != 2 )
    {
        cerr << "Usage: ./zipf filename" << endl;
        exit ( 1 );
    }
    //error check file name
    ifstream file ( argv[1], ios::in );

    if ( !file )
    {
        cerr << "ERROR: Could not open " << argv[1] << endl;
        exit ( 1 );
    }
    //start clocking for program runtime
    clock_t time = clock();

    //create hashmap initial size 1000
    //constructor auto increases the value to a prime number
    HashMap<string, int> map ( 1000, "", stringHash );
    string in, fileName = argv[1], wrd, csv;    //track file names and extensions
    vector<string> token;       //vector to process input file line by line
    int totalWords = 0, shrink = 0;

    //Will hold our copy of the map entries
    HashMap<string, int>::MapEntry *arr;

    cout << "Creating Hashmap of size 1009." << endl;
    //loop through file line by line
    while ( getline ( file, in ) )
    {
        //take out punctuations and unwanted characters
        tokenize ( in, token, VALID );
        processTokens ( token );
        
        for ( size_t i = 0; i < token.size(); i++ )
        {   //if already in map, incriment count
            if ( map.contains ( token[i] ) )
                map[token[i]] += 1;
            //if not in map insert
            else
                map.insert ( token[i], 1 );
            totalWords += 1;    //incriment total word count
        }
        //clear out vector so it will be empty for next line
        token.clear();
        //insert function handles when near full and auto resizes
    }

    //Output
    cout << "Final table size is " << map.capacity() << '.' << endl
         << "Read a total of " << totalWords << " from " << argv[1] << '.' << endl
         << "Inserted " << map.size() << " unique words into the table." << endl
         << "Sorting Table ...";

    //allocate copy array
    //we do this for style reasons. Generally we do not want to
    //break the exisiting HashMap. For instance, if someone
    //were to come back to this program later, they would
    //expect the hashmap to work.
    arr = new  HashMap<string, int>::MapEntry [map.capacity()];

    //copy values into new array
    for ( int i = 0; i < map.capacity(); i++ )
    {
        arr[i] = map.getEntries() [i];
    }

    //move empty spots to back and get good size value
    shrink = moveNulls ( arr, map.capacity() );

    //sort
    qsort ( arr, shrink, sizeof ( HashMap<string, int>::MapEntry ), qsorter );
    cout << " ... Done." << endl;
    //assign proper file names and call function to output the file contents
    wrd = fileName.replace ( fileName.end() - 3, fileName.end(), "wrd" );
    csv = fileName.replace ( fileName.end() - 3, fileName.end(), "csv" );
    printFiles ( arr, shrink, fileName, wrd, csv, totalWords );
    
    //end clocking for runtime
    time = clock() - time;
    //decimal precision of 1
    cout.precision ( 1 );
    cout << fixed << "Runtime: " << double ( time ) / CLOCKS_PER_SEC * 1000 << " msec" << endl;
    
    
    //close input file
    file.close();

    delete[] arr;
    return 0;
}


//FNV-1a hash
//http://www.isthe.com/chongo/tech/comp/fnv/#FNV-1a
unsigned int stringHash ( string s )
{
    unsigned int h = 2166136261;
    for ( unsigned char c : s )
    {
        h = h ^ c;
        h = h * 16777619;
    }
    return h;
}

void printFiles ( HashMap<string, int>::MapEntry * arr, int size, string f0, string f1, string f2, int total )
{
    //Open output streams
    ofstream wrd ( f1 );
    ofstream csv ( f2 );

    //Various counters for formatting
    int i = 0, j, t, x, start, end, num;

    float avg = 0.0;
    if ( !wrd )
    {
        cout << "Error writing .wrd file" << endl;
        exit ( 1 );
    }
    if ( !csv )
    {
        cout << "Error writing .csv file" << endl;
        exit ( 1 );
    }
    //wrd file header
    wrd << "\nZipf's Law" << endl
        << "----------" << endl
        << "File: " << f0 << endl
        << "Total Words: " << total << endl
        << "Total Unique Words: " << size << endl << endl
        << "Word Frequencies" << setw ( 20 ) << "Ranks" << setw ( 10 ) << "Avg Rank" << endl
        << "----------------" << setw ( 20 ) << "-----" << setw ( 10 ) << "--------";

    //csv file header
    csv << "\n    Zipf's Law" << endl
        << "    ----------" << endl
        << "    File: " << f0 << endl
        << "    Total Words: " << total << endl
        << "    Total Unique Words: " << size << endl << endl
        << "    Rank    Freq    Rank*Freq" << endl
        << "    ----    ----    ---------";
    //decimal output precisions set to 1
    wrd.precision ( 1 );
    wrd << fixed;
    csv.precision ( 1 );
    csv << fixed;
    //while looping through valid table contents
    while ( arr[i].value != 0 )
    {
        wrd << endl << endl << endl;
        csv << endl;
        //
        x = arr[i].value;
        j = 0;
        //loop up to word with different count value
        //count amount of words with same value
        do
        {
            i++;
            j++;
        }
        while ( x == arr[i].value );
        i = i - j;
        start = i + 1;
        end = start + j - 1;

        num = start;
        while ( end - num > 0 )
        {
            //i - 1 to preserve starting position
            start = i + 1 + ( start + end - num );
            num++;
        }
        //num = values summed between start and end
        num = start;
        //reset start value
        start = i + 1;
        avg = float ( num ) / ( end - start + 1 );

        csv << "    " << avg << ",    " << x << ",    " << avg*x;
        if ( x != 1 )
        {
            if ( start == end ) //only show start position for rank
                wrd << "Words occurring " << x << " times:" << setw ( 8 ) << start << setw ( 10 ) << avg;
            else        // start position -> end position for rank
                wrd << "Words occurring " << x << " times:" << setw ( 8 ) << start << '-' << end << setw ( 10 ) << avg;
        }
        if ( x == 1 )
        {
            if ( start == end )
                wrd << "Words occurring once:" << setw ( 8 ) << start << setw ( 10 ) << avg;
            else
                wrd << "Words occurring once:" << setw ( 8 ) << start << '-' << end << setw ( 10 ) << avg;
        }
        t = 0;
        //output line of -'s to format
        wrd << endl << setfill ( '-' ) << setw ( 60 ) << "" << endl;
        //offset j so it is 'j' greater than 'i' in order to loop
        j = j + ( i - 1 );
        while ( i <= j )
        {
            // 20 spaces between words for more readability
            wrd << arr[i].key << setw ( 19 - arr[i].key.size() ) << setfill ( ' ' ) << ' ';
            t++;
            // 4 words per line so long as there are more words to output
            if ( t % 4 == 0 && i != j )
                wrd << endl << setw ( 0 );
            i++;
        }
        //reset j
        j = j - ( i - 1 );
        //incriment i by j (i position + number of words with same value)
        i = i + j;
    }
    //close output files
    wrd.close();
    csv.close();
}

int moveNulls ( HashMap<string, int>::MapEntry * arr, int size )
{
    int begin = 0, i;
    //move all filled values to the front of the array
    //to ignore empty values for easier sorting
    for ( i = 0; i < size; i++ )
    {
        if ( arr[i].value != 0 && !arr[i].deleted )
        {
            //move good items to beginning portion of array
            swap ( arr[i], arr[begin] );
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

int qsorter ( const void * a, const void * b )
{
    HashMap<string, int>::MapEntry first = * ( HashMap<string, int>::MapEntry * ) a;
    HashMap<string, int>::MapEntry second = * ( HashMap<string, int>::MapEntry * ) b;

    //return positive if out order
    int value = second.value - first.value;
    if ( value == 0 )
    {
        return strcmp ( first.key.c_str(), second.key.c_str() );
    }
    return value;
}
