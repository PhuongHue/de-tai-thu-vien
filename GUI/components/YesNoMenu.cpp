#ifndef _YESNOMENU_CPP_
#define _YESNOMENU_CPP_

#include "Menu.cpp"
#include <algorithm>

namespace YESNOMENU {
MenuView _Menu;
bool _KetQua = false;
void initYesNoMenu(int left, int top, int right)
{
  _Menu.left = left;
  _Menu.top = top + 1;
  _Menu.right = right;
  _Menu.count = 2;
  _Menu.options[0] = "Co.";
  _Menu.options[1] = "Khong.";
}
void action(MenuView &menu, int keyPressed, bool &ret)
{
  switch (menu.select) {
  case 0:
    _KetQua = true;
    break;
  case 1:
    _KetQua = false;
  }
  ret = true; // bat buoc return;
}
} // namespace YESNOMENU

bool YesNoMenu(string message, int left = 0, int top = 0)
{
  int right = left + max((int)message.length(), 6);
  YESNOMENU::initYesNoMenu(left, top, right);
  gotoxy(left, top);
  cout << message;
  runMenu(YESNOMENU::_Menu, YESNOMENU::action);
  clearPage(left, top, right, top + 3);
  return YESNOMENU::_KetQua;
}
#endif