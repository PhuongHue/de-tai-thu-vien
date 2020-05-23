#ifndef _THEDOCGIA_CPP_
#define _THEDOCGIA_CPP_

#include <cstring>
#include <fstream>
#include <iostream>

#include "MuonTra.cpp"

using namespace std;

struct TNData {
  long long maThe;
  string ho;
  string ten;
  string phai;
  int trangThai;
  ListMuonTra *lmt = NULL;
};

int keyCmp(long long a, long long b) { return a - b; }

struct TreeNode {
  TNData *data = NULL;
  struct TreeNode *left = NULL;
  struct TreeNode *right = NULL;
} * _ListTheDocGia_root;

void insertTreeNode(TreeNode *&root, TNData *data)
{
  if (root == NULL) {
    TreeNode *p = new TreeNode;
    p->data = data;
    root->left = p;
  }

  int keyCmpRes = keyCmp(data->maThe, root->data->maThe);
  if (keyCmpRes < 0) {
    if (root->left != NULL)
      insertTreeNode(root->left, data);
    else {
      TreeNode *p = new TreeNode;
      p->data = data;
      root->left = p;
    }
    return;
  }
  if (keyCmpRes > 0) {
    if (root->right != NULL) {
      insertTreeNode(root->right, data);
    }
    else {
      TreeNode *p = new TreeNode;
      p->data = data;
      root->right = p;
    }
    return;
  }
}

#endif