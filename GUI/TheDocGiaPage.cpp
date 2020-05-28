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

#define NORMAL 0
#define CREATE 1
#define EDIT 2

int BOOK_MODE = NORMAL;

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
  // DauSach *ds = findDauSachByISBN(_ListDauSach, _TheDocGiaBookView.keys[_TheDocGiaBookView.select]);
  _DauSachContentView.lines[0] = ds->ISBN;
  _DauSachContentView.lines[1] = to_string(ds->namXB);
  _DauSachContentView.lines[2] = to_string(ds->soTrang);
  _DauSachContentView.lines[3] = ds->tacGia;
  _DauSachContentView.lines[4] = ds->ten;
  _DauSachContentView.lines[5] = ds->theLoai;
  // DMSACHPAGE::_ListDMSach = ds->dms;
  // DMSACHPAGE::_CurrentNodeDMSach = ds->dms;
}

void updateContent(ContentView &content)
{
  int bookIndex = getIndex(_TheDocGiaBookView);
  DauSach *ds;
  if (BOOK_MODE == EDIT) {
    ds = _ListDauSach.data[bookIndex];
    _TheDocGiaBookView.keys[_TheDocGiaBookView.select] = content.lines[0];
  }
  if (BOOK_MODE == CREATE) ds = new DauSach;

  ds->ISBN = content.lines[0];
  ds->namXB = stoi(content.lines[1]);
  ds->soTrang = stoi(content.lines[2]);
  ds->tacGia = content.lines[3];
  ds->ten = content.lines[4];
  ds->theLoai = content.lines[5];

  if (BOOK_MODE == CREATE) {
    addLast(_ListDauSach_Root, ds);
    _ListDauSach = _ListDauSach_Root;
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
    BOOK_MODE = EDIT;
    runContentViewEditMode(_DauSachContentView, handleContentAction);
    loadDauSachBook(_TheDocGiaBookView);
    drawBookView(_TheDocGiaBookView);
    BOOK_MODE = NORMAL;
    break;
  case F3: {
    BOOK_MODE = CREATE;
    clearContentView(_DauSachContentView);
    ContentView createCV = getEmptyView(_DauSachContentView);
    runContentViewEditMode(createCV, handleContentAction);
    loadDauSachBook(_TheDocGiaBookView);
    drawBookView(_TheDocGiaBookView);
    BOOK_MODE = NORMAL;
  } break;
  case F4:
    clearBookView(_TheDocGiaBookView);
    if (appYesNo("Ban co muon xoa dau sach nay?", _TheDocGiaBookView.left, _TheDocGiaBookView.top)) {
      deleteTDG(stoll(book.keys[book.select]));
    }
    loadDauSachBook(_TheDocGiaBookView);
    drawBookView(_TheDocGiaBookView);
    break;
    // case F5:
    //   luuFile(_ListTheDocGia_root);
    //   // thong bao
    //   clearBookView(_TheDocGiaBookView);
    //   appPause("Da luu vao file!", _TheDocGiaBookView.left, _TheDocGiaBookView.top);
    //   drawBookView(_TheDocGiaBookView);
    //   break;
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