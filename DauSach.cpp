#ifndef _DAUSACH_CPP_
#define _DAUSACH_CPP_

#include <fstream>
#include <iostream>
#include <string>

#include "DMSach.cpp"
#include "StringLib.cpp"

struct DauSach {
  string ISBN;
  string tenSach;
  int soTrang = 0;
  string tacGia;
  int namXB;
  string theLoai;
  DMSach *dms = NULL;
};

DauSach *clipboardDauSach = NULL;

#define MAX_LIST 500

struct ListDauSach {
  DauSach *data[MAX_LIST];
  int length = 0;
};

ListDauSach _ListDauSach_Root;

void swapDauSach(DauSach *&a, DauSach *&b)
{
  DauSach *c = a;
  a = b;
  b = c;
}

DauSach *findDauSachByISBN(ListDauSach &list, string ISBN)
{
  for (int i = 0; i < list.length; i++) {
    if (list.data[i]->ISBN.compare(ISBN) == 0) return list.data[i];
  }
  return NULL;
}

int findIndexDauSachByISBN(ListDauSach &list, string ISBN)
{
  for (int i = 0; i < list.length; i++) {
    if (list.data[i]->ISBN.compare(ISBN) == 0) return i;
  }
  return -1;
}

int addLast(ListDauSach &list, DauSach *ds)
{
  if (list.length == MAX_LIST) return 1;
  list.data[list.length] = ds;
  list.length++;
  return 0;
}

int deleteByIndex(ListDauSach &list, int index)
{
  if (index >= list.length || index < 0 || list.length == 0) return 1;
  delete list.data[index];
  for (int i = index; i < list.length - 1; i++) {
    list.data[i] = list.data[i + 1];
  }
  list.length--;
  return 0;
}

void findAndDelete(ListDauSach &list, string ISBN)
{
  int index = findIndexDauSachByISBN(list, ISBN);
  deleteByIndex(list, index);
}

void luuFileDS(ListDauSach &list)
{
  ofstream fout;
  fout.open("data/DauSach.data");
  fout << list.length << endl;
  for (int i = 0; i < list.length; i++) {
    fout << list.data[i]->ISBN << endl
         << list.data[i]->tenSach << endl
         << list.data[i]->soTrang << endl
         << list.data[i]->tacGia << endl
         << list.data[i]->namXB << endl
         << list.data[i]->theLoai << endl;
    luuFileDMS((list.data[i]->dms), fout);
  }
}

bool docFileDS(ListDauSach &list)
{
  fstream fin("data/DauSach.data", fstream::in);
  if (!fin.is_open()) return false;
  int count;
  fin >> count;
  // file rong
  if (fin.eof()) return true;
  fin.ignore();
  for (int i = 0; i < count; i++) {
    DauSach *ds = new DauSach;
    getline(fin, ds->ISBN);
    getline(fin, ds->tenSach);
    fin >> ds->soTrang;
    fin.ignore();
    getline(fin, ds->tacGia);
    fin >> ds->namXB;
    fin.ignore();
    getline(fin, ds->theLoai);
    docFileDMS(ds->dms, fin);
    addLast(list, ds);
  }
  return true;
}

ListDauSach filterDauSach(ListDauSach list, string key)
{
  ListDauSach temp;
  for (int i = 0; i < list.length; i++) {
    if (find(list.data[i]->tenSach, key) >= 0) {
      addLast(temp, list.data[i]);
    }
  }
  return temp;
}

DMSach *findMaSach(ListDauSach &list, long long key)
{
  for (int i = 0; i < list.length; i++) {
    DMSach *ans = findByMaSach(list.data[i]->dms, key);
    if (ans != NULL) {
      return ans;
    }
  }
} 

DauSach *tim_DauSach_theo_MaSach(ListDauSach &list, long long key)
{
  for (int i = 0; i < list.length; i++) {
    DMSach *ans = findByMaSach(list.data[i]->dms, key);
    if (ans != NULL) {
      return list.data[i];
    }
  }
  return NULL;
}

struct DauSachMaSach {
  DauSach *dauSach = NULL;
  DMSach *dmSach = NULL;
};

DauSachMaSach tim_DauSachMaSach_theo_MaSach(ListDauSach &list, long long key)
{
  DauSachMaSach temp;
  for (int i = 0; i < list.length; i++) {
    DMSach *dms = findByMaSach(list.data[i]->dms, key);
    if (dms != NULL) {
      temp.dmSach = dms;
      temp.dauSach = list.data[i];
      break;
    }
  }
  return temp;
}

int soSanhDauSach(DauSach *a, DauSach *b)
{
  int cmpTheLoai = a->theLoai.compare(b->theLoai);
  if (cmpTheLoai == 0) {
    return a->tenSach.compare(b->tenSach);
  }
  else
    return cmpTheLoai;
}

void sortBy_TheLoai_Ten(ListDauSach &list)
{
  for (int i = 0; i < list.length; i++) {
    int minDS = i;
    for (int j = i + 1; j < list.length; j++) {
      int kq = soSanhDauSach(list.data[minDS], list.data[j]);
      if (kq > 0) minDS = j;
    }
    if (minDS != i) {
      swapDauSach(list.data[i], list.data[minDS]);
    }
  }
}

#endif
