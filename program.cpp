#include "GUI/MainPage.cpp"
#include "StringLib.cpp"
using namespace std;

int main()
{
  fstream finDS("data/DauSach.data", fstream::in);
  docFile(_ListDauSach_Root, finDS);
  finDS.close();
  fstream finTDG("data/TheDocGia.data", fstream::in);
  docFile(_ListTheDocGia_root, finTDG);
  finTDG.close();
  fullScreen();
  appPause("Load du lieu xong!");
  initMainMenu();
  runMainPage();
};
