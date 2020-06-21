#ifndef _TOPDAUSACHPAGE_CPP_
#define _TOPDAUSACHPAGE_CPP_

#include "../DauSach.cpp"
#include "../TheDocGia.cpp"
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

/* ---------- thong ke data ----------------*/
const int MAX_TOP_DATA_SIZE = 1000;

struct Row {
  DauSach *ds;
  int count;
};

struct TopData {
  Row data[MAX_TOP_DATA_SIZE];
  int length = 0;
} _TopData_Root;

void duyetTDG(NodeTheDocGia *node)
{
  if (node != NULL) {
    // duyet list muon tra
    ListMuonTra *p = node->data.lmt;
    while (p != NULL) {
      // duyet mang top data tang count
      for (int i = 0; i < _TopData_Root.length; i++) {
        DMSach *sach = findByMaSach(_TopData_Root.data[i].ds->dms, p->data.maSach);
        if (sach != NULL) _TopData_Root.data[i].count++;
      }
      // next
      p = p->next;
    }
    // duyet tiep
    duyetTDG(node->left);
    duyetTDG(node->right);
  }
}

void thongKeDauSach()
{
  _TopData_Root.length = 0;
  for (int i = 0; i < _ListDauSach_Root.length; i++) {
    _TopData_Root.data[_TopData_Root.length].ds = _ListDauSach_Root.data[i];
    _TopData_Root.data[_TopData_Root.length].count = 0;
    _TopData_Root.length++;
  }

  duyetTDG(_ListTheDocGia_root);

  // sort lai mang
  for (int i = 0; i < _TopData_Root.length; i++) {
    int max = _TopData_Root.data[i].count;
    int maxIndex = i;
    for (int j = i + 1; j < _TopData_Root.length; j++) {
      if (_TopData_Root.data[j].count > max) {
        max = _TopData_Root.data[j].count;
        maxIndex = j;
      }
    }
    if (maxIndex != i) {
      // swap
      Row c = _TopData_Root.data[i];
      _TopData_Root.data[i] = _TopData_Root.data[maxIndex];
      _TopData_Root.data[maxIndex] = c;
    }
  }

  // bo dau sach khong co nguoi doc
  for (int i = 0; i < _TopData_Root.length; i++) {
    if (_TopData_Root.data[i].count == 0) {
      _TopData_Root.length = i;
      break;
    }
  }

  if (_TopData_Root.length > 10) {
    int lastIndex = 10;
    while (
        lastIndex < _TopData_Root.length - 1 &&
        _TopData_Root.data[lastIndex].count == _TopData_Root.data[lastIndex + 1].count) {
      // neu thg sau co cung luot doc tang lastIndex len ~ lay 10+
      // vd: 1 2 3 4 5 6 7 8 9 10 10 10 11 12
      //                             ^
      //                           lastIndex
      lastIndex++;
    }
    // dat lai gioi han ve last index ~ xoa nhung thang ngoai top 10
    _TopData_Root.length = lastIndex + 1;
    // kq: 1 2 3 4 5 6 7 8 9 10 10 10
  }
}
/* ---------- view table -------------------*/

const int MAX_PAGE_SIZE = 100;

struct Table {
  int left, top, right, bottom;
  int columns[7] = {0, 8, 29, 60, 86, 112, 133};
  Row rows[MAX_PAGE_SIZE];
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

int getIndex(int lineNumber){
  return _TopDauSachTable.pageIndex * _TopDauSachTable.pageSize + lineNumber + 1;
}

void drawTable()
{
  for (int i = 0; i < _TopDauSachTable.lineCount; i++) {
    gotoxy(_TopDauSachTable.left + _TopDauSachTable.columns[0], _TopDauSachTable.top + i);
    cout <<right <<setw(4) << getIndex(i);
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