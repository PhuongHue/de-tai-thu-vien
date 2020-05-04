#ifndef _DAUSACHPAGE_CPP_
#define _DAUSACHPAGE_CPP_

#include <string>
#include <sstream>
#include <math.h>
#include "components/BookView.cpp"
#include "components/ContentView.cpp"
#include "../DauSach.cpp"

using namespace std;

BookView DauSachBook;
ContentView DauSachContent;

ListDauSach listDauSach;

void DauSachContentLoad()
{
}

void dauSachBookLoad(BookView &book)
{
  int startIndex = book.pageIndex * book.pageSize;
  int endIndex = startIndex + book.pageSize - 1;
  if (endIndex > listDauSach.length - 1)
    endIndex = listDauSach.length - 1;
  book.lineCount = endIndex - startIndex + 1;
  // load data trang moi
  for (int i = startIndex, j = 0; i <= endIndex; i++, j++)
  {
    book.lines[j] = listDauSach.data[i].ten;
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
  DauSachBook.allPage = (int)ceil(listDauSach.length * 1.0 / DauSachBook.pageSize);
  if (DauSachBook.allPage == 0)
    DauSachBook.allPage = 1;
  dauSachBookLoad(DauSachBook);
  DauSachContent.top = 3;
  DauSachContent.left = 72;
  DauSachContent.right = 154;
  DauSachContent.bottom = 26;
}

void runDauSachPage()
{
  loadLayout("layout/DauSach.layout");
  setHeader("Quan ly dau sach");
  runBookView(DauSachBook, dauSachPageAction, dauSachBookLoad, handleBookSelectChange);
  clearPage(1, 3, 154, 26);
}

#endif