#ifndef _MUONTRAPAGE_CPP_
#define _MUONTRAPAGE_CPP_

#include <math.h>
#include <sstream>
#include <string>

#include "../MuonTra.cpp"
#include "../StringLib.cpp"
#include "components/BookView.cpp"
#include "components/ContentView.cpp"

using namespace std;

namespace MUONTRAPAGE {

int _top = 3;
int _left = 1;
int _right = 154;
int _bottom = 26;

ListMuonTra *_ListMuonTra = NULL;
ListMuonTra *_CurrentNodeMuonTra = NULL;

BookView _DMSachBookView;
BookView _defaultDMSachBookView;

ContentView _MuonTraContentView;
ContentView _defaultMuonTraContentView;

#define NORMAL 0
#define CREATE 1
#define EDIT 2

int DMSACH_MODE = NORMAL;

const vector<string> _DauSachBookFooter = {
    "ESC: Tro ve",
    ">>: Trang sau",
    "<<: Trang truoc",
    "F2: Sua",
    "F3: Them moi",
    "F4: Xoa"};

/* -------------------- _MuonTraContentView funtions -------------------- */
void loadContent(BookView &book, ContentView &content)
{
  if (book.lineCount <= 0) return;
  long long key;
  try {
    key = stoll(book.keys[book.select]);
  }
  catch (const exception &e) {
    return;
  }

  ListMuonTra *node = findByMaSach(_ListMuonTra, key);
  _CurrentNodeMuonTra = node;
  _MuonTraContentView.lines[0] = to_string(node->data->maSach);
  _MuonTraContentView.lines[1] = getDateString(node->data->ngayMuon);
  _MuonTraContentView.lines[2] = getDateString(node->data->ngayTra);
  ;
  _MuonTraContentView.lines[3] = to_string(node->data->trangThai);
}

void updateContent(ContentView &content)
{
  int bookIndex = getIndex(_DMSachBookView);
  MuonTra *mt;
  if (DMSACH_MODE == EDIT) {
    mt = _CurrentNodeMuonTra->data;
  }
  if (DMSACH_MODE == CREATE) {
    mt = new MuonTra;
  }

  mt->maSach = stoll(content.lines[0]);
  mt->ngayMuon = getDateNumber(content.lines[1]);
  mt->ngayTra = getDateNumber(content.lines[2]);
  mt->trangThai = stoi(content.lines[3]);
  if (DMSACH_MODE == CREATE) {
    addLast(_ListMuonTra, mt);
  }
}

/* -------------------- _DMSachBookView functions -------------------- */
void loadList(BookView &book)
{
  int dataCount = countAll(_ListMuonTra);
  int newAllPage = countAllPage(dataCount, book.pageSize);
  book.allPage = newAllPage;
  if (book.pageIndex >= book.allPage)
    book.pageIndex = book.allPage - 1;

  int startIndex = book.pageIndex * book.pageSize;
  int endIndex = startIndex + book.pageSize - 1;
  if (endIndex > dataCount - 1) endIndex = dataCount - 1;
  book.lineCount = endIndex - startIndex + 1;
  // load data trang moi
  int j = 0;
  for (ListMuonTra *i = _ListMuonTra; i != NULL; i = i->next) {
    if (j < startIndex) continue;
    if (j > endIndex) break;
    string maSach = to_string(i->data->maSach);
    book.lines[j - startIndex] = maSach;
    book.keys[j - startIndex] = maSach;
    j++;
  }
  // change select
  if (book.lineCount == 0) {
    book.select = 0;
    return;
  }
  if (book.select > book.lineCount - 1) book.select = book.lineCount - 1;
}

void deleteDMSach(string key)
{
  deleteByMaSach(_ListMuonTra, stoll(key));
}

/* -------------------- _MuonTraContentView handles -------------------- */
void handleContentAction(ContentView &content, int key, bool &breaker)
{
  switch (key) {
  case F2:
    updateContent(content);
    clearContentView(_MuonTraContentView);
    drawContentView(_MuonTraContentView);
    breaker = true;
    break;
  }
}

/* -------------------- _DMSachBookView handles -------------------- */
void handleSelectChange(BookView &book)
{
  loadContent(book, _MuonTraContentView);
  clearContentView(_MuonTraContentView);
  drawContentView(_MuonTraContentView);
}

void handleListAction(BookView &book, int keyPressed)
{
  switch (keyPressed) {
  case F2:
    DMSACH_MODE = EDIT;
    runContentViewEditMode(_MuonTraContentView, handleContentAction);
    loadList(_DMSachBookView);
    drawBookView(_DMSachBookView);
    DMSACH_MODE = NORMAL;
    break;
  case F3: {
    DMSACH_MODE = CREATE;
    clearContentView(_MuonTraContentView);
    ContentView createCV = getEmptyView(_MuonTraContentView);
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
  resetBookIndex(_DMSachBookView, countAll(_ListMuonTra));
  /* load _DMSachBookView */
  loadList(_DMSachBookView);
  // backup
  _defaultDMSachBookView = _DMSachBookView;

  /* init _MuonTraContentView */
  _MuonTraContentView.top = 3;
  _MuonTraContentView.left = 72;
  _MuonTraContentView.right = 154;
  _MuonTraContentView.bottom = 26;
  _MuonTraContentView.lineCount = 3;
  _MuonTraContentView.labelColumnSize = 10;
  _MuonTraContentView.labels[0] = "Ma Sach";
  _MuonTraContentView.labels[1] = "Ngay muon";
  _MuonTraContentView.labels[2] = "Ngay tra";
  _MuonTraContentView.labels[3] = "Trang thai";
  _MuonTraContentView = getInitalView(_MuonTraContentView);
  _MuonTraContentView.isNumberType[0] = true;
  _MuonTraContentView.isNumberType[3] = true;
  _MuonTraContentView.isEditable[0] = false;
  /* load _DMSachBookView */
  loadContent(_DMSachBookView, _MuonTraContentView);
  // backup
  _defaultMuonTraContentView = _MuonTraContentView;

  drawContentView(_MuonTraContentView);
}

void runDMSachPage()
{
  loadLayout("layout/DMSach.layout");
  setHeader("Quan ly dau sach");
  setFooter(_DauSachBookFooter);
  runBookView(_DMSachBookView, handleListAction, loadList, handleSelectChange);
  clearPage(_left, _top, _right, _bottom);
}
} // namespace MUONTRAPAGE

#endif