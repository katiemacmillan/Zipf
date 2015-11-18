/*
zipf.cpp
Main program for programming assignment 3 of CSC300
This program tests Zipf's law by counting occurences of words in text and writing statistics about their rankings and frequency.

Author: Alex Iverson
*/

#include "hashtable.hpp"
#include "tokenizer.hpp"
#include "sort.hpp"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string.h>
#include <time.h>

using namespace std;

//comparator function for tableEntries to be used with qsort
int table_comparator(const void* a, const void* b) {
  int result = ((tableEntry *) b)->first - ((tableEntry *) a)->first;
  if (!result) {
    result = ((tableEntry *) a)->second.compare(((tableEntry *) b)->second);
  }
  return result;
}

int main(int argc, char** argv) {
  //store start time
  clock_t t = clock();
  Hashtable tab;
  if (argc < 2) {
    cout << "requires filename argument" << endl;
    cout << "expected usage:" << endl;
    cout << "  zipf filename.txt" <<endl;
    cout << "produces filename.wrd and filename.csv" << endl;
    return 0;
  }
  ifstream file(argv[1]);
  Tokenizer tok(file);
  int maxStrLen = 0;
  int nWords = 0;
  //count words in source file
  for (string s = tok.Next(); s != ""; s = tok.Next()) {
    int len = s.length();
    if (len > maxStrLen) {
      maxStrLen = len;
    }
    ++nWords;
    tab.Increment(s);
  }
  //copy frequency pairs to a new array
  tableEntry* freq = new tableEntry[tab.Size() + 1];
  auto freqit = freq;
  for (auto ent : tab) {
    *freqit++ = ent;
  }
  //sort the array of frequency pairs.
#ifdef USE_RADIX_SORT
  sortWords(freq, tab.Size(), maxStrLen);
#else
  qsort(freq, tab.Size(), sizeof(tableEntry), table_comparator);
#endif
  //determine output filenames
  char* wrdFname = strdup(argv[1]);
  strcpy(wrdFname + strlen(wrdFname) - 3, "wrd");
  char* csvFname = strdup(argv[1]);
  strcpy(csvFname + strlen(csvFname) - 3, "csv");
  ofstream wrdout(wrdFname);
  ofstream csvout(csvFname);
  //print header of wrd file
  wrdout << "Zipf's Law: word concordance" << endl;
  wrdout << "----------------------------" << endl;
  wrdout << "File:            " << right << argv[1] << endl;
  wrdout << "Total words:     " << right << nWords << endl;
  wrdout << "Distinct words:  " << right << tab.Size() << endl;
  wrdout << endl;
  wrdout << "Word Frequencies                                               Ranks    Avg Rank" << endl;
  wrdout << "----------------                                               -----    --------" << endl;
  //print header of csv file
  csvout << "Zipf's Law: word concordance" << endl;
  csvout << "----------------------------" << endl;
  csvout << "File:            " << right << argv[1] << endl;
  csvout << "Total words:     " << right << nWords << endl;
  csvout << "Distinct words:  " << right << tab.Size() << endl;
  csvout << endl;
  csvout << "rank, freq, r*f" << endl;
  auto freqend = &freq[tab.Size() + 1];
  int rank = 1;
  int nOccs = freq[0].first;
  int rankCount = 0;
  auto rankBegin = freq;
  //iterate over frequency entries detecting changes in number of occurences
  for (freqit = freq; freqit < freqend; ++freqit) {
    if (nOccs != freqit->first) {
      int colIdx = 0;
      float avgRank = rank + double(rankCount - 1) / 2;
      //output line of csv data
      csvout << avgRank << ", " << nOccs << ", " << avgRank*nOccs << endl;
      //handle pluralization
      string wordHeader;
      if (nOccs > 1) {
	wordHeader = "Words occurring " + to_string(nOccs) + " times:";
      } else {
	wordHeader = "Words occurring once:";
      }
      wrdout << wordHeader;
      //handle rank ranges
      if (rankCount == 1) {
	wrdout << setw(68 - wordHeader.length()) << setfill(' ') << right << rank;
      } else {
	string rankstring = to_string(rank) + "-" + to_string(rank+rankCount-1);
	wrdout << string(68 - wordHeader.length() - rankstring.length(), ' ') << rankstring;
      }
      //finish the heading for words of a given rank
      wrdout << setw(12) << right << avgRank << endl;
      int nCols = 80 / maxStrLen;
      //write the words for a given rank category
      for (auto rankit = rankBegin; rankit < freqit; ++rankit) {
	wrdout << setw(maxStrLen+1) << setfill(' ') << left << rankit->second;
	if (++colIdx == nCols) {
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
  free(wrdFname);
  free(csvFname);
  //write timing
  t = clock() - t;
  cout << "Program finished in " << float(t)/CLOCKS_PER_SEC << " seconds" << endl;
}
