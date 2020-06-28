#ifndef _MUONSACHPAGE_CPP_
#define _MUONSACHPAGE_CPP_

#include <math.h>
#include <sstream>
#include <string>

#include "../DauSach.cpp"
#include "../StringLib.cpp"
#include "../TheDocGia.cpp"
#include "components/BookView.cpp"
#include "components/ContentView.cpp"
#include "components/YesNoMenu.cpp"

using namespace std;

namespace MUONSACHPAGE {

int _top = 3;
int _left = 1;
int _right = 154;
int _bottom = 26;

string _MuonSachHeaderText = "Muon sach";
string _PageLayout = "layout/MuonSach.layout";

NodeTheDocGia *_CurrentNodeTDG = NULL;
DMSach *_CurrentDMSach = NULL;
DauSach *_CurrentDauSach = NULL;

ContentView _TheDocGiaContentView;
string _TheDocGiaSearchString;
int _TheDocGiaSearch_left = 14;
int _TheDocGiaSearch_top = 3;

ContentView _SachContentView;
string _DMSachSearchString;
int _DMSachSearch_left = 94;
int _DMSachSearch_top = 3;

struct Row {
  ListMuonTra *lmtNode;
  DauSach *ds;
};

const int MAX_SachDangMuonTable = 3;

struct SachDangMuonTable {
  Row rows[MAX_SachDangMuonTable];
  int left = 1, top = 16, right = 77, bottom = 26;
  int columns[4] = {0, 15, 28, 45};
  int select = 0;
  int length;
} _SachDaMuonTable;

const vector<string> _MuonSachFooter = {
    "ESC: Tro ve",
    "F1: Nhap ma TDG",
    "F2: Nhap ma DMS",
    "F3: Muon sach",
    "F4: Tra sach"};
const vector<string> _EditStringFooter = {
    "ESC: Huy",
    "ENTER: Tim"};

void drawSeachString()
{
  gotoxy(_TheDocGiaSearch_left, _TheDocGiaSearch_top);
  cout << _TheDocGiaSearchString;
  gotoxy(_DMSachSearch_left, _DMSachSearch_top);
  cout << _DMSachSearchString;
}

/* -------------------- _SachDangMuonTable funtions ------------------- */
void drawTable()
{
  for (int i = 0; i < _SachDaMuonTable.length; i++) {
    if (i == _SachDaMuonTable.select) setSelectText();
    gotoxy(_SachDaMuonTable.left + _SachDaMuonTable.columns[0], _SachDaMuonTable.top + i);
    cout << _SachDaMuonTable.rows[i].lmtNode->data.maSach;
    gotoxy(_SachDaMuonTable.left + _SachDaMuonTable.columns[1], _SachDaMuonTable.top + i);
    cout << getDateString(_SachDaMuonTable.rows[i].lmtNode->data.ngayMuon);
    gotoxy(_SachDaMuonTable.left + _SachDaMuonTable.columns[2], _SachDaMuonTable.top + i);
    cout << _SachDaMuonTable.rows[i].ds->ISBN;
    gotoxy(_SachDaMuonTable.left + _SachDaMuonTable.columns[3], _SachDaMuonTable.top + i);
    cout << _SachDaMuonTable.rows[i].ds->tenSach;
    if (i == _SachDaMuonTable.select) setNormalText();
  }
}

void clearTable()
{
  for (int i = 0; i < _SachDaMuonTable.length; i++) {
    gotoxy(_SachDaMuonTable.left + _SachDaMuonTable.columns[0], _SachDaMuonTable.top + i);
    cout << string(14, ' ');
    gotoxy(_SachDaMuonTable.left + _SachDaMuonTable.columns[1], _SachDaMuonTable.top + i);
    cout << string(12, ' ');
    gotoxy(_SachDaMuonTable.left + _SachDaMuonTable.columns[2], _SachDaMuonTable.top + i);
    cout << string(16, ' ');
    gotoxy(_SachDaMuonTable.left + _SachDaMuonTable.columns[3], _SachDaMuonTable.top + i);
    cout << string(30, ' ');
  }
}

void addRow(ListMuonTra *lmt)
{
  // tao row
  Row row;
  DauSach *ds = tim_DauSach_theo_MaSach(_ListDauSach_Root, lmt->data.maSach);
  // gan data
  row.ds = ds;
  row.lmtNode = lmt;
  // them vao table
  _SachDaMuonTable.rows[_SachDaMuonTable.length] = row;
  _SachDaMuonTable.length++;
}

void loadTableChuaTra()
{
  // clear data
  _SachDaMuonTable.length = 0;
  _SachDaMuonTable.select = 0;
  ListMuonTra *sachChuaTra = filterSachChuaTra(_CurrentNodeTDG->data.lmt);
  ListMuonTra *p = sachChuaTra;
  while (p != NULL) {
    // them vao bang data
    addRow(p);
    p = p->next;
  }
}
/* -------------------- _TheDocGiaContentView funtions -------------------- */
void loadContentTDG()
{
  if (_CurrentNodeTDG == NULL) return;
  _TheDocGiaContentView.lines[0] = to_string(_CurrentNodeTDG->data.maThe);
  _TheDocGiaContentView.lines[1] = _CurrentNodeTDG->data.ho;
  _TheDocGiaContentView.lines[2] = _CurrentNodeTDG->data.ten;
  _TheDocGiaContentView.lines[3] = _CurrentNodeTDG->data.phai ? "nam" : "nu";
  switch (_CurrentNodeTDG->data.trangThai) {
  case TDG_TT_HOATDONG:
    _TheDocGiaContentView.lines[4] = "Hoat dong";
    break;
  case TDG_TT_KHOA:
    _TheDocGiaContentView.lines[4] = "Da khoa";
    break;
  }
  // load danh sach dang muon
  loadTableChuaTra();
}

void searchTDG()
{
  setFooter(_EditStringFooter);
  //clear data
  _CurrentNodeTDG = NULL;
  // clear view
  clearContentView(_TheDocGiaContentView);
  _TheDocGiaContentView = getEmptyView(_TheDocGiaContentView);
  drawContentView(_TheDocGiaContentView);
  clearTable();
  _SachDaMuonTable.length = 0;
  _SachDaMuonTable.select = 0;

  inputText(_TheDocGiaSearchString, 20, _TheDocGiaSearch_left, _TheDocGiaSearch_top, true);
  if (_TheDocGiaSearchString.compare("") == 0) return;
  NodeTheDocGia *tdgNode = find(_ListTheDocGia_root, stoll(_TheDocGiaSearchString));
  if (tdgNode == NULL) {
    clearContentView(_TheDocGiaContentView);
    appPause("The doc gia khong ton tai!", _TheDocGiaContentView.left, _TheDocGiaContentView.top);
    drawContentView(_TheDocGiaContentView);
    return;
  }
  _CurrentNodeTDG = tdgNode;
  loadContentTDG();
  drawContentView(_TheDocGiaContentView);
  drawTable();
}

/* -------------------- _SachContentView functions -------------------- */
void loadContentSach()
{
  if (_CurrentDMSach == NULL) return;
  _SachContentView.lines[0] = to_string(_CurrentDMSach->data.maSach);
  switch (_CurrentDMSach->data.trangThai) {
  case SACH_TT_MUONDUOC:
    _SachContentView.lines[1] = "Muon duoc";
    break;
  case SACH_TT_DAMUON:
    _SachContentView.lines[1] = "Da duoc muon";
    break;
  case SACH_TT_THANHLY:
    _SachContentView.lines[1] = "Da thanh ly";
    break;
  }
  _SachContentView.lines[2] = _CurrentDMSach->data.viTri;
  _SachContentView.lines[3] = _CurrentDauSach->ISBN;
  _SachContentView.lines[4] = _CurrentDauSach->tenSach;
  _SachContentView.lines[5] = to_string(_CurrentDauSach->soTrang);
  _SachContentView.lines[6] = _CurrentDauSach->tacGia;
  _SachContentView.lines[7] = to_string(_CurrentDauSach->namXB);
  _SachContentView.lines[8] = _CurrentDauSach->theLoai;
}

void searchMS()
{
  // clear data
  _CurrentDMSach = NULL;
  _CurrentDauSach = NULL;
  // clear view
  clearContentView(_SachContentView);
  _SachContentView = getEmptyView(_SachContentView);
  drawContentView(_SachContentView);
  // nhap ma
  inputText(_DMSachSearchString, 20, _DMSachSearch_left, _DMSachSearch_top, true);
  if (_DMSachSearchString.empty()) return;
  // search
  long long key = stoll(_DMSachSearchString);
  DauSachMaSach dsms = tim_DauSachMaSach_theo_MaSach(_ListDauSach_Root, key);
  if (dsms.dmSach != NULL) {
    _CurrentDMSach = dsms.dmSach;
    _CurrentDauSach = dsms.dauSach;
    loadContentSach();
    drawContentView(_SachContentView);
  }
}
/* -------------------- MuonSachPage functions -------------------- */
bool checkTDG()
{
  if (_CurrentNodeTDG->data.trangThai == TDG_TT_KHOA) {
    clearContentView(_TheDocGiaContentView);
    appPause("The doc gia da bi khoa!", _TheDocGiaContentView.left, _TheDocGiaContentView.top);
    drawContentView(_TheDocGiaContentView);
    return false;
  }
  if (!checkDieuKienMuonSach(_CurrentNodeTDG->data.lmt)) {
    clearContentView(_TheDocGiaContentView);
    appPause("Doc gia khong du dieu kien muon!", _TheDocGiaContentView.left, _TheDocGiaContentView.top);
    drawContentView(_TheDocGiaContentView);
    return false;
  }
  return true;
}

bool checkSach()
{
  if (!kiemTraDieuKienMuon(_CurrentDMSach)) {
    clearContentView(_SachContentView);
    appPause("Sach da duoc muon.", _SachContentView.left, _SachContentView.top);
    drawContentView(_SachContentView);
    return false;
  }
  if (!kiemTraSachDaMuon(_CurrentNodeTDG->data.lmt, _CurrentDMSach->data.maSach, getDate())) {
    clearContentView(_SachContentView);
    appPause("Khong duoc muon sach 2 lan trong ngay.", _SachContentView.left, _SachContentView.top);
    drawContentView(_SachContentView);
    return false;
  }
  return true;
}

void muonSach()
{
  if (_CurrentDMSach == NULL || _CurrentNodeTDG == NULL || checkSach() == false || checkTDG() == false) return;
  MuonTra mt;
  mt.maSach = _CurrentDMSach->data.maSach;
  mt.ngayMuon = getDate();
  addLast(_CurrentNodeTDG->data.lmt, mt);
  _CurrentDMSach->data.trangThai = SACH_TT_DAMUON;
  clearContentView(_TheDocGiaContentView);
  appPause("Muon sach thanh cong!", _TheDocGiaContentView.left, _TheDocGiaContentView.top);
  drawContentView(_TheDocGiaContentView);
  // clear table view
  clearTable();
  // load data
  loadTableChuaTra();
  loadContentSach();
  // update view
  drawTable();
  clearContentView(_SachContentView);
  drawContentView(_SachContentView);
}

void traSach()
{
  if (_SachDaMuonTable.length == 0) return;
  clearContentView(_TheDocGiaContentView);
  bool kq = YesNoMenu("Ban co chac chan muon tra sach?", _TheDocGiaContentView.left, _TheDocGiaContentView.top);
  drawContentView(_TheDocGiaContentView);

  if (kq == true) {
    // set ngay tra, chuyen trang thai
    Row currentRow = _SachDaMuonTable.rows[_SachDaMuonTable.select];
    long long ngayMuon = currentRow.lmtNode->data.ngayMuon;
    long long maSach = currentRow.lmtNode->data.maSach;
    ListMuonTra *node = find_LMT_by_MaSach_NgayMuon(_CurrentNodeTDG->data.lmt, maSach, ngayMuon);
    node->data.ngayTra = getDate();
    node->data.trangThai = MT_TT_DATRA;
    // set trang thai sach
    DMSach *sach = findByMaSach(currentRow.ds->dms, maSach);
    sach->data.trangThai = SACH_TT_MUONDUOC;
    clearTable();
    loadTableChuaTra();
    drawTable();
  }
}

void initMuonSachPage()
{
  _TheDocGiaContentView.top = _top + 2;
  _TheDocGiaContentView.left = 1;
  _TheDocGiaContentView.right = 76;
  _TheDocGiaContentView.bottom = _top + 2 + 5;
  _TheDocGiaContentView.lineCount = 5;
  _TheDocGiaContentView.labelColumnSize = 10;
  _TheDocGiaContentView.labels[0] = "Ma the";
  _TheDocGiaContentView.labels[1] = "Ho";
  _TheDocGiaContentView.labels[2] = "Ten";
  _TheDocGiaContentView.labels[3] = "Phai";
  _TheDocGiaContentView.labels[4] = "Trang thai";
  _TheDocGiaContentView = getInitalView(_TheDocGiaContentView);
  _TheDocGiaContentView.select = -1;

  _SachContentView.top = _top + 2;
  _SachContentView.left = 78;
  _SachContentView.right = 154;
  _SachContentView.bottom = _top + 2 + 3;
  _SachContentView.lineCount = 9;
  _SachContentView.labelColumnSize = 10;
  _SachContentView.labels[0] = "Ma sach";
  _SachContentView.labels[1] = "Trang thai";
  _SachContentView.labels[2] = "Vi tri";
  _SachContentView.labels[3] = "ISBN";
  _SachContentView.labels[4] = "Ten sach";
  _SachContentView.labels[5] = "So trang";
  _SachContentView.labels[6] = "Tac gia";
  _SachContentView.labels[7] = "Nam XB";
  _SachContentView.labels[8] = "The loai";
  _SachContentView = getInitalView(_SachContentView);
  _SachContentView.select = -1;
}

void runMuonSachPage()
{
  loadLayout(_PageLayout);
  setHeader(_MuonSachHeaderText);
  setFooter(_MuonSachFooter);
  drawSeachString();
  // da chon TDG
  if (clipboardNodeTDG != NULL) {
    _CurrentNodeTDG = clipboardNodeTDG;
    clipboardNodeTDG = NULL;
  }

  // da chon sach
  if (clipboardDMSach != NULL && clipboardDauSach != NULL) {
    _CurrentDMSach = clipboardDMSach;
    _CurrentDauSach = clipboardDauSach;
    clipboardDMSach = NULL;
    clipboardDauSach = NULL;
  }

  loadContentTDG();
  loadContentSach();
  drawContentView(_TheDocGiaContentView);
  drawTable();
  drawContentView(_SachContentView);
  bool ret = false;
  while (!ret) {
    int key = getch();
    if (key == 0 || key == 224) key = getch();
    showConsoleCursor(false);
    switch (key) {
    case ARROW_DOWN:
      if (_SachDaMuonTable.length == 0) break;
      _SachDaMuonTable.select = (_SachDaMuonTable.select + 1) % _SachDaMuonTable.length;
      drawTable();
      break;
    case ARROW_UP:
      if (_SachDaMuonTable.length == 0) break;
      _SachDaMuonTable.select = (_SachDaMuonTable.length + _SachDaMuonTable.select - 1) % _SachDaMuonTable.length;
      drawTable();
      break;
    case F1:
      searchTDG();
      break;
    case F2:
      setFooter(_EditStringFooter);
      searchMS();
      break;
    case F3:
      muonSach();
      break;
    case F4:
      traSach();
      break;
    case ESC:
      ret = true;
      break;
    }
    setFooter(_MuonSachFooter);
  }
  clearPage(_left, _top, _right, _bottom);
}

} // namespace MUONSACHPAGE

#endif