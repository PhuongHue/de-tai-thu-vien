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

struct TreeNode {
  TheDocGia *data = NULL;
  struct TreeNode *left = NULL;
  struct TreeNode *right = NULL;
} * _ListTheDocGia_root;

void insert(TreeNode *&root, TheDocGia *tdg)
{
  if (root == NULL) {
    root = new TreeNode;
    root->data = tdg;
    return;
  }
  if (tdg->maThe < root->data->maThe) {
    if (root->left != NULL)
      insert(root->left, tdg);
    else {
      TreeNode *p = new TreeNode;
      p->data = tdg;
      root->left = p;
    }
    return;
  }
  if (tdg->maThe > root->data->maThe) {
    if (root->right != NULL) {
      insert(root->right, tdg);
    }
    else {
      TreeNode *p = new TreeNode;
      p->data = tdg;
      root->right = p;
    }
    return;
  }
  if (tdg->maThe == root->data->maThe) {
    root->data = tdg;
  }
}

void findAndDelete(TreeNode *&node, long long maThe, bool &deleted)
{
  if (node == NULL) return;
  if (maThe < node->data->maThe && node->left != NULL)
    findAndDelete(node->left, maThe, deleted);
  else if (maThe > node->data->maThe && node->right != NULL)
    findAndDelete(node->right, maThe, deleted);
  else {
    delete node->data;
    TreeNode *p = node;
    deleted = true;
    if (p->left != NULL) {
      p = p->left;
      if (p->right == NULL) {
        node->left = p->left;
      }
      else {
        while (p->right != NULL)
          p = p->right;
      }
      node->data = p->data;
    }
    else if (p->right != NULL) {
      p = p->right;
      if (p->left == NULL) {
        node->right = p->right;
      }
      else {
        while (p->left != NULL) {
          node->left = p->left;
        }
      }
      node->data = p->data;
    }
    else {
      node = NULL;
    }
    delete p;
  }
}

int countAll(TreeNode *&node)
{
  if (node == NULL) return 0;
  if (node->left == NULL && node->right == NULL) return 1;
  return countAll(node->left) + countAll(node->right);
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

void duyetLNR(TreeNode *node, int from = -1, int to = -1)
{
  if (node != NULL) {
    duyetLNR(node->left, from, to);
    if (from == -1 && to == -1) {
      _TDGArray_temp.data[_TDGArray_temp.length] = node->data;
      _TDGArray_temp.length++;
    }
    else {
      if (_LNR_temp_index >= from && _LNR_temp_index <= to) {
        _TDGArray_temp.data[_TDGArray_temp.length] = node->data;
        _TDGArray_temp.length++;
      }
    }
    _LNR_temp_index++;
    duyetLNR(node->right, from, to);
  }
}

void luuFile(TheDocGia *tdg, ofstream &fout)
{
  fout << tdg->maThe << endl
       << tdg->ho << endl
       << tdg->ten << endl
       << (tdg->phai ? 1 : 0) << endl
       << tdg->trangThai << endl;
}

void duyetLNRLuuFile(TreeNode *node, ofstream &fout)
{
  if (node != NULL) {
    duyetLNR(node->left);
    luuFile(node->data, fout);
    duyetLNR(node->right);
  }
}

void luuFile(TreeNode *node)
{
  ofstream fout;
  fout.open("data/TheDocGia.data");
  fout << countAll(node) << endl;
  duyetLNRLuuFile(node, fout);
  fout.close();
}

void docFile(TheDocGia *tdg, ifstream &fin)
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

void docFile(TreeNode *node, ifstream &fin)
{
  int all;
  fin >> all;
  fin.ignore();
  TheDocGia *tdg = new TheDocGia;
  docFile(tdg, fin);
  insert(node, tdg);
}

#endif