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

long long getNewMaTheDocGia()
{
  return time(0);
}

struct NodeTheDocGia {
  TheDocGia data;
  NodeTheDocGia *left = NULL;
  NodeTheDocGia *right = NULL;
};

NodeTheDocGia *_ListTheDocGia_root = NULL;

NodeTheDocGia *clipboardNodeTDG = NULL;

void insert(NodeTheDocGia *&node, TheDocGia tdg)
{
  if (node == NULL) {
    node = new NodeTheDocGia;
    node->data = tdg;
    return;
  }
  if (tdg.maThe < node->data.maThe) {
    insert(node->left, tdg);
  }
  else if (tdg.maThe > node->data.maThe) {
    insert(node->right, tdg);
  }
  else {
    node->data = tdg;
  }
}

NodeTheDocGia *rp;

void deleteCase3(NodeTheDocGia *&r)
{
  if (r->left != NULL)
    deleteCase3(r->left);
  else {
    rp->data = r->data;
    rp = r;
    r = rp->right;
  }
}

void findAndDelete(NodeTheDocGia *&node, long long maThe, bool &deleted)
{
  if (node == NULL) return;
  if (maThe < node->data.maThe)
    findAndDelete(node->left, maThe, deleted);
  else if (maThe > node->data.maThe)
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
      if (node == clipboardNodeTDG)
        clipboardNodeTDG = NULL;

      deleteCase3(rp->right);
    }
    delete rp;
  }
}

NodeTheDocGia *find(NodeTheDocGia *&node, long long maThe)
{
  if (node == NULL) return NULL;
  if (maThe < node->data.maThe)
    return find(node->left, maThe);
  else if (maThe > node->data.maThe)
    return find(node->right, maThe);
  else {
    return node;
  }
}

int countAll(NodeTheDocGia *&node)
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
  NodeTheDocGia *data[MAX_TDG_ARRAY_LENGTH];
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

void duyetLNR(NodeTheDocGia *node, int from, int to)
{
  if (node != NULL) {
    duyetLNR(node->left, from, to);

    if (_LNR_temp_index >= from && _LNR_temp_index <= to) {
      _TDGArray_temp.data[_TDGArray_temp.length] = node;
      _TDGArray_temp.length++;
    }
    _LNR_temp_index++;

    duyetLNR(node->right, from, to);
  }
}

void luuFileNodeTDG(TheDocGia tdg, fstream &fout)
{
  fout << tdg.maThe << endl
       << tdg.ho << endl
       << tdg.ten << endl
       << (tdg.phai ? 1 : 0) << endl
       << tdg.trangThai << endl;
  luuFileLMT(tdg.lmt, fout);
}

void duyetNLRLuuFile(NodeTheDocGia *node, fstream &fout)
{
  if (node != NULL) {
    luuFileNodeTDG(node->data, fout);
    duyetNLRLuuFile(node->left, fout);
    duyetNLRLuuFile(node->right, fout);
  }
}

void luuFileTDG(NodeTheDocGia *node)
{
  fstream fout;
  fout.open("data/TheDocGia.data");
  fout << countAll(node) << endl;
  duyetNLRLuuFile(node, fout);
  fout.close();
}

void docFileNodeTDG(TheDocGia &tdg, fstream &fin)
{
  fin >> tdg.maThe;
  fin.ignore();
  getline(fin, tdg.ho);
  getline(fin, tdg.ten);

  int phai;
  fin >> phai;
  fin.ignore();
  if (phai == 1)
    tdg.phai = true;
  else
    tdg.phai = false;

  fin >> tdg.trangThai;
  fin.ignore();
  docFileLMT(tdg.lmt, fin);
}

bool docFileTDG(NodeTheDocGia *&tree)
{
  fstream fin("data/TheDocGia.data", fstream::in);
  if (!fin.is_open()) return false;
  int n;
  fin >> n;
  // file rong
  if (fin.eof()) return true;
  fin.ignore();
  for (int i = 0; i < n; i++) {
    TheDocGia tdg;
    docFileNodeTDG(tdg, fin);
    insert(tree, tdg);
  }
  return true;
}

// for DMSach page
bool kiemTraXoaDMSach(NodeTheDocGia *node, long long maSach)  
{
  if (node == NULL) {
    return true;
  }
  else {
    bool mt = check_MaSach_DaTonTai(node->data.lmt, maSach);
    if (mt == true) return false;
    return kiemTraXoaDMSach(node->left, maSach) && kiemTraXoaDMSach(node->right, maSach);
  }
}

#endif