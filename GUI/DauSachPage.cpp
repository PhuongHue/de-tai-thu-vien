#ifndef _DAUSACHPAGE_CPP_
#define _DAUSACHPAGE_CPP_

#include <math.h>

#include <sstream>
#include <string>

#include "../DauSach.cpp"
#include "components/BookView.cpp"
#include "components/ContentView.cpp"

using namespace std;

ListDauSach listDauSach;

BookView _DauSachBookView;
ContentView _DauSachContentView;
string _DauSachSearchString;

vector<string> _DauSachBookFooter = {"ESC: Tro ve", "\xAE: Trang sau", "\xAF: Trang truoc"};
vector<string> _DauSachBookSearchFooter = {"ESC: Huy", "ENTER: Tim kiem"};

/**
 * Duoc goi khi co event doi select DauSach trang hoac init view
 * @param book: BookView instant
 * @param content: ContentView instant
 * @
 */
void loadDauSachContent(BookView &book, ContentView &content) {
  DauSach *ds = findDauSachByISBN(listDauSach, _DauSachBookView.keys[_DauSachBookView.select]);
  _DauSachContentView.lines[0] = ds->ISBN;
  _DauSachContentView.lines[1] = to_string(ds->namXB);
  _DauSachContentView.lines[2] = to_string(ds->soTrang);
  _DauSachContentView.lines[3] = ds->tacGia;
  _DauSachContentView.lines[4] = ds->ten;
  _DauSachContentView.lines[5] = ds->theLoai;
}

/**
 * Duoc goi khi co event doi trang hoac init view
*/
void loadDauSachBook(BookView &book) {
  int startIndex = book.pageIndex * book.pageSize;
  int endIndex = startIndex + book.pageSize - 1;
  if (endIndex > listDauSach.length - 1) endIndex = listDauSach.length - 1;
  book.lineCount = endIndex - startIndex + 1;
  // load data trang moi
  for (int i = startIndex, j = 0; i <= endIndex; i++, j++) {
    book.lines[j] = listDauSach.data[i]->ten;
    book.keys[j] = listDauSach.data[i]->ISBN;
  }
  // change select
  if (book.select > book.lineCount - 1) book.select = book.lineCount - 1;
}

void handleBookSelectChange(BookView &book) {
  loadDauSachContent(book, _DauSachContentView);
  clearContentView(_DauSachContentView);
  drawContentView(_DauSachContentView);
}

void searchDauSach() {
  gotoxy(11, 3);
  showConsoleCursor(true);
  setNormalText();
  customCin(_DauSachSearchString, 10);
  showConsoleCursor(false);
}

void dauSachPageAction(BookView &book, int keyPressed) {
  switch (keyPressed) {
    case F1:
      searchDauSach();
      break;
    case F2:
      luuFile(listDauSach);
      break;
    default:
      break;
  }
}

void initDauSachPage() {
  /* init _DauSachBookView */
  _DauSachBookView = defaultBookView;
  _DauSachBookView.left = 1;
  _DauSachBookView.top = 5;
  _DauSachBookView.right = 40;
  _DauSachBookView.bottom = 26;
  _DauSachBookView.pageSize = 20;
  _DauSachBookView.lineCount = 10;
  _DauSachBookView.allPage = (int)ceil(listDauSach.length * 1.0 / _DauSachBookView.pageSize);
  if (_DauSachBookView.allPage == 0) _DauSachBookView.allPage = 1;
  /* load _DauSachBookView */
  loadDauSachBook(_DauSachBookView);

  /* init _DauSachContentView */
  _DauSachContentView = defaultContentView;
  _DauSachContentView.top = 3;
  _DauSachContentView.left = 72;
  _DauSachContentView.right = 154;
  _DauSachContentView.bottom = 26;
  _DauSachContentView.lineCount = 6;
  /* load _DauSachBookView */
  loadDauSachContent(_DauSachBookView, _DauSachContentView);
  drawContentView(_DauSachContentView);
}

void runDauSachPage() {
  loadLayout("layout/DauSach.layout");
  setHeader("Quan ly dau sach");
  setFooter(_DauSachBookFooter);
  runBookView(_DauSachBookView, dauSachPageAction, loadDauSachBook, handleBookSelectChange);
  clearPage(1, 3, 154, 26);
}

#endif