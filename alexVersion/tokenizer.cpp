/*
tokenizer.cpp
Tokenize an input stream into words

Author: Alex Iverson
*/

#include "tokenizer.hpp"
#include <iostream>

Tokenizer::Tokenizer(std::istream &stream): stream(stream), buffer(new char[65]), bufflen(64) {};

Tokenizer::~Tokenizer() {
  delete[] buffer;
}

//Get next word from input stream
std::string Tokenizer::Next() {
  int index = -1;
  int c = stream.get();
  while (c == ' ' || c == '\n' || c == '\t') {c = stream.get();};
  while(true) {
    if (c >= 'A' && c <= 'Z') {
      buffer[++index] = c;
    }
    else if (c >= 'a' && c <= 'z') {
      buffer[++index] = c - 'a' + 'A'; // capitalize to generate all upper case words
    }
    else if (c >= '0' && c <= '9') {
      buffer[++index] = c;
    }
    else if (c == '-') {
      //combine hyphenated forms
      while (c && (c == '-' || c == '\n' || c == ' ' || c == '\t')) {
	c = stream.get();
      }
      continue;
    }
    //handle whitespace
    else if (c == ' ' || c == '\n' || c == '\t') {
      if (index  == -1) {
	c = stream.get();
	continue;
      }
      buffer[++index] = 0;
      break;
    } else if (c <= 0) {
      buffer[++index] = 0;
      break;
    }
    //check to grow buffer
    if (index >= bufflen) {
      char* tmp = buffer;
      buffer = new char[bufflen * 2 + 1];
      for (int i = 0; i <= bufflen; ++i) {
	buffer[i] = tmp[i];
      }
      bufflen *= 2;
      delete[] tmp;
    }
    c = stream.get();
  }
  return std::string(buffer);
}
