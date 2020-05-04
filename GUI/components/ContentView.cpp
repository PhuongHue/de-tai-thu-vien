#ifndef _CONTENTVIEW_CPP_
#define _CONTENTVIEW_CPP_

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "../consolelib.h"
#include "StaticDefine.cpp"

#define MAX_CONTENT_VIEW_LINE 10

struct ContentView
{
  string lines[MAX_CONTENT_VIEW_LINE];
  int lineCount = 0;
  int select = 0;
  int cursor;
  int top;
  int left;
  int right;
  int bottom;
};

void drawContentView(ContentView content)
{
  for (int i = 0; i < content.lineCount; i++)
  {
    gotoxy(content.left, content.top + i);
    cout << content.lines[i];
  }
}

void clearContentView(ContentView content)
{
  string str(content.right - content.left, ' ');
  setNormalText();
  for (int i = 0; i < content.lineCount; i++)
  {
    gotoxy(content.left, content.top + i);
    cout << str;
  }
  gotoxy(content.left, content.bottom);
  cout << str;
}

void changeContentLineSelect(ContentView &content, int newSelectIndex)
{
}

void changeCursor(ContentView &content, int keyPressed)
{
  if (keyPressed == ARROW_LEFT)
  {
    content.cursor--;
    if (content.cursor < 0)
      content.cursor = 0;
  }
  if (keyPressed == ARROW_RIGHT)
  {
    content.cursor++;
    if (content.cursor > content.lines[content.select].length())
      content.cursor = content.lines[content.select].length();
  }
}

typedef void (*ContentAction)(ContentView &content, int key);

void runContentViewEditMode(
    ContentView &content,
    ContentAction onInsert,
    ContentAction onAction)
{
  drawContentView(content);
  bool ret = false;
  while (!ret)
  {
    int padKey = -1;
    int key = getch();
    if (key == 0 || key == 224)
    {
      padKey = key;
      key = getch();
    }
    ShowConsoleCursor(false);
    switch (key)
    {
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
      switch (padKey)
      {
      case -1:
        // a-z A-Z 0-9 pressed
        onInsert(content, key);
        break;
      case 0:
        // F1 - f12 pressed
        onAction(content, key);
        break;
      }
    }
  }

#endif