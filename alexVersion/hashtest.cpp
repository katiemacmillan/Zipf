#include "hashtable.hpp"
#include "tokenizer.hpp"
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char** argv) {
  Hashtable tab;
  if (argc < 1) {
    cout << "requires filename argument";
    return 0;
  }
  ifstream file(argv[1]);
  Tokenizer tok{file};
  for (string s = tok.Next(); s != ""; s = tok.Next()) {
    tab.Increment(s);
  }
  for (auto ent : tab) {
    cout << ent.first << "\t" << ent.second << endl;
  }
}
