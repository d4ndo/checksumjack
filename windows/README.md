# Windows 7/8 installation guide

Install nsis.
http://nsis.sourceforge.net/Main_Page
Install QT5.x 
http://qt.digia.com/
Download the newest OpenSSL library.
http://www.openssl.org/source/

# Compile 

Important note:

Everything should be compiled with the same compiler. If you download the QT5.2(MVSC2012) library, then please use MVSC2012.
First thing to do is to compile the openSSL library. Please follow instuctions on README openssl.

Adapt the ChecksumJack.pro file. So the compiler can link the libraries correct.

My default path is:
* C:/Qt/
* c:/openssl-win64/


# Prepare and create installer (Setup.exe)

Copy the following files to the /checksumJack/windows directory:

### ChecksumJack

* ../build/checksumJack.exe
* ../src/icon/checksumJackIcon.ico
* ../LICENSE
* ./filter.txt
* ./Basic.nsi <- open this file with nsis to create the Installer.

### Qt

* C:\Qt\Qt5.2.1\5.2.1\msvc2012_64\bin
* Qt5Core.dll
* Qt5Gui.dll
* Qt5Widgets.dll
* libEGL.dll
* libGLESv2.dll
* icudt51.dll
* icuin51.dll
* icuuc51.dll
* d3dcompiler_46.dll
* C:\Qt\Qt5.2.1\5.2.1\msvc2012_64\plugins\platforms
* qwindows.dll
* qminimal.dll

### openSSL

* libeay32.dll

### Microsoft Redistributable

* vcredist_x64.exe


# NSIS

Compile Basic.nsis with NSIS to create the Setup.exe.
