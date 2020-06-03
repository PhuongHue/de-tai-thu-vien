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

string _HeaderText = "Muon sach";
string _PageLayout = "layout/MuonSach.layout";

TheDocGia *_CurrentTDG = NULL;
DMSach *_CurrentDMSach = NULL;

ContentView _TheDocGiaContentView;
string _TheDocGiaSearchString;

ContentView _DMSachContentView;
string _DMSachSearchString;

const vector<string> _DauSachBookFooter = {
    "ESC: Tro ve",
    "F1: Nhap ma TDG",
    "F2: Nhap ma DMS",
    "F3: Muon sach",
    "F4: Luu"};
/* -------------------- _TheDocGiaContentView funtions -------------------- */
void loadContentTDG()
{
  if (!_CurrentTDG) return;
  _TheDocGiaContentView.lines[0] = to_string(_CurrentTDG->maThe);
  _TheDocGiaContentView.lines[1] = _CurrentTDG->ho;
  _TheDocGiaContentView.lines[2] = _CurrentTDG->ten;
  _TheDocGiaContentView.lines[3] = _CurrentTDG->phai ? "nam" : "nu";
  _TheDocGiaContentView.lines[4] = _CurrentTDG->trangThai == 0 ? "hoat dong" : "khoa";
}

/* -------------------- _DMSachContentView functions -------------------- */
void loadContentDMS()
{
  if (!_CurrentDMSach) return;
  _DMSachContentView.lines[0] = to_string(_CurrentDMSach->data->maSach);
  switch (_CurrentDMSach->data->trangThai) {
  case 0:
    _DMSachContentView.lines[1] = "Muon duoc";
    break;
  case 1:
    _DMSachContentView.lines[1] = "Da duoc";
    break;
  case 2:
    _DMSachContentView.lines[1] = "Thanh ly";
    break;
  }
  _DMSachContentView.lines[2] = _CurrentDMSach->data->viTri;
}

/* -------------------- DMSachPage functions -------------------- */
void initMuonSachPage()
{
  _TheDocGiaContentView.top = _top + 2;
  _TheDocGiaContentView.left = 1;
  _TheDocGiaContentView.right = 78;
  _TheDocGiaContentView.bottom = _top + 2 + 5;
  _TheDocGiaContentView.lineCount = 5;
  _TheDocGiaContentView.labelColumnSize = 30;
  _TheDocGiaContentView.labels[0] = "Ma the";
  _TheDocGiaContentView.labels[1] = "Ho";
  _TheDocGiaContentView.labels[2] = "Ten";
  _TheDocGiaContentView.labels[3] = "Phai (nam | nu)";
  _TheDocGiaContentView.labels[4] = "Trang thai (0=h.dong | 1=khoa)";
  _TheDocGiaContentView = getInitalView(_TheDocGiaContentView);
  _TheDocGiaContentView.select = -1;

  _DMSachContentView.top = _top + 2;
  _DMSachContentView.left = 78;
  _DMSachContentView.right = 154;
  _DMSachContentView.bottom = _top + 2 + 3;
  _DMSachContentView.lineCount = 3;
  _DMSachContentView.labelColumnSize = 11;
  _DMSachContentView.labels[0] = "Ma sach";
  _DMSachContentView.labels[1] = "Trang thai";
  _DMSachContentView.labels[2] = "Vi tri";
  _DMSachContentView = getInitalView(_DMSachContentView);
  _DMSachContentView.select = -1;
}

void runMuonSachPage()
{
  loadLayout(_PageLayout);
  drawContentView(_TheDocGiaContentView);
  drawContentView(_DMSachContentView);
  bool ret = false;
  while (!ret) {
    int key = getch();
    if (key == 0 || key == 224) key = getch();
    showConsoleCursor(false);
    switch (key) {
    case ESC:
      ret = true;
      break;
    }
  }
  clearPage(_left, _top, _right, _bottom);
}

} // namespace MUONSACHPAGE

#endif