#ifndef _DAUSACHPAGE_CPP_
#define _DAUSACHPAGE_CPP_

#include <math.h>

#include <sstream>
#include <string>

#include "../DauSach.cpp"
#include "components/BookView.cpp"
#include "components/ContentView.cpp"

using namespace std;

ListDauSach _ListDauSach_Root;
ListDauSach _ListDauSach;

BookView _DauSachBookView;
BookView _defaultDauSachBookView;

ContentView _DauSachContentView;
ContentView _defaultDauSachContentView;
string _DauSachSearchString;

const vector<string> _DauSachBookFooter = {"ESC: Tro ve", "\xAE: Trang sau", "\xAF: Trang truoc", "F2: Luu"};
const vector<string> _DauSachBookSearchFooter = {"ESC: Huy", "ENTER: Tim kiem"};

/**
 * _DauSachContentView funtions
 */
void loadDauSachContent(BookView &book, ContentView &content) {
  if (book.lineCount <= 0) return;
  DauSach *ds = findDauSachByISBN(_ListDauSach, _DauSachBookView.keys[_DauSachBookView.select]);
  _DauSachContentView.lines[0] = ds->ISBN;
  _DauSachContentView.lines[1] = to_string(ds->namXB);
  _DauSachContentView.lines[2] = to_string(ds->soTrang);
  _DauSachContentView.lines[3] = ds->tacGia;
  _DauSachContentView.lines[4] = ds->ten;
  _DauSachContentView.lines[5] = ds->theLoai;
}

/**
 * _DauSachBookView functions
 */

void loadDauSachBook(BookView &book) {
  int startIndex = book.pageIndex * book.pageSize;
  int endIndex = startIndex + book.pageSize - 1;
  if (endIndex > _ListDauSach.length - 1) endIndex = _ListDauSach.length - 1;
  book.lineCount = endIndex - startIndex + 1;
  // load data trang moi
  for (int i = startIndex, j = 0; i <= endIndex; i++, j++) {
    book.lines[j] = _ListDauSach.data[i]->ten;
    book.keys[j] = _ListDauSach.data[i]->ISBN;
  }
  // change select
  if (book.lineCount == 0) {
    book.select = 0;
    return;
  }
  if (book.select > book.lineCount - 1) book.select = book.lineCount - 1;
}

void handleBookSelectChange(BookView &book) {
  loadDauSachContent(book, _DauSachContentView);
  clearContentView(_DauSachContentView);
  drawContentView(_DauSachContentView);
}

void searchDauSach() {
  gotoxy(11, 3);
  showConsoleCursor(true);
  setNormalText();
  cout << string(20, ' ');
  gotoxy(11, 3);
  customCin(_DauSachSearchString, 20);
  showConsoleCursor(false);
  if (_DauSachSearchString.compare("") == 0) {
    // reset data
    _ListDauSach = _ListDauSach_Root;
    // reset view
    _DauSachBookView = _defaultDauSachBookView;
    log("string empty");
  } else {
    DauSach ds;
    ds.ISBN = ds.tacGia = ds.ten = ds.theLoai = _DauSachSearchString;
    _ListDauSach = filterDauSach(_ListDauSach, ds);
    _DauSachSearchString = "";
    log(_ListDauSach);
  }
  // load data
  clearBookView(_DauSachBookView);
  resetBookIndex(_DauSachBookView, _ListDauSach.length);
  loadDauSachBook(_DauSachBookView);
  drawBookView(_DauSachBookView);
}

void dauSachPageAction(BookView &book, int keyPressed) {
  switch (keyPressed) {
    case F1:
      setFooter(_DauSachBookSearchFooter);
      searchDauSach();
      break;
    case F2:
      luuFile(_ListDauSach_Root);
      break;
    default:
      break;
  }
  setFooter(_DauSachBookFooter);
}

/* Load lan dau */
void initDauSachPage() {
  /* Copy data */
  _ListDauSach = _ListDauSach_Root;

  /* init _defaultDauSachBookView, _DauSachBookView */
  _defaultDauSachBookView.left = 1;
  _defaultDauSachBookView.top = 5;
  _defaultDauSachBookView.right = 40;
  _defaultDauSachBookView.bottom = 26;
  _defaultDauSachBookView.pageSize = 20;
  _defaultDauSachBookView.lineCount = 20;
  // tinh so trang, dua page index ve 0
  resetBookIndex(_defaultDauSachBookView, _ListDauSach.length);
  /* load _DauSachBookView */
  loadDauSachBook(_defaultDauSachBookView);
  _DauSachBookView = _defaultDauSachBookView;

  /* init _defaultDauSachContentView */
  _defaultDauSachContentView.top = 3;
  _defaultDauSachContentView.left = 72;
  _defaultDauSachContentView.right = 154;
  _defaultDauSachContentView.bottom = 26;
  _defaultDauSachContentView.lineCount = 6;
  /* load _DauSachBookView */
  loadDauSachContent(_DauSachBookView, _DauSachContentView);
  _DauSachContentView = _defaultDauSachContentView;

  drawContentView(_DauSachContentView);
}

void runDauSachPage() {
  loadLayout("layout/DauSach.layout");
  setHeader("Quan ly dau sach");
  setFooter(_DauSachBookFooter);
  runBookView(_DauSachBookView, dauSachPageAction, loadDauSachBook, handleBookSelectChange);
  clearPage(1, 3, 154, 26);
}

#endif