#ifndef _THEDOCGIAPAGE_CPP_
#define _THEDOCGIAPAGE_CPP_

#include "../TheDocGia.cpp"
#include "MuonTraPage.cpp"
#include "components/BookView.cpp"
#include "components/ContentView.cpp"
#include "components/YesNoMenu.cpp"

using namespace std;

namespace THEDOCGIAPAGE {

int _top = 3;
int _left = 1;
int _right = 154;
int _bottom = 26;

string _HeaderText = "Quan ly the doc gia";
string _PageLayout = "layout/TheDocGia.layout";

BookView _TheDocGiaBookView;

ContentView _TheDocGiaContentView;

string _DauSachSearchString;

TheDocGia *_CurrentTDG = NULL;

#define NORMAL 0
#define CREATE 1
#define EDIT 2

int MODE = NORMAL;

const vector<string> _TDGBookFooter = {
    "ESC: Tro ve",
    ">>: Trang sau",
    "<<: Trang truoc",
    "F2: Sua",
    "F3: Them moi",
    "F4: Xoa",
    "F6: Chon TDG",
    "ENTER: Xem Danh muc sach"};
const vector<string> _TDGBookSearchFooter = {"ESC: Huy", "ENTER: Tim kiem"};

/* -------------------- _TheDocGiaContentView funtions -------------------- */
void loadContent(BookView &book, ContentView &content)
{
  if (book.lineCount <= 0) {
    _TheDocGiaContentView = getEmptyView(_TheDocGiaContentView);
    return;
  }
  TheDocGia *tdg = find(_ListTheDocGia_root, stoll(book.keys[book.select]))->data;
  _CurrentTDG = tdg;
  _TheDocGiaContentView.lines[0] = to_string(tdg->maThe);
  _TheDocGiaContentView.lines[1] = tdg->ho;
  _TheDocGiaContentView.lines[2] = tdg->ten;
  _TheDocGiaContentView.lines[3] = tdg->phai ? "nam" : "nu";
  _TheDocGiaContentView.lines[4] = to_string(tdg->trangThai);
  MUONTRAPAGE::_ListMuonTra = tdg->lmt;
  MUONTRAPAGE::_CurrentNode = tdg->lmt;
}

void formatTDG(ContentView &content)
{
  chuanHoa(content.lines[1]);
  chuanHoa(content.lines[2]);
}

string checkTDG(ContentView content)
{
  if (content.lines[1].empty()) {
    return "Ho khong duoc rong.";
  }
  if (content.lines[2].empty()) {
    return "Ten khong duoc rong.";
  }
  if (content.lines[3].empty()) {
    return "Phai khong duoc rong.";
  }
  if (content.lines[3].compare("nam") != 0 && content.lines[3].compare("nu") != 0) {
    return "Phai phai la 'nam' hoac 'nu'.";
  }
  if (content.lines[4].compare("0") != 0 && content.lines[4].compare("1") != 0) {
    return "Trang thai chi nhan '0' (hoat dong) hoac '1' (khoa).";
  }
  return "";
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
  tdg->phai = content.lines[3].compare("nam") == 0;
  tdg->trangThai = stoi(content.lines[4]);
  if (MODE == CREATE) {
    insert(_ListTheDocGia_root, tdg);
  }
}

/* -------------------- _TheDocGiaBookView functions -------------------- */
void loadTDGBook(BookView &book)
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
    book.lines[i] = to_string(_TDGArray_temp.data[i]->maThe) + ": " + _TDGArray_temp.data[i]->ho + ' ' + _TDGArray_temp.data[i]->ten;
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

void coppyToClipboard()
{
  clipboardTDG = _CurrentTDG;
}
/* -------------------- _TDGContentView handles -------------------- */
void handleContentAction(ContentView &content, int key, bool &breaker)
{
  switch (key) {
  case F2:
    formatTDG(content);
    string error = checkTDG(content);
    if (!error.empty()) {
      clearContentView(content);
      appPause(error, content.left, content.top);
      drawContentView(content);
      drawFooter(content);
      _gotoSelect(content);
      return;
    }
    updateContent(content);
    clearContentView(_TheDocGiaContentView);
    drawContentView(_TheDocGiaContentView);
    breaker = true;
    break;
  }
}

/* -------------------- _TDGBookView handles -------------------- */
void handleBookSelectChange(BookView &book)
{
  loadContent(book, _TheDocGiaContentView);
  clearContentView(_TheDocGiaContentView);
  drawContentView(_TheDocGiaContentView);
}

void handleBookAction(BookView &book, int keyPressed)
{
  switch (keyPressed) {
  case F2:
    if (_TheDocGiaContentView.lineCount <= 0) break;
    MODE = EDIT;
    runContentViewEditMode(_TheDocGiaContentView, handleContentAction);
    loadTDGBook(_TheDocGiaBookView);
    drawBookView(_TheDocGiaBookView);
    MODE = NORMAL;
    break;
  case F3: {
    MODE = CREATE;
    clearContentView(_TheDocGiaContentView);
    ContentView createCV = getEmptyView(_TheDocGiaContentView);
    createCV.lines[0] = to_string(getNewMaTheDocGia());
    createCV.lines[4] = "0";
    createCV.isEditable[4] = false;
    runContentViewEditMode(createCV, handleContentAction);
    loadTDGBook(_TheDocGiaBookView);
    drawBookView(_TheDocGiaBookView);
    MODE = NORMAL;
  } break;
  case F4:
    if (_TheDocGiaContentView.lineCount <= 0) break;
    clearBookView(_TheDocGiaBookView);
    if (YesNoMenu("Ban co muon xoa dau sach nay?", _TheDocGiaBookView.left, _TheDocGiaBookView.top)) {
      deleteTDG(stoll(book.keys[book.select]));
    }
    loadTDGBook(_TheDocGiaBookView);
    drawBookView(_TheDocGiaBookView);
    break;
  case F6:
    if (_TheDocGiaContentView.lineCount <= 0) break;
    coppyToClipboard();
    break;
  case ENTER:
    if (_TheDocGiaContentView.lineCount <= 0) break;
    clearPage(_left, _top, _right, _bottom);
    MUONTRAPAGE::initMuonTraPage();
    MUONTRAPAGE::runMuonTraPage();
    loadLayout(_PageLayout);
    setHeader(_HeaderText);
    drawBookView(_TheDocGiaBookView);
    drawContentView(_TheDocGiaContentView);
    break;
  }
  setFooter(_TDGBookFooter);
}

/* -------------------- TDGPage functions -------------------- */
void initTheDocGiaPage()
{
  /* init _TheDocGiaBookView */
  _TheDocGiaBookView.left = _left;
  _TheDocGiaBookView.top = _top;
  _TheDocGiaBookView.right = 40;
  _TheDocGiaBookView.bottom = 26;
  _TheDocGiaBookView.pageSize = 20;
  _TheDocGiaBookView.lineCount = 20;

  /* init _TheDocGiaContentView */
  _TheDocGiaContentView.top = _top;
  _TheDocGiaContentView.left = 72;
  _TheDocGiaContentView.right = 154;
  _TheDocGiaContentView.bottom = 26;
  _TheDocGiaContentView.lineCount = 5;
  _TheDocGiaContentView.labelColumnSize = 30;
  _TheDocGiaContentView.labels[0] = "Ma the";
  _TheDocGiaContentView.labels[1] = "Ho";
  _TheDocGiaContentView.labels[2] = "Ten";
  _TheDocGiaContentView.labels[3] = "Phai (nam | nu)";
  _TheDocGiaContentView.labels[4] = "Trang thai (0=h.dong | 1=khoa)";
  _TheDocGiaContentView = getInitalView(_TheDocGiaContentView);
  _TheDocGiaContentView.isNumberType[0] = true;
  _TheDocGiaContentView.isNumberType[4] = true;
  _TheDocGiaContentView.isEditable[0] = false;
  _TheDocGiaContentView.maxLength[2] = 10;
  _TheDocGiaContentView.maxLength[3] = 3;
  _TheDocGiaContentView.maxLength[4] = 1;
}

void runTheDocGiaPage()
{
  /* tinh so trang, dua page index ve 0 */
  resetBookIndex(_TheDocGiaBookView, countAll(_ListTheDocGia_root));
  /* load _TheDocGiaBookView */
  loadTDGBook(_TheDocGiaBookView);
  /* load _TheDocGiaContentView */
  loadContent(_TheDocGiaBookView, _TheDocGiaContentView);

  loadLayout(_PageLayout);
  setHeader(_HeaderText);
  setFooter(_TDGBookFooter);
  drawContentView(_TheDocGiaContentView);
  runBookView(_TheDocGiaBookView, handleBookAction, loadTDGBook, handleBookSelectChange);
  clearPage(_left, _top, _right, _bottom);
}
} // namespace THEDOCGIAPAGE

#endif