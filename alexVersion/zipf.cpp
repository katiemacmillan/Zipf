#include "hashtable.hpp"
#include "tokenizer.hpp"
#include "sort.hpp"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string.h>

using namespace std;

int table_comparator(const void* a, const void* b) {
  int result = ((tableEntry *) b)->first - ((tableEntry *) a)->first;
  if (!result) {
    result = ((tableEntry *) a)->second.compare(((tableEntry *) b)->second);
  }
  return result;
}

int main(int argc, char** argv) {
  Hashtable tab;
  if (argc < 1) {
    cout << "requires filename argument";
    return 0;
  }
  ifstream file(argv[1]);
  Tokenizer tok(file);
  int maxStrLen = 0;
  int nWords = 0;
  for (string s = tok.Next(); s != ""; s = tok.Next()) {
    int len = s.length();
    if (len > maxStrLen) {
      maxStrLen = len;
    }
    ++nWords;
    tab.Increment(s);
  }
  tableEntry* freq = new tableEntry[tab.Size() + 1];
  auto freqit = freq;
  for (auto ent : tab) {
    *freqit++ = ent;
  }
  //sortWords(freq, tab.Size(), maxStrLen);
  qsort(freq, tab.Size(), sizeof(tableEntry), table_comparator);
  char* wrdFname = strdup(argv[1]);
  strcpy(wrdFname + strlen(wrdFname) - 3, "wrd");
  char* csvFname = strdup(argv[1]);
  strcpy(csvFname + strlen(csvFname) - 3, "csv");
  ofstream wrdout(wrdFname);
  ofstream csvout(csvFname);
  wrdout << "Zipf's Law: word concordance" << endl;
  wrdout << "----------------------------" << endl;
  wrdout << "File:            " << right << argv[1] << endl;
  wrdout << "Total words:     " << right << nWords << endl;
  wrdout << "Distinct words:  " << right << tab.Size() << endl;
  wrdout << endl;
  wrdout << "Word Frequencies                                               Ranks    Avg Rank" << endl;
  wrdout << "----------------                                               -----    --------" << endl;
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
  for (freqit = freq; freqit < freqend; ++freqit) {
    if (nOccs != freqit->first) {
      int colIdx = 0;
      float avgRank = rank + double(rankCount - 1) / 2;
      csvout << avgRank << ", " << nOccs << ", " << avgRank*nOccs << endl;
      string wordHeader;
      if (nOccs > 1) {
	wordHeader = "Words occurring " + to_string(nOccs) + " times:";
      } else {
	wordHeader = "Words occurring once:";
      }
      wrdout << wordHeader;
      if (rankCount == 1) {
	wrdout << setw(68 - wordHeader.length()) << setfill(' ') << right << rank;
      } else {
	string rankstring = to_string(rank) + "-" + to_string(rank+rankCount-1);
	wrdout << string(68 - wordHeader.length() - rankstring.length(), ' ') << rankstring;
      }
      wrdout << setw(12) << right << avgRank << endl;
      int nCols = 80 / maxStrLen;
      for (auto rankit = rankBegin; rankit < freqit; ++rankit) {
	wrdout << setw(maxStrLen+1) << setfill(' ') << left << rankit->second;
	if (++colIdx == nCols) {
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
  delete[] freq;
  free(wrdFname);
  free(csvFname);
}
