#ifndef _STRINGLIB_CPP_
#define _STRINGLIB_CPP_

#include <iomanip>
#include <sstream>
#include <string>
#include <time.h>
using namespace std;

int find(string str1, string str2, int begin = 0)
{
  if (str2.length() > str1.length()) return -1;
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
  if (dateNumber < 0) return "";
  tm *t = localtime(&dateNumber);
  stringstream ss;
  ss << t->tm_mday << '/' << t->tm_mon + 1 << '/' << t->tm_year + 1900;
  string dateString;
  ss >> dateString;
  return dateString;
}

time_t getDateNumber(string dateString)
{
  if (dateString.empty()) return -1;
  tm *t = new tm;
  int day, month, year;
  sscanf(dateString.data(), "%d/%d/%d", &day, &month, &year);
  t->tm_mday = day;
  t->tm_mon = month - 1;
  t->tm_year = year - 1900;
  return mktime(t);
}

void chuanHoa(string &str)
{
  while (str[0] == ' ')
    str.erase(0, 1); // xoa khoang trang dau
  while (str[str.length() - 1] == ' ')
    str.erase(str.length() - 1, 1); // xoa khoang trang cuoi
  for (int i = 0; i < str.length() - 1; i++) {
    if (str[i] == ' ' && str[i + 1] == ' ') str.erase(i, 1);
  }
  int x = str.find("  ");
  while (x != -1) {
    str.erase(x, 1);
    x = str.find("  ");
  }
}

#endif