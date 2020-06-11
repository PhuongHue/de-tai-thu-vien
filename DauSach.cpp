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

#define MAX_LIST 500

struct ListDauSach {
  DauSach *data[MAX_LIST];
  int length = 0;
} _ListDauSach_Root;

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

void luuFile(ListDauSach &list)
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
    luuFile((list.data[i]->dms), fout);
  }
}

bool docFile(ListDauSach &list)
{
  fstream fin("data/DauSach.data", fstream::in);
  if (!fin.is_open() || fin.eof()) return false;
  int count;
  fin >> count;
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
    docFile(ds->dms, fin);
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
  DMSach *ans = NULL;
  for (int i = 0; i < list.length; i++) {
    ans = findByMaSach(list.data[i]->dms, key);
    if (ans != NULL) break;
  }
  return ans;
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

void sortByTheLoaiTen(ListDauSach &list)
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
