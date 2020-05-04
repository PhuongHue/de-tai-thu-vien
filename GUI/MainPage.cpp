#ifndef _MAINPAGE_CPP_
#define _MAINPAGE_CPP_

#include "components/Menu.cpp"
#include "DauSachPage.cpp"

using namespace std;

MenuView MainMenu;

void initMainMenu()
{
  MainMenu.left = 1;
  MainMenu.top = 3;
  MainMenu.right = 40;
  MainMenu.count = 1;
  MainMenu.options[0] = "Quan ly dau sach.";
  // MainMenu.options[1] = "";
  // MainMenu.options[2] = "";
  // MainMenu.options[3] = "";
  // MainMenu.options[4] = "";
  // MainMenu.options[5] = "";
  // MainMenu.options[6] = "";
  // MainMenu.options[7] = "";
  // MainMenu.options[8] = "";
  // MainMenu.options[9] = "";
}

void mainPageAction(MenuView &menu, int keyPressed)
{

  switch (menu.select)
  {
  case 0:
    clearMenu(menu);
    for (int i = 0; i < testLength; i++)
    {
      stringstream sst;
      sst << "This is line " << i + 1;
      string str;
      getline(sst, str);
      test[i] = str;
    }
    initDauSachPage();
    runDauSachPage();
    break;
  default:
    break;
  }
  setHeader("Quan ly thu vien");
  drawMenu(menu);
}

void runMainPage()
{
  loadLayout("layout/Main.layout");
  setHeader("Quan ly thu vien");
  runMenu(MainMenu, mainPageAction);
}

#endif