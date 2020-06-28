#ifndef _TOPDAUSACH_CPP_
#define _TOPDAUSACH_CPP_

#include "DauSach.cpp"
#include "TheDocGia.cpp"

const int MAX_TOP_DATA_SIZE = 1000;

struct TopDauSach_Row {
  DauSach *ds;
  int count;
};

struct TopDauSach_Data {
  TopDauSach_Row data[MAX_TOP_DATA_SIZE];
  int length = 0;
} _TopData_Root;

void duyetTDG_TDS(NodeTheDocGia *node)
{
  if (node != NULL) {
    // duyet list muon tra
    ListMuonTra *p = node->data.lmt;
    while (p != NULL) {
      // duyet tang count
      for (int i = 0; i < _TopData_Root.length; i++) {
        DMSach *sach = findByMaSach(_TopData_Root.data[i].ds->dms, p->data.maSach);
        if (sach != NULL) _TopData_Root.data[i].count++;
      }
      // next
      p = p->next;
    }
    duyetTDG_TDS(node->left);
    duyetTDG_TDS(node->right);
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

  duyetTDG_TDS(_ListTheDocGia_root);

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
      TopDauSach_Row c = _TopData_Root.data[i];
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
    int lastIndex = 9;
    while (lastIndex < _TopData_Root.length - 1) {
      if (_TopData_Root.data[9].count != _TopData_Root.data[lastIndex + 1].count) break;
      lastIndex++;
    }
    _TopData_Root.length = lastIndex + 1;
  }
}

#endif