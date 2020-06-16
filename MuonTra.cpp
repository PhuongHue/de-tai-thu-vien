#ifndef _MUONTRA_CPP_
#define _MUONTRA_CPP_

#include <fstream>
#include <iostream>
#include <string>
#include <time.h>

#include "GUI/consolelib.h"
using namespace std;

#define MT_TT_DANGMUON 0
#define MT_TT_DATRA 1
#define MT_TT_MATSACH 2

struct MuonTra {
  long long maSach;
  long long ngayMuon;
  long long ngayTra = -1;
  int trangThai = MT_TT_DANGMUON;
};

struct ListMuonTra {
  MuonTra *data = NULL;
  ListMuonTra *next = NULL;
  ListMuonTra *prev = NULL;
};

long long getDate()
{
  // get time vd: 15931520
  time_t t = time(0);
  // doi time => object ngay + gio vd: 20/6/2020 16h32p
  tm *timeTemp = localtime(&t);
  // dua ve 00h 00m vd: 20/6/2020 0h0p
  timeTemp->tm_hour = 0;
  timeTemp->tm_min = 0;
  timeTemp->tm_sec = 0;
  // doi lai thanh so time theo giay tinh tu 1990 vd: 15911000
  return mktime(timeTemp);
}

ListMuonTra *getNewListMuonTra()
{
  ListMuonTra *lmt = new ListMuonTra;
  lmt->data = new MuonTra;
  return lmt;
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
  while (p != NULL) {
    if (p->data->maSach == key) {
      return p;
    }
    p = p->next;
  }
  return NULL;
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
      ListMuonTra *left = p->prev;
      ListMuonTra *right = p->next;
      left->next = right;
      if (right != NULL)
        right->prev = left;
    }
    delete p->data;
    delete p;
    return true;
  }
}
void luuFile(ListMuonTra *first, fstream &fout)
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

long long const TIME_7_NGAY = 604800;

bool checkDieuKienMuonSach(ListMuonTra *first)
{
  int demDangMuon = 0;
  ListMuonTra *p = first;
  while (p != NULL) {
    if (p->data->trangThai == MT_TT_DANGMUON) {
      demDangMuon++;
      if (demDangMuon == 3 || (getDate() - p->data->ngayMuon) >= TIME_7_NGAY) {
        return false;
      }
    }
    p = p->next;
  }
  return true;
}

bool kiemTraSachDaMuon(ListMuonTra *first, long long maSach, long long ngayMuon)
{
  ListMuonTra *p = first;
  while (p != NULL) {
    if (p->data->maSach == maSach && p->data->ngayMuon == ngayMuon) {
      return false;
    }
    p = p->next;
  }
  return true;
}

ListMuonTra *filterSachChuaTra(ListMuonTra *first)
{
  ListMuonTra *temp = NULL;
  ListMuonTra *p = first;
  while (p != NULL) {
    if (p->data->trangThai == MT_TT_DANGMUON) {
      addLast(temp, p->data);
    }
    p = p->next;
  }
  return temp;
}

#endif
