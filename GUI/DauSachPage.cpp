#ifndef _DAUSACHPAGE_CPP_
#define _DAUSACHPAGE_CPP_

#include <string>
#include <sstream>
#include "components/BookView.cpp"
#include "../DauSach.cpp"
#include <math.h>

using namespace std;

BookView DauSachBook;

string test[43];
int testLength = 43;

void dauSachPageLoad(BookView &book)
{
  int startIndex = book.pageIndex * book.pageSize;
  int endIndex = startIndex + book.pageSize - 1;
  if (endIndex > testLength - 1)
    endIndex = testLength - 1;
  book.lineCount = endIndex - startIndex + 1;
  for (int i = startIndex, j = 0; i <= endIndex; i++, j++)
  {
    book.lines[j] = test[i];
  }
  // gotoxy(0, 50);
  // cout << "startIndex" << startIndex << endl;
  // cout << "endIndex" << endIndex << endl;
  // cout << "allPage: " << book.allPage << endl;
  // cout << "lineCount: " << book.lineCount << endl;
  // cout << "pageSize: " << book.pageSize << endl;
  // cout << "pageIndex: " << book.pageIndex << endl;
  // cout << "select: " << book.select << endl;
  // for (int i = 0; i <= book.lineCount; i++)
  // {
  //   cout << book.lines[i] << endl;
  // }
}

void dauSachPageAction(BookView &book, int keyPressed)
{
}

void initDauSachPage()
{
  DauSachBook.left = 1;
  DauSachBook.top = 3;
  DauSachBook.right = 40;
  DauSachBook.pageSize = 10;
  DauSachBook.lineCount = 10;
  DauSachBook.allPage = (int)ceil(testLength * 1.0 / DauSachBook.pageSize);
  dauSachPageLoad(DauSachBook);
}

void runDauSachPage()
{
  loadLayout("layout/DauSach.layout");
  runBookView(DauSachBook, dauSachPageAction, dauSachPageLoad);
  clearPage(1, 3, 154, 26);
}

#endif