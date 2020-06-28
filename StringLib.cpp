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
  for (int i = begin; i <= str1.length() - str2.length(); i++) {
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

long long getDate()
{
  // get time vd: 15931520
  time_t t = time(0);
  // doi time => object ngay + gio vd: 20/6/2020 16h32p
  tm *timeTemp = localtime(&t);
  // dua ve 00h 00m vd: 20/6/2020 0h0p
  timeTemp->tm_hour = 0;
  timeTemp->tm_min = 0;
  timeTemp->tm_sec = 0;
  // doi lai thanh so time theo giay tinh tu 1990 vd: 15911000
  return mktime(timeTemp);
}

long long const TIME_7_NGAY = 604800;
long long const TIME_1_NGAY = 86400;

string getDateString(long long dateNumber)
{
  if (dateNumber < 0) return "";
  tm *t = localtime(&dateNumber);
  string dateString = to_string(t->tm_mday) + '/' + to_string(t->tm_mon + 1) + '/' + to_string(t->tm_year + 1900);
  return dateString;
}

//ko dung
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
  if (str.empty()) return;
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

string toUpperCase(string str)
{
  for (int i = 0; i < str.length(); i++) {
    str[i] = toupper(str[i]);
  }
  return str;
}

#endif