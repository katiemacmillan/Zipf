#include "hashtable.hpp"
#include <iostream>

#define PROBE_LINEAR 1
#define PROBE_QUADRATIC 2
#define PROBE_DOUBLEHASH 3
#ifndef PROBE_SEQUENCE
#define PROBE_SEQUENCE PROBE_LINEAR
#endif

using namespace std;

bool checkPrime(int val) {
  int n = 2;
  while (n * n <= val) {
    if (val % n == 0) {
      return true;
    }
    ++n;
  }
  return false;
}

//credit goes to http://www.cse.yorku.ca/~oz/hash.html djb2
unsigned long Hashtable::hash(string& str) {
  unsigned long hash = 5381;
  for (auto c : str) {
    hash = ((hash << 5) + hash) + c;
  }
  return hash;
}

//credit goes to http://www.cse.yorku.ca/~oz/hash.html sdbm
unsigned long Hashtable::hash2(string& str) {
  unsigned long hash = 0;
  for (auto c : str) {
    hash = c + (hash << 6) + (hash  << 16) - hash;
  }
  return hash;
}

Hashtable::Hashtable(): table(new tableEntry[10011] + 1), size(0), capacity(1009) {
  //sentinel entries to guarantee that the iterators don't go into invalid memory before being checked
  table[-1].first = 1;
  table[capacity].first = 1;
};

Hashtable::Hashtable(int capacity): table(nullptr), size(0), capacity(capacity) {
  while (!checkPrime(capacity)) {++capacity;}
  table = new tableEntry[capacity + 2] + 1;
  //sentinel entries
  table[-1].first = 1;
  table[capacity].first = 1;
  this->capacity = capacity;
}

Hashtable::~Hashtable() {
  delete[] (table - 1);
}

int Hashtable::Find(string& key) {
  int index = hash(key) % capacity;
#if PROBE_SEQUENCE == PROBE_QUADRATIC
  int delta = -1;
#elif PROBE_SEQUENCE == PROBE_DOUBLEHASH
  int delta = hash2(key) % (capacity - 1) + 1; //guarantee nonzero
#endif
  while (table[index].first && table[index].second != key) {
#if PROBE_SEQUENCE == PROBE_LINEAR
    ++index;
#elif PROBE_SEQUENCE == PROBE_QUADRATIC
    index += (delta += 2);
#elif PROBE_SEQUENCE == PROBE_DOUBLEHASH
    index += delta;
#endif
    if (index >= capacity) {
      index -= capacity;
    }
  }
  if (table[index].second != key) {
    index = -1;
  }
  return index;
}

int Hashtable::Insert(string& key) {
  ++size;
  if (size*4 > capacity*3) { //Check 0.75 load factor
    EnsureCapacity(capacity*2);
    cout << "rehashing to " << capacity << endl;
  }
  int index = hash(key) % capacity;
#if PROBE_SEQUENCE == PROBE_QUADRATIC
  int delta = -1;
#elif PROBE_SEQUENCE == PROBE_DOUBLEHASH
  int delta = hash2(key) % (capacity - 1) + 1; //guarantee nonzero
#endif
  while (table[index].first > 0) {
#if PROBE_SEQUENCE == PROBE_LINEAR
    ++index;
#elif PROBE_SEQUENCE == PROBE_QUADRATIC
    index += (delta += 2);
#elif PROBE_SEQUENCE == PROBE_DOUBLEHASH
    index += delta;
#endif
    if (index >= capacity) {
      index -= capacity;
    }
  }
  table[index].second = key;
  table[index].first = 0;
  return index;
}

int Hashtable::FindOrInsert(string& key) {
  int index = Find(key);
  if (index == -1) {
    index = Insert(key);
  }
  return index;
}

int Hashtable::Put(tableEntry& entry) {
  int index = FindOrInsert(entry.second);
  table[index].first = entry.first ? entry.first : -1;
  return entry.first;
}

int Hashtable::Set(string& key, int val) {
  int index = FindOrInsert(key);
  table[index].first = val ? val : -1;
  return val;
}

int Hashtable::Increment(string& key) {
  int index = FindOrInsert(key);
  return ++table[index].first;
}

int Hashtable::Decrement(string& key) {
  int index = Find(key);
  int val = 0;
  if (index != -1) {
    val = --table[index].first;
    if (!table[index].first) {
      table[index].first = -1;
      --size;
    }
  }
  return val;
}

int Hashtable::Remove(string& key) {
  int index = Find(key);
  int val = 0;
  if (index != -1) {
    val = table[index].first;
    table[index].first = -1;
  }
  --size;
  return val;
}

void Hashtable::EnsureCapacity(int cap) {
  if (cap <= capacity) {return;}
  while (!checkPrime(cap)) {++cap;}
  auto tmp = table;
  table = new tableEntry[cap];
  for (auto ent = tmp; ent < &tmp[capacity]; ++ent) {
    if (ent->first > 0) {
      Put(*ent);
    }
  }
  delete[] tmp;
  capacity = cap;
}

int Hashtable::Size() {
  return size;
}

int Hashtable::Capacity() {
  return capacity;
}

Hashtable::iterator::iterator(tableEntry *loc): loc(loc) {};

Hashtable::iterator::~iterator() {};

Hashtable::iterator Hashtable::iterator::operator++(int ignore) {
  Hashtable::iterator tmp = *this;
  do {
    ++this->loc;
  } while (this->loc->first <= 0);
  return tmp;
}

Hashtable::iterator& Hashtable::iterator::operator++() {
  do {
    ++this->loc;
  } while (this->loc->first <= 0);
  return *this;
}

Hashtable::iterator Hashtable::iterator::operator--(int ignore) {
  Hashtable::iterator tmp = *this;
  do {
    --this->loc;
  } while (this->loc->first <= 0);
  return tmp;
}

Hashtable::iterator& Hashtable::iterator::operator--() {
  do {
    --this->loc;
  } while (this->loc->first <= 0);
  return *this;
}

const tableEntry& Hashtable::iterator::operator*() {
  return *loc;
}

Hashtable::iterator Hashtable::begin() {
  return ++Hashtable::iterator(table - 1);
}

Hashtable::iterator Hashtable::end() {
  return Hashtable::iterator(&table[capacity]);
}
