#ifndef _MENU_CPP_
#define _MENU_CPP_

#include <fstream>
#include <iostream>
#include <string>

#include "../consolelib.h"
#include "StaticDefine.cpp"

using namespace std;

#define MAX_MENU_LINE 10

struct MenuView {
  string options[MAX_MENU_LINE];
  int count = 0;
  int select = 0;
  int top;
  int left;
  int right;
};

void printMenuOption(int number, string option)
{
  cout << number << ". " << option;
}

void drawMenu(MenuView menu)
{
  int left = menu.left;
  int top = menu.top;
  for (int i = 0; i < menu.count; i++) {
    gotoxy(left, top + i);
    if (i == menu.select) {
      setSelectText();
    };
    // ----------------------------------
    printMenuOption(i + 1, menu.options[i]);
    // ----------------------------------
    if (i == menu.select) {
      setNormalText();
    };
  }
  showConsoleCursor(false);
}

void clearMenu(MenuView menu)
{
  int left = menu.left;
  int top = menu.top;
  int right = menu.right;
  string str(right - left + 1, ' ');
  setNormalText();
  for (int i = 0; i < menu.count; i++) {
    gotoxy(left, top + i);
    cout << str;
  }
}

void changeMenuSelect(MenuView &menu, int newSelect)
{
  gotoxy(menu.left, menu.top + menu.select);
  setNormalText();
  printMenuOption(menu.select + 1, menu.options[menu.select]);
  gotoxy(menu.left, menu.top + newSelect);
  setSelectText();
  printMenuOption(newSelect + 1, menu.options[newSelect]);
  setNormalText();
  menu.select = newSelect;
}

typedef void (*DispathMenuAction)(MenuView &menu, int keyPressed, bool &ret);

void runMenu(MenuView &menu, DispathMenuAction action)
{
  drawMenu(menu);
  bool ret = false;
  while (!ret) {
    int key = getch();
    if (key == 0 || key == 224) key = getch();
    showConsoleCursor(false);
    switch (key) {
    case ARROW_DOWN:
      changeMenuSelect(menu, (menu.select + 1) % menu.count);
      break;
    case ARROW_UP:
      changeMenuSelect(menu, (menu.select + menu.count - 1) % menu.count);
      break;
    case ESC:
      ret = true;
      break;
    default:
      action(menu, key, ret);
    }
  }
  clearMenu(menu);
}

#endif