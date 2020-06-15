#include "GUI/MainPage.cpp"
#include "StringLib.cpp"
using namespace std;

int main()
{
  fullScreen();
  if (!(docFile(_ListDauSach_Root) && docFile(_ListTheDocGia_root))) {
    appPause("Load du lieu loi!");
    return 0;
  };
  sortBy_TheLoai_Ten(_ListDauSach_Root);

  appPause("Load du lieu xong!");
  // khoi tao page
  MAINPAGE::initMainMenu();
  DAUSACHPAGE::initDauSachPage();
  MUONSACHPAGE::initMuonSachPage();
  THEDOCGIAPAGE::initTheDocGiaPage();
  // chay trang dau
  MAINPAGE::runMainPage();
};
