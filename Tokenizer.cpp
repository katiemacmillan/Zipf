/**********************************************************************
                        Tokenizer.cpp
**********************************************************************

String tokenizer, similar to strtok() - but for strings (and safer!).

Ref: http://oopweb.com/CPP/Documents/CPPHOWTO/Volume/C++Programming-HOWTO-7.html

Author: John M. Weiss, Ph.D.
Class:  CSC 300 Data Structures
Date:   Fall 2015

Modifications: Utilized only tokenize2 from tokenize.cpp, and modified
to include single quotes, but only as embeded characters.
**********************************************************************/
#include "Tokenizer.h"

/**********************************************************************
                        Tokenize
***********************************************************************
Tokenize reads a string of characters and looks for unbroken
collections of predefined valid characters. Each time a new collection 
of characters is found it is considered a word, and is stored in a
vector called 'tokens'. The only characters considered valid to form
words are capital and lowercase letters and single quote characters.

However, the only time we want to include single quote characters is
when they're embedded in a word. Therefore, when the first valid
character in a collection is found, it is checked to see if it is a
single quote character. If it is, the itterator is moved forward one
position.
Likewise, when the last valid character in a collection is found it
is checked to see if it is a single quote character. If it is, the
itterator for the end of the word is moved back one position.
Both of these actions will exclude single quote characters that appear
at the begining or end of a word, but any single quotes within the
word will still be considered valid.
***********************************************************************/
void tokenize( const string& str, vector<string>& tokens, const string& valid = " " )
{
    // skip delimiters to start of first token
    int tokenStart = str.find_first_of( valid, 0 );
    // find next delimiter (i.e., end of first token)
    int tokenEnd = str.find_first_not_of( valid, tokenStart );

    // loop through input string
    while ( (unsigned)tokenStart != string::npos )
    {
        if ( str[tokenStart] == '\'')
            tokenStart++;
        if ( str[tokenEnd] == '\'')
            tokenEnd--;
        // found a token, add it to the vector
        tokens.push_back( str.substr( tokenStart, tokenEnd - tokenStart ) );
        // skip delimiters to start of next token
        tokenStart = str.find_first_of( valid, tokenEnd );
        // find next delimiter (end of token)
        tokenEnd = str.find_first_not_of( valid, tokenStart );
    }
}
