#include "GUI/MainPage.cpp"

using namespace std;

int main() {
  fstream fin("data/DauSach.data", fstream::in);
  docFile(_ListDauSach_Root, fin);
  fin.close();
  fullScreen();
  appPause("Load du lieu xong!");
  initMainMenu();
  runMainPage();
};
