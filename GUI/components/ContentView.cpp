#ifndef _CONTENTVIEW_CPP_
#define _CONTENTVIEW_CPP_

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "../consolelib.h"
#include "StaticDefine.cpp"

#define MAX_CONTENT_VIEW_LINE 20

struct ContentView {
  string lines[MAX_CONTENT_VIEW_LINE];
  string labels[MAX_CONTENT_VIEW_LINE];
  bool isNumberType[MAX_CONTENT_VIEW_LINE];
  bool isEditable[MAX_CONTENT_VIEW_LINE];
  int labelColumnSize = 10;
  int lineCount = 0;
  int select = 0;
  int cursor;
  int top;
  int left;
  int right;
  int bottom;
  vector<string> footer = {"ESC: Cancel", "F2: Luu"};
};

ContentView getInitalView(ContentView base)
{
  ContentView content = base;
  content.cursor = 0;
  content.select = 0;
  for (int i = 0; i < content.lineCount; i++) {
    content.lines[i] = "";
    content.isNumberType[i] = false;
    content.isEditable[i] = true;
  }
  return content;
}

ContentView getEmptyView(ContentView base)
{
  ContentView content = base;
  content.cursor = 0;
  for (int i = 0; i < content.lineCount; i++) {
    content.lines[i] = "";
  }
  return content;
}

void printContentLine(string label, string line, int width)
{
  cout << left << setw(width) << label << setw(0) << ": " << line;
}

void drawFooter(ContentView &content)
{
  gotoxy(content.left, content.bottom);
  for (int i = 0; i < content.footer.size(); i++) {
    cout << content.footer[i] << " | ";
  }
}

void clearFooter(ContentView &content)
{
  gotoxy(content.left, content.bottom);
  int length = 0;
  for (int i = 0; i < content.footer.size(); i++) {
    length += content.footer[i].length() + 3;
  }
  cout << string(length, ' ');
}

void drawContentView(ContentView &content)
{
  setNormalText();
  for (int i = 0; i < content.lineCount; i++) {
    gotoxy(content.left, content.top + i);
    printContentLine(content.labels[i], content.lines[i],
        content.labelColumnSize);
  }
}

void clearContentView(ContentView &content)
{
  string str(content.right - content.left, ' ');
  setNormalText();
  for (int i = 0; i < content.lineCount; i++) {
    gotoxy(content.left, content.top + i);
    cout << str;
  }
  gotoxy(content.left, content.bottom);
  cout << str;
}

void _gotoSelect(ContentView &content)
{
  gotoxy(content.left + content.labelColumnSize + 2 + content.cursor,
      content.top + content.select);
}

void editLine(ContentView &content, int key)
{
  // return neu khong duoc sua;
  if (content.isEditable[content.select] == false) {
    cout << BELL;
    return;
  }
  int select = content.select;
  if (key == BACKSPACE) {
    if (content.cursor == 0) return;
    string right = "";
    if (content.cursor < content.lines[select].length()) {
      right = content.lines[select].substr(content.cursor);
    }
    cout << (char)BACKSPACE << right << ' ';
    content.cursor--;
    _gotoSelect(content);
    content.lines[select].erase(content.cursor, 1);
  }
  else {
    if (content.isNumberType[select] && !(key >= '0' && key <= '9')) return;
    content.lines[select].insert(content.lines[select].begin() + content.cursor, (char)key);
    string right = "";
    if (content.cursor < content.lines[select].length()) {
      right = content.lines[select].substr(content.cursor);
    }
    cout << right;
    content.cursor++;
    _gotoSelect(content);
  }
}

void changeContentLineSelect(ContentView &content, int newSelectIndex)
{
  content.select = newSelectIndex;
  content.cursor = content.lines[content.select].length();
  _gotoSelect(content);
}

void changeCursor(ContentView &content, int keyPressed)
{
  if (keyPressed == ARROW_LEFT) {
    content.cursor--;
    if (content.cursor < 0) content.cursor = 0;
  }
  if (keyPressed == ARROW_RIGHT) {
    content.cursor++;
    if (content.cursor > content.lines[content.select].length())
      content.cursor = content.lines[content.select].length();
  }
  _gotoSelect(content);
}

typedef void (*ContentAction)(ContentView &content, int key, bool &breaker);

void runContentViewEditMode(ContentView &content, ContentAction onAction)
{
  content.select = 0;
  content.cursor = content.lines[0].length();
  drawContentView(content);
  drawFooter(content);
  showConsoleCursor(true);
  _gotoSelect(content);
  bool ret = false;
  while (!ret) {
    int padKey = -1;
    int key = getch();
    if (key == 0 || key == 224) {
      padKey = key;
      key = getch();
    }
    switch (key) {
    case ARROW_DOWN:
      changeContentLineSelect(content, (content.select + 1) % content.lineCount);
      break;
    case ARROW_UP:
      changeContentLineSelect(content, (content.select + content.lineCount - 1) % content.lineCount);
      break;
    case ARROW_RIGHT:
      changeCursor(content, key);
      break;
    case ARROW_LEFT:
      changeCursor(content, key);
      break;
    case ESC:
      ret = true;
      break;
    default:
      switch (padKey) {
      case -1:
        // a-z A-Z 0-9 pressed
        editLine(content, key);
        break;
      case 0:
        // F1 - f12 pressed
        if (onAction) onAction(content, key, ret);
        break;
      }
    }
  }
  clearFooter(content);
  showConsoleCursor(false);
}
#endif