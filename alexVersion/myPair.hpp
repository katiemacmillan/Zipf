#ifndef __MYPAIR_HPP__
#define __MYPAIR_HPP__

/*
myPair.hpp
replacement for std::pair since we aren't supposed to use STL

Author: Alex Iverson
*/

template <class A, class B>
struct myPair {
  A first;
  B second;
  myPair(): first(), second() {};
  myPair(A first, B second): first(first), second(second) {};
};

#endif
