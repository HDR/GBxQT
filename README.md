# GBxQT
 Custom software for InsideGadget's GBx devices
 
## Compiling

### Windows
1. Install [QT](https://www.qt.io/download-open-source)
2. Install [mingw-w64](http://mingw-w64.org/)
3. Run `qmake.exe GBxQT.pro -spec win32-g++ "CONFIG+=qtquickcompiler"`
4. Run mingw32-make

### Linux
1. Install qt using your distro's package manager (apt install qt5-qmake if your distro uses apt)
2. Run `qmake GBxQT.pro "CONFIG+=qtquickcompiler"`
3. Run make

### MacOS
1. Install qt using `brew install qt` and `brew link qt --force`
2. Run `qmake GBxQT.pro "CONFIG+=qtquickcompiler"`
3. Run make