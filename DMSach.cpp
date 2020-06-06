#ifndef _DANHMUC_CPP_
#define _DANHMUC_CPP_

#include <fstream>
#include <iostream>
#include <string>
#include <time.h>

#include "GUI/consolelib.h"
using namespace std;

#define CHO_MUON_DUOC 0
#define DA_CHO_MUON 1
#define DA_THANH_LY 2

struct Sach {
  long long maSach;
  int trangThai;
  string viTri;
};

Sach *clipboardSach = NULL;

long long getNewMaSach()
{
  time_t currentTime;
  time(&currentTime);
  return currentTime;
}

int compareData(Sach *a, Sach *b)
{
  int cmpMaSach = a->maSach - b->maSach;
  if (cmpMaSach != 0)
    return cmpMaSach;
  else {
    int cmpTrangThai = a->trangThai - b->trangThai;
    if (cmpTrangThai != 0)
      return cmpTrangThai;
    else {
      int cmpViTri = a->viTri.compare(b->viTri);
      return cmpViTri;
    }
  }
};

struct DMSach {
  Sach *data = NULL;
  DMSach *next = NULL;
};

DMSach *getNewDMSach()
{
  DMSach *dms = new DMSach;
  dms->data = new Sach;
  return dms;
}

struct PairNode {
  DMSach *before = NULL;
  DMSach *value = NULL;
};

typedef void (*CallBackDMSach)(DMSach *node);

void foreachDMSach(DMSach *first, CallBackDMSach callBack)
{
  DMSach *p = first;
  while (p != NULL) {
    callBack(p);
    p = p->next;
  }
}

void swapNodeData(DMSach *&a, DMSach *&b)
{
  Sach *c = a->data;
  a->data = b->data;
  b->data = c;
}

void addFirst(DMSach *&first, Sach *info)
{
  if (first == NULL) {
    first = new DMSach;
    first->data = info;
    return;
  }
  DMSach *p = new DMSach;
  p->data = info;
  p->next = first;
  first = p;
}

void addLast(DMSach *&first, Sach *info)
{
  if (first == NULL) {
    first = new DMSach;
    first->data = info;
    return;
  }
  DMSach *p = first;
  while (p->next != NULL)
    p = p->next;
  DMSach *q = new DMSach;
  q->data = info;
  p->next = q;
}

int countAll(DMSach *first)
{
  DMSach *p = first;
  int count = 0;
  while (p != NULL) {
    count++;
    p = p->next;
  }
  return count;
}

DMSach *findByMaSach(DMSach *first, long long key)
{
  DMSach *p = first;
  while (p != NULL) {
    if (p->data->maSach == key) {
      return p;
    }
    p = p->next;
  }
  return NULL;
}

void deleteAll(DMSach *&first)
{
  DMSach *p = first;
  first = NULL;
  while (p != NULL) {
    DMSach *x = p;
    p = p->next;
    delete x->data;
    delete x;
  }
}

bool deleteByMaSach(DMSach *&first, long long key)
{
  DMSach *p = first;
  DMSach *q = NULL;
  while (p != NULL) {
    if (p->data->maSach == key) {
      if (q == NULL) {
        // delete first
        if (first->next == NULL) {
          first = NULL;
        }
        else {
          first = first->next;
        }
      }
      else {
        q->next = p->next;
      }
      if (p->data == clipboardSach) clipboardSach = NULL;
      delete p;
      return true;
    }
    q = p;
    p = p->next;
  }
  return false;
}
void luuFile(DMSach *first, ofstream &fout)
{
  DMSach *p = first;
  fout << countAll(first) << endl;
  while (p != NULL) {
    Sach *data = p->data;
    fout << data->maSach << endl
         << data->trangThai << endl
         << data->viTri << endl;
    p = p->next;
  }
}

void docFile(DMSach *&list, fstream &fin)
{
  int n;
  fin >> n;
  fin.ignore();
  if (n == 0) return;
  for (int i = 0; i < n; i++) {
    Sach *data = new Sach;
    fin >> data->maSach >> data->trangThai;
    fin.ignore();
    getline(fin, data->viTri);
    addLast(list, data);
  }
}

void sortByMS(DMSach *&first)
{
  for (DMSach *i = first; i != NULL && i->next != NULL; i = i->next) {
    for (DMSach *j = i->next; j != NULL; j = j->next) {
      if (i->data->maSach > j->data->maSach) swapNodeData(i, j);
    }
  }
}

int compareDataDanhMuc(Sach *a, Sach *b)
{
  int cmpTrangThai = a->trangThai - b->trangThai;
  if (cmpTrangThai != 0)
    return cmpTrangThai;
  else {
    return a->viTri.compare(a->viTri);
  }
}

void sortByTTVT(DMSach *&first)
{
  for (DMSach *i = first; i != NULL && i->next != NULL; i = i->next) {
    for (DMSach *j = i->next; j != NULL; j = j->next) {
      if (compareDataDanhMuc(i->data, j->data) > 0) swapNodeData(i, j);
    }
  }
}

#endif
