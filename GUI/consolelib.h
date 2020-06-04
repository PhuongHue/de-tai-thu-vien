#ifndef _CONSOLE_LIB_H_
#define _CONSOLE_LIB_H_

#include <conio.h>
#include <dos.h>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include <string>
#include <vector>

#include "components/StaticDefine.cpp"

int wherex(void)
{
  HANDLE hConsoleOutput;
  hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
  GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);
  return screen_buffer_info.dwCursorPosition.X;
}

int wherey(void)
{
  HANDLE hConsoleOutput;
  hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
  GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);
  return screen_buffer_info.dwCursorPosition.Y;
}

void gotoxy(short x, short y)
{
  HANDLE hConsoleOutput;
  COORD Cursor_an_Pos = {x, y};
  hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleCursorPosition(hConsoleOutput, Cursor_an_Pos);
}

void gotoLeft(int count = 1) { gotoxy(wherex() - count, wherey()); }

void gotoRight(int count = 1) { gotoxy(wherex() + count, wherey()); }

void clreol()
{
  COORD coord;
  DWORD written;
  CONSOLE_SCREEN_BUFFER_INFO info;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
  coord.X = info.dwCursorPosition.X;
  coord.Y = info.dwCursorPosition.Y;
  FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ',
      info.dwSize.X - info.dwCursorPosition.X * info.dwCursorPosition.Y, coord, &written);
  gotoxy(info.dwCursorPosition.X, info.dwCursorPosition.Y);
}

void SetColor(WORD color)
{
  HANDLE hConsoleOutput;
  hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
  GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);
  WORD wAttributes = screen_buffer_info.wAttributes;
  color &= 0x000f;
  wAttributes &= 0xfff0;
  wAttributes |= color;
  SetConsoleTextAttribute(hConsoleOutput, wAttributes);
}
void SetBGColor(WORD color)
{
  HANDLE hConsoleOutput;
  hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
  GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);
  WORD wAttributes = screen_buffer_info.wAttributes;
  color &= 0x000f;
  color <<= 4;
  wAttributes &= 0xff0f;
  wAttributes |= color;
  SetConsoleTextAttribute(hConsoleOutput, wAttributes);
}

void showConsoleCursor(bool showFlag)
{
  HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO cursorInfo;
  GetConsoleCursorInfo(out, &cursorInfo);
  cursorInfo.bVisible = showFlag;
  SetConsoleCursorInfo(out, &cursorInfo);
}

using namespace std;

void fullScreen()
{
  system("mode con COLS=700");
  ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
}

void clrscr() { system("cls"); }

void setSelectText()
{
  SetColor(7);
  SetBGColor(8);
}

void setNormalText()
{
  SetColor(15);
  SetBGColor(0);
}

struct DebugLog {
  int lineN = 1;
  int logX = 32;
} _DebugLog;

template <class T>
void consoleLog(T s)
{
  int cX = wherex();
  int cY = wherey();
  gotoxy(0, _DebugLog.logX);
  cout << setw(3) << _DebugLog.lineN << setw(0) << ":::" << s << "�" << endl;
  _DebugLog.logX++;
  _DebugLog.lineN++;
  gotoxy(cX, cY);
}

void loadLayout(string fileName)
{
  int y = 0;
  ifstream fin(fileName.data());
  while (!fin.eof()) {
    string line;
    getline(fin, line);
    if (line.compare("") != 0) {
      for (int x = 0; x < line.length(); x++) {
        if (line[x] != ' ') {
          gotoxy(x, y);
          cout << line[x];
        }
      }
    }
    y++;
  }
  fin.close();
}

struct Header {
  int top = 1;
  int left = 1;
  int length = 0;
} cacheHeader;

void setHeader(string header)
{
  gotoxy(cacheHeader.left, cacheHeader.top);
  cout << header;
  if (cacheHeader.length > header.length()) {
    cout << setw(cacheHeader.length - header.length()) << setfill(' ') << ' ' << setw(0);
  }
  cacheHeader.length = header.length();
}

void clearPage(int left, int top, int right, int bottom)
{
  string str(right - left + 1, ' ');
  for (int i = top; i <= bottom; i++) {
    gotoxy(left, i);
    cout << str;
  }
}

void appPause(string message, int x = 0, int y = 0)
{
  int top = y;
  int left = x;
  int bottom = y;
  int right = left + max((int)message.length(), 29);
  gotoxy(x, y);
  cout << message;
  if (message.length() > 0) bottom++;
  gotoxy(left, bottom);
  cout << "Nhan phim bat ky de tiep tuc.";
  int key = getch();
  if (key == 0 || key == 224) getch();
  clearPage(left, top, right, bottom);
}

bool appYesNo(string message = "", int x = 0, int y = 0)
{
  int top = y;
  int left = x;
  int right = max((int)message.length(), 29);
  gotoxy(x, y);
  cout << message;
  if (message.length() > 0) y++;
  gotoxy(x, y);
  cout << "[y/n]: ";
  int cX = wherex();
  int cY = wherey();
  string res;
  bool ans;
  showConsoleCursor(true);
  while (true) {
    getline(cin, res);
    for (int i = 0; i < res.length(); i++) {
      res[i] = toupper(res[i]);
    }

    if (res.compare("YES") == 0 || res.compare("Y") == 0) {
      ans = true;
      break;
    }
    if (res.compare("NO") == 0 || res.compare("N") == 0) {
      ans = false;
      break;
    }
    gotoxy(cX, cY);
    cout << setw(res.length()) << setfill(' ') << ' ' << setw(0);
    gotoxy(cX, cY);
  }
  showConsoleCursor(false);
  clearPage(left, top, right, y);
  return ans;
}

void inputText(string &str, int limit, int left, int top, bool isNumber = false)
{
  gotoxy(left, top);
  showConsoleCursor(true);
  setNormalText();
  cout << str;
  int x;
  while (x != ENTER && x != ESC) {
    x = getch();
    if (x == 0 || x == 224) {
      getch();
      continue;
    }
    if (str.length() < limit && (x >= ' ' && x <= 'z')) {
      if (isNumber)
        if (x < '0' || x > '9') continue;
      cout << (char)x;
      str += x;
    }
    else if (x == 8) {
      if (str.length() > 0) {
        str.pop_back();
        cout << (char)8 << ' ' << (char)8;
      }
      else
        cout << BELL; // keu thong bao
    }
  }
  if (x == ESC) {
    gotoxy(left, top);
    cout << setw(str.length()) << setfill(' ') << ' ' << setw(0);
    str = "";
  }
  showConsoleCursor(false);
}

#define MAX_APP_FOOTER_LINES 2

struct Footer {
  int left = 2;
  int top = 28;
  int right = 154;
  string lines[MAX_APP_FOOTER_LINES];
  string separator = " | ";
} appFooter;

void setFooter(vector<string> cmds)
{
  // clear
  for (int i = 0; i < MAX_APP_FOOTER_LINES; i++) {
    gotoxy(appFooter.left, appFooter.top + i);
    cout << setw(appFooter.lines[i].length()) << setfill(' ') << ' ' << setw(0);
    appFooter.lines[i] = "";
  }
  // set up lines
  int maxWidth = appFooter.right - appFooter.left + 1;
  int lineIndex = 0;
  for (int i = 0; i < cmds.size(); i++) {
    if (appFooter.lines[lineIndex].length() + cmds[i].length() + appFooter.separator.length() > maxWidth)
      lineIndex++;
    appFooter.lines[lineIndex] += cmds[i] + appFooter.separator;
  }
  // print lines
  for (int i = 0; i < MAX_APP_FOOTER_LINES; i++) {
    gotoxy(appFooter.left, appFooter.top + i);
    cout << appFooter.lines[i];
  }
}

#endif