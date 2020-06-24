#ifndef _TOPDAUSACHPAGE_CPP_
#define _TOPDAUSACHPAGE_CPP_

#include "../TopDauSach.cpp"
#include "components/BookView.cpp"
#include <vector>

using namespace std;

namespace TOPDAUSACHPAGE {

int _top = 3;
int _left = 1;
int _right = 154;
int _bottom = 26;

string _HeaderText = "Top dau sach duoc muon";
string _PageLayout = "layout/TopDauSach.layout";

const vector<string> _PageFooter = {
    "ESC: Tro ve",
    ">>: Trang sau",
    "<<: Trang truoc"};

/* ---------- view table -------------------*/

const int MAX_PAGE_SIZE = 100;

struct Table {
  int left, top, right, bottom;
  int columns[7] = {0, 8, 29, 60, 86, 112, 133};
  TopDauSach_Row rows[MAX_PAGE_SIZE];
  int pageIndex = 0;
  int pageSize = 20;
  int allPage = 0;
  int lineCount;
} _TopDauSachTable;

void loadTable()
{
  _TopDauSachTable.allPage = countAllPage(_TopData_Root.length, _TopDauSachTable.pageSize);
  if (_TopDauSachTable.pageIndex >= _TopDauSachTable.allPage)
    _TopDauSachTable.pageIndex = _TopDauSachTable.allPage - 1;

  int startIndex = _TopDauSachTable.pageIndex * _TopDauSachTable.pageSize;
  int endIndex = startIndex + _TopDauSachTable.pageSize - 1;
  if (endIndex > _TopData_Root.length - 1) endIndex = _TopData_Root.length - 1;
  _TopDauSachTable.lineCount = endIndex - startIndex + 1;

  for (int i = startIndex, j = 0; i <= endIndex; i++, j++) {
    _TopDauSachTable.rows[j] = _TopData_Root.data[i];
  }
}

int getSTT(int lineNumber){
  return _TopDauSachTable.pageIndex * _TopDauSachTable.pageSize + lineNumber + 1;
}

void drawTable()
{
  for (int i = 0; i < _TopDauSachTable.lineCount; i++) {
    gotoxy(_TopDauSachTable.left + _TopDauSachTable.columns[0], _TopDauSachTable.top + i);
    cout <<right <<setw(4) << getSTT(i);
    gotoxy(_TopDauSachTable.left + _TopDauSachTable.columns[1], _TopDauSachTable.top + i);
    cout << _TopDauSachTable.rows[i].ds->ISBN;
    gotoxy(_TopDauSachTable.left + _TopDauSachTable.columns[2], _TopDauSachTable.top + i);
    cout << _TopDauSachTable.rows[i].ds->tenSach;
    gotoxy(_TopDauSachTable.left + _TopDauSachTable.columns[3], _TopDauSachTable.top + i);
    cout << _TopDauSachTable.rows[i].ds->theLoai;
    gotoxy(_TopDauSachTable.left + _TopDauSachTable.columns[4], _TopDauSachTable.top + i);
    cout << _TopDauSachTable.rows[i].ds->tacGia;
    gotoxy(_TopDauSachTable.left + _TopDauSachTable.columns[5], _TopDauSachTable.top + i);
    cout << _TopDauSachTable.rows[i].ds->namXB;
    gotoxy(_TopDauSachTable.left + _TopDauSachTable.columns[6], _TopDauSachTable.top + i);
    cout << _TopDauSachTable.rows[i].count;
  }
  gotoxy(_TopDauSachTable.left, _TopDauSachTable.bottom);
  cout << "trang " << _TopDauSachTable.pageIndex + 1 << '/' << _TopDauSachTable.allPage;
}

void clearTable()
{
  for (int i = 0; i < _TopDauSachTable.lineCount; i++) {
    gotoxy(_TopDauSachTable.left + _TopDauSachTable.columns[0], _TopDauSachTable.top + i);
    cout << string(7, ' ');
    gotoxy(_TopDauSachTable.left + _TopDauSachTable.columns[1], _TopDauSachTable.top + i);
    cout << string(20, ' ');
    gotoxy(_TopDauSachTable.left + _TopDauSachTable.columns[2], _TopDauSachTable.top + i);
    cout << string(30, ' ');
    gotoxy(_TopDauSachTable.left + _TopDauSachTable.columns[3], _TopDauSachTable.top + i);
    cout << string(25, ' ');
    gotoxy(_TopDauSachTable.left + _TopDauSachTable.columns[4], _TopDauSachTable.top + i);
    cout << string(25, ' ');
    gotoxy(_TopDauSachTable.left + _TopDauSachTable.columns[5], _TopDauSachTable.top + i);
    cout << string(20, ' ');
    gotoxy(_TopDauSachTable.left + _TopDauSachTable.columns[6], _TopDauSachTable.top + i);
    cout << string(29, ' ');
  }
  gotoxy(_TopDauSachTable.left, _TopDauSachTable.bottom);
  cout << string(_TopDauSachTable.right - _TopDauSachTable.left, ' ');
}

void initTopDauSachPage()
{
  _TopDauSachTable.left = _left;
  _TopDauSachTable.top = _top + 2;
  _TopDauSachTable.right = _right;
  _TopDauSachTable.bottom = _bottom - 1;

  thongKeDauSach();
  loadTable();
}

void runTopDauSachPage()
{
  loadLayout(_PageLayout);
  setHeader(_HeaderText);
  setFooter(_PageFooter);
  drawTable();
  bool ret = false;
  while (!ret) {
    int key = getch();
    if (key == 0 || key == 224) key = getch();
    showConsoleCursor(false);
    switch (key) {
    case ARROW_RIGHT:
      if (_TopDauSachTable.allPage > 0) {
        clearTable();
        _TopDauSachTable.pageIndex = (_TopDauSachTable.pageIndex + 1) % _TopDauSachTable.allPage;
        loadTable();
        drawTable();
      }
      break;
    case ARROW_LEFT:
      if (_TopDauSachTable.allPage > 0) {
        clearTable();
        _TopDauSachTable.pageIndex = (_TopDauSachTable.pageIndex + _TopDauSachTable.allPage - 1) % _TopDauSachTable.allPage;
        loadTable();
        drawTable();
      }
      break;
    case ESC:
      ret = true;
      break;
    }
  }
  clearPage(_left, _top, _right, _bottom);
}

} // namespace TOPDAUSACHPAGE

#endif