#ifndef _TOPDAUSACHPAGE_CPP_
#define _TOPDAUSACHPAGE_CPP_
// TODO: trang top dau sach

#include <string>
#include <vector>

using namespace std;

namespace TOPDAUSACHPAGE {
int _top = 3;
int _left = 1;
int _right = 154;
int _bottom = 26;

string _HeaderText = "Top dau sach duoc muon";
const vector<string> _PageFooter = {
    "ESC: Tro ve",
    ">>: Trang sau",
    "<<: Trang truoc"};

struct Row {
};

#define MAX_ROW_NUMBER 100

struct Table {
  Row row[MAX_ROW_NUMBER];
  int left, top, right, bottom;
  int length = 0;
  int pageIndex = 0;
  int pageSize = 20;
  int allPage;
} _TopDauSachTable;

void initTopDauSachPage()
{
}

void runTopDauSachPage()
{
}

} // namespace TOPDAUSACHPAGE

#endif