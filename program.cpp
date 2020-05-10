#include "GUI/MainPage.cpp"

using namespace std;

int main() {
  fstream fin("data/DauSach.data", fstream::in);
  docFile(listDauSach, fin);
  fin.close();
  system("pause");
  fullScreen();
  initMainMenu();
  runMainPage();
};
