#ifndef _DMSACHPAGE_CPP_
#define _DMSACHPAGE_CPP_

#include "../TheDocGia.cpp"
#include "components/BookView.cpp"
#include "components/ContentView.cpp"

using namespace std;

namespace THEDOCGIAPAGE {

int _top = 3;
int _left = 1;
int _right = 154;
int _bottom = 26;

BookView _TheDocGiaBookView;

ContentView _DauSachContentView;

string _DauSachSearchString;

TheDocGia *_CurrentTDG = NULL;

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
    "F5: Luu",
    "ENTER: Xem Danh muc sach"};
const vector<string> _DauSachBookSearchFooter = {"ESC: Huy", "ENTER: Tim kiem"};

/* -------------------- _DauSachContentView funtions -------------------- */
void loadDauSachContent(BookView &book, ContentView &content)
{
  if (book.lineCount <= 0) return;
  TheDocGia *tdg = find(_ListTheDocGia_root, stoll(book.keys[book.select]))->data;
  _CurrentTDG = tdg;
  _DauSachContentView.lines[0] = to_string(tdg->maThe);
  _DauSachContentView.lines[1] = tdg->ho;
  _DauSachContentView.lines[2] = tdg->ten;
  _DauSachContentView.lines[3] = tdg->phai ? "Nam" : "Nu";
  _DauSachContentView.lines[4] = to_string(tdg->trangThai);
  // DMSACHPAGE::_ListDMSach = ds->dms;
  // DMSACHPAGE::_CurrentNodeDMSach = ds->dms;
}

void updateContent(ContentView &content)
{
  TheDocGia *tdg;
  if (MODE == EDIT) {
    tdg = _CurrentTDG;
    _TheDocGiaBookView.keys[_TheDocGiaBookView.select] = content.lines[0];
  }
  if (MODE == CREATE) {
    tdg = new TheDocGia;
    tdg->maThe = stoll(content.lines[0]);
  }
  tdg->ho = content.lines[1];
  tdg->ten = content.lines[2];
  tdg->phai = content.lines[3].compare("Nam") == 0;
  tdg->trangThai = stoi(content.lines[4]);
  if (MODE == CREATE) {
    insert(_ListTheDocGia_root, tdg);
  }
}

/* -------------------- _DauSachBookView functions -------------------- */
void loadDauSachBook(BookView &book)
{
  int dataLength = countAll(_ListTheDocGia_root);
  int newAllPage = countAllPage(dataLength, book.pageSize);
  if (newAllPage < book.allPage) {
    book.allPage = newAllPage;
    if (book.pageIndex >= book.allPage)
      book.pageIndex = book.allPage - 1;
  }
  int startIndex = book.pageIndex * book.pageSize;
  int endIndex = startIndex + book.pageSize - 1;
  if (endIndex > dataLength - 1) endIndex = dataLength - 1;
  book.lineCount = endIndex - startIndex + 1;
  // load data trang moi
  initDuyetLNR();
  duyetLNR(_ListTheDocGia_root, startIndex, endIndex);
  for (int i = 0; i < _TDGArray_temp.length; i++) {
    book.lines[i] = _TDGArray_temp.data[i]->maThe;
    book.keys[i] = to_string(_TDGArray_temp.data[i]->maThe);
  }
  // change select
  if (book.lineCount == 0) {
    book.select = 0;
    return;
  }
  if (book.select > book.lineCount - 1) book.select = book.lineCount - 1;
}

void deleteTDG(long long maThe)
{
  bool deleted = false;
  findAndDelete(_ListTheDocGia_root, maThe, deleted);
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
  case F2:
    MODE = EDIT;
    runContentViewEditMode(_DauSachContentView, handleContentAction);
    loadDauSachBook(_TheDocGiaBookView);
    drawBookView(_TheDocGiaBookView);
    MODE = NORMAL;
    break;
  case F3: {
    MODE = CREATE;
    clearContentView(_DauSachContentView);
    ContentView createCV = getEmptyView(_DauSachContentView);
    runContentViewEditMode(createCV, handleContentAction);
    loadDauSachBook(_TheDocGiaBookView);
    drawBookView(_TheDocGiaBookView);
    MODE = NORMAL;
  } break;
  case F4:
    clearBookView(_TheDocGiaBookView);
    if (appYesNo("Ban co muon xoa dau sach nay?", _TheDocGiaBookView.left, _TheDocGiaBookView.top)) {
      deleteTDG(stoll(book.keys[book.select]));
    }
    loadDauSachBook(_TheDocGiaBookView);
    drawBookView(_TheDocGiaBookView);
    break;
    case F5:
      luuFile(_ListTheDocGia_root);
      // thong bao
      clearBookView(_TheDocGiaBookView);
      appPause("Da luu vao file!", _TheDocGiaBookView.left, _TheDocGiaBookView.top);
      drawBookView(_TheDocGiaBookView);
      break;
    // case ENTER:
    //   clearPage(_left, _top, _right, _bottom);
    //   DMSACHPAGE::initDMSachPage();
    //   DMSACHPAGE::runDMSachPage();
    //   loadLayout("layout/DauSach.layout");
    //   setHeader("Quan ly dau sach");
    //   drawBookView(_TheDocGiaBookView);
    //   drawContentView(_DauSachContentView);
    //   break;
  }
  setFooter(_DauSachBookFooter);
}

/* -------------------- DauSachPage functions -------------------- */
void initDauSachPage()
{
  /* init _DauSachBookView, _DauSachBookView */
  _TheDocGiaBookView.left = 1;
  _TheDocGiaBookView.top = 5;
  _TheDocGiaBookView.right = 40;
  _TheDocGiaBookView.bottom = 26;
  _TheDocGiaBookView.pageSize = 20;
  _TheDocGiaBookView.lineCount = 20;
  // tinh so trang, dua page index ve 0
  resetBookIndex(_TheDocGiaBookView, countAll(_ListTheDocGia_root));
  /* load _DauSachBookView */
  loadDauSachBook(_TheDocGiaBookView);
  /* init _DauSachContentView */
  _DauSachContentView.top = 3;
  _DauSachContentView.left = 72;
  _DauSachContentView.right = 154;
  _DauSachContentView.bottom = 26;
  _DauSachContentView.lineCount = 6;
  _DauSachContentView.labelColumnSize = 8;
  _DauSachContentView.labels[0] = "Ma the";
  _DauSachContentView.labels[1] = "Ho";
  _DauSachContentView.labels[2] = "Ten";
  _DauSachContentView.labels[3] = "Phai";
  _DauSachContentView.labels[4] = "Trang thai";
  _DauSachContentView = getInitalView(_DauSachContentView);
  _DauSachContentView.isNumberType[0] = true;
  _DauSachContentView.isNumberType[4] = true;
  /* load _DauSachBookView */
  loadDauSachContent(_TheDocGiaBookView, _DauSachContentView);
  drawContentView(_DauSachContentView);
}

void runDauSachPage()
{
  loadLayout("layout/DauSach.layout");
  setHeader("Quan ly dau sach");
  setFooter(_DauSachBookFooter);
  runBookView(_TheDocGiaBookView, handleDauSachBookAction, loadDauSachBook, handleBookSelectChange);
  clearPage(_left, _top, _right, _bottom);
}
} // namespace THEDOCGIAPAGE

#endif