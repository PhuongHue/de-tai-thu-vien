#ifndef _MAINPAGE_CPP_
#define _MAINPAGE_CPP_

#include "DauSachPage.cpp"
#include "TheDocGiaPage.cpp"
#include "components/Menu.cpp"

using namespace std;

MenuView _MainMenuView;
vector<string> _MainMenuFooter = {"ESC: Thoat", "ENTER: Chon"};
string _MainHeaderTitle = "Quan ly thu vien";

void initMainMenu()
{
  _MainMenuView.left = 1;
  _MainMenuView.top = 3;
  _MainMenuView.right = 40;
  _MainMenuView.count = 2;
  _MainMenuView.options[0] = "Quan ly dau sach.";
  _MainMenuView.options[1] = "Quan ly the doc gia.";
  // _MainMenuView.options[2] = "";
  // _MainMenuView.options[3] = "";
  // _MainMenuView.options[4] = "";
  // _MainMenuView.options[5] = "";
  // _MainMenuView.options[6] = "";
  // _MainMenuView.options[7] = "";
  // _MainMenuView.options[8] = "";
  // _MainMenuView.options[9] = "";
}

void mainPageAction(MenuView &menu, int keyPressed)
{
  if (keyPressed != ENTER) return;
  switch (menu.select) {
  case 0:
    clearMenu(menu);
    DAUSACHPAGE::initDauSachPage();
    DAUSACHPAGE::runDauSachPage();
    break;
  case 1:
    clearMenu(menu);
    THEDOCGIAPAGE::initTheDocGiaPage();
    THEDOCGIAPAGE::runTheDocGiaPage();
    break;
  default:
    break;
  }
  setHeader(_MainHeaderTitle);
  setFooter(_MainMenuFooter);
  drawMenu(menu);
}

void runMainPage()
{
  loadLayout("layout/Main.layout");
  setHeader(_MainHeaderTitle);
  setFooter(_MainMenuFooter);
  runMenu(_MainMenuView, mainPageAction);
}

#endif