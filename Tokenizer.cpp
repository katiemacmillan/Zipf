/*
            ***** tokenizer.cpp *****

String tokenizer, similar to strtok() - but for strings (and safer!).

Ref: http://oopweb.com/CPP/Documents/CPPHOWTO/Volume/C++Programming-HOWTO-7.html

Author: John M. Weiss, Ph.D.
Class:  CSC 300 Data Structures
Date:   Fall 2015

Modifications:
*/
#include "Tokenizer.h"

// string tokenizer that searches for symbols of a "valid chars" string

void Tokenize( const string& str, vector<string>& tokens, const string& valid = " " )
{
    // skip delimiters to start of first token
    int tokenStart = str.find_first_of( valid, 0 );
    // find next delimiter (i.e., end of first token)
    int tokenEnd = str.find_first_not_of( valid, tokenStart );

    // loop through input string
    while ( tokenStart != string::npos )
    {
        // found a token, add it to the vector
        tokens.push_back( str.substr( tokenStart, tokenEnd - tokenStart ) );
        // skip delimiters to start of next token
        tokenStart = str.find_first_of( valid, tokenEnd );
        // find next delimiter (end of token)
        tokenEnd = str.find_first_not_of( valid, tokenStart );
    }
}
