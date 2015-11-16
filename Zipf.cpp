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
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include "Globals.h"

#include "Tokenizer.h"
#include "sort.hpp"
#include "Hashtable.h"

using namespace std;


typedef pair<int, std::string> tableEntry;
int Compare(const void* a, const void* b);
void WriteFiles (char* origFile, tableEntry* freq, int nWords, int maxStrLen, int tabSize );


int main (int argc, char** argv)
{

	Hashtable* table = new Hashtable();
	vector<string> tokens;
	string temp;
    ifstream infile;
    int maxStrLen = 0;
    int wordCount = 0;


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
			wordCount++;
			if ( t.length() > maxStrLen )
				maxStrLen = t.length();			
		}
	}

	int count = table->GetEntryCount();
	int size = table->GetSize();
	tableEntry* wordList = new pair<int, string>[count];
    int j = 0;
    for (int i = 0; i < size; i++)
    {
    	int k = table->GetCount(i);
    	if (k != 0)
    	{
    		wordList[j].first = k;
    		wordList[j].second = table->GetKey(i);
    		j++;
    	}
    }


    qsort(wordList, count, sizeof(tableEntry), Compare);

    for (int i = 0; i < count; i++)
    	cout << wordList[i].second << ": " << wordList[i].first << endl;
    WriteFiles(argv[1],wordList, wordCount, maxStrLen, size);



	return 0;
}

int Compare(const void* a, const void* b) 
{
  int result = ((tableEntry *) b)->first - ((tableEntry *) a)->first;
  if (!result) {
    result = ((tableEntry *) a)->second.compare(((tableEntry *) b)->second);
  }
  return result;
}


/**********************************************************************
                            WriteFiles
***********************************************************************
Author: Alex Iverson
***********************************************************************

**********************************************************************/
void WriteFiles (char* origFile, tableEntry* freq, int nWords, int maxStrLen, int tabSize )
{
	char* wrdFname = strdup(origFile);
  	strcpy(wrdFname + strlen(wrdFname) - 3, "wrd");
  	char* csvFname = strdup(origFile);
  	strcpy(csvFname + strlen(csvFname) - 3, "csv");
  	ofstream wrdout(wrdFname);
  	ofstream csvout(csvFname);
	wrdout << "Zipf's Law: word concordance" << endl;
	wrdout << "----------------------------" << endl;
	wrdout << "File:            " << right << origFile << endl;
	wrdout << "Total words:     " << right << nWords << endl;
	wrdout << "Distinct words:  " << right << tabSize << endl;
	wrdout << endl;
	wrdout << "Word Frequencies                                               Ranks    Avg Rank" << endl;
	wrdout << "----------------                                               -----    --------" << endl;
	csvout << "Zipf's Law: word concordance" << endl;
	csvout << "----------------------------" << endl;
	csvout << "File:            " << right << origFile << endl;
	csvout << "Total words:     " << right << nWords << endl;
	csvout << "Distinct words:  " << right << tabSize << endl;
	csvout << endl;
	csvout << "rank, freq, r*f" << endl;
	auto freqend = &freq[tabSize + 1];
	int rank = 1;
	int nOccs = freq[0].first;
	int rankCount = 0;
	auto rankBegin = freq;
	for (auto freqit = freq; freqit < freqend; ++freqit) 
	{
		if (nOccs != freqit->first) 
		{
			int colIdx = 0;
			float avgRank = rank + double(rankCount - 1) / 2;
			csvout << avgRank << ", " << nOccs << ", " << avgRank*nOccs << endl;
			string wordHeader;
			if ( nOccs > 1 )
				wordHeader = "Words occurring " + to_string(nOccs) + " times:";
      		else 
      			wordHeader = "Words occurring once:";
    		wrdout << wordHeader;
	    	if (rankCount == 1) 
	    	{
				wrdout << setw(68 - wordHeader.length()) << setfill(' ') << right << rank;
	      	} 
	      	else 
	      	{
				string rankstring = to_string(rank) + "-" + to_string(rank+rankCount-1);
				wrdout << string(68 - wordHeader.length() - rankstring.length(), ' ') << rankstring;
	      	}
	      	wrdout << setw(12) << right << avgRank << endl;
	      	int nCols = 80 / maxStrLen;
	      	for (auto rankit = rankBegin; rankit < freqit; ++rankit) 
	      	{

				wrdout << setw(maxStrLen+1) << setfill(' ') << left << rankit->second;
				if (++colIdx == nCols) 
				{
	  				colIdx = 0;
	  				wrdout << endl;
				}
      		}
      		wrdout << endl;
      		wrdout << endl;
   	   		rank += rankCount;
    	  	rankCount = 0;
      		nOccs = freqit->first;
	      	rankBegin = freqit;
		}
    	++rankCount;
	}
  	cout << "seg" << endl;
  	delete[] freq;
  	free(wrdFname);
  	free(csvFname);
}