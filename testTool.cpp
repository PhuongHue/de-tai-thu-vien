#include "StringLib.cpp"
#include <iostream>
#include <time.h>
using namespace std;

int main()
{
  while (true) {
    int day, month, year;
    cout << "Ngay: ";
    cin >> day;
    cout << "Thang: ";
    cin >> month;
    cout << "Nam: ";
    cin >> year;
    tm *t = new tm;
    t->tm_sec = 0;
    t->tm_min = 0;
    t->tm_hour = 0;
    t->tm_mday = day;
    t->tm_mon = month - 1;
    t->tm_year = year - 1900;
    time_t time = mktime(t);
    cout << time << endl;
    delete t;
  }
}