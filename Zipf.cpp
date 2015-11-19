/**********************************************************************
                            Zipf.cpp
***********************************************************************
			Authors: Alex Iverson & Katherine MacMillan
***********************************************************************
This Zipf program takes in a textfile name from the user through the
commandline and uses a hashtable to illustrate Zipf's Law.

The file is read in and all of the words are stored in a hashtable,
along with a count of how many times they appeard in the text. These
words and frequencies are then sorted, placing the most frequently
occuring words first.

The sorted words and frequencies are then written to two types of
files, one is a .wrd file, which lists the words, their frequencies,
and their ranks according to how often the appear.

The second file which is written is a .cvs file, which is a comma
separated file that can be imported into an excell spread sheet and
graphed to show the relationship between a word's frequency and its
rank, as described by Zipfs law.

***********************************************************************
Usage:		zipf filename
Example:	$ ./zipf example.txt
***********************************************************************
Issues/Bugs:
	none found *yet*
***********************************************************************
Formatting:
	1. Opperators are surrouned by white space
	2. Parameter list parenthesis have only internal white space
	3. Conditional statement parenthesis are surrounded by white space
	4. Typecast parethesis have only external white space
	5. Square brackets do not have whitespace
	6. Angle brakets do not have whitespace
	7. Comment slashes are followed by whitespace
	8. Comments are in line and the line above code blocks
	9. Curly bracers are in line with outter condition statement
**********************************************************************/

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <ctime>

#include "Tokenizer.h"
#include "Hashtable.h"

using namespace std;

/****Type Definition****/
typedef pair<int, std::string> tableEntry;

/****Global Variables****/
const char* VALID = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz'";

/****Function Declarations****/
int TableComparator( const void* a, const void* b );
void WriteFiles( char* origFile, tableEntry* freq, int nWords, int maxStrLen, int tabSize );
//void WriteWRD( char* origFile, tableEntry* freq, int nWords, int maxStrLen, int tabSize );
//void WriteCSV( char* origFile, tableEntry* freq, int nWords, int maxStrLen, int tabSize );

/**********************************************************************
                            main
***********************************************************************
Author: Katherine MacMillan
***********************************************************************
The main function creates an instance of the hashtable class, and then
uses the second commannd line argument to open a text file.
Once the file has been verified as opened, main reads in all of the
lines in the file, converting them to lowercase, and then parsing it
into individual word tokens.

The word tokens are stored in a vector of strings, and then as they are
inserted into the hashtable, they are counted to keep track of the
total number of words read in.

After all of the words are inserted into the hashtable all of the 
entries in the hashtable are copied into an array of integer-string
pairs. This array is then sorted, first by integer value and then
alphabetically.

After the list is sorted two files, a .wrd and a .cvs file are written
outputting the words and their frequency count.

The main function times the process of reading in, hashing, sorting,
and outputting the data. This time is displayed to the user before 
the program exits.

A return of 0 indicates that the program ran correctly.
A return of 1 indicates that an incorrect number of commandline 
	arguments was entered by the user.
A return of 2 indicates that the file indicated by the user could
	not be opened.
**********************************************************************/
int main( int argc, char** argv )
{

	Hashtable table;
	vector<string> tokens;
	string temp;

	// track longest string count and total word count
    int maxStrLen = 0;
    int wordCount = 0;

    // must be two arguments
    if ( argc != 2 )
    {
		cout << "Wrong number of arguments\nUsage: zipf <file.txt>" << endl;
		return 1;
    }

	// make sure file opens
    ifstream infile( argv[1] );
    if ( !infile )
	{
		cout << "Unable to open the file " << argv[1] << endl;
		return 2;
	}

	// time process
    clock_t t = clock();

	while ( !infile.eof() )
	{
		// read in line, make all characters lowercase and tokenize into words
		getline( infile, temp );
		transform( temp.begin(), temp.end(), temp.begin(), ::tolower );
		Tokenize( temp, tokens, VALID );
	}

	for ( auto t: tokens ) 
	{
		// insert each token, count total words, and check string length
		table.Insert( t );
		wordCount++;
		if ( t.length() > (unsigned) maxStrLen )
			maxStrLen = t.length();			
	}

	// retrieve total entry count and table size
	int count = table.GetEntryCount();
	int size = table.GetSize();

    // copy table data to array of  pairs
	tableEntry* wordList = new pair<int, string> [count+1];
    int j = 0;
    for ( int i = 0; i < size; i++ )
    {
    	int c = table.GetCount( i );
    	if ( c != 0 )
    	{
    		wordList[j].first = c;
    		wordList[j].second = table.GetKey( i );
    		j++;
    	}
    }

    // sort list and write data to file
    qsort( wordList, count, sizeof( tableEntry ), TableComparator );
    WriteFiles( argv[1],wordList, wordCount, maxStrLen, count );
	//WriteWRD( argv[1],wordList, wordCount, maxStrLen, count );
    //WriteCSV( argv[1],wordList, wordCount, maxStrLen, count );

    // display program runtime
    t = clock() - t;
    cout << "Time to read in, hash, sort and write out data: " << (float) t / CLOCKS_PER_SEC << " seconds" << endl;

    // clean heap-allocated memory
  	//delete[] wordList;

	return 0;
}

/**********************************************************************
                            TableComparator
***********************************************************************
Author: Alex Iverson
***********************************************************************
TableComparator takes two objects of the same type, in this case two 
tableEntry pointers which contain an integer in the first field and
a string in the second.

Objects are ordered based on the integers they hold. If both integers
are the same, then they are instead ordered based on the string values
they hold.

This function's purpose is to be used in conjunction with the standard
template library's qsort method.
**********************************************************************/
int TableComparator( const void* a, const void* b ) 
{
	// if integers are same, compare strings
	int result = ( (tableEntry *) b )->first - ( (tableEntry *) a )->first;
  	if ( !result ) 
  		result = ( (tableEntry *) a )->second.compare( ( (tableEntry *) b )->second );
  	return result;
}


/**********************************************************************
                            WriteFiles
***********************************************************************
Author: Alex Iverson
***********************************************************************
WriteFiles takes a sorted list of integer-string pairs and prints the
data to two separate files. Both files use the same name as the 
original input, but one is a .csv file and one is a .wrd file.

To create the .wrd file, the strings in the table are written out
according to their frequency group and then alphabetically within the
groups. It also calculates the ranks of each string, and displays the 
range of ranks contained within a frequency group, and the average
rank composed by the total of the averag of the ranks in the group.

To write the .csv file, the function writes out the average rank of 
each frequency group, along with the frequency and the product of the 
two. This file can then be imported into Excel to produce graphs
representing the data.
**********************************************************************/
void WriteFiles( char* origFile, tableEntry* freq, int nWords, int maxStrLen, int tabSize )
{  
	//determine output filenames
	char* wrdFname = strdup( origFile );
	strcpy ( wrdFname + strlen( wrdFname ) - 3, "wrd" );
	char* csvFname = strdup( origFile );
	strcpy( csvFname + strlen( csvFname ) - 3, "csv" );
	ofstream wrdout( wrdFname );
	ofstream csvout( csvFname );

	//print header of wrd file
	wrdout << "Zipf's Law: word concordance" << endl;
	wrdout << "----------------------------" << endl;
	wrdout << "File:            " << right << origFile << endl;
	wrdout << "Total words:     " << right << nWords << endl;
	wrdout << "Distinct words:  " << right << tabSize << endl;
	wrdout << endl;
	wrdout << "Word Frequencies                                               Ranks    Avg Rank" << endl;
	wrdout << "----------------                                               -----    --------" << endl;

	//print header of csv file
	csvout << "Zipf's Law: word concordance" << endl;
	csvout << "----------------------------" << endl;
	csvout << "File:            " << right << origFile << endl;
	csvout << "Total words:     " << right << nWords << endl;
	csvout << "Distinct words:  " << right << tabSize << endl;
	csvout << endl;
	csvout << "rank, freq, r*f" << endl;

	auto freqit = &freq[tabSize+1];
	auto freqend = &freq[tabSize + 1];
	int rank = 1;
	int nOccs = freq[0].first;
	int rankCount = 0;
	auto rankBegin = freq;

	//iterate over frequency entries detecting changes in number of occurences
	for ( freqit = freq; freqit < freqend; ++freqit ) 
	{
		if ( nOccs != freqit->first ) 
		{
			int colIdx = 0;
			float avgRank = rank + double (rankCount - 1) / 2;
	
			//output line of csv data
			csvout << avgRank << ", " << nOccs << ", " << avgRank*nOccs << endl;
	
			//handle pluralization
			string wordHeader;
			if ( nOccs > 1 )
				wordHeader = "Words occurring " + to_string( nOccs ) + " times:";
			else 
				wordHeader = "Words occurring once:";
			wrdout << wordHeader;
	
			//handle rank ranges
			if ( rankCount == 1 )
				wrdout << setw( 68 - wordHeader.length() ) << setfill( ' ' ) << right << rank;
			else 
			{
				string rankstring = to_string( rank ) + "-" + to_string( rank + rankCount - 1 );
				wrdout << string( 68 - wordHeader.length() - rankstring.length(), ' ' ) << rankstring;
			}
	
			//finish the heading for words of a given rank
			wrdout << setw( 12 ) << right << avgRank << endl;
			int nCols = 80 / maxStrLen;
	
			//write the words for a given rank category
			for ( auto rankit = rankBegin; rankit < freqit; ++rankit ) 
			{
				wrdout << setw( maxStrLen + 1 ) << setfill( ' ' ) << left << rankit->second;
				if ( ++colIdx == nCols ) 
				{
					colIdx = 0;
					wrdout << endl;
				}
			}
			wrdout << endl;
			wrdout << endl;
			
			//update ranking information
			rank += rankCount;
			rankCount = 0;
			nOccs = freqit->first;
			rankBegin = freqit;
		}
		++rankCount;
	}
	//clean heap-allocated memory
	delete[] freq;
	free( wrdFname );
	free( csvFname );
}
/**********************************************************************
                            WriteWRD
***********************************************************************
Author: Alex Iverson
***********************************************************************
WriteWRD takes a sorted list of integer-string pairs and prints the
strings and their counts first in frequency groups and alphabetically
within those groups.

It also calculates the ranks of each string, and displays the range
of ranks contained within a frequency group, as well as the average
rank composed by the total of the averag of the ranks in the group.

This data, along with a file header is written to a .wrd file with
the same name as the original text file from which the strings were
obtained.
**********************************************************************/
/*void WriteWRD ( char* origFile, tableEntry* freq, int nWords, int maxStrLen, int tabSize )
{

	// determine output filename
	char* wrdFname = strdup ( origFile );
  	strcpy ( wrdFname + strlen ( wrdFname ) - 3, "wrd" );
  	ofstream wrdout ( wrdFname );
  	
  	// print header of wrd file
	wrdout << "Zipf's Law: word concordance" << endl;
	wrdout << "----------------------------" << endl;
	wrdout << "File:            " << right << origFile << endl;
	wrdout << "Total words:     " << right << nWords << endl;
	wrdout << "Distinct words:  " << right << tabSize << "\n" << endl;
	wrdout << "Word Frequencies                                               Ranks    Avg Rank" << endl;
	wrdout << "----------------                                               -----    --------" << endl;

	// set up initial itterators, ranks and counts
	auto freqend = &freq[tabSize + 1];
	int rank = 1;
	int nOccs = freq[0].first;
	int rankCount = 0;
	auto rankBegin = freq;

	// itterate over frequency entries detecting changes in number of occurences
	for ( auto freqit = freq; freqit < freqend; ++freqit ) 
	{
		if ( nOccs != freqit->first ) 
		{
			int colIdx = 0;
			float avgRank = rank + double ( rankCount - 1 ) / 2;

			// handle pluralization
			string wordHeader;
			if ( nOccs > 1 )
				wordHeader = "Words occurring " + to_string ( nOccs ) + " times:";
      		else 
      			wordHeader = "Words occurring once:";
    		wrdout << wordHeader;

    		// handle rank ranges
	    	if ( rankCount == 1 ) 
				wrdout << setw ( 68 - wordHeader.length () ) << setfill ( ' ' ) << right << rank;
	      	else 
	      	{
				string rankstring = to_string ( rank ) + "-" + to_string ( rank + rankCount - 1 );
				wrdout << string ( 68 - wordHeader.length () - rankstring.length (), ' ' ) << rankstring;
	      	}

	      	// finish the heading for words of a given rank
	      	wrdout << setw ( 12 ) << right << avgRank << endl;
	      	int nCols = 80 / maxStrLen;

	      	// write the words for a given rank category
	      	for ( auto rankit = rankBegin; rankit < freqit; ++rankit ) 
	      	{
				wrdout << setw ( maxStrLen + 1 ) << setfill ( ' ' ) << left << rankit->second;
				if ( ++colIdx == nCols ) 
				{
	  				colIdx = 0;
	  				wrdout << endl;
				}
      		}

      		wrdout << endl;
      		wrdout << endl;

      		// update ranking information
   	   		rank += rankCount;
    	  	rankCount = 0;
      		nOccs = freqit->first;
	      	rankBegin = freqit;
		}
    	++rankCount;
	}

    // clean heap-allocated memory
  	free ( wrdFname );
}*/

/**********************************************************************
                            WriteCSV
***********************************************************************
Author: Alex Iverson
Edited: Katherine MacMillan
***********************************************************************
WriteCSV takes a sorted list of integer-string pairs and writes the
Zipf's law concordance information to a .csv file.

It writes out the average rank of each frequency group, along with the 
frequency and the product of the two to a .csv file with the same name 
as the orginal text file from which the strings were obtained.
**********************************************************************/
/*void WriteCSV ( char* origFile, tableEntry* freq, int nWords, int maxStrLen, int tabSize )
{
	// determine output filename
  	char* csvFname = strdup ( origFile );
  	strcpy ( csvFname + strlen ( csvFname ) - 3, "csv" );
  	ofstream csvout ( csvFname );

  	// print header of csv file
	csvout << "Zipf's Law: word concordance" << endl;
	csvout << "----------------------------" << endl;
	csvout << "File:            " << right << origFile << endl;
	csvout << "Total words:     " << right << nWords << endl;
	csvout << "Distinct words:  " << right << tabSize << "\n" << endl;
	csvout << "rank, freq, r*f" << endl;

	auto freqend = &freq[tabSize + 1];
	int rank = 1;
	int nOccs = freq[0].first;
	int rankCount = 0;

	// iterate over frequency entries detecting changes in number of occurences
	for ( auto freqit = freq; freqit < freqend; ++freqit ) 
	{
		if ( nOccs != freqit->first ) 
		{
			float avgRank = rank + double ( rankCount - 1 ) / 2;
			// output line of csv data
			csvout << avgRank << ", " << nOccs << ", " << avgRank*nOccs << endl;

			// update ranking information
   	   		rank += rankCount;
    	  	rankCount = 0;
      		nOccs = freqit->first;
		}
    	++rankCount;
	}

    // clean heap-allocated memory
  	free ( csvFname );
}*/