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

void dauSachPageReload(BookView &book)
{
  int startIndex = book.pageIndex * book.pageSize;
  int endIndex = startIndex + book.pageSize - 1;
  if (endIndex > testLength - 1)
    endIndex = testLength - 1;
  book.lineCount = endIndex - startIndex + 1;
  for (int i = startIndex; i <= endIndex; i++)
  {
    book.lines[i] = test[i];
  }
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
  dauSachPageReload(DauSachBook);
}

void runDauSachPage()
{
  for (int i = 0; i < 43; i++)
  {
    stringstream sst;
    sst << "This is line " << i + 1;
    string str;
    getline(sst, str);
    test[i] = str;
  }

  loadLayout("layout/DauSach.layout");
  runBookView(DauSachBook, dauSachPageAction, dauSachPageReload);
  clearPage(1, 3, 154, 26);
}

#endif