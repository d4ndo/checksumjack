;
; checksumJack is a multi platform hash calculating software.
; Copyright (C) 2014 www.checksumJack.org
;
; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program.  If not, see <http://www.gnu.org/licenses/>.
;

  !include "MUI2.nsh"
  !include "FileAssociation.nsh"
  !include "x64.nsh"

!define MUI_ICON "checksumJackIcon.ico"

; *** General ***

  ;Name and file
  Name "checksumJack"
  OutFile "SetupChecksumJack_x64.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES64\D4NDO\checksumJack\"

; *** Variables ***

  Var StartMenuFolder

; *** Interface Settings ***

  !define MUI_ABORTWARNING

; *** Pages ***

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "LICENSE"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  ;Start Menu Folder Page Configuration
  !define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU" 
  !define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\Modern UI Test" 
  !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
  !insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder
  ;end Etart Menu Folder
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH
  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH

; *** languages ***

  !insertmacro MUI_LANGUAGE "English"

; *** Installer Sections ***

Section "checksumJack" checksumJack

  ${registerExtension} "$INSTDIR\checksumJack.exe" ".md5" "checksumJack"
  ${registerExtension} "$INSTDIR\checksumJack.exe" ".sha1" "checksumJack"
  ${registerExtension} "$INSTDIR\checksumJack.exe" ".rmd160" "checksumJack"
  ${registerExtension} "$INSTDIR\checksumJack.exe" ".sha224" "checksumJack"
  ${registerExtension} "$INSTDIR\checksumJack.exe" ".sha256" "checksumJack"
  ${registerExtension} "$INSTDIR\checksumJack.exe" ".sha384" "checksumJack"
  ${registerExtension} "$INSTDIR\checksumJack.exe" ".sha512" "checksumJack"
  ${registerExtension} "$INSTDIR\checksumJack.exe" ".whirlpool" "checksumJack"
  
  ;Refresh so windows knows the new extensions.
  System::Call 'shell32.dll::SHChangeNotify(i, i, i, i) v (0x08000000, 0, 0, 0)'

  SetOutPath "$PROFILE\checksumJack"
  file filter.xml
  
  SetOutPath "$INSTDIR"
  
  ;INSTALL FILES
  file checksumJack.exe
  file Qt5Core.dll
  file Qt5Gui.dll
  file Qt5Widgets.dll
  file icuin51.dll
  file icuuc51.dll
  file icudt51.dll    
  file libeay32.dll
  file libGLESv2.dll
  file libEGL.dll
  file d3dcompiler_46.dll
  
  SetOutPath "$INSTDIR\platforms"
  
  file qwindows.dll
  file qminimal.dll

  ;Store installation folder
  WriteRegStr HKCU "Software\Modern UI Test" "" $INSTDIR

  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    
    ;Create shortcuts
    CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
	CreateShortCut "$SENDTO\checksumJack.lnk" "$INSTDIR\checksumJack.exe"	
    CreateShortCut "$SMPROGRAMS\checksumJack.lnk" "$INSTDIR\checksumJack.exe"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\checksumJack.lnk" "$INSTDIR\checksumJack.exe"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
	
  !insertmacro MUI_STARTMENU_WRITE_END

SectionEnd

Section "msvc2012" msvc2012

SetOutPath $INSTDIR\msvc2012redist
  
    file vcredist_x64.exe
    ExecWait '"$INSTDIR\msvc2012redist\vcredist_x64.exe"'
	
SectionEnd

;Section "German" Sec2checksumJack

  ;file bom_de.qm
  ;Store installation folder
  ;WriteRegStr HKCU "Software\Modern UI Test" "" $INSTDIR
;  SectionEnd

;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_checksumJack ${LANG_ENGLISH} "Install checksumJack."
  LangString DESC_msvc2012 ${LANG_ENGLISH} "Visual Studio 2012 Redistributeable."
  ;LangString DESC_Sec2checksumJack ${LANG_ENGLISH} "German language pack."

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${checksumJack} $(DESC_checksumJack)
  !insertmacro MUI_DESCRIPTION_TEXT ${msvc2012} $(DESC_msvc2012)  
  ;!insertmacro MUI_DESCRIPTION_TEXT ${Sec2checksumJack} $(DESC_Sec2checksumJack)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

; *** Uninstaller Section ***

Section "Uninstall"

  ;UNINSTALL FILES
  ${unregisterExtension} ".md5" "checksumJack"
  ${unregisterExtension} ".sha1" "checksumJack"
  ${unregisterExtension} ".rmd160" "checksumJack"
  ${unregisterExtension} ".sha224" "checksumJack"
  ${unregisterExtension} ".sha256" "checksumJack"
  ${unregisterExtension} ".sha384" "checksumJack"
  ${unregisterExtension} ".sha512" "checksumJack"
  ${unregisterExtension} ".whirlpool" "checksumJack"
  
  Delete "$INSTDIR\Uninstall.exe"
  Delete "$INSTDIR\checksumJack.exe"
  Delete "$INSTDIR\Qt5Core.dll"
  Delete "$INSTDIR\Qt5Gui.dll"
  Delete "$INSTDIR\Qt5Concurrent.dll"
  Delete "$INSTDIR\Qt5Widgets.dll"
  Delete "$INSTDIR\Qt5Xml.dll"
  Delete "$INSTDIR\libeay32.dll"
  Delete "$INSTDIR\icuuc51.dll"
  Delete "$INSTDIR\icuin51.dll"
  Delete "$INSTDIR\icudt51.dll"
  Delete "$INSTDIR\libGLESv2.dll"
  Delete "$INSTDIR\libEGL.dll"
  Delete "$INSTDIR\D3DCompiler_46.dll"
  Delete "$INSTDIR\platforms\qwindows.dll"
  Delete "$INSTDIR\platforms\qminimal.dll"
  Delete "$INSTDIR\msvc2012redist\vcredist_x64.exe"
 
  RMDir "$INSTDIR\platforms\"
  RMDir "$INSTDIR\msvc2012redist\"
  RMDir "$INSTDIR"
  
  !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
  
  Delete "$SENDTO\checksumJack.lnk"	  
  Delete "$SMPROGRAMS\checksumJack.lnk"  
  Delete "$SMPROGRAMS\$StartMenuFolder\checksumJack.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk"
  RMDir "$SMPROGRAMS\$StartMenuFolder"

  DeleteRegKey /ifempty HKCU "Software\Modern UI Test"

SectionEnd
