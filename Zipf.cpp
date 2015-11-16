/**********************************************************************
                            Zipf.cpp
***********************************************************************
			Authors: Alex Iverson & Katherine MacMillan
***********************************************************************

***********************************************************************
Usage:

***********************************************************************
Issues/Bugs:

***********************************************************************
Formatting:

**********************************************************************/

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include "Globals.h"

#include "Tokenizer.h"
#include "sort.hpp"
#include "Hashtable.h"

using namespace std;



void Swap (pair<int, string>& leftElement, pair<int, string>& rightElement)
{
	pair<int, string> temp = leftElement;
	leftElement = rightElement;
	rightElement = temp;
}


int main (int argc, char** argv)
{

	Hashtable* table = new Hashtable();
	vector<string> tokens;
	string temp;
    ifstream infile;
    int max = 0;

    if ( argc != 2 )
    {
		cout << "Wrong number of arguments\nUsage: zipf <file.txt>" << endl;
		return 1;
    }

	infile.open(argv[1]);
	if( !infile )
	{
		cout << "Unable to open the file " << argv[1] << endl;
		return 2;
	}
    
	while ( !infile.eof() )
	{
		getline ( infile, temp );
		cout << temp << endl;
		Tokenize( temp, tokens, VALID );

		for ( auto t: tokens ) 
		{
			transform(t.begin(), t.end(), t.begin(), ::tolower);
			table->Insert(t);
			if ( t.length() > max )
				max = t.length();			
		}
	}

	int count = table->GetEntryCount();
	int size = table->GetSize();
	pair<int, string>* wordList = new pair<int, string>[size];
    int j = 0;


    for ( int i = 0; i < size; i++ )
    {
    	if ( table->GetCount(i) != 0 )
    	{
    		wordList[j].first = table->GetCount(i);
    		wordList[j].second = table->GetKey(i);
    		j++;
    	}
    }


    for ( int i = 0; i < count; i++ )
    	cout << "wordList[" << i << "]... " << wordList[i].second <<"\t\t" << wordList[i].first << endl;

    cout << table->GetIndex("i'm") << endl;
    //make array of pairs

    cout << "items: " << table->GetEntryCount() << endl;
    cout << "bunyip: " << table->GetIndex("bunyip") << endl;



	return 0;
}

