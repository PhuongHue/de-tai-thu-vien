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

TheDocGia *_CurrentTDG = NULL;
Sach *_CurrentSach = NULL;
DauSach *_CurrentDauSach = NULL;

ContentView _TheDocGiaContentView;
string _TheDocGiaSearchString;

ContentView _SachContentView;
string _DMSachSearchString;

const vector<string> _MuonSachFooter = {
    "ESC: Tro ve",
    "F1: Nhap ma TDG",
    "F2: Nhap ma DMS",
    "F3: Muon sach"};
const vector<string> _EditStringFooter = {
    "ESC: Huy",
    "ENTER: Tim"};
/* -------------------- _TheDocGiaContentView funtions -------------------- */
void loadContentTDG()
{
  if (!_CurrentTDG) return;
  _TheDocGiaContentView.lines[0] = to_string(_CurrentTDG->maThe);
  _TheDocGiaContentView.lines[1] = _CurrentTDG->ho;
  _TheDocGiaContentView.lines[2] = _CurrentTDG->ten;
  _TheDocGiaContentView.lines[3] = _CurrentTDG->phai ? "nam" : "nu";
  switch (_CurrentTDG->trangThai) {
  case TDG_TT_HOATDONG:
    _TheDocGiaContentView.lines[4] = "Hoat dong";
    break;
  case TDG_TT_KHOA:
    _TheDocGiaContentView.lines[4] = "Da khoa";
    break;
  case TDG_TT_DAXOA:
    _TheDocGiaContentView.lines[4] = "Da xoa";
    break;
  }
}

void searchTDG()
{
  inputText(_TheDocGiaSearchString, 20, 13, 3, true);
  if (_TheDocGiaSearchString.compare("") == 0) return;
  _CurrentTDG = find(_ListTheDocGia_root, stoll(_TheDocGiaSearchString))->data;
  loadContentTDG();
  drawContentView(_TheDocGiaContentView);
}

bool checkTDG()
{
  if (!checkDieuKienMuonSach(_CurrentTDG->lmt)) {
    clearContentView(_TheDocGiaContentView);
    appPause("Doc gia khong du dieu kien muon", _TheDocGiaContentView.left, _TheDocGiaContentView.top);
    drawContentView(_TheDocGiaContentView);
    return false;
  }
  return true;
}

/* -------------------- _SachContentView functions -------------------- */
void loadContentSach()
{
  if (!_CurrentSach) return;
  _SachContentView.lines[0] = to_string(_CurrentSach->maSach);
  switch (_CurrentSach->trangThai) {
  case 0:
    _SachContentView.lines[1] = "Muon duoc";
    break;
  case 1:
    _SachContentView.lines[1] = "Da duoc muon";
    break;
  case 2:
    _SachContentView.lines[1] = "Da thanh ly";
    break;
  }
  _SachContentView.lines[2] = _CurrentSach->viTri;
  _SachContentView.lines[3] = _CurrentDauSach->ISBN;
  _SachContentView.lines[4] = _CurrentDauSach->tenSach;
  _SachContentView.lines[5] = to_string(_CurrentDauSach->soTrang);
  _SachContentView.lines[6] = _CurrentDauSach->tacGia;
  _SachContentView.lines[7] = to_string(_CurrentDauSach->namXB);
  _SachContentView.lines[8] = _CurrentDauSach->theLoai;
}

void searchMS()
{
  inputText(_DMSachSearchString, 20, 94, 3, true);
  if (_DMSachSearchString.empty()) return;
  _CurrentSach = findMaSach(_ListDauSach_Root, stoll(_DMSachSearchString))->data;
  _CurrentDauSach = clipboardDauSach;
  loadContentSach();
  drawContentView(_SachContentView);
}

bool checkSach()
{
  if (!kiemTraDieuKienMuon(_CurrentSach)) {
    clearContentView(_SachContentView);
    appPause("Sach da duoc muon.", _SachContentView.left, _SachContentView.top);
    drawContentView(_SachContentView);
    return false;
  }
  // if (!kiemTraSachDaMuon(_CurrentTDG->lmt, _CurrentSach->maSach, )) {
  //   clearContentView(_SachContentView);
  //   appPause("Khong duoc muon sach 2 lan trong ngay.", _SachContentView.left, _SachContentView.top);
  //   drawContentView(_SachContentView);
  //   return false;
  // }
  return true;
}

/* -------------------- DMSachPage functions -------------------- */
void muonSach()
{
  if (!_CurrentSach || !_CurrentTDG || !checkSach() || !checkTDG()) return;
  MuonTra *mt = new MuonTra;
  mt->maSach = _CurrentSach->maSach;
  mt->ngayMuon = getDate();
  addLast(_CurrentTDG->lmt, mt);
  _CurrentSach->trangThai = 1;
  clearContentView(_TheDocGiaContentView);
  appPause("Muon sach thanh cong!", _TheDocGiaContentView.left, _TheDocGiaContentView.top);
  drawContentView(_TheDocGiaContentView);
  // update view
  clearContentView(_SachContentView);
  loadContentSach();
  drawContentView(_SachContentView);
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

  if (clipboardTDG != NULL) {
    _CurrentTDG = clipboardTDG;
    loadContentTDG();
  }
  else {
    _TheDocGiaContentView = getEmptyView(_TheDocGiaContentView);
  }

  if (clipboardSach != NULL && clipboardDauSach != NULL) {
    _CurrentSach = clipboardSach;
    _CurrentDauSach = clipboardDauSach;
    loadContentSach();
  }
  else {
    _SachContentView = getEmptyView(_SachContentView);
  }

  drawContentView(_TheDocGiaContentView);
  drawContentView(_SachContentView);

  bool ret = false;
  while (!ret) {
    int key = getch();
    if (key == 0 || key == 224) key = getch();
    showConsoleCursor(false);
    switch (key) {
    case F1:
      setFooter(_EditStringFooter);
      searchTDG();
      break;
    case F2:
      setFooter(_EditStringFooter);
      searchMS();
      break;
    case F3:
      muonSach();
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