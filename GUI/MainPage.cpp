#ifndef _MAINPAGE_CPP_
#define _MAINPAGE_CPP_

#include "DauSachPage.cpp"
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
  _MainMenuView.count = 1;
  _MainMenuView.options[0] = "Quan ly dau sach.";
  // _MainMenuView.options[1] = "";
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
  switch (menu.select) {
  case 0:
    clearMenu(menu);
    initDauSachPage();
    runDauSachPage();
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