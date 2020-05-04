#ifndef _DAUSACH_CPP_
#define _DAUSACH_CPP_

#include <iostream>
#include <fstream>
#include <string>
#include "DMSach.cpp"

struct DauSach
{
  string ISBN = "";
  string ten = "";
  int soTrang = 0;
  string tacGia = "";
  int namXB = 1990;
  string theLoai = "";
  DMSach *dms = NULL;
};

#define MAX_LIST 500

struct ListDauSach
{
  DauSach data[MAX_LIST];
  int length = 0;
};

typedef void (*CallBackDauSach)(DauSach &node);

void foreachListDauSach(ListDauSach &list, CallBackDauSach callBack)
{
  for (int i = 0; i < list.length; i++)
  {
    callBack(list.data[i]);
  }
}

void addLast(ListDauSach &list, DauSach ds)
{
  list.data[list.length] = ds;
  list.length++;
}

void luuFile(ListDauSach &list)
{
  ofstream fout;
  fout.open("data/DauSach.data");
  for (int i = 0; i < list.length; i++)
  {
    fout << list.data[i].ISBN << endl
         << list.data[i].ten << endl
         << list.data[i].soTrang << endl
         << list.data[i].tacGia << endl
         << list.data[i].namXB << endl
         << list.data[i].theLoai << endl;
    luuFile((list.data[i].dms), fout);
  }
}

void docFileDauSach(ListDauSach &list)
{
  ifstream fin;
  fin.open("data/DauSach.data");
  while (!fin.eof())
  {
    DauSach ds;
    fin >> ds.ISBN >> ds.ten >> ds.soTrang >> ds.tacGia >> ds.namXB >> ds.theLoai;
    docFile(ds.dms, fin);
    addLast(list, ds);
  }
}

#endif
