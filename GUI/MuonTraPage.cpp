#ifndef _MUONTRAPAGE_CPP_
#define _MUONTRAPAGE_CPP_

#include <math.h>
#include <sstream>
#include <string>

#include "../MuonTra.cpp"
#include "../StringLib.cpp"
#include "components/BookView.cpp"
#include "components/ContentView.cpp"
#include "components/YesNoMenu.cpp"

using namespace std;

namespace MUONTRAPAGE {

int _top = 3;
int _left = 1;
int _right = 154;
int _bottom = 26;

string _HeaderText = "Danh sach muon tra";
string _PageLayout = "layout/MuonTra.layout";

ListMuonTra *_ListMuonTra = NULL;
ListMuonTra *_CurrentNode = NULL;

BookView _MuonTraBookView;

ContentView _MuonTraContentView;

const vector<string> _DauSachBookFooter = {
    "ESC: Tro ve",
    ">>: Trang sau",
    "<<: Trang truoc",
    "F4: Tra sach"};
/* -------------------- _MuonTraContentView funtions -------------------- */
void loadContent(BookView &book, ContentView &content)
{
  if (book.lineCount <= 0) return;
  ListMuonTra *node = findByMaSach(_ListMuonTra, stoll(book.keys[book.select]));
  _CurrentNode = node;
  _MuonTraContentView.lines[0] = to_string(node->data->maSach);
  _MuonTraContentView.lines[1] = getDateString(node->data->ngayMuon);
  _MuonTraContentView.lines[2] = getDateString(node->data->ngayTra);
  _MuonTraContentView.lines[3] = to_string(node->data->trangThai);
}

/* -------------------- _DMSachBookView functions -------------------- */
void loadList(BookView &book)
{
  int dataCount = countAll(_ListMuonTra);
  int newAllPage = countAllPage(dataCount, book.pageSize);
  book.allPage = newAllPage;
  if (book.pageIndex >= book.allPage)
    book.pageIndex = book.allPage - 1;

  int startIndex = book.pageIndex * book.pageSize;
  int endIndex = startIndex + book.pageSize - 1;
  if (endIndex > dataCount - 1) endIndex = dataCount - 1;
  book.lineCount = endIndex - startIndex + 1;
  // load data trang moi
  int j = 0;
  for (ListMuonTra *i = _ListMuonTra; i != NULL; i = i->next) {
    if (j < startIndex) continue;
    if (j > endIndex) break;
    string maSach = to_string(i->data->maSach);
    book.lines[j - startIndex] = maSach;
    book.keys[j - startIndex] = maSach;
    j++;
  }
  // change select
  if (book.lineCount == 0) {
    book.select = 0;
    return;
  }
  if (book.select > book.lineCount - 1) book.select = book.lineCount - 1;
}

void traSach()
{
  time_t t;
  time(&t);
  _CurrentNode->data->ngayTra = t;
  DMSach *dms = findMaSach(_ListDauSach_Root, _CurrentNode->data->maSach);
  dms->data->trangThai = 0;
}

/* -------------------- _DMSachBookView handles -------------------- */
void handleSelectChange(BookView &book)
{
  loadContent(book, _MuonTraContentView);
  clearContentView(_MuonTraContentView);
  drawContentView(_MuonTraContentView);
}

void handleListAction(BookView &book, int keyPressed)
{
  switch (keyPressed) {
  case F4:
    clearBookView(_MuonTraBookView);
    if (YesNoMenu("Ban co muon tra sach?", _MuonTraBookView.left, _MuonTraBookView.top)) {
      traSach();
    }
    loadList(_MuonTraBookView);
    drawBookView(_MuonTraBookView);
    break;
  }
  setFooter(_DauSachBookFooter);
}

/* -------------------- DMSachPage functions -------------------- */
void initMuonTraPage()
{
  /* init _DMSachBookView, _DMSachBookView */
  _MuonTraBookView.left = 1;
  _MuonTraBookView.top = 3;
  _MuonTraBookView.right = 40;
  _MuonTraBookView.bottom = 26;
  _MuonTraBookView.pageSize = 20;
  _MuonTraBookView.lineCount = 20;
  /* init _MuonTraContentView */
  _MuonTraContentView.top = 3;
  _MuonTraContentView.left = 72;
  _MuonTraContentView.right = 154;
  _MuonTraContentView.bottom = 26;
  _MuonTraContentView.lineCount = 3;
  _MuonTraContentView.labelColumnSize = 10;
  _MuonTraContentView.labels[0] = "Ma Sach";
  _MuonTraContentView.labels[1] = "Ngay muon";
  _MuonTraContentView.labels[2] = "Ngay tra";
  _MuonTraContentView.labels[3] = "Trang thai";
  _MuonTraContentView = getInitalView(_MuonTraContentView);
  _MuonTraContentView.isNumberType[0] = true;
  _MuonTraContentView.isNumberType[3] = true;
  _MuonTraContentView.isEditable[0] = false;
}

void runMuonTraPage()
{
  /* tinh so trang, dua page index ve 0 */
  resetBookIndex(_MuonTraBookView, countAll(_ListMuonTra));
  /* load _DMSachBookView */
  loadList(_MuonTraBookView);
  /* load _DMSachBookView */
  loadContent(_MuonTraBookView, _MuonTraContentView);
  
  loadLayout(_PageLayout);
  setHeader(_HeaderText);
  setFooter(_DauSachBookFooter);
  drawContentView(_MuonTraContentView);
  runBookView(_MuonTraBookView, handleListAction, loadList, handleSelectChange);
  clearPage(_left, _top, _right, _bottom);
}
} // namespace MUONTRAPAGE

#endif