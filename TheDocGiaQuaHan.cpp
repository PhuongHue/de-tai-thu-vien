#ifndef _THEDOCGIAQUAHAN_CPP_
#define _THEDOCGIAQUAHAN_CPP_

#include "DauSach.cpp"
#include "TheDocGia.cpp"

struct QuaHan_Row {
  long long mathe;
  string hoTen;
  long long maSach;
  string tenSach;
  long long ngayMuon;
  int ngayQuaHan;
};

const int MAX_QUAHAN_DATA_SIZE = 1000;

struct QuaHan_Data {
  QuaHan_Row data[MAX_QUAHAN_DATA_SIZE];
  int length = 0;
} _QuaHanData_Root;

void addLast(TheDocGia tdg, ListMuonTra *quaHan)
{
  ListMuonTra *p = quaHan;
  while (p != NULL) {
    if (_QuaHanData_Root.length == MAX_QUAHAN_DATA_SIZE) return;
    _QuaHanData_Root.data[_QuaHanData_Root.length].mathe = tdg.maThe;
    _QuaHanData_Root.data[_QuaHanData_Root.length].hoTen = tdg.ho + ' ' + tdg.ten;
    _QuaHanData_Root.data[_QuaHanData_Root.length].maSach = p->data.maSach;
    _QuaHanData_Root.data[_QuaHanData_Root.length].ngayMuon = p->data.ngayMuon;
    DauSach *ds = tim_DauSach_theo_MaSach(_ListDauSach_Root, p->data.maSach);
    _QuaHanData_Root.data[_QuaHanData_Root.length].tenSach = ds->tenSach;
    if (p->data.trangThai == MT_TT_DANGMUON) {
      _QuaHanData_Root.data[_QuaHanData_Root.length].ngayQuaHan = (getDate() - p->data.ngayMuon) / TIME_1_NGAY;
    }
    else {
      _QuaHanData_Root.data[_QuaHanData_Root.length].ngayQuaHan = (p->data.ngayTra - p->data.ngayMuon) / TIME_1_NGAY;
    }
    _QuaHanData_Root.length++;
    p = p->next;
  }
}

void duyetTDG_QH(NodeTheDocGia *node)
{
  if (node != NULL) {
    ListMuonTra *quaHan = filterQuaHan(node->data.lmt);
    if (quaHan != NULL) {
      addLast(node->data, quaHan);
    }
    duyetTDG_QH(node->left);
    duyetTDG_QH(node->right);
  }
}

void sortTDGQuaHan()
{
  for (int i = 0; i < _QuaHanData_Root.length; i++) {
    QuaHan_Row max = _QuaHanData_Root.data[i];
    int maxIndex = i;
    for (int j = i + 1; j < _QuaHanData_Root.length; j++) {
      if (_QuaHanData_Root.data[j].ngayQuaHan > max.ngayQuaHan) {
        max = _QuaHanData_Root.data[j];
        maxIndex = j;
      }
    }
    // swap
    if (maxIndex != i) {
      QuaHan_Row c = _QuaHanData_Root.data[i];
      _QuaHanData_Root.data[i] = max;
      _QuaHanData_Root.data[maxIndex] = c;
    }
  }
}

void thongKeQuaHan()
{
  _QuaHanData_Root.length = 0;
  duyetTDG_QH(_ListTheDocGia_root);
  sortTDGQuaHan();
}

#endif