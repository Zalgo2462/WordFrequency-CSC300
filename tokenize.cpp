/*
            ***** tokenizer.cpp *****

String tokenizer, similar to strtok() - but for strings (and safer!).

Ref: http://oopweb.com/CPP/Documents/CPPHOWTO/Volume/C++Programming-HOWTO-7.html

Author: John M. Weiss, Ph.D.
Class:  CSC 300 Data Structures
Date:   Fall 2015

Modifications:
Logan Lembke: Split into cpp and header
Brady Shimp: Removed the tokenize1 function and DELIMETERS
*/

#include "tokenize.h"

using namespace std;

// string tokenizer that searches for symbols of a "valid chars" string
void tokenize ( const string& str, vector<string>& tokens, const string& valid )
{
    // skip delimiters to start of first token
    size_t tokenStart = str.find_first_of ( valid, 0 );
    // find next delimiter (i.e., end of first token)
    size_t tokenEnd = str.find_first_not_of ( valid, tokenStart );

    // loop through input string
    while ( tokenStart != string::npos )
    {
        // found a token, add it to the vector
        tokens.push_back ( str.substr ( tokenStart, tokenEnd - tokenStart ) );
        // skip delimiters to start of next token
        tokenStart = str.find_first_of ( valid, tokenEnd );
        // find next delimiter (end of token)
        tokenEnd = str.find_first_not_of ( valid, tokenStart );
    }

}

void processTokens ( vector<string> & tokens )
{
    size_t size = tokens.size();
    for ( size_t i = 0; i < tokens.size(); i++ )
    {
        for ( size_t j = 0; j < tokens[i].size(); j++ )
            tokens[i][j] = tolower ( tokens[i][j] );

        //remove apostrophes from beginnings of words
        while ( tokens[i].front() == '\'' )
        {
            tokens[i].erase ( tokens[i].begin() );
        }
        //remove apostrophes from ends of words
        while ( tokens[i].back() == '\'' )
        {
            tokens[i].erase ( tokens[i].end() - 1 );
        }

        if ( tokens[i].size() == 0 )
        {
            tokens.erase ( tokens.begin() + i );
            size--;
            i--;
        }
    }
}
