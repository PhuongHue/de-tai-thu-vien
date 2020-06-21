#ifndef _THEDOCGIA_CPP_
#define _THEDOCGIA_CPP_

#include <cstring>
#include <fstream>
#include <iostream>

#include "MuonTra.cpp"

using namespace std;

#define TDG_TT_KHOA 0
#define TDG_TT_HOATDONG 1

struct TheDocGia {
  long long maThe;
  string ho;
  string ten;
  bool phai; // true=nam false=nu
  int trangThai;
  ListMuonTra *lmt = NULL;
};

TheDocGia *clipboardTDG = NULL;

long long getNewMaTheDocGia()
{
  return time(0);
}

struct TreeNode {
  TheDocGia *data = NULL;
  struct TreeNode *left = NULL;
  struct TreeNode *right = NULL;
};

TreeNode *_ListTheDocGia_root = NULL;

void insert(TreeNode *&node, TheDocGia *tdg)
{
  if (node == NULL) {
    node = new TreeNode;
    node->data = tdg;
    return;
  }
  if (tdg->maThe < node->data->maThe) {
    insert(node->left, tdg);
  }
  else if (tdg->maThe > node->data->maThe) {
    insert(node->right, tdg);
  }
  else {
    node->data = tdg;
  }
}

TreeNode *rp;

void deleteCase3(TreeNode *&r)
{
  if (r->left != NULL)
    deleteCase3(r->left);
  else {
    rp->data = r->data;
    rp = r;
    r = rp->right;
  }
}

void findAndDelete(TreeNode *&node, long long maThe, bool &deleted)
{
  if (node == NULL) return;
  if (maThe < node->data->maThe)
    findAndDelete(node->left, maThe, deleted);
  else if (maThe > node->data->maThe)
    findAndDelete(node->right, maThe, deleted);
  else {
    rp = node;
    if (node->right == NULL) {
      node = rp->left;
    }
    else if (node->left == NULL) {
      node = rp->right;
    }
    else {
      // xoa bo nho copy tam
      if (node->data == clipboardTDG)
        clipboardTDG = NULL;

      deleteCase3(rp->right);
    }
    delete rp;
  }
}

TreeNode *find(TreeNode *&node, long long maThe)
{
  if (node == NULL) return NULL;
  if (maThe < node->data->maThe)
    return find(node->left, maThe);
  else if (maThe > node->data->maThe)
    return find(node->right, maThe);
  else {
    return node;
  }
}

int countAll(TreeNode *&node)
{
  if (node == NULL)
    return 0;
  else {
    int c = 1;
    c += countAll(node->left);
    c += countAll(node->right);
    return c;
  }
}

#define MAX_TDG_ARRAY_LENGTH 50

struct TDGArray {
  TheDocGia *data[MAX_TDG_ARRAY_LENGTH];
  int length;
} _TDGArray_temp;

int _LNR_temp_index;

void initDuyetLNR()
{
  for (int i = 0; i < MAX_TDG_ARRAY_LENGTH; i++) {
    _TDGArray_temp.data[i] = NULL;
  }

  _TDGArray_temp.length = 0;
  _LNR_temp_index = 0;
}

void duyetLNR(TreeNode *node, int from, int to)
{
  if (node != NULL) {
    duyetLNR(node->left, from, to);

    if (_LNR_temp_index >= from && _LNR_temp_index <= to) {
      _TDGArray_temp.data[_TDGArray_temp.length] = node->data;
      _TDGArray_temp.length++;
    }
    _LNR_temp_index++;

    duyetLNR(node->right, from, to);
  }
}

void luuFileNodeTDG(TheDocGia *tdg, fstream &fout)
{
  fout << tdg->maThe << endl
       << tdg->ho << endl
       << tdg->ten << endl
       << (tdg->phai ? 1 : 0) << endl
       << tdg->trangThai << endl;
  luuFileLMT(tdg->lmt, fout);
}

void duyetNLRLuuFile(TreeNode *node, fstream &fout)
{
  if (node != NULL) {
    luuFileNodeTDG(node->data, fout);
    duyetNLRLuuFile(node->left, fout);
    duyetNLRLuuFile(node->right, fout);
  }
}

void luuFileTDG(TreeNode *node)
{
  fstream fout;
  fout.open("data/TheDocGia.data");
  fout << countAll(node) << endl;
  duyetNLRLuuFile(node, fout);
  fout.close();
}

void docFileNodeTDG(TheDocGia *tdg, fstream &fin)
{
  fin >> tdg->maThe;
  fin.ignore();
  getline(fin, tdg->ho);
  getline(fin, tdg->ten);

  int phai;
  fin >> phai;
  fin.ignore();
  if (phai == 1)
    tdg->phai = true;
  else
    tdg->phai = false;

  fin >> tdg->trangThai;
  fin.ignore();
  docFileLMT(tdg->lmt, fin);
}

bool docFileTDG(TreeNode *&tree)
{
  fstream fin("data/TheDocGia.data", fstream::in);
  if (!fin.is_open()) return false;
  int n;
  fin >> n;
  // file rong
  if (fin.eof()) return true;
  fin.ignore();
  for (int i = 0; i < n; i++) {
    TheDocGia *tdg = new TheDocGia;
    docFileNodeTDG(tdg, fin);
    insert(tree, tdg);
  }
  return true;
}

long long countQuaHan(TheDocGia *tdg)
{
  long long date = -1;
  long long currentDate = getDate();
  ListMuonTra *p = tdg->lmt;
  while (p != NULL) {
    if (p->data->trangThai == MT_TT_DANGMUON) {
      // qua 7 ngay
      if (currentDate - p->data->ngayMuon >= TIME_7_NGAY)
        // tim min => ngay muon chua tra lau nhat
        if (date == -1 || date > p->data->ngayMuon) {
          date = p->data->ngayMuon;
        }
    }
    p = p->next;
  }
  return date;
}

bool kiemTraXoaDMSach(TreeNode *node, long long maSach)
{
  if (node == NULL) {
    return true;
  }
  else {
    ListMuonTra *mt = findByMaSach(node->data->lmt, maSach);
    if (mt != NULL) return false;
    return kiemTraXoaDMSach(node->left, maSach) && kiemTraXoaDMSach(node->right, maSach);
  }
}

#endif