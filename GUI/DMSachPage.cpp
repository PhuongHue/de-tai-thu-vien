#ifndef _DMSACHPAGE_CPP_
#define _DMSACHPAGE_CPP_

#include <math.h>
#include <sstream>
#include <string>

#include "../DauSach.cpp"
#include "components/BookView.cpp"
#include "components/ContentView.cpp"

using namespace std;

ListDauSach _ListDMSach_Root;
ListDauSach _ListDMSach;

BookView _DMSachBookView;
BookView _defaultDMSachBookView;

ContentView _DMSachContentView;
ContentView _defaultDMSachContentView;

string _DauSachSearchString;

#define DMSACH_NORMAL 0
#define DMSACH_CREATE 1
#define DMSACH_EDIT 2

int DMSACH_MODE = DMSACH_NORMAL;

const vector<string> _DauSachBookFooter = {
    "ESC: Tro ve",
    ">>: Trang sau",
    "<<: Trang truoc",
    "F1: Tim kiem",
    "F2: Sua",
    "F3: Them moi",
    "F4: Xoa",
    "F5: Luu"};
const vector<string> _DauSachBookSearchFooter = {"ESC: Huy", "ENTER: Tim kiem"};

/* -------------------- _DMSachContentView funtions -------------------- */
void loadDauSachContent(BookView &book, ContentView &content)
{
  if (book.lineCount <= 0) return;
  DauSach *ds = findDauSachByISBN(_ListDMSach, _DMSachBookView.keys[_DMSachBookView.select]);
  _DMSachContentView.lines[0] = ds->ISBN;
  _DMSachContentView.lines[1] = to_string(ds->namXB);
  _DMSachContentView.lines[2] = to_string(ds->soTrang);
  _DMSachContentView.lines[3] = ds->tacGia;
  _DMSachContentView.lines[4] = ds->ten;
  _DMSachContentView.lines[5] = ds->theLoai;
}

void updateContent(ContentView &content)
{
  int bookIndex = getIndex(_DMSachBookView);
  DauSach *ds;
  if (DMSACH_MODE == DMSACH_EDIT) {
    ds = _ListDMSach.data[bookIndex];
    _DMSachBookView.keys[_DMSachBookView.select] = content.lines[0];
  }
  if (DMSACH_MODE == DMSACH_CREATE) ds = new DauSach;

  ds->ISBN = content.lines[0];
  ds->namXB = stoi(content.lines[1]);
  ds->soTrang = stoi(content.lines[2]);
  ds->tacGia = content.lines[3];
  ds->ten = content.lines[4];
  ds->theLoai = content.lines[5];

  if (DMSACH_MODE == DMSACH_CREATE) {
    addLast(_ListDMSach_Root, ds);
    _ListDMSach = _ListDMSach_Root;
  }
}

/* -------------------- _DMSachBookView functions -------------------- */
void loadDauSachBook(BookView &book)
{
  int newAllPage = countAllPage(_ListDMSach.length, book.pageSize);
  if (newAllPage < book.allPage) {
    book.allPage = newAllPage;
    if (book.pageIndex >= book.allPage)
      book.pageIndex = book.allPage - 1;
  }
  int startIndex = book.pageIndex * book.pageSize;
  int endIndex = startIndex + book.pageSize - 1;
  if (endIndex > _ListDMSach.length - 1) endIndex = _ListDMSach.length - 1;
  book.lineCount = endIndex - startIndex + 1;
  // load data trang moi
  for (int i = startIndex, j = 0; i <= endIndex; i++, j++) {
    book.lines[j] = _ListDMSach.data[i]->ten;
    book.keys[j] = _ListDMSach.data[i]->ISBN;
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
    _ListDMSach = _ListDMSach_Root;
    // reset view
    _DMSachBookView = _defaultDMSachBookView;
  }
  else {
    DauSach ds;
    ds.ISBN = ds.tacGia = ds.ten = ds.theLoai = _DauSachSearchString;
    _ListDMSach = filterDauSach(_ListDMSach_Root, ds);
    _DauSachSearchString = "";
  }
}

void deleteDauSach(string ISBN)
{
  findAndDelete(_ListDMSach_Root, ISBN);
  _ListDMSach = _ListDMSach_Root;
}

/* -------------------- _DMSachContentView handles -------------------- */
void handleContentAction(ContentView &content, int key, bool &breaker)
{
  switch (key) {
  case F2:
    updateContent(content);
    clearContentView(_DMSachContentView);
    drawContentView(_DMSachContentView);
    breaker = true;
    break;
  }
}

/* -------------------- _DMSachBookView handles -------------------- */
void handleBookSelectChange(BookView &book)
{
  loadDauSachContent(book, _DMSachContentView);
  clearContentView(_DMSachContentView);
  drawContentView(_DMSachContentView);
}

void handleDauSachBookAction(BookView &book, int keyPressed)
{
  switch (keyPressed) {
  case F1:
    setFooter(_DauSachBookSearchFooter);
    searchDauSach();
    clearBookView(_DMSachBookView);
    resetBookIndex(_DMSachBookView, _ListDMSach.length);
    loadDauSachBook(_DMSachBookView);
    drawBookView(_DMSachBookView);
    break;
  case F2:
    DMSACH_MODE = DMSACH_EDIT;
    runContentViewEditMode(_DMSachContentView, handleContentAction);
    loadDauSachBook(_DMSachBookView);
    drawBookView(_DMSachBookView);
    DMSACH_MODE = DMSACH_NORMAL;
    break;
  case F3: {
    DMSACH_MODE = DMSACH_CREATE;
    clearContentView(_DMSachContentView);
    ContentView createCV = getEmptyView(_DMSachContentView);
    runContentViewEditMode(createCV, handleContentAction);
    loadDauSachBook(_DMSachBookView);
    drawBookView(_DMSachBookView);
    DMSACH_MODE = DMSACH_NORMAL;
  } break;
  case F4:
    clearBookView(_DMSachBookView);
    if (appYesNo("Ban co muon xoa sach nay?", _DMSachBookView.left, _DMSachBookView.top)) {
      deleteDauSach(book.keys[book.select]);
    }
    loadDauSachBook(_DMSachBookView);
    drawBookView(_DMSachBookView);
    break;
  case F5:
    luuFile(_ListDMSach_Root);
    // thong bao
    clearBookView(_DMSachBookView);
    appPause("Da luu vao file!", _DMSachBookView.left, _DMSachBookView.top);
    drawBookView(_DMSachBookView);
    break;
  }
  setFooter(_DauSachBookFooter);
}

/* -------------------- DMSachPage functions -------------------- */
void initDMSachPage()
{
  /* Copy data */
  _ListDMSach = _ListDMSach_Root;

  /* init _DMSachBookView, _DMSachBookView */
  _DMSachBookView.left = 1;
  _DMSachBookView.top = 5;
  _DMSachBookView.right = 40;
  _DMSachBookView.bottom = 26;
  _DMSachBookView.pageSize = 20;
  _DMSachBookView.lineCount = 20;
  // tinh so trang, dua page index ve 0
  resetBookIndex(_DMSachBookView, _ListDMSach.length);
  /* load _DMSachBookView */
  loadDauSachBook(_DMSachBookView);
  // backup
  _defaultDMSachBookView = _DMSachBookView;

  /* init _DMSachContentView */
  _DMSachContentView.top = 3;
  _DMSachContentView.left = 72;
  _DMSachContentView.right = 154;
  _DMSachContentView.bottom = 26;
  _DMSachContentView.lineCount = 6;
  _DMSachContentView.labelColumnSize = 8;
  _DMSachContentView.labels[0] = "ISBN";
  _DMSachContentView.labels[1] = "Nam XB";
  _DMSachContentView.labels[2] = "So trang";
  _DMSachContentView.labels[3] = "Tac gia";
  _DMSachContentView.labels[4] = "Ten";
  _DMSachContentView.labels[5] = "The loai";
  _DMSachContentView.isNumberType[0] = false;
  _DMSachContentView.isNumberType[1] = true;
  _DMSachContentView.isNumberType[2] = true;
  _DMSachContentView.isNumberType[3] = false;
  _DMSachContentView.isNumberType[4] = false;
  _DMSachContentView.isNumberType[5] = false;
  /* load _DMSachBookView */
  loadDauSachContent(_DMSachBookView, _DMSachContentView);
  // backup
  _defaultDMSachContentView = _DMSachContentView;

  drawContentView(_DMSachContentView);
}

void runDMSachPage()
{
  loadLayout("layout/DauSach.layout");
  setHeader("Quan ly dau sach");
  setFooter(_DauSachBookFooter);
  runBookView(_DMSachBookView, handleDauSachBookAction, loadDauSachBook, handleBookSelectChange);
  clearPage(1, 3, 154, 26);
}

#endif