#ifndef _DAUSACH_CPP_
#define _DAUSACH_CPP_

#include <fstream>
#include <iostream>
#include <string>

#include "DMSach.cpp"
#include "StringLib.cpp"

struct DauSach {
  string ISBN;
  string ten;
  int soTrang = 0;
  string tacGia;
  int namXB = 1990;
  string theLoai;
  DMSach *dms = NULL;
};

void log(DauSach *&ds) {
  gotoxy(0, _DebugLog.logX);
  cout << "ISBN: " << ds->ISBN << endl
       << "namXB: " << ds->namXB << endl
       << "soTrang: " << ds->soTrang << endl
       << "tacGia: " << ds->tacGia << endl
       << "ten: " << ds->ten << endl
       << "theLoai: " << ds->theLoai << endl;
  _DebugLog.logX += 6;
}

void log(DauSach &ds) {
  gotoxy(0, _DebugLog.logX);
  cout << "ISBN: " << ds.ISBN << endl
       << "namXB: " << ds.namXB << endl
       << "soTrang: " << ds.soTrang << endl
       << "tacGia: " << ds.tacGia << endl
       << "ten: " << ds.ten << endl
       << "theLoai: " << ds.theLoai << endl;
  _DebugLog.logX += 6;
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

void log(ListDauSach &lds) { foreachListDauSach(lds, log); }

DauSach *findDauSachByISBN(ListDauSach &list, string ISBN) {
  for (int i = 0; i < list.length; i++) {
    if (list.data[i]->ISBN.compare(ISBN) == 0) return list.data[i];
  }
}

void addLast(ListDauSach &list, DauSach *ds) {
  list.data[list.length] = ds;
  list.length++;
}

void luuFile(ListDauSach &list) {
  ofstream fout;
  fout.open("data/DauSach.data");
  fout << list.length << endl;
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

void docFile(ListDauSach &list, fstream &fin) {
  int all;
  fin >> all;
  fin.ignore();
  for (int i = 0; i < all; i++) {
    DauSach *ds = new DauSach;
    getline(fin, ds->ISBN);
    getline(fin, ds->ten);
    fin >> ds->soTrang;
    fin.ignore();
    getline(fin, ds->tacGia);
    fin >> ds->namXB;
    fin.ignore();
    getline(fin, ds->theLoai);
    // log(ds);
    docFile(ds->dms, fin);
    addLast(list, ds);
  }
}

ListDauSach filterDauSach(ListDauSach list, DauSach key) {
  ListDauSach temp;
  for (int i = 0; i < list.length; i++) {
    if (find(list.data[i]->ISBN, key.ISBN) >= 0) {
      addLast(temp, list.data[i]);
    }
  }
  return temp;
}

#endif
