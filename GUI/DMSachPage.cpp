#ifndef _DMSACHPAGE_CPP_
#define _DMSACHPAGE_CPP_

#include <math.h>
#include <sstream>
#include <string>

#include "../DMSach.cpp"
#include "../DauSach.cpp"
#include "../StringLib.cpp"
#include "components/BookView.cpp"
#include "components/ContentView.cpp"
#include "components/YesNoMenu.cpp"

using namespace std;

namespace DMSACHPAGE {

int _top = 3;
int _left = 1;
int _right = 154;
int _bottom = 26;

string _HeaderText = "Danh muc sach";
string _PageLayout = "layout/DMSach.layout";

DauSach *_CurrentListDauSach = NULL;
DMSach *_CurrentNodeDMSach = NULL;

BookView _DMSachBookView;

ContentView _DMSachContentView;

#define NORMAL 0
#define CREATE 1
#define EDIT 2

int MODE = NORMAL;

const vector<string> _DauSachBookFooter = {
    "ESC: Tro ve",
    ">>: Trang sau",
    "<<: Trang truoc",
    "F2: Sua",
    "F3: Them moi",
    "F4: Xoa",
    "F5: Thanh ly sach",
    "F6: Chon sach"};

/* -------------------- _DMSachContentView funtions -------------------- */
void loadContent(BookView &book, ContentView &content)
{
  if (book.lineCount == 0) {
    _CurrentNodeDMSach = NULL;
    _DMSachContentView = getEmptyView(_DMSachContentView);
    return;
  }
  DMSach *ds = findByMaSach(_CurrentListDauSach->dms, stoll(book.keys[book.select]));
  if (!ds) return;
  _CurrentNodeDMSach = ds;
  _DMSachContentView.lines[0] = to_string(ds->data->maSach);
  switch (ds->data->trangThai) {
  case SACH_TT_MUONDUOC:
    _DMSachContentView.lines[1] = "Muon duoc";
    break;
  case SACH_TT_DAMUON:
    _DMSachContentView.lines[1] = "Da duoc muon";
    break;
  case SACH_TT_THANHLY:
    _DMSachContentView.lines[1] = "Da thanh ly";
    break;
  }
  _DMSachContentView.lines[2] = ds->data->viTri;
}

void formatDMSach(ContentView &content)
{
  chuanHoa(content.lines[2]);
}

string checkDMSach(ContentView content)
{
  if (content.lines[2].empty()) {
    return "Vi tri khong duoc rong.";
  }
  // TODO: khong xoa khi co lich su muon
  return "";
}

void updateContent(ContentView &content)
{
  int bookIndex = getIndex(_DMSachBookView);
  Sach *sach;
  if (MODE == EDIT) {
    sach = _CurrentNodeDMSach->data;
  }
  if (MODE == CREATE) {
    sach = new Sach;
  }

  sach->maSach = stoll(content.lines[0]);
  sach->viTri = content.lines[2];
  if (MODE == CREATE) {
    sach->trangThai = SACH_TT_MUONDUOC;
    addLast(_CurrentListDauSach->dms, sach);
  }
}

void thanhLySach()
{
  clearContentView(_DMSachContentView);
  if (_CurrentNodeDMSach->data->trangThai == SACH_TT_MUONDUOC) {
    if (YesNoMenu("Ban co muon thanh ly sach nay?", _DMSachContentView.left, _DMSachContentView.top)) {
      _CurrentNodeDMSach->data->trangThai = SACH_TT_THANHLY;
      loadContent(_DMSachBookView, _DMSachContentView);
    }
  }
  else {
    if (_CurrentNodeDMSach->data->trangThai == SACH_TT_DAMUON) {
      appPause("Sach da muon khong duoc thanh ly.", _DMSachContentView.left, _DMSachContentView.top);
    }
    else {
      appPause("Sach nay da duoc thanh ly roi, khong duoc thanh ly nua!", _DMSachContentView.left, _DMSachContentView.top);
    }
  }
  drawContentView(_DMSachContentView);
}

/* -------------------- _DMSachBookView functions -------------------- */
void loadList(BookView &book)
{
  int dataCount = countAll(_CurrentListDauSach->dms);
  book.allPage = countAllPage(dataCount, book.pageSize);
  if (book.pageIndex >= book.allPage)
    book.pageIndex = book.allPage - 1;

  int startIndex = book.pageIndex * book.pageSize;
  int endIndex = startIndex + book.pageSize - 1;
  if (endIndex > dataCount - 1) endIndex = dataCount - 1;
  book.lineCount = endIndex - startIndex + 1;
  // load data trang moi
  int j = 0;
  for (DMSach *i = _CurrentListDauSach->dms; i != NULL; i = i->next, j++) {
    if (j >= startIndex) {
      if (j > endIndex) break;
      string maSach = to_string(i->data->maSach);
      book.lines[j - startIndex] = maSach;
      book.keys[j - startIndex] = maSach;
    }
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
  deleteByMaSach(_CurrentListDauSach->dms, stoll(key));
}

void coppyToClipboard()
{
  if (_CurrentNodeDMSach == NULL || _CurrentListDauSach == NULL) return;
  clipboardDauSach = _CurrentListDauSach;
  clipboardSach = _CurrentNodeDMSach->data;
}

/* -------------------- _DMSachContentView handles -------------------- */
void handleContentAction(ContentView &content, int key, bool &breaker)
{
  switch (key) {
  case F2:
    formatDMSach(content);
    string error = checkDMSach(content);
    if (!error.empty()) {
      clearContentView(content);
      appPause(error, content.left, content.top);
      drawContentView(content);
      drawFooter(content);
      _gotoSelect(content);
      return;
    }
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
    if (_DMSachBookView.lineCount == 0) break;
    MODE = EDIT;
    runContentViewEditMode(_DMSachContentView, handleContentAction);
    clearBookView(_DMSachBookView);
    loadList(_DMSachBookView);
    drawBookView(_DMSachBookView);
    MODE = NORMAL;
    break;
  case F3: {
    MODE = CREATE;
    clearContentView(_DMSachContentView);
    ContentView createCV = getEmptyView(_DMSachContentView);
    createCV.lines[0] = to_string(getNewMaSach());
    createCV.lines[1] = "Muon duoc";
    createCV.isEditable[1] = false;
    runContentViewEditMode(createCV, handleContentAction);
    clearBookView(_DMSachBookView);
    loadList(_DMSachBookView);
    drawBookView(_DMSachBookView);
    MODE = NORMAL;
    break;
  }
  case F4:
    if (_DMSachBookView.lineCount == 0) break;
    clearBookView(_DMSachBookView);
    if (YesNoMenu("Ban co muon xoa sach nay?", _DMSachBookView.left, _DMSachBookView.top)) {
      deleteDMSach(book.keys[book.select]);
    }
    loadList(_DMSachBookView);
    drawBookView(_DMSachBookView);
    break;
  case F5:
    thanhLySach();
    break;
  case F6:
    if (_DMSachBookView.lineCount == 0) break;
    coppyToClipboard();
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
  _DMSachBookView.right = 70;
  _DMSachBookView.bottom = 26;
  _DMSachBookView.pageSize = 20;
  _DMSachBookView.lineCount = 20;
  // tinh so trang, dua page index ve 0
  resetBookIndex(_DMSachBookView, countAll(_CurrentListDauSach->dms));
  /* load _DMSachBookView */
  loadList(_DMSachBookView);

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
  _DMSachContentView.isEditable[0] = false;
  _DMSachContentView.isEditable[1] = false;

  /* load _DMSachBookView */
  loadContent(_DMSachBookView, _DMSachContentView);
}

void runDMSachPage()
{
  loadLayout(_PageLayout);
  setHeader(_HeaderText);
  setFooter(_DauSachBookFooter);
  drawContentView(_DMSachContentView);
  runBookView(_DMSachBookView, handleListAction, loadList, handleSelectChange);
  clearPage(_left, _top, _right, _bottom);
}
} // namespace DMSACHPAGE

#endif