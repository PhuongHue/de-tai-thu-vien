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

ContentView _DMSachContentView;
ContentView _defaultDMSachContentView;

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

/* -------------------- _DMSachContentView funtions -------------------- */
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

  DMSach *ds = findByMaSach(_ListMuonTra, key).value;
  _CurrentNodeMuonTra = ds;
  _DMSachContentView.lines[0] = to_string(ds->data->maSach);
  _DMSachContentView.lines[1] = to_string(ds->data->trangThai);
  _DMSachContentView.lines[2] = ds->data->viTri;
}

void updateContent(ContentView &content)
{
  int bookIndex = getIndex(_DMSachBookView);
  MuonTra *sach;
  if (DMSACH_MODE == EDIT) {
    sach = _CurrentNodeMuonTra->data;
  }
  if (DMSACH_MODE == CREATE) {
    sach = new MuonTra;
  }

  sach->maSach = stoll(content.lines[0]);
  sach->ngayMuon
  sach->trangThai = stoi(content.lines[1]);
  if (DMSACH_MODE == CREATE) {
    consoleLog<long long>(sach->maSach);
    consoleLog<int>(sach->trangThai);
    addLast(_ListMuonTra, sach);
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
  for (DMSach *i = _ListMuonTra; i != NULL; i = i->next) {
    string maSach = to_string(i->data->maSach);
    book.lines[j] = maSach;
    book.keys[j] = maSach;
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
void handleSelectChange(BookView &book)
{
  loadContent(book, _DMSachContentView);
  clearContentView(_DMSachContentView);
  drawContentView(_DMSachContentView);
}

void handleListAction(BookView &book, int keyPressed)
{
  switch (keyPressed) {
  case F2:
    DMSACH_MODE = EDIT;
    runContentViewEditMode(_DMSachContentView, handleContentAction);
    loadList(_DMSachBookView);
    drawBookView(_DMSachBookView);
    DMSACH_MODE = NORMAL;
    break;
  case F3: {
    DMSACH_MODE = CREATE;
    clearContentView(_DMSachContentView);
    ContentView createCV = getEmptyView(_DMSachContentView);
    createCV.lines[0] = to_string(getNewMaSach());
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

  /* init _DMSachContentView */
  _DMSachContentView.top = 3;
  _DMSachContentView.left = 72;
  _DMSachContentView.right = 154;
  _DMSachContentView.bottom = 26;
  _DMSachContentView.lineCount = 3;
  _DMSachContentView.labelColumnSize = 10;
  _DMSachContentView.labels[0] = "Ma Sach";
  _DMSachContentView.labels[1] = "Trang thai";
  _DMSachContentView.labels[2] = "Vi tri";
  _DMSachContentView = getInitalView(_DMSachContentView);
  _DMSachContentView.isNumberType[0] = true;
  _DMSachContentView.isNumberType[1] = true;
  _DMSachContentView.isEditable[0] = false;
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
  runBookView(_DMSachBookView, handleListAction, loadList, handleSelectChange);
  clearPage(_left, _top, _right, _bottom);
}
} // namespace DMSACHPAGE

#endif