#ifndef _MUONTRA_CPP_
#define _MUONTRA_CPP_

#include <fstream>
#include <iostream>
#include <string>
#include <time.h>

#include "GUI/consolelib.h"
using namespace std;

#define CHO_MUON_DUOC 0
#define DA_CHO_MUON 1
#define DA_THANH_LY 2

struct MuonTra {
  long long maSach;
  long long ngayMuon;
  long long ngayTra;
  int trangThai;
};

struct ListMuonTra {
  MuonTra *data = NULL;
  ListMuonTra *next = NULL;
  ListMuonTra *prev = NULL;
};

ListMuonTra *getNewListMuonTra()
{
  ListMuonTra *lmt = new ListMuonTra;
  lmt->data = new MuonTra;
  return lmt;
}

typedef void (*CallBackDMSach)(ListMuonTra *node);

void foreachDMSach(ListMuonTra *first, CallBackDMSach callBack)
{
  ListMuonTra *p = first;
  while (p != NULL) {
    callBack(p);
    p = p->next;
  }
}

void swapNodeData(ListMuonTra *&a, ListMuonTra *&b)
{
  MuonTra *c = a->data;
  a->data = b->data;
  b->data = c;
}

void addFirst(ListMuonTra *&first, MuonTra *info)
{
  if (first == NULL) {
    first = new ListMuonTra;
    first->data = info;
    return;
  }
  ListMuonTra *p = new ListMuonTra;
  p->data = info;
  p->next = first;
  first->prev = p;
}

void addLast(ListMuonTra *&first, MuonTra *info)
{
  if (first == NULL) {
    first = new ListMuonTra;
    first->data = info;
    return;
  }
  ListMuonTra *p = first;
  while (p->next)
    p = p->next;
  ListMuonTra *q = new ListMuonTra;
  q->data = info;
  p->next = q;
  q->prev = p;
}

int countAll(ListMuonTra *first)
{
  ListMuonTra *p = first;
  int count = 0;
  while (p != NULL) {
    count++;
    p = p->next;
  }
  return count;
}

ListMuonTra *findByMaSach(ListMuonTra *first, long long key)
{
  ListMuonTra *p = first;
  ListMuonTra *kq = NULL;
  while (p != NULL) {
    MuonTra *info = p->data;
    if (info->maSach == key) {
      kq = p;
      break;
    }
    p = p->next;
  }
  return kq;
}

void deleteAll(ListMuonTra *&first)
{
  ListMuonTra *p = first;
  first = NULL;
  while (p != NULL) {
    ListMuonTra *x = p;
    p = p->next;
    delete x->data;
    delete x;
  }
}

bool deleteByMaSach(ListMuonTra *&first, long long key)
{
  ListMuonTra *p = findByMaSach(first, key);
  if (p == NULL)
    return false;
  else {
    if (p->prev == NULL) {
      first = first->next;
      first->prev = NULL;
    }
    else {
      p->prev->next = p->next;
      p->next->prev = p->prev;
      delete p->data;
      delete p;
    }
    return true;
  }
}
void luuFile(ListMuonTra *&first, ofstream &fout)
{
  ListMuonTra *p = first;
  fout << countAll(first) << endl;
  while (p != NULL) {
    MuonTra *data = p->data;
    fout << data->maSach << endl
         << data->ngayMuon << endl
         << data->ngayTra << endl
         << data->trangThai << endl;
    p = p->next;
  }
}

void docFile(ListMuonTra *&list, fstream &fin)
{
  int n;
  fin >> n;
  for (int i = 0; i < n; i++) {
    MuonTra *data = new MuonTra;
    fin >> data->maSach >> data->ngayMuon >> data->ngayTra >> data->trangThai;
    fin.ignore();
    addLast(list, data);
  }
}

void sortByMS(ListMuonTra *&first)
{
  for (ListMuonTra *i = first; i != NULL && i->next != NULL; i = i->next) {
    for (ListMuonTra *j = i->next; j != NULL; j = j->next) {
      if (i->data->maSach > j->data->maSach) swapNodeData(i, j);
    }
  }
}

#endif
