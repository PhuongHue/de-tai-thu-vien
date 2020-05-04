#ifndef _DAUSACHPAGE_CPP_
#define _DAUSACHPAGE_CPP_

#include <string>
#include <sstream>
#include "components/BookView.cpp"
#include "../DauSach.cpp"
#include <math.h>

using namespace std;

BookView DauSachBook;

string test[97];
int testLength = 97;

void dauSachPageLoad(BookView &book)
{
  int startIndex = book.pageIndex * book.pageSize;
  int endIndex = startIndex + book.pageSize - 1;
  if (endIndex > testLength - 1)
    endIndex = testLength - 1;
  book.lineCount = endIndex - startIndex + 1;
  // load data trang moi
  for (int i = startIndex, j = 0; i <= endIndex; i++, j++)
  {
    book.lines[j] = test[i];
  }
  // change select
  if (book.select > book.lineCount - 1)
    book.select = book.lineCount - 1;
}

void handleBookSelectChange(BookView &book)
{
}

void dauSachPageAction(BookView &book, int keyPressed)
{
}

void initDauSachPage()
{
  DauSachBook.left = 1;
  DauSachBook.top = 3;
  DauSachBook.right = 40;
  DauSachBook.bottom = 26;
  DauSachBook.pageSize = 20;
  DauSachBook.lineCount = 10;
  DauSachBook.allPage = (int)ceil(testLength * 1.0 / DauSachBook.pageSize);
  dauSachPageLoad(DauSachBook);
}

void runDauSachPage()
{
  loadLayout("layout/DauSach.layout");
  setHeader("Quan ly dau sach");
  runBookView(DauSachBook, dauSachPageAction, dauSachPageLoad, handleBookSelectChange);
  clearPage(1, 3, 154, 26);
}

#endif