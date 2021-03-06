#ifndef _TDGQUAHANPAGE_CPP_
#define _TDGQUAHANPAGE_CPP_

#include "../TheDocGiaQuaHan.cpp"
#include "components/BookView.cpp"
#include <vector>

using namespace std;

namespace TDGQUAHANPAGE {

int _top = 3;
int _left = 1;
int _right = 154;
int _bottom = 26;

string _HeaderText = "Danh sach doc gia muon qua han";
string _PageLayout = "layout/TDGQuaHan.layout";

const vector<string> _PageFooter = {
    "ESC: Tro ve",
    ">>: Trang sau",
    "<<: Trang truoc"};


/* ---------- view table -------------------*/

const int MAX_PAGE_SIZE = 100;

struct Table {
  int left, top, right, bottom;
  int columns[7] = {0, 8, 29, 60, 81, 112, 133};
  QuaHan_Row rows[MAX_PAGE_SIZE];
  int pageIndex = 0;
  int pageSize = 20;
  int allPage = 0;
  int lineCount;
} _QuaHanTable;

void loadTable()
{
  _QuaHanTable.allPage = countAllPage(_QuaHanData_Root.length, _QuaHanTable.pageSize);
  if (_QuaHanTable.pageIndex >= _QuaHanTable.allPage)
    _QuaHanTable.pageIndex = _QuaHanTable.allPage - 1;

  int startIndex = _QuaHanTable.pageIndex * _QuaHanTable.pageSize;
  int endIndex = startIndex + _QuaHanTable.pageSize - 1;
  if (endIndex > _QuaHanData_Root.length - 1) endIndex = _QuaHanData_Root.length - 1;
  _QuaHanTable.lineCount = endIndex - startIndex + 1;

  for (int i = startIndex, j = 0; i <= endIndex; i++, j++) {
    _QuaHanTable.rows[j] = _QuaHanData_Root.data[i];
  }
}

int getSTT(int lineNumber)
{
  return _QuaHanTable.pageIndex * _QuaHanTable.pageSize + lineNumber + 1;
}

void drawTable()
{
  for (int i = 0; i < _QuaHanTable.lineCount; i++) {
    gotoxy(_QuaHanTable.left + _QuaHanTable.columns[0], _QuaHanTable.top + i);
    cout << right << setw(4) << getSTT(i);
    gotoxy(_QuaHanTable.left + _QuaHanTable.columns[1], _QuaHanTable.top + i);
    cout << _QuaHanTable.rows[i].mathe;
    gotoxy(_QuaHanTable.left + _QuaHanTable.columns[2], _QuaHanTable.top + i);
    cout << _QuaHanTable.rows[i].hoTen;
    gotoxy(_QuaHanTable.left + _QuaHanTable.columns[3], _QuaHanTable.top + i);
    cout << _QuaHanTable.rows[i].maSach;
    gotoxy(_QuaHanTable.left + _QuaHanTable.columns[4], _QuaHanTable.top + i);
    cout << _QuaHanTable.rows[i].tenSach;
    gotoxy(_QuaHanTable.left + _QuaHanTable.columns[5], _QuaHanTable.top + i);
    cout << getDateString(_QuaHanTable.rows[i].ngayMuon);
    gotoxy(_QuaHanTable.left + _QuaHanTable.columns[6], _QuaHanTable.top + i);
    cout << _QuaHanTable.rows[i].ngayQuaHan;
  }
  gotoxy(_QuaHanTable.left, _QuaHanTable.bottom);
  cout << "trang " << _QuaHanTable.pageIndex + 1 << '/' << _QuaHanTable.allPage;
}

void clearTable()
{
  for (int i = 0; i < _QuaHanTable.lineCount; i++) {
    gotoxy(_QuaHanTable.left + _QuaHanTable.columns[0], _QuaHanTable.top + i);
    cout << string(7, ' ');
    gotoxy(_QuaHanTable.left + _QuaHanTable.columns[1], _QuaHanTable.top + i);
    cout << string(20, ' ');
    gotoxy(_QuaHanTable.left + _QuaHanTable.columns[2], _QuaHanTable.top + i);
    cout << string(30, ' ');
    gotoxy(_QuaHanTable.left + _QuaHanTable.columns[3], _QuaHanTable.top + i);
    cout << string(20, ' ');
    gotoxy(_QuaHanTable.left + _QuaHanTable.columns[4], _QuaHanTable.top + i);
    cout << string(30, ' ');
    gotoxy(_QuaHanTable.left + _QuaHanTable.columns[5], _QuaHanTable.top + i);
    cout << string(20, ' ');
    gotoxy(_QuaHanTable.left + _QuaHanTable.columns[6], _QuaHanTable.top + i);
    cout << string(21, ' ');
  }
  gotoxy(_QuaHanTable.left, _QuaHanTable.bottom);
  cout << string(_QuaHanTable.right - _QuaHanTable.left, ' ');
}

void initTDGQuaHanPage()
{
  _QuaHanTable.left = _left;
  _QuaHanTable.top = _top + 2;
  _QuaHanTable.right = _right;
  _QuaHanTable.bottom = _bottom - 1;
  thongKeQuaHan();
  loadTable();
}

void runTDGQuaHanPage()
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
      if (_QuaHanTable.allPage > 0) {
        clearTable();
        _QuaHanTable.pageIndex = (_QuaHanTable.pageIndex + 1) % _QuaHanTable.allPage;
        loadTable();
        drawTable();
      }
      break;
    case ARROW_LEFT:
      if (_QuaHanTable.allPage > 0) {
        clearTable();
        _QuaHanTable.pageIndex = (_QuaHanTable.pageIndex + _QuaHanTable.allPage - 1) % _QuaHanTable.allPage;
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

} // namespace TDGQUAHANPAGE

#endif