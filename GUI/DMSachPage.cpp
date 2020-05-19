#ifndef _DMSACHPAGE_CPP_
#define _DMSACHPAGE_CPP_

#include <math.h>
#include <sstream>
#include <string>

#include "../DMSach.cpp"
#include "components/BookView.cpp"
#include "components/ContentView.cpp"

using namespace std;

namespace DMSACHPAGE {

DMSach *_ListDMSach = NULL;
DMSach *_CurrentNodeDMSach = NULL;

BookView _DMSachBookView;
BookView _defaultDMSachBookView;

ContentView _DMSachContentView;
ContentView _defaultDMSachContentView;

string _DauSachSearchString;

#define NORMAL 0
#define CREATE 1
#define DMSACH_EDIT 2

int DMSACH_MODE = NORMAL;

const vector<string> _DauSachBookFooter = {
    "ESC: Tro ve",
    ">>: Trang sau",
    "<<: Trang truoc",
    "F2: Sua",
    "F3: Them moi",
    "F4: Xoa"};

/* -------------------- _DMSachContentView funtions -------------------- */
void loadContent(BookView &book, ContentView &content)
{
  if (book.lineCount <= 0) return;
  DMSach *ds = findByMaSach(_ListDMSach, stoi(book.keys[book.select])).value;
  _CurrentNodeDMSach = ds;
  _DMSachContentView.lines[0] = to_string(ds->data->maSach);
  _DMSachContentView.lines[1] = to_string(ds->data->trangThai);
  _DMSachContentView.lines[2] = ds->data->viTri;
}

void updateContent(ContentView &content)
{
  int bookIndex = getIndex(_DMSachBookView);
  DMSach *ds;
  if (DMSACH_MODE == DMSACH_EDIT) {
    ds = _CurrentNodeDMSach;
  }
  if (DMSACH_MODE == CREATE) ds = new DMSach;

  ds->data->maSach = stoi(content.lines[0]);
  ds->data->trangThai = stoi(content.lines[1]);
  ds->data->viTri = content.lines[2];
  if (DMSACH_MODE == CREATE) {
    addLast(_ListDMSach, ds->data);
  }
}

/* -------------------- _DMSachBookView functions -------------------- */
void loadList(BookView &book)
{
  int newAllPage = countAllPage(countAll(_ListDMSach), book.pageSize);
  if (newAllPage < book.allPage) {
    book.allPage = newAllPage;
    if (book.pageIndex >= book.allPage)
      book.pageIndex = book.allPage - 1;
  }
  int startIndex = book.pageIndex * book.pageSize;
  int endIndex = startIndex + book.pageSize - 1;
  if (endIndex > countAll(_ListDMSach) - 1) endIndex = countAll(_ListDMSach) - 1;
  book.lineCount = endIndex - startIndex + 1;
  // load data trang moi
  int j = 0;
  for (DMSach *i = _ListDMSach; i->next != NULL; i = i->next) {
    book.lines[j] = to_string(i->data->maSach);
    j++;
  }
  // change select
  if (book.lineCount == 0) {
    book.select = 0;
    return;
  }
  if (book.select > book.lineCount - 1) book.select = book.lineCount - 1;
}

// void searchDauSach()
// {
//   gotoxy(11, 3);
//   showConsoleCursor(true);
//   setNormalText();
//   cout << string(20, ' ');
//   gotoxy(11, 3);
//   customCin(_DauSachSearchString, 20);
//   showConsoleCursor(false);
//   if (_DauSachSearchString.compare("") == 0) {
//     // reset data
//     _ListDMSach = _ListDMSach_Root;
//     // reset view
//     _DMSachBookView = _defaultDMSachBookView;
//   }
//   else {
//     DauSach ds;
//     ds.ISBN = ds.tacGia = ds.ten = ds.theLoai = _DauSachSearchString;
//     _ListDMSach = filterDauSach(_ListDMSach_Root, ds);
//     _DauSachSearchString = "";
//   }
// }

void deleteDMSach(string key)
{
  deleteByMaSach(_ListDMSach, stoi(key));
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
  loadContent(book, _DMSachContentView);
  clearContentView(_DMSachContentView);
  drawContentView(_DMSachContentView);
}

void handleDauSachBookAction(BookView &book, int keyPressed)
{
  switch (keyPressed) {
  case F2:
    DMSACH_MODE = DMSACH_EDIT;
    runContentViewEditMode(_DMSachContentView, handleContentAction);
    loadList(_DMSachBookView);
    drawBookView(_DMSachBookView);
    DMSACH_MODE = NORMAL;
    break;
  case F3: {
    DMSACH_MODE = CREATE;
    clearContentView(_DMSachContentView);
    ContentView createCV = getEmptyView(_DMSachContentView);
    runContentViewEditMode(createCV, handleContentAction);
    loadList(_DMSachBookView);
    drawBookView(_DMSachBookView);
    DMSACH_MODE = NORMAL;
  } break;
  case F4:
    clearBookView(_DMSachBookView);
    if (appYesNo("Ban co muon xoa sach nay?", _DMSachBookView.left, _DMSachBookView.top)) {
      deleteDMSach(book.keys[book.select]);
    }
    loadList(_DMSachBookView);
    drawBookView(_DMSachBookView);
    break;
  }
  setFooter(_DauSachBookFooter);
}

/* -------------------- DMSachPage functions -------------------- */
void initDMSachPage()
{
  /* init _DMSachBookView, _DMSachBookView */
  _DMSachBookView.left = 1;
  _DMSachBookView.top = 3;
  _DMSachBookView.right = 40;
  _DMSachBookView.bottom = 26;
  _DMSachBookView.pageSize = 20;
  _DMSachBookView.lineCount = 20;
  // tinh so trang, dua page index ve 0
  resetBookIndex(_DMSachBookView, countAll(_ListDMSach));
  /* load _DMSachBookView */
  loadList(_DMSachBookView);
  // backup
  _defaultDMSachBookView = _DMSachBookView;

  /* init _DMSachContentView */
  _DMSachContentView.top = 3;
  _DMSachContentView.left = 72;
  _DMSachContentView.right = 154;
  _DMSachContentView.bottom = 26;
  _DMSachContentView.lineCount = 6;
  _DMSachContentView.labelColumnSize = 8;
  _DMSachContentView.labels[0] = "Ma Sach";
  _DMSachContentView.labels[1] = "Trang thai";
  _DMSachContentView.labels[2] = "Vi tri";
  _DMSachContentView.isNumberType[0] = true;
  _DMSachContentView.isNumberType[1] = true;
  _DMSachContentView.isNumberType[2] = false;
  /* load _DMSachBookView */
  loadContent(_DMSachBookView, _DMSachContentView);
  // backup
  _defaultDMSachContentView = _DMSachContentView;

  drawContentView(_DMSachContentView);
}

void runDMSachPage()
{
  loadLayout("layout/DMSach.layout");
  setHeader("Quan ly dau sach");
  setFooter(_DauSachBookFooter);
  runBookView(_DMSachBookView, handleDauSachBookAction, loadList, handleBookSelectChange);
  clearPage(1, 3, 154, 26);
}

} // namespace DMSACHPAGE

#endif