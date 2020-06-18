rm testTool.exe
g++ -o testTool.exe testTool.cpp -std=c++11
IF EXIST testTool.exe START testTool.exe