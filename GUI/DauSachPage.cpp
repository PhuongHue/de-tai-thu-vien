#ifndef _DAUSACHPAGE_CPP_
#define _DAUSACHPAGE_CPP_

#include <math.h>
#include <sstream>
#include <string>

#include "../DauSach.cpp"
#include "DMSachPage.cpp"
#include "components/BookView.cpp"
#include "components/ContentView.cpp"
#include "components/YesNoMenu.cpp"

using namespace std;

namespace DAUSACHPAGE {

int _left = 1;
int _top = 3;
int _right = 154;
int _bottom = 26;

string _HeaderText = "Quan ly dau sach";
string _PageLayout = "layout/DauSach.layout";

ListDauSach _ListDauSach;

BookView _DauSachBookView;
BookView _defaultDauSachBookView;

ContentView _DauSachContentView;

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
    "ENTER: Xem Danh muc sach"};
const vector<string> _DauSachBookSearchFooter = {"ESC: Huy", "ENTER: Tim kiem"};

/* -------------------- _DauSachContentView funtions -------------------- */
void loadDauSachContent(BookView &book, ContentView &content)
{
  if (book.lineCount <= 0) {
    _DauSachContentView = getEmptyView(_DauSachContentView);
    return;
  }
  DauSach *ds = findDauSachByISBN(_ListDauSach, _DauSachBookView.keys[_DauSachBookView.select]);
  _DauSachContentView.lines[0] = ds->ISBN;
  _DauSachContentView.lines[1] = to_string(ds->namXB);
  _DauSachContentView.lines[2] = to_string(ds->soTrang);
  _DauSachContentView.lines[3] = ds->tacGia;
  _DauSachContentView.lines[4] = ds->tenSach;
  _DauSachContentView.lines[5] = ds->theLoai;
  DMSACHPAGE::_CurrentListDauSach = ds;
  DMSACHPAGE::_CurrentNodeDMSach = ds->dms;
}

void formatDauSach(ContentView &content)
{
  chuanHoa(content.lines[0]);
  chuanHoa(content.lines[3]);
  chuanHoa(content.lines[4]);
  chuanHoa(content.lines[5]);
}

string checkDauSach(ContentView content)
{
  if (content.lines[0].empty()) {
    return "ISBN khong duoc rong.";
  }
  if (content.lines[1].empty()) {
    return "Nam XB khong duoc rong.";
  }
  if (content.lines[2].empty()) {
    return "So trang khong duoc rong.";
  }
  if (content.lines[3].empty()) {
    return "Tac gia khong duoc rong.";
  }
  if (content.lines[4].empty()) {
    return "The loai khong duoc rong.";
  }
  for (int i = 0; i < content.lines[0].length(); i++) {
    if (!((content.lines[0][i] >= '0' && content.lines[0][i] <= '9') || content.lines[0][i] == '-')) {
      // khong phai so va dau '-'
      return "ISBN chi duoc dung so va dau '-'.";
    }
  }
  // TODO: khong trung ISBN
  return "";
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
  ds->tenSach = content.lines[4];
  ds->theLoai = content.lines[5];

  if (BOOK_MODE == BOOK_CREATE) {
    addLast(_ListDauSach_Root, ds);
  }
  sortByTheLoaiTen(_ListDauSach_Root);
  _ListDauSach = _ListDauSach_Root;
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
    book.lines[j] = _ListDauSach.data[i]->ISBN + " - " + _ListDauSach.data[i]->tenSach;
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
  cout << setw(20) << setfill(' ') << ' ' << setw(0);
  gotoxy(11, 3);
  inputText(_DauSachSearchString, 20, 11, 3);
  if (_DauSachSearchString.compare("") == 0) {
    // reset data
    _ListDauSach = _ListDauSach_Root;
    // reset view
    _DauSachBookView = _defaultDauSachBookView;
  }
  else {
    _ListDauSach = filterDauSach(_ListDauSach_Root, _DauSachSearchString);
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
    formatDauSach(content);
    string error = checkDauSach(content);
    if (!error.empty()) {
      clearContentView(content);
      appPause(error, content.left, content.top);
      drawContentView(content);
      _gotoSelect(content);
      return;
    }
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
    if (_DauSachBookView.lineCount == 0) break;
    BOOK_MODE = BOOK_EDIT;
    runContentViewEditMode(_DauSachContentView, handleContentAction);
    loadDauSachBook(_DauSachBookView);
    clearBookView(_DauSachBookView);
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
    if (_DauSachBookView.lineCount == 0) break;
    clearBookView(_DauSachBookView);
    if (YesNoMenu("Ban co muon xoa dau sach nay?", _DauSachBookView.left, _DauSachBookView.top)) {
      deleteDauSach(book.keys[book.select]);
    }
    loadDauSachBook(_DauSachBookView);
    drawBookView(_DauSachBookView);
    break;
  case ENTER:
    if (_DauSachBookView.lineCount == 0) break;
    clearPage(_left, _top, _right, _bottom);
    DMSACHPAGE::initDMSachPage();
    DMSACHPAGE::runDMSachPage();
    loadLayout(_PageLayout);
    setHeader(_HeaderText);
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
  _DauSachBookView.left = _left;
  _DauSachBookView.top = 5;
  _DauSachBookView.right = 70;
  _DauSachBookView.bottom = _bottom;
  _DauSachBookView.pageSize = 20;
  _DauSachBookView.lineCount = 20;
  // tinh so trang, dua page index ve 0
  resetBookIndex(_DauSachBookView, _ListDauSach.length);
  /* load _DauSachBookView */
  loadDauSachBook(_DauSachBookView);
  // backup
  _defaultDauSachBookView = _DauSachBookView;

  /* init _DauSachContentView */
  _DauSachContentView.left = 72;
  _DauSachContentView.top = 3;
  _DauSachContentView.right = _right;
  _DauSachContentView.bottom = _bottom;
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
  _DauSachContentView.maxLength[0] = 13;
  _DauSachContentView.maxLength[1] = 4;
  _DauSachContentView.maxLength[2] = 4;

  /* load _DauSachBookView */
  loadDauSachContent(_DauSachBookView, _DauSachContentView);
}

void runDauSachPage()
{
  loadLayout(_PageLayout);
  setHeader(_HeaderText);
  setFooter(_DauSachBookFooter);
  drawBookView(_DauSachBookView);
  drawContentView(_DauSachContentView);
  runBookView(_DauSachBookView, handleDauSachBookAction, loadDauSachBook, handleBookSelectChange);
  clearPage(_left, _top, _right, _bottom);
}

} // namespace DAUSACHPAGE

#endif