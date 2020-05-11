#ifndef _BOOKVIEW_CPP_
#define _BOOKVIEW_CPP_

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <math.h>

#include "../consolelib.h"
#include "StaticDefine.cpp"

using namespace std;

#define MAX_BOOK_VIEW_LINE 30

struct BookView {
  string lines[MAX_BOOK_VIEW_LINE];
  string keys[MAX_BOOK_VIEW_LINE];
  int lineCount = 0;
  int select = 0;
  int top;
  int left;
  int right;
  int bottom;
  int pageSize = 20;
  int pageIndex = 0;
  int allPage = 0;
};

void debugBook(BookView book) {
  gotoxy(0, 50);
  cout << "allPage: " << book.allPage << endl;
  cout << "lineCount: " << book.lineCount << endl;
  cout << "pageSize: " << book.pageSize << endl;
  cout << "pageIndex: " << book.pageIndex << endl;
  cout << "select: " << book.select << endl;
  for (int i = 0; i <= book.lineCount; i++) {
    cout << book.lines[i] << endl;
  }
}

void printLine(int number, string line) { cout << setw(3) << number << (char)179 << " " << line; }

void drawBookView(BookView &book) {
  // draw book's lines
  for (int i = 0; i < book.lineCount; i++) {
    gotoxy(book.left, book.top + i);
    if (i == book.select % book.lineCount) {
      setSelectText();
    }
    printLine(book.pageIndex * book.pageSize + i + 1, book.lines[i]);
    if (i == book.select % book.lineCount) {
      setNormalText();
    }
  }
  // draw
  gotoxy(book.left, book.bottom);
  cout << "Trang " << book.pageIndex + 1 << " / " << book.allPage;
  showConsoleCursor(false);
}

void clearBookView(BookView book) {
  string str(book.right - book.left, ' ');
  setNormalText();
  for (int i = 0; i < book.lineCount; i++) {
    gotoxy(book.left, book.top + i);
    cout << str;
  }
  gotoxy(book.left, book.bottom);
  cout << str;
}

void changeBookSelect(BookView &book, int select) {
  gotoxy(book.left, book.top + book.select);
  setNormalText();
  printLine(book.pageIndex * book.pageSize + book.select + 1, book.lines[book.select]);
  gotoxy(book.left, book.top + select);
  setSelectText();
  printLine(book.pageIndex * book.pageSize + select + 1, book.lines[select]);
  book.select = select;
}

void resetBookIndex(BookView &book, int dataCount) {
  book.pageIndex = 0;
  book.allPage = (int)ceil(dataCount * 1.0 / book.pageSize);
  if (book.allPage == 0) book.allPage = 1;
}

typedef void (*BookKeyPressedHandle)(BookView &book, int keyPressed);
typedef void (*BookAction)(BookView &book);

void runBookView(BookView &book, BookKeyPressedHandle onPressed, BookAction load, BookAction onChange) {
  drawBookView(book);
  bool ret = false;
  while (!ret) {
    int key = getch();
    if (key == 0 || key == 224) key = getch();
    showConsoleCursor(false);
    switch (key) {
      case ARROW_DOWN:
        if (book.lineCount > 0) changeBookSelect(book, (book.select + 1) % book.lineCount);
        break;
      case ARROW_UP:
        if (book.lineCount > 0) changeBookSelect(book, (book.select + book.lineCount - 1) % book.lineCount);
        break;
      case ARROW_RIGHT:
        if (book.allPage > 0) {
          clearBookView(book);
          book.pageIndex = (book.pageIndex + 1) % book.allPage;
          load(book);
          drawBookView(book);
        }
        break;
      case ARROW_LEFT:
        if (book.allPage > 0) {
          clearBookView(book);
          book.pageIndex = (book.pageIndex + book.allPage - 1) % book.allPage;
          load(book);
          drawBookView(book);
        }
        break;
      case ESC:
        ret = true;
        break;
      default:
        // nhan nhung phim con lai F1, F2, F3, F4
        onPressed(book, key);
    }
    onChange(book);
  }
  clearBookView(book);
}
#endif