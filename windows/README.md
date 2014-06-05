How to create the ChecksumJack Installer for Windows 7/8
--------------------------------------------------------

Install nsis.
http://nsis.sourceforge.net/Main_Page

#COPY

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
