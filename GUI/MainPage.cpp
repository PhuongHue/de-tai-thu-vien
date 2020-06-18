#ifndef _MAINPAGE_CPP_
#define _MAINPAGE_CPP_

#include "DauSachPage.cpp"
#include "MuonSachPage.cpp"
#include "TDGQuaHanPAge.cpp"
#include "TheDocGiaPage.cpp"
#include "TopDauSachPage.cpp"
#include "components/Menu.cpp"

using namespace std;

namespace MAINPAGE {

int _top = 3;
int _left = 1;
int _right = 40;
int _bottom = 26;

MenuView _MainMenuView;
vector<string> _MainMenuFooter = {"ESC: Thoat", "ENTER: Chon"};
string _PageLayout = "layout/Main.layout";
string _HeaderText = "Quan ly thu vien";

void mainPageAction(MenuView &menu, int keyPressed, bool &ret)
{
  if (keyPressed != ENTER) return;
  switch (menu.select) {
  case 0:
    clearMenu(menu);
    DAUSACHPAGE::runDauSachPage();
    break;
  case 1:
    clearMenu(menu);
    THEDOCGIAPAGE::runTheDocGiaPage();
    break;
  case 2:
    clearMenu(menu);
    MUONSACHPAGE::runMuonSachPage();
    break;
  case 3:
    luuFile(_ListDauSach_Root);
    luuFile(_ListTheDocGia_root);
    clearMenu(menu);
    appPause("Da luu vao file!", _left, _top);
    break;
  case 4:
    clearMenu(menu);
    TOPDAUSACHPAGE::initTopDauSachPage();
    TOPDAUSACHPAGE::runTopDauSachPage();
    break;
  case 5:
    clearMenu(menu);
    TDGQUAHANPAGE::initTDGQuaHanPage();
    TDGQUAHANPAGE::runTDGQuaHanPage();
    break;
  }
  setHeader(_HeaderText);
  setFooter(_MainMenuFooter);
  drawMenu(menu);
} // namespace MAINPAGE

void initMainMenu()
{
  _MainMenuView.left = _left;
  _MainMenuView.top = _top;
  _MainMenuView.right = _right;
  _MainMenuView.count = 6;
  _MainMenuView.options[0] = "Quan ly dau sach.";
  _MainMenuView.options[1] = "Quan ly the doc gia.";
  _MainMenuView.options[2] = "Muon sach.";
  _MainMenuView.options[3] = "Luu file.";
  _MainMenuView.options[4] = "Top 10 dau sach.";
  _MainMenuView.options[5] = "Danh sach qua han.";
  // _MainMenuView.options[6] = "";
  // _MainMenuView.options[7] = "";
  // _MainMenuView.options[8] = "";
  // _MainMenuView.options[9] = "";
}

void runMainPage()
{
  loadLayout(_PageLayout);
  setHeader(_HeaderText);
  setFooter(_MainMenuFooter);
  runMenu(_MainMenuView, mainPageAction);
}

} // namespace MAINPAGE
#endif