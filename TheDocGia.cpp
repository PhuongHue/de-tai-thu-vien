#ifndef _THEDOCGIA_CPP_
#define _THEDOCGIA_CPP_

#include <cstring>
#include <fstream>
#include <iostream>

#include "MuonTra.cpp"

using namespace std;

struct TheDocGia {
  long long maThe;
  string ho;
  string ten;
  bool phai;
  int trangThai;
  ListMuonTra *lmt = NULL;
};

TheDocGia *clipboardTDG = NULL;

long long getNewMaTheDocGia()
{
  time_t currentTime;
  time(&currentTime);
  return currentTime;
}

struct TreeNode {
  TheDocGia *data = NULL;
  struct TreeNode *left = NULL;
  struct TreeNode *right = NULL;
} * _ListTheDocGia_root;

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

void findAndDelete(TreeNode *&node, long long maThe, bool &deleted)
{
  if (node == NULL) return;
  if (maThe < node->data->maThe)
    findAndDelete(node->left, maThe, deleted);
  else if (maThe > node->data->maThe)
    findAndDelete(node->right, maThe, deleted);
  else {
    if (node->right == NULL) {
      node = node->left;
    }
    else if (node->left == NULL) {
      node = node->right;
    }
    else {
      delete node->data;
      TreeNode *pParent = node;
      TreeNode *p = node->right;

      while (p->left != NULL) {
        pParent = p;
        p = p->left;
      }
      if (p != node) {
        pParent->left = p->right;
      }
      else {
        pParent->right = p->right;
      }
      delete p;
      deleted = true;
    }
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
  int c = 1;
  if (node == NULL)
    return 0;
  else {
    c += countAll(node->left);
    c += countAll(node->right);
    return c;
  }
}

void consoleLogLNR(TreeNode *node)
{
  if (node != NULL) {
    consoleLogLNR(node->left);
    consoleLog<string>(to_string(node->data->maThe) + ": " + node->data->ho + ' ' + node->data->ten);
    consoleLogLNR(node->right);
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

void luuFile(TheDocGia *tdg, fstream &fout)
{
  fout << tdg->maThe << endl
       << tdg->ho << endl
       << tdg->ten << endl
       << (tdg->phai ? 1 : 0) << endl
       << tdg->trangThai << endl;
}

void duyetLNRLuuFile(TreeNode *node, fstream &fout)
{
  if (node != NULL) {
    duyetLNRLuuFile(node->left, fout);
    luuFile(node->data, fout);
    duyetLNRLuuFile(node->right, fout);
  }
}

void luuFile(TreeNode *node)
{
  fstream fout;
  fout.open("data/TheDocGia.data");
  fout << countAll(node) << endl;
  duyetLNRLuuFile(node, fout);
  fout.close();
}

void docFile(TheDocGia *tdg, fstream &fin)
{
  int phai;
  fin >> tdg->maThe;
  fin.ignore();
  getline(fin, tdg->ho);
  getline(fin, tdg->ten);
  fin >> phai;
  tdg->phai = (phai == 1);
  fin.ignore();
  fin >> tdg->trangThai;
  fin.ignore();
}

bool docFile(TreeNode *&node)
{
  fstream fin("data/TheDocGia.data", fstream::in);
  if (!fin.is_open() || fin.eof()) return false;
  int all;
  fin >> all;
  fin.ignore();
  for (int i = 0; i < all; i++) {
    TheDocGia *tdg = new TheDocGia;
    docFile(tdg, fin);
    insert(node, tdg);
  }
  return true;
}

#endif