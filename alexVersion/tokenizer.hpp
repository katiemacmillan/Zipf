#ifndef __TOKENIZER_HPP__
#define __TOKENIZER_HPP__
/*
tokenizer.hpp
Tokenize an input stream into words

Author: Alex Iverson
*/

//Next returns the next word in the given input stream.
//An empty string indicates the end of the stream.
//Hyphenated forms are combined, ignoring whitespace if necessary.
//This means that a word hyphenated because of a line break will be combined correctly and hyphenated compound forms will be treated as a single word.
//Hyphens used as dynamics markers may not be treated correctly, but handling this case correctly requires a sophistication beyond the scope of this project.
//However, the vast majority of cases where hyphens used as dynamics markers produces incorrect results will occur rarely and so will have minimal effect on most of the data.

#include <istream>
#include <string>

class Tokenizer {
  std::istream &stream;
  char* buffer;
  int bufflen;
public:
  Tokenizer(std::istream&);
  ~Tokenizer();
  std::string Next();
};

#endif
