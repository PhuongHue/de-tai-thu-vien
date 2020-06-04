#include "GUI/MainPage.cpp"
#include "StringLib.cpp"
using namespace std;

int main()
{
  if (!(docFile(_ListDauSach_Root) && docFile(_ListTheDocGia_root))) {
    appPause("Load du lieu loi!");
    return 0;
  };
  fullScreen();
  appPause("Load du lieu xong!");
  MAINPAGE::initMainMenu();
  DAUSACHPAGE::initDauSachPage();
  MUONSACHPAGE::initMuonSachPage();
  THEDOCGIAPAGE::initTheDocGiaPage();
  MAINPAGE::runMainPage();
};
