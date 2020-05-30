#ifndef _STRINGLIB_CPP_
#define _STRINGLIB_CPP_

#include <iomanip>
#include <sstream>
#include <string>
#include <time.h>
using namespace std;

int find(string str1, string str2, int begin = 0)
{
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

template <class T>
T convert(string str)
{
  stringstream ss;
  ss << str;
  T x;
  ss >> x;
  return x;
}

#define VNM 7

string getDateString(long long dateNumber)
{
  tm *t = localtime(&dateNumber);
  stringstream ss;
  ss << t->tm_mday << '/' << t->tm_mon + 1 << '/' << t->tm_year + 1900;
  string dateString;
  ss >> dateString;
  return dateString;
}

time_t getDateNumber(string dateString)
{
  tm *t = new tm;
  int day, month, year;
  sscanf(dateString.data(), "%d/%d/%d", &day, &month, &year);
  t->tm_mday = day;
  t->tm_mon = month - 1;
  t->tm_year = year - 1900;
  return mktime(t);
}

#endif