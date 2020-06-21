#ifndef _DANHMUC_CPP_
#define _DANHMUC_CPP_

#include <fstream>
#include <iostream>
#include <string>
#include <time.h>

#include "GUI/consolelib.h"
using namespace std;

#define SACH_TT_MUONDUOC 0
#define SACH_TT_DAMUON 1
#define SACH_TT_THANHLY 2

struct Sach {
  long long maSach;
  int trangThai;
  string viTri;
};


struct DMSach {
  Sach data;
  DMSach *next = NULL;
};

DMSach *clipboardDMSach = NULL;

long long getNewMaSach()
{
  return time(0);
}

bool kiemTraDieuKienMuon(DMSach *sach)
{
  return sach->data.trangThai == SACH_TT_MUONDUOC;
}

void swapNodeData(DMSach *&a, DMSach *&b)
{
  Sach c = a->data;
  a->data = b->data;
  b->data = c;
}

void addFirst(DMSach *&first, Sach info)
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

void addLast(DMSach *&first, Sach info)
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
    if (p->data.maSach == key) {
      return p;
    }
    p = p->next;
  }
  return NULL;
}

bool deleteByMaSach(DMSach *&first, long long key)
{
  DMSach *p = first;
  DMSach *q = NULL;
  while (p != NULL) {
    if (p->data.maSach == key) {
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
      if (p == clipboardDMSach) clipboardDMSach = NULL;
      delete p;
      return true;
    }
    q = p;
    p = p->next;
  }
  return false;
}
void luuFileDMS(DMSach *first, ofstream &fout)
{
  DMSach *p = first;
  fout << countAll(first) << endl;
  while (p != NULL) {
    fout << p->data.maSach << endl
         << p->data.trangThai << endl
         << p->data.viTri << endl;
    p = p->next;
  }
}

void docFileDMS(DMSach *&list, fstream &fin)
{
  int n;
  fin >> n;
  fin.ignore();
  if (n == 0) return;
  for (int i = 0; i < n; i++) {
    Sach data;
    fin >> data.maSach >> data.trangThai;
    fin.ignore();
    getline(fin, data.viTri);
    addLast(list, data);
  }
}

void sortByMS(DMSach *&first)
{
  for (DMSach *i = first; i != NULL && i->next != NULL; i = i->next) {
    for (DMSach *j = i->next; j != NULL; j = j->next) {
      if (i->data.maSach > j->data.maSach) swapNodeData(i, j);
    }
  }
}

#endif
