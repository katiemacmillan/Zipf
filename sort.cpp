#include <utility>


using namespace std;

typedef pair<int, string> tableEntry;

inline bool emptyOrDeleted(tableEntry &data) {
  return data->first <= 0;
}

void sortWords(tableEntry words[], int length, int maxStrLen) { //O(N) sort for the table entries
  //Partition empty elements
  auto part1 = &words[-1];
  auto part2 = &words[length];
  while (part1 < part2) {
    while (!emptyOrDeleted(*(++part1)));
    while (emptyOrDeleted(*(--part2)));
    swap(*part1, *part2);
  };
  //distribution counting sort on word length
  //This eliminates a large number of unnecessary iterations in the following radix sort and eliminates the need for bounds checking on the strings.
  auto endWord = part1;
  int count[] = new int[maxStrLen]{0};
  for (auto it = &words[0]; it < endWord; ++it) {
    ++count[it->second.length()];
  }
  int offset = 0;
  int oldCount = 0;
  //convert counts to offsets
  for (int n = 0; n < maxStrLen; ++n) {
    oldCount = count[n];
    count[n] = offset;
    offset += oldCount;
  }
  length = offset;
  tableEntry tmp[] = new tableEntry[length];
  for (auto it = &words[0]; it < lastWord; ++it) {
    tmp[count[it->second.length()]++] = *it;
  }
  //shift count array to indicate beginning positions rather than end positions
  for (int n = 1; n < maxStrLen; ++n) {
    count[n] = count[n-1];
  }
  count[0] = 0;
  swap(tmp, words); //This destroys the original words array. If this is undesirable, we need to allocate a new one here.
  //Perform RtL radix sort on the strings restricting the bounds of the sort to the portion of strings long enough to matter
  int byteCount = new int[256];
  for (int charPos = maxStrLen - 1; charPos >= 0; --charPos) {
    //counting sort component of the radix sort
    for (int n = 0; n < 256; ++n) {
      byteCount[n] = 0;
    }
    for (int n = count[charPos]; n < length; ++n) {
      ++byteCount[words[n].second[charPos]];
    }
    int offset = count[charPos];
    int oldCount = 0;
    //convert counts to offsets
    for (int n = 0; n < 256; ++n) {
      oldCount = byteCount[n];
      byteCount[n] = offset;
      offset += oldCount;
    }
    for (auto it = &words[count[charPos]]; it < &words[length]; ++it) {
      tmp[byteCount[it->second[charPos]]++] = *it;
    }
    swap(tmp, words); //After this the partially sorted output will be in words rather than tmp
  }
  //RtL radix sort on keys
  for (int bytePos = 0; bytePos < 32; bytePos += 8) {
    //counting sort component of the radix sort
    for (int n = 0; n < 256; ++n) {
      byteCount[n] = 0;
    }
    for (int n = 0; n < length; ++n) {
      ++byteCount[(words[n].first >> bytePos) & 0xFF];
    }
    int offset = 0;
    int oldCount = 0;
    //convert counts to offsets
    for (int n = 0; n < 256; ++n) {
      oldCount = byteCount[n];
      byteCount[n] = offset;
      offset += oldCount;
    }
    for (auto it = &words[0]; it < &words[length]; ++it) {
      tmp[byteCount[(words[n].first >> bytePos) & 0xFF]++] = *it;
    }
    swap(tmp, words); //After this the partially sorted output will be in words rather than tmp
  }
  //protect against parity issues
  for (int n = 0; n < length; ++n) {
    tmp[n] = words[n];
  }
}
