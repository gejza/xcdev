;NSIS Modern User Interface
;Basic Example Script

!define BUILDDIR "..\build"

;--------------------------------
;Include Modern UI

  !include "MUI.nsh"

;--------------------------------
;General

  ;Name and file
  Name "XC Analyze Tools"
  OutFile "XCProfiler.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\XCProfiler"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\XCProfiler" ""

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_LICENSE "..\LICENSE"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section "Devel files" SecDevel

  SectionIn RO

  SetOutPath "$INSTDIR\include\xcprof"  
  File "..\include\xcprof\*.h*"
  SetOutPath "$INSTDIR\include\xc"  
  File "..\include\xc\*.h*"
  
  SetOutPath "$INSTDIR\lib"  
  File "${BUILDDIR}\debug\*.lib"
  File "${BUILDDIR}\debugts\*.lib"
  File "${BUILDDIR}\debugtss\*.lib"
  File "${BUILDDIR}\release\*.lib"
  File "${BUILDDIR}\releasets\*.lib"
  File "${BUILDDIR}\releasetss\*.lib"

  SetOutPath "$INSTDIR\bin"  
  File "${BUILDDIR}\debugtss\*.dll"
  File "${BUILDDIR}\debugtss\*.dll.manifest"
  File "${BUILDDIR}\releasetss\*.dll"
  File "${BUILDDIR}\releasetss\*.dll.manifest"
  File "${BUILDDIR}\releasetss\*.pyd"
  File "${BUILDDIR}\releasetss\*.pyd.manifest"
  File "${BUILDDIR}\releasetss\*.so"
  File "${BUILDDIR}\releasetss\*.so.manifest"

  SetOutPath "$INSTDIR\bin\debug"  
  File "${BUILDDIR}\debugtss\*.pyd"
  File "${BUILDDIR}\debugtss\*.pyd.manifest"
  File "${BUILDDIR}\debugtss\*.so"
  File "${BUILDDIR}\debugtss\*.so.manifest"
  
  SetOutPath "$INSTDIR\pdb"  
  File "${BUILDDIR}\debug\xcprof*.pdb"
  File "${BUILDDIR}\debugts\xcprof*.pdb"
  File "${BUILDDIR}\debugtss\xcprof*.pdb"
  File "${BUILDDIR}\debugtss\python*.pdb"
  File "${BUILDDIR}\debugtss\mod*.pdb"
  File "${BUILDDIR}\release\xcprof*.pdb"
  File "${BUILDDIR}\releasets\xcprof*.pdb"
  File "${BUILDDIR}\releasetss\xcprof*.pdb"
  File "${BUILDDIR}\releasetss\python*.pdb"
  File "${BUILDDIR}\releasetss\mod*.pdb"

  
  ;Store installation folder
  WriteRegStr HKCU "Software\XCProfiler" "" $INSTDIR
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd

Section "Tools" SecTools
  SetOutPath "$INSTDIR\bin"
  
  SetOutPath "$INSTDIR\bin"  
  File "${BUILDDIR}\release\xcprof*.exe"
  File "${BUILDDIR}\release\xcprof*.exe.manifest"

  SetOutPath "$INSTDIR\bin\debug"  
  File "${BUILDDIR}\debug\xcprof*.exe"
  File "${BUILDDIR}\debug\xcprof*.exe.manifest"
SectionEnd

;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_SecDevel ${LANG_ENGLISH} "Development files."
  LangString DESC_SecTools ${LANG_ENGLISH} "Admin tools."

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecDevel} $(DESC_SecDevel)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecTools} $(DESC_SecTools)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  ;ADD YOUR OWN FILES HERE...

  Delete "$INSTDIR\Uninstall.exe"

  RMDir "$INSTDIR"

  DeleteRegKey /ifempty HKCU "Software\Profiler"

SectionEnd