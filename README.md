# **Chess**
**Classic chess game**

## **Required**
**Cmake 3.18**\
**Visual Studio 2019 for Windows**\
**GCC or Clang for UNIX systems**

## **Installation:**
**`git clone --recursive https://github.com/wizaral/Chess`**
#### **For Windows:**
**`cmake . -Bbuild -Thost=[x86|x64] -Wdev -Werror=dev && cmake --build ./build --config [Debug|Release]`**\
**`./[Debug|Release]/chess.exe`**
#### **For UNIX systems:**
**`cmake . -Bbuild -Wdev -Werror=dev && cmake --build ./build`**\
**`./chess`**
