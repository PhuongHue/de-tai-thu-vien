#ifndef _DAUSACHPAGE_CPP_
#define _DAUSACHPAGE_CPP_

#include <math.h>
#include <sstream>
#include <string>

#include "../DauSach.cpp"
#include "DMSachPage.cpp"
#include "components/BookView.cpp"
#include "components/ContentView.cpp"

using namespace std;

namespace DAUSACHPAGE {

int _top = 3;
int _left = 1;
int _right = 154;
int _bottom = 26;

ListDauSach _ListDauSach_Root;
ListDauSach _ListDauSach;

BookView _DauSachBookView;
BookView _defaultDauSachBookView;

ContentView _DauSachContentView;
ContentView _defaultDauSachContentView;

string _DauSachSearchString;

#define BOOK_NORMAL 0
#define BOOK_CREATE 1
#define BOOK_EDIT 2

int BOOK_MODE = BOOK_NORMAL;

const vector<string> _DauSachBookFooter = {
    "ESC: Tro ve",
    ">>: Trang sau",
    "<<: Trang truoc",
    "F1: Tim kiem",
    "F2: Sua",
    "F3: Them moi",
    "F4: Xoa",
    "F5: Luu",
    "ENTER: Xem Danh muc sach"};
const vector<string> _DauSachBookSearchFooter = {"ESC: Huy", "ENTER: Tim kiem"};

/* -------------------- _DauSachContentView funtions -------------------- */
void loadDauSachContent(BookView &book, ContentView &content)
{
  if (book.lineCount <= 0) return;
  DauSach *ds = findDauSachByISBN(_ListDauSach, _DauSachBookView.keys[_DauSachBookView.select]);
  _DauSachContentView.lines[0] = ds->ISBN;
  _DauSachContentView.lines[1] = to_string(ds->namXB);
  _DauSachContentView.lines[2] = to_string(ds->soTrang);
  _DauSachContentView.lines[3] = ds->tacGia;
  _DauSachContentView.lines[4] = ds->ten;
  _DauSachContentView.lines[5] = ds->theLoai;
  DMSACHPAGE::_ListDMSach = ds->dms;
  DMSACHPAGE::_CurrentNodeDMSach = ds->dms;
}

void updateContent(ContentView &content)
{
  int bookIndex = getIndex(_DauSachBookView);
  DauSach *ds;
  if (BOOK_MODE == BOOK_EDIT) {
    ds = _ListDauSach.data[bookIndex];
    _DauSachBookView.keys[_DauSachBookView.select] = content.lines[0];
  }
  if (BOOK_MODE == BOOK_CREATE) ds = new DauSach;

  ds->ISBN = content.lines[0];
  ds->namXB = stoi(content.lines[1]);
  ds->soTrang = stoi(content.lines[2]);
  ds->tacGia = content.lines[3];
  ds->ten = content.lines[4];
  ds->theLoai = content.lines[5];

  if (BOOK_MODE == BOOK_CREATE) {
    addLast(_ListDauSach_Root, ds);
    _ListDauSach = _ListDauSach_Root;
  }
}

/* -------------------- _DauSachBookView functions -------------------- */
void loadDauSachBook(BookView &book)
{
  int newAllPage = countAllPage(_ListDauSach.length, book.pageSize);
  if (newAllPage < book.allPage) {
    book.allPage = newAllPage;
    if (book.pageIndex >= book.allPage)
      book.pageIndex = book.allPage - 1;
  }
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

void searchDauSach()
{
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
  }
  else {
    DauSach ds;
    ds.ISBN = ds.tacGia = ds.ten = ds.theLoai = _DauSachSearchString;
    _ListDauSach = filterDauSach(_ListDauSach_Root, ds);
    _DauSachSearchString = "";
  }
}

void deleteDauSach(string ISBN)
{
  findAndDelete(_ListDauSach_Root, ISBN);
  _ListDauSach = _ListDauSach_Root;
}

/* -------------------- _DauSachContentView handles -------------------- */
void handleContentAction(ContentView &content, int key, bool &breaker)
{
  switch (key) {
  case F2:
    updateContent(content);
    clearContentView(_DauSachContentView);
    drawContentView(_DauSachContentView);
    breaker = true;
    break;
  }
}

/* -------------------- _DauSachBookView handles -------------------- */
void handleBookSelectChange(BookView &book)
{
  loadDauSachContent(book, _DauSachContentView);
  clearContentView(_DauSachContentView);
  drawContentView(_DauSachContentView);
}

void handleDauSachBookAction(BookView &book, int keyPressed)
{
  switch (keyPressed) {
  case F1:
    setFooter(_DauSachBookSearchFooter);
    searchDauSach();
    clearBookView(_DauSachBookView);
    resetBookIndex(_DauSachBookView, _ListDauSach.length);
    loadDauSachBook(_DauSachBookView);
    drawBookView(_DauSachBookView);
    break;
  case F2:
    BOOK_MODE = BOOK_EDIT;
    runContentViewEditMode(_DauSachContentView, handleContentAction);
    loadDauSachBook(_DauSachBookView);
    drawBookView(_DauSachBookView);
    BOOK_MODE = BOOK_NORMAL;
    break;
  case F3: {
    BOOK_MODE = BOOK_CREATE;
    clearContentView(_DauSachContentView);
    ContentView createCV = getEmptyView(_DauSachContentView);
    runContentViewEditMode(createCV, handleContentAction);
    loadDauSachBook(_DauSachBookView);
    drawBookView(_DauSachBookView);
    BOOK_MODE = BOOK_NORMAL;
  } break;
  case F4:
    clearBookView(_DauSachBookView);
    if (appYesNo("Ban co muon xoa dau sach nay?", _DauSachBookView.left, _DauSachBookView.top)) {
      deleteDauSach(book.keys[book.select]);
    }
    loadDauSachBook(_DauSachBookView);
    drawBookView(_DauSachBookView);
    break;
  case F5:
    luuFile(_ListDauSach_Root);
    // thong bao
    clearBookView(_DauSachBookView);
    appPause("Da luu vao file!", _DauSachBookView.left, _DauSachBookView.top);
    drawBookView(_DauSachBookView);
    break;
  case ENTER:
    clearPage(_left, _top, _right, _bottom);
    DMSACHPAGE::initDMSachPage();
    DMSACHPAGE::runDMSachPage();
    loadLayout("layout/DauSach.layout");
    setHeader("Quan ly dau sach");
    drawBookView(_DauSachBookView);
    drawContentView(_DauSachContentView);
    break;
  }
  setFooter(_DauSachBookFooter);
}

/* -------------------- DauSachPage functions -------------------- */
void initDauSachPage()
{
  /* Copy data */
  _ListDauSach = _ListDauSach_Root;

  /* init _DauSachBookView, _DauSachBookView */
  _DauSachBookView.left = 1;
  _DauSachBookView.top = 5;
  _DauSachBookView.right = 40;
  _DauSachBookView.bottom = 26;
  _DauSachBookView.pageSize = 20;
  _DauSachBookView.lineCount = 20;
  // tinh so trang, dua page index ve 0
  resetBookIndex(_DauSachBookView, _ListDauSach.length);
  /* load _DauSachBookView */
  loadDauSachBook(_DauSachBookView);
  // backup
  _defaultDauSachBookView = _DauSachBookView;

  /* init _DauSachContentView */
  _DauSachContentView.top = 3;
  _DauSachContentView.left = 72;
  _DauSachContentView.right = 154;
  _DauSachContentView.bottom = 26;
  _DauSachContentView.lineCount = 6;
  _DauSachContentView.labelColumnSize = 8;
  _DauSachContentView.labels[0] = "ISBN";
  _DauSachContentView.labels[1] = "Nam XB";
  _DauSachContentView.labels[2] = "So trang";
  _DauSachContentView.labels[3] = "Tac gia";
  _DauSachContentView.labels[4] = "Ten";
  _DauSachContentView.labels[5] = "The loai";
  _DauSachContentView = getInitalView(_DauSachContentView);
  _DauSachContentView.isNumberType[1] = true;
  _DauSachContentView.isNumberType[2] = true;
  /* load _DauSachBookView */
  loadDauSachContent(_DauSachBookView, _DauSachContentView);
  // backup
  _defaultDauSachContentView = _DauSachContentView;

  drawContentView(_DauSachContentView);
}

void runDauSachPage()
{
  loadLayout("layout/DauSach.layout");
  setHeader("Quan ly dau sach");
  setFooter(_DauSachBookFooter);
  runBookView(_DauSachBookView, handleDauSachBookAction, loadDauSachBook, handleBookSelectChange);
  clearPage(_left, _top, _right, _bottom);
}

} // namespace DAUSACHPAGE

#endif