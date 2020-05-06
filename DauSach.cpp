#ifndef _DAUSACH_CPP_
#define _DAUSACH_CPP_

#include <fstream>
#include <iostream>
#include <string>

#include "DMSach.cpp"

struct DauSach {
  string ISBN;
  string ten;
  int soTrang = 0;
  string tacGia;
  int namXB = 1990;
  string theLoai;
  DMSach *dms = NULL;
};

void debugDauSach(DauSach *ds) {
  cout << ds->ISBN << endl
       << ds->namXB << endl
       << ds->soTrang << endl
       << ds->tacGia << endl
       << ds->ten << endl
       << ds->theLoai << endl;
}

#define MAX_LIST 500

struct ListDauSach {
  DauSach *data[MAX_LIST];
  int length = 0;
};

typedef void (*CallBackDauSach)(DauSach *&node);

void foreachListDauSach(ListDauSach &list, CallBackDauSach callBack) {
  for (int i = 0; i < list.length; i++) {
    callBack(list.data[i]);
  }
}

void addLast(ListDauSach &list, DauSach *ds) {
  list.data[list.length] = ds;
  list.length++;
}

void luuFile(ListDauSach &list) {
  ofstream fout;
  fout.open("data/DauSach.data");
  for (int i = 0; i < list.length; i++) {
    fout << list.data[i]->ISBN << endl
         << list.data[i]->ten << endl
         << list.data[i]->soTrang << endl
         << list.data[i]->tacGia << endl
         << list.data[i]->namXB << endl
         << list.data[i]->theLoai << endl;
    luuFile((list.data[i]->dms), fout);
  }
}

void docFileDauSach(ListDauSach &list) {
  ifstream fin;
  fin.open("data/DauSach.data");
  while (!fin.eof()) {
    DauSach *ds = new DauSach;
    fin >> ds->ISBN >> ds->ten >> ds->soTrang >> ds->tacGia >> ds->namXB >> ds->theLoai;
    docFile(ds->dms, fin);
    addLast(list, ds);
  }
}

ListDauSach filterDauSach(ListDauSach list, DauSach key) {
  ListDauSach temp;
  for (int i = 0; i < list.length; i++) {
    if (list.data[i]->ISBN.compare(key.ISBN) == 0 || list.data[i]->namXB == key.namXB ||
        list.data[i]->soTrang == key.soTrang || list.data[i]->tacGia.compare(key.tacGia) == 0 ||
        list.data[i]->ten.compare(key.ten) == 0 || list.data[i]->theLoai.compare(key.theLoai) == 0) {
    }
  }
}

#endif
