#ifndef _BOOKVIEW_CPP_
#define _BOOKVIEW_CPP_

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "../consolelib.h"
#include "StaticDefine.cpp"

using namespace std;

#define MAX_BOOK_VIEW_LINE 30

struct BookView
{
  string lines[MAX_BOOK_VIEW_LINE];
  int lineCount = 0;
  int select = 0;
  int top;
  int left;
  int right;
  int pageSize = 20;
  int pageIndex = 0;
  int allPage = 0;
};

void printLine(int number, string line)
{
  cout << setw(3) << number << (char)179 << line;
}

void drawBook(BookView &book)
{
  for (int i = 0; i < book.lineCount; i++)
  {
    gotoxy(book.left, book.top + i);
    if (i == book.select % book.lineCount)
    {
      setSelectText();
    }
    printLine(book.pageIndex * book.pageSize + i + 1, book.lines[i]);
    if (i == book.select % book.lineCount)
    {
      setNormalText();
    }
  }
  ShowConsoleCursor(false);
}

void clearBook(BookView book)
{
  int left = book.left;
  int top = book.top;
  int right = book.right;
  string str(right - left, ' ');
  setNormalText();
  for (int i = 0; i < book.lineCount; i++)
  {
    gotoxy(left, top + i);
    cout << str;
  }
}

void changeSelect(BookView &book, int select)
{
  gotoxy(book.left, book.top + book.select);
  setNormalText();
  printLine(book.pageIndex * book.pageSize + book.select + 1, book.lines[book.select]);
  gotoxy(book.left, book.top + select);
  setSelectText();
  printLine(book.pageIndex * book.pageSize + select + 1, book.lines[select]);
  book.select = select;
}

typedef void (*DispathBookAction)(BookView &book, int keyPressed);
typedef void (*DispathBookLoad)(BookView &book);

void runBookView(BookView &book, DispathBookAction action, DispathBookLoad load)
{
  drawBook(book);
  bool ret = false;
  while (!ret)
  {
    int key = getch();
    if (key == 0 || key == 224)
      key = getch();
    ShowConsoleCursor(false);
    switch (key)
    {
    case ARROW_DOWN:
      changeSelect(book, (book.select + 1) % book.lineCount);
      break;
    case ARROW_UP:
      changeSelect(book, (book.select + book.lineCount - 1) % book.lineCount);
      break;
    case ARROW_RIGHT:
      if (book.allPage > 0)
      {
        clearBook(book);
        book.pageIndex = (book.pageIndex + 1) % book.allPage;
        load(book);
        drawBook(book);
      }
      break;
    case ARROW_LEFT:
      if (book.allPage > 0)
      {
        clearBook(book);
        book.pageIndex = (book.pageIndex + book.allPage - 1) % book.allPage;
        load(book);
        drawBook(book);
      }
      break;
    case ESC:
      ret = true;
      break;
    default:
      action(book, key);
    }
  }
  clearBook(book);
}
// TODO: page info missing
#endif