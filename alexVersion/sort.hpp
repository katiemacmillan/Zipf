#ifndef __SORT_HPP__
#define __SORT_HPP__
#include <utility>
#include <string>

typedef std::pair<int, std::string> tableEntry;

void sortWords(tableEntry words[], int length, int maxStrLen); //O(N) sort for the table entries

#endif
