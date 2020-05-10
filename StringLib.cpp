#ifndef _STRINGLIB_CPP_
#define _STRINGLIB_CPP_

#include <string>
using namespace std;

int find(string str1, string str2, int begin = 0) {
  for (int i = begin; i < str1.length() - str2.length(); i++) {
    bool f = true;
    for (int j = 0; j < str2.length(); j++) {
      if (str1[i + j] != str2[j]) {
        f = false;
        break;
      }
    }
    if (f) return i;
  }
  return -1;
}

#endif