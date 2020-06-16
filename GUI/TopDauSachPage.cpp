#ifndef _TOPDAUSACHPAGE_CPP_
#define _TOPDAUSACHPAGE_CPP_
// TODO: trang top dau sach

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

void duyetTDG(TreeNode *node)
{
  if (node != NULL) {
    // duyet list muon tra
    ListMuonTra *p = node->data->lmt;
    while (p != NULL) {
      // duyet mang top data tang count
      for (int i = 0; i < _TopData_Root.length; i++) {
        DMSach *sach = findByMaSach(_TopData_Root.data[i].ds->dms, p->data->maSach);
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

  // bo dau sach khong co nguoi doc
  for (int i = 0; i < _TopData_Root.length; i++) {
    if (_TopData_Root.data[i].count == 0) {
      // xoa khoi mang
      for (int j = i; i < _TopData_Root.length - 1; i++) {
        _TopData_Root.data[j] = _TopData_Root.data[j + 1];
      }
      _TopData_Root.length--;
    }
  }

  // sort lai mang
  for (int i = 0; i < _TopData_Root.length; i++) {
    int max = _TopData_Root.data[i + 1].count;
    int maxIndex = i + 1;
    for (int j = i + 2; i < _TopData_Root.length; i++) {
      if (_TopData_Root.data[j].count > max) {
        max = _TopData_Root.data[j].count;
        maxIndex = j;
      }
    }
    if (_TopData_Root.data[i].count < max) {
      // swap
      Row c = _TopData_Root.data[i];
      _TopData_Root.data[i] = _TopData_Root.data[maxIndex];
      _TopData_Root.data[maxIndex] = c;
    }
  }
  if (_TopData_Root.length > 10) {
    int lastIndex = 10;
    while (lastIndex < _TopData_Root.length - 1 && _TopData_Root.data[lastIndex].count == _TopData_Root.data[lastIndex + 1].count) {
      // neu thg sau co cung luot doc tang lastIndex len ~ lay 10+
      lastIndex++;
    }
    // dat lai gioi han ve last index ~ xoa nhung thang ngoai top 10
    _TopData_Root.length = lastIndex + 1;
  }
}
/* ---------- view table -------------------*/

const int MAX_PAGE_SIZE = 100;

struct Table {
  Row row[MAX_PAGE_SIZE];
  int left, top, right, bottom;
  int length = 0;
  int pageIndex = 0;
  int pageSize = 20;
  int allPage;
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
    _TopDauSachTable.row[j] = _TopData_Root.data[i];
  }
}

void drawTable(){

}

void clearTable(){
  
}

void initTopDauSachPage()
{
}

void runTopDauSachPage()
{
}

} // namespace TOPDAUSACHPAGE

#endif