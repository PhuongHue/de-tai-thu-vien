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

NodeTheDocGia *_CurrentNodeTDG = NULL;

#define NORMAL 0
#define CREATE 1
#define EDIT 2

int MODE = NORMAL;

int _SelectFooter = 0;
const vector<string> _TDGBookFooter = {
    "ESC: Tro ve",
    ">>: Trang sau",
    "<<: Trang truoc",
    "F2: Sua",
    "F3: Them moi",
    "F4: Xoa",
    "F6: Chon TDG",
    "ENTER: Xem Danh muc sach"};

const vector<string> _TDGBookFooter_KhoaThe = {
    "ESC: Tro ve",
    ">>: Trang sau",
    "<<: Trang truoc",
    "F2: Sua",
    "F3: Them moi",
    "F4: Xoa",
    "F5: Mo khoa",
    "F6: Chon TDG",
    "ENTER: Xem Danh muc sach"};
const vector<string> _TDGBookSearchFooter = {"ESC: Huy", "ENTER: Tim kiem"};

/* -------------------- _TheDocGiaContentView funtions -------------------- */
void loadContent(BookView &book, ContentView &content)
{
  if (book.lineCount == 0) {
    _TheDocGiaContentView = getEmptyView(_TheDocGiaContentView);
    return;
  }
  NodeTheDocGia *tdg = find(_ListTheDocGia_root, stoll(book.keys[book.select]));
  _CurrentNodeTDG = tdg;
  _TheDocGiaContentView.lines[0] = to_string(tdg->data.maThe);
  _TheDocGiaContentView.lines[1] = tdg->data.ho;
  _TheDocGiaContentView.lines[2] = tdg->data.ten;
  _TheDocGiaContentView.lines[3] = tdg->data.phai ? "Nam" : "Nu";
  switch (tdg->data.trangThai) {
  case TDG_TT_HOATDONG:
    _TheDocGiaContentView.lines[4] = "Hoat dong";
    _SelectFooter = 0;
    break;
  case TDG_TT_KHOA:
    _TheDocGiaContentView.lines[4] = "Da khoa";
    _SelectFooter = 1;
    break;
  }
  MUONTRAPAGE::_CurrentTDG = tdg;
  MUONTRAPAGE::_ListMuonTra = tdg->data.lmt;
  MUONTRAPAGE::_CurrentNode = tdg->data.lmt;
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
  string phai = toUpperCase(content.lines[3]);
  if (phai.compare("NAM") != 0 && phai.compare("NU") != 0) {
    return "Phai phai la 'nam' hoac 'nu'.";
  }
  return "";
}

void updateContent(ContentView &content)
{
  if (MODE == EDIT) {
    _CurrentNodeTDG->data.ho = content.lines[1];
    _CurrentNodeTDG->data.ten = content.lines[2];
    if (toUpperCase(content.lines[3]).compare("NAM") == 0)
      _CurrentNodeTDG->data.phai = true;
    else
      _CurrentNodeTDG->data.phai = false;
  }
  else {
    TheDocGia tdg;
    tdg.maThe = stoll(content.lines[0]);
    tdg.ho = content.lines[1];
    tdg.ten = content.lines[2];
    if (toUpperCase(content.lines[3]).compare("NAM") == 0)
      tdg.phai = true;
    else
      tdg.phai = false;
    tdg.trangThai = TDG_TT_HOATDONG;
    insert(_ListTheDocGia_root, tdg);
  }
}

/* -------------------- _TheDocGiaBookView functions -------------------- */
void loadTDGBook(BookView &book)
{
  int dataLength = countAll(_ListTheDocGia_root);
  book.allPage = countAllPage(dataLength, book.pageSize);

  if (book.pageIndex >= book.allPage)
    book.pageIndex = book.allPage - 1;

  int startIndex = book.pageIndex * book.pageSize;
  int endIndex = startIndex + book.pageSize - 1;
  if (endIndex > dataLength - 1) endIndex = dataLength - 1;
  book.lineCount = endIndex - startIndex + 1;
  // load data trang moi
  initDuyetLNR();
  duyetLNR(_ListTheDocGia_root, startIndex, endIndex);
  for (int i = 0; i < _TDGArray_temp.length; i++) {
    book.lines[i] = to_string(_TDGArray_temp.data[i]->data.maThe) + ": " + _TDGArray_temp.data[i]->data.ho + ' ' + _TDGArray_temp.data[i]->data.ten;
    book.keys[i] = to_string(_TDGArray_temp.data[i]->data.maThe);
  }
  // change select
  if (book.lineCount == 0) {
    book.select = 0;
    return;
  }
  if (book.select > book.lineCount - 1) book.select = book.lineCount - 1;
}

void deleteTDG()
{
  clearBookView(_TheDocGiaBookView);
  if (_CurrentNodeTDG->data.lmt != NULL) {
    appPause("Doc gia da muon sach. Khong duoc xoa!", _TheDocGiaBookView.left, _TheDocGiaBookView.top);
    drawBookView(_TheDocGiaBookView);
    return;
  }

  if (YesNoMenu("Ban co muon xoa the doc gia nay?", _TheDocGiaBookView.left, _TheDocGiaBookView.top)) {
    bool deleted = false;
    findAndDelete(_ListTheDocGia_root, _CurrentNodeTDG->data.maThe, deleted);
  }
  loadTDGBook(_TheDocGiaBookView);
  drawBookView(_TheDocGiaBookView);
}

void coppyToClipboard()
{
  clipboardNodeTDG = _CurrentNodeTDG;
}

void moKhoaTDG()
{
  if (_CurrentNodeTDG->data.trangThai == TDG_TT_KHOA) {
    clearBookView(_TheDocGiaBookView);
    if (YesNoMenu("Mo khoa cho doc gia: " + _CurrentNodeTDG->data.ten + "?", _TheDocGiaBookView.left, _TheDocGiaBookView.top)) {
      _CurrentNodeTDG->data.trangThai = TDG_TT_HOATDONG;
    }
    drawBookView(_TheDocGiaBookView);
  }
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
  switch (_SelectFooter) {
  case 0:
    setFooter(_TDGBookFooter);
    break;
  case 1:
    setFooter(_TDGBookFooter_KhoaThe);
    break;
  }
}

void handleBookAction(BookView &book, int keyPressed)
{
  switch (keyPressed) {
  case F2:
    // ds doc ga rong khong sua duoc
    if (_ListTheDocGia_root == NULL) break;
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
    createCV.lines[4] = "Hoat dong";
    createCV.isEditable[4] = false;
    runContentViewEditMode(createCV, handleContentAction);
    loadTDGBook(_TheDocGiaBookView);
    drawBookView(_TheDocGiaBookView);
    MODE = NORMAL;
  } break;
  case F4:
    if (_TheDocGiaContentView.lineCount <= 0) break;
    deleteTDG();
    break;
  case F5:
    moKhoaTDG();
    break;
  case F6:
    if (_TheDocGiaContentView.lineCount <= 0) break;
    coppyToClipboard();
    break;
  case ENTER:
    if (_TheDocGiaContentView.lineCount <= 0) break;
    if (_CurrentNodeTDG->data.lmt == NULL) {
      clearBookView(_TheDocGiaBookView);
      appPause(
          _CurrentNodeTDG->data.ho + " " + _CurrentNodeTDG->data.ten + " chua muon sach nao!",
          _TheDocGiaBookView.left,
          _TheDocGiaBookView.top);
      drawBookView(_TheDocGiaBookView);
      break;
    }
    clearPage(_left, _top, _right, _bottom);
    MUONTRAPAGE::initMuonTraPage();
    MUONTRAPAGE::runMuonTraPage();
    loadLayout(_PageLayout);
    setHeader(_HeaderText);
    drawBookView(_TheDocGiaBookView);
    drawContentView(_TheDocGiaContentView);
    break;
  }
  switch (_SelectFooter) {
  case 0:
    setFooter(_TDGBookFooter);
    break;
  case 1:
    setFooter(_TDGBookFooter_KhoaThe);
    break;
  }
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
  _TheDocGiaContentView.labelColumnSize = 15;
  _TheDocGiaContentView.labels[0] = "Ma the";
  _TheDocGiaContentView.labels[1] = "Ho";
  _TheDocGiaContentView.labels[2] = "Ten";
  _TheDocGiaContentView.labels[3] = "Phai (nam | nu)";
  _TheDocGiaContentView.labels[4] = "Trang thai";
  _TheDocGiaContentView = getInitalView(_TheDocGiaContentView);
  _TheDocGiaContentView.isNumberType[0] = true;
  _TheDocGiaContentView.isNumberType[4] = true;
  _TheDocGiaContentView.isEditable[0] = false;
  _TheDocGiaContentView.isEditable[4] = false;
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
  switch (_SelectFooter) {
  case 0:
    setFooter(_TDGBookFooter);
    break;
  case 1:
    setFooter(_TDGBookFooter_KhoaThe);
    break;
  }
  drawContentView(_TheDocGiaContentView);
  runBookView(_TheDocGiaBookView, handleBookAction, loadTDGBook, handleBookSelectChange);
  clearPage(_left, _top, _right, _bottom);
}
} // namespace THEDOCGIAPAGE

#endif