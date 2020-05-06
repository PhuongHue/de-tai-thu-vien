#ifndef _CONSOLE_LIB_H_
#define _CONSOLE_LIB_H_

#include <conio.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include <string>

#include "components/StaticDefine.cpp"

void gotoxy(short x, short y) {
  HANDLE hConsoleOutput;
  COORD Cursor_an_Pos = {x, y};
  hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleCursorPosition(hConsoleOutput, Cursor_an_Pos);
}

int wherex(void) {
  HANDLE hConsoleOutput;
  hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
  GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);
  return screen_buffer_info.dwCursorPosition.X;
}

int wherey(void) {
  HANDLE hConsoleOutput;
  hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
  GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);
  return screen_buffer_info.dwCursorPosition.Y;
}
void clreol() {
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

void SetColor(WORD color) {
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
void SetBGColor(WORD color) {
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

void showConsoleCursor(bool showFlag) {
  HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO cursorInfo;
  GetConsoleCursorInfo(out, &cursorInfo);
  cursorInfo.bVisible = showFlag;
  SetConsoleCursorInfo(out, &cursorInfo);
}

using namespace std;

void fullScreen() {
  system("mode con COLS=700");
  ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
}

void clrscr() { system("cls"); }

void setSelectText() {
  SetColor(7);
  SetBGColor(8);
}

void setNormalText() {
  SetColor(15);
  SetBGColor(0);
}

void loadLayout(string fileName) {
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

void setHeader(string header) {
  string str(40, ' ');
  gotoxy(1, 1);
  cout << str;
  gotoxy(1, 1);
  cout << header;
}

void clearPage(int left, int top, int right, int bottom) {
  string str(right - left + 1, ' ');
  for (int i = top; i <= bottom; i++) {
    gotoxy(left, i);
    cout << str;
  }
}

void customCin(string &str, int limit) {
  int cX = wherex();
  int cY = wherey();
  string temp = "";
  int x;
  while (x != ENTER && x != ESC) {
    x = getch();
    if (x == 0 || x == 224) x = getch();
    if (temp.length() <= limit && (x >= '0' && x <= '9' || x >= 'A' && x <= 'Z' || x >= 'a' && x <= 'z')) {
      cout << (char)x;
      temp += x;
    } else if (x == 8) {
      if (temp.length() > 0) {
        temp.pop_back();
        cout << (char)8 << ' ' << (char)8;
      } else
        cout << (char)7;  // keu thong bao
    }
  }
  if (x == ENTER)
    str = temp;
  else {
    string spaceStr(temp.length(), ' ');
    gotoxy(cX, cY);
    cout << spaceStr;
  }
}
void customCin(int &a, int limit) {
  if (limit > 10) limit = 10;
  char x;
  while (x != ENTER) {
    x = getch();
    if (x >= 48 && x <= 57) {
      a += a * 10 + x;
    }
  }
}

#endif