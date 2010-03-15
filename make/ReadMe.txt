========================================================================
    CONSOLE APPLICATION : abs Project Overview
========================================================================
nova arch

definice:


zavislosti:
cas, SHA-1

generator skriptu 

skript featury
knihovny: lib, include, bin
solution -> folder -> projekt -> filter -> soubor
soubor - tool, tool2 -> projekt(tool)


zavislost resi generator
skupiny konfiguraci
Debug GL / Release GL / Tools
Jak resit update?

old:

features:
 skupiny souboru (pro prehled i pro jednotne nastaveni)
 tools pro build (napriklad flex)
 architektury
 konfigurace
 predkomilovane hlavicky

styl podle VS?
 dedeni nastaveni
 
objekty a zavislosti
objekty a mnoziny? (tools)
 
zavislost:
 filter - rozdeleni do skupin (soubor jako skupina)
 tool:
  c++ preklad
  
  generators:
  flex preklad
  asm
  m4 prep
  python gen
 link - link knihovny
 install ?
 
 // jak skladat?
 // predat jeden objekt ze slozenych presetu?
 
 platform
   win32
   xbox
   windows mobile
   linux
   mac os x
 architektura
   32
   64
   apr
   intel
   powerpc
 konfigurace - ruzne 
   bool: unicode
   enum: debug
         release
         public release
   enum: static
         dynamic
   enum: opengl
         directx
   enum: directsound
         openal
   24 kominaci?? jednotlive presety
         dedicnost?
         kombinace
 platform -> architektura -> konfigurace = properties
  | tools
 tool c++
 tool flex
 tool asm
 tool link
 
 kazde properties definuje tool
  tool ma vlastnosti
objekty:
  vlastnost objektu build -> 

  util
  module
  tutorial
  library
  filter
  projekt
  test
  
projekt -> ?filter? -> tutorial, util, module, test -> ?filter? -> soubor, link

tools:
  c++
  flex
  asm
  linker 
  
// tools je jen jeden
// nastaveni pres properties
// zvysovani buildu?

// dependency walking?
// soubory se buildi pokud maji ruzne nastaveni

// properties & property tool
tools -> tools -> 

properties -> inherid tool 

inherid -> inherid atd.... az na strop

prop -> prop 

[aa:vlc]
[bb]
aa="cc"

sekce -> "value"

/////////////////////////////////////////////////////////////////////////////
autoconf, configure, make, cmake, idl, svn, generator

features:
build system
generator system
spoluprace s VS
spoluprace s xcode
spoluprace s make (umi zpracovavat Makefile)
integrated svn
integrated doxygen
install generator (generuje istalacni baliky)
vlastni tools:
  generatory c++:
    flex
    bison
    idl (gen.h, pythonembeded, soapproxy)
    python script
    vlastni preprocessor
definice automaker (svnversion, build id)
generovani unit testu -> test na nejake funkce, generator moznych parametru, nebo nahodnych
uploader:
nocni buildy
build release
  ftp
indent -> autoformatovani kodu podle centralniho coding style
nastavovani properties do svn
rozesilani souhrneho logu o build a chybach
kombinovani projektu (projekt muze souhrne buildit i framework (vyvoj frameworku))
sprava uzivatelu (ruzne konfig pro ruzne uzivate)
sprava packages (zavisle knihovny (hledani buildu, kompilace ze zdrojaku))
ochrana kodu (hlidani zanaseni chyb do kodu)
sprava komentaru (organizace todo a not implement)
statistiky a prehledy
gen dokumentace

<Solution Hoe>
 
 AddProject HoeCore
 AddProject HoeEditor


</Solution>


# Projekt hoe core
# hlavni projekt pro ostatni

<Project HoeCore>

 <Filter Math>
   Sources *.cpp

 </Filter>

 <Conf opengl>
   cc.Define=_OPENGL_
 </Conf>

 

</Project>



