# **Chess**
**Classic chess game**

## **Required**
**Cmake 3.7.2**\
**Any C++ compiler**\
**Any build system**

## **Installation**
**`git clone --recursive https://github.com/wizaral/Chess`**\
**`cmake -H. -Bbuild -G"[build system name]" -Wdev -Werror=dev`**\
**`cmake --build ./build --config [Debug|Release] --target chess -- [build system input flags]`**

### **Makefile Example**
**`cmake -H. -Bbuild -G"Unix Makefiles" -Wdev -Werror=dev`**\
**`cmake --build ./build --config Release --target chess -- -j`**
