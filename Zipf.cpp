#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "Globals.h"

#include "Tokenizer.h"

#include "Hashtable.h"

using namespace std;

int main (int argc, char** argv)
{

	Hashtable* table = new Hashtable();
	vector<string> tokens;
	string temp;
    ifstream infile;

	infile.open("example.txt");
    
	while (!infile.eof())
	{
		getline (infile, temp);
		cout << temp << endl;
		Tokenize( temp, tokens, VALID);
		for (auto t: tokens) 
			table->Insert(t);
	}

    for (int i = 0; i < 1000; i++)
    {
    	if (table->getCount(i) != 0)
    		cout << "Index: "<< i <<"\tKey: "<<table->getKey(i)<<"\tCount: "<< table->getCount(i) << endl;
    }


	return 0;
}