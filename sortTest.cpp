#include "sort.hpp"
#include <iostream>

int main(int argc, char** argv) {
  tableEntry tab[] = {
    tableEntry{4, "tes"},
    tableEntry{5, "blah"},
    tableEntry{0, ""}};
  sortWords(tab, 2, 4);
  for (auto it : tab) {
    std::cout << it.first << " " << it.second << std::endl;
  }
}
