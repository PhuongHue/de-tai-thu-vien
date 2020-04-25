#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

struct TNData
{
  int key;
  int value;
};

int keyCmp(int a, int b)
{
  return a - b;
}

struct TreeNode
{
  TNData *data = NULL;
  struct TreeNode *left = NULL;
  struct TreeNode *right = NULL;
  TreeNode() {}
  TreeNode(TNData *data)
  {
    this->data = data;
  }
};

void insertTreeNode(TreeNode *&root, TNData *data)
{
  if (root == NULL)
  {
    TreeNode *p = new TreeNode(data);
    root->left = p;
  }

  int keyCmpRes = keyCmp(data->key, root->data->key);
  if (keyCmpRes < 0)
  {
    if (root->left != NULL)
      insertTreeNode(root->left, data);
    else
    {
      TreeNode *p = new TreeNode(data);
      root->left = p;
    }
    return;
  }
  if (keyCmpRes > 0)
  {
    if (root->right != NULL)
    {
      insertTreeNode(root->right, data);
    }
    else
    {
      TreeNode *p = new TreeNode(data);
      root->right = p;
    }
    return;
  }
}