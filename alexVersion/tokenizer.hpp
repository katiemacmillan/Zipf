#ifndef __TOKENIZER_HPP__
#define __TOKENIZER_HPP__

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
