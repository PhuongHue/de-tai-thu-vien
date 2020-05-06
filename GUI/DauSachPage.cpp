#ifndef _DAUSACHPAGE_CPP_
#define _DAUSACHPAGE_CPP_

#include <math.h>

#include <sstream>
#include <string>

#include "../DauSach.cpp"
#include "components/BookView.cpp"
#include "components/ContentView.cpp"

using namespace std;

BookView DauSachBook;
ContentView DauSachContent;

ListDauSach listDauSach;

void DauSachContentLoad() {}

void dauSachBookLoad(BookView &book) {
  int startIndex = book.pageIndex * book.pageSize;
  int endIndex = startIndex + book.pageSize - 1;
  if (endIndex > listDauSach.length - 1) endIndex = listDauSach.length - 1;
  book.lineCount = endIndex - startIndex + 1;
  // load data trang moi
  for (int i = startIndex, j = 0; i <= endIndex; i++, j++) {
    book.lines[j] = listDauSach.data[i]->ten;
    book.keys[j] = listDauSach.data[i]->ISBN;
  }
  // change select
  if (book.select > book.lineCount - 1) book.select = book.lineCount - 1;
}

void handleBookSelectChange(BookView &book) {
  DauSach *ds = findDauSachByISBN(listDauSach, book.keys[book.select]);
  DauSachContent.lines[0] = ds->ISBN;
  // TODO: bug not show number
  DauSachContent.lines[1] = ds->namXB;
  DauSachContent.lines[2] = "" + ds->soTrang;
  DauSachContent.lines[3] = ds->tacGia;
  DauSachContent.lines[4] = ds->ten;
  DauSachContent.lines[5] = ds->theLoai;
  drawContentView(DauSachContent);
}

void searchDauSach() {
  gotoxy(11, 3);
  showConsoleCursor(true);
  string key;
  setNormalText();
  customCin(key, 10);
  showConsoleCursor(false);
}

void dauSachPageAction(BookView &book, int keyPressed) {
  switch (keyPressed) {
    case F1:
      searchDauSach();
      break;

    default:
      break;
  }
}

void initDauSachPage() {
  gotoxy(0, 50);
  for (int i = 0; i < 2; i++) {
    DauSach *ds = new DauSach;
    ds->ISBN = "15-204-19144-56422";
    ds->namXB = 2000 + i;
    ds->soTrang = 99;
    ds->tacGia = "Anh Le";
    ds->ten = "Sach ";
    ds->theLoai = "Ngon lu";
    ds->dms = new DMSach;
    for (int j; j < i + 2; j++) {
      DMSach *dm = new DMSach;
      Sach *sach = new Sach;
      sach->maSach = i * 1000 + j;
      sach->trangThai = j % 3;
      dm->data = sach;
      addLast(ds->dms, sach);
    }
    addLast(listDauSach, ds);
  }

  DauSachBook.left = 1;
  DauSachBook.top = 5;
  DauSachBook.right = 40;
  DauSachBook.bottom = 26;
  DauSachBook.pageSize = 20;
  DauSachBook.lineCount = 10;
  DauSachBook.allPage = (int)ceil(listDauSach.length * 1.0 / DauSachBook.pageSize);
  if (DauSachBook.allPage == 0) DauSachBook.allPage = 1;
  dauSachBookLoad(DauSachBook);
  DauSachContent.top = 3;
  DauSachContent.left = 72;
  DauSachContent.right = 154;
  DauSachContent.bottom = 26;
  DauSachContent.lineCount = 6;
}

void runDauSachPage() {
  loadLayout("layout/DauSach.layout");
  setHeader("Quan ly dau sach");
  runBookView(DauSachBook, dauSachPageAction, dauSachBookLoad, handleBookSelectChange);
  clearPage(1, 3, 154, 26);
}

#endif