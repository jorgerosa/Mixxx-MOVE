
/*
Developer: Jorge Rosa (jorge.bigarte@gmail.com) - With love to: ASCR
Website: https://sites.google.com/site/jorgerosaportfolio
Mixxx DarkMetal Skin: http://www.mixxx.org/forums/viewtopic.php?f=8&t=6052
*/

/// --------------------------------------------------------------------------------------------------------------------------
/// Topic: SYSTEM
/// --------------------------------------------------------------------------------------------------------------------------

/// CURRENT OS: MS Windows 7
/// CURRENT SOFTWARE: My CODE::BLOCKS v.16.01 targets the MinGW as the external compiler. (Its installed at: C:\MinGW 4.7.2)

/// EXCLUSIVE INFO ABOUT MY COMPILER: (YOURS MAY VARY)
/// Since OGRE3D (OgreSDK_MinGW_v1-8-1 and the included BOOST libraries) were compiled using MinGW v.4.7, so (instead of recompile the downloaded libraries) I´ve done:
/// 1) Downloaded and installed the MinGW v.4.7.2, its path is now: "C:\MinGW\4.7.2"
/// 2) Copy Paste all that files to "C:\Users\<YourName>\gcc", (inside "gcc", should be "bin", "opt", "etc", "include", "share", etc... folders) just in case you may need it, later for CMake or something...
/// 3) In CODEBLOCK gone to: "Settings" --> "Compiler" --> "Toolchain executables", then set...
///    Selected Compiler: GNU GCC Compiler
///    Compiler Installation Directory: "C:\MinGW\4.7.2\mingw32\bin"
///    C Compiler: "i686-w64-mingw32-gcc.exe"
///    C++ Compiler: "i686-w64-mingw32-c++.exe"
///    Linker for dynamic libs: "i686-w64-mingw32-g++.exe"
///    Linker for static libs: "i686-w64-mingw32-gcc-ar.exe"
///    Debugger: "GDB/CDB debugger : Default"
///    Resource Compiler: "windres.exe"
///    Make program: "mingw32-make.exe"

/// --------------------------------------------------------------------------------------------------------------------------
/// Topic: COMPILING LIBRARIES
/// --------------------------------------------------------------------------------------------------------------------------

/// There are multiple variants of GCC for Windows, which are incompatible with each other (different exception management, threading model, etc.).
/// If you are unsure, check which of the files: "libgcc_s_sjlj-1.dll" - OR - "libgcc_s_dw2-1.dll" is present in your ".\MinGW\bin" folder.
/// SJLJ ==> Set Jump Long Jump an Exception handling method common to Windows.
/// DW2  ==> Dwarf 2 the new Exception handling method used by default in Recent Standard MinGW Builds and Linux GCC.
/// NOTE: If MinGW was installed along with Code::Blocks, you probably have an SJLJ version.

/// --------------------------------------------------------------------------------------------------------------------------
/// Topic: COMMON ERRORS
/// --------------------------------------------------------------------------------------------------------------------------

/// 1) ERROR: undefined reference to "Ogre::NedPoolingImpl::allocBytes(unsigned int, char const*, int, char const*)", etc...
/// FIX: Read the above COMPILING LIBRARIES topic. (Pre-compiled Ogre3dSDK and your MinGW versions MUST Match!)

/// 2) ERROR: These errors are related with the pre-compiled versions of the libraries (Were used many MinGW different versions)
/// a) The sjlj exception model will link to: __gxx_personality_sj0
/// b) The DW2 exception model links to: __gxx_personality_v0
/// c) undefined reference to: _Unwind_Resume
/// d) undefined reference to: _imp___ZN2sf11Sou...
/// e) etc...
/// FIX: Read the above COMPILING LIBRARIES topic.

/// 3) ERROR: "boost config.hpp: No such file or directory"
/// FIX: We comment or remove the line: "#include <config.hpp>" at file "config.hpp" (Its the same file that trows that error)

/// 4) ERROR: Undefined reference to "boost::system::system_category()"
/// FIX: #ifndef BOOST_SYSTEM_NO_DEPRECATED
///      #define BOOST_SYSTEM_NO_DEPRECATED 1
///      #endifws

/// 1) Configure the software:
///    ./Configure
/// 2) Build the software:
///    make
/// 3) Install the software:
///    make install




/// IMPORTANT about Libraries: Read the text file, in the "myLibraries" folder!

/// Standard...
#include <iostream>
#include <exception>
#include <map>
using namespace std;

/// OGRE3D Main library: (Included)
#include <Ogre.h>
#include <OIS/OIS.h>
/// OGRE3D Exclusive for Overlays and Texts:
#include "OgreTextAreaOverlayElement.h"
#include "OgreFontManager.h"
using namespace Ogre;

/// PUGIXML library: (Included)
#define PUGIXML_WCHAR_MODE  /// Needed for foreign (Portuguese) characters. Not needed in this app, but stays here.
#include "pugiconfig.hpp"   /// To load ".xml" files (In this case the "custom.xml" file)
#include "pugixml.hpp"      /// To load ".xml" files (In this case the "custom.xml" file)
#include "pugixml.cpp"      /// To load ".xml" files (In this case the "custom.xml" file)
using namespace pugi;

/// IMPORTANT:
/// ".scene" files are being exported using "EasyOgreExporter.x64.dle" (an 3D StudioMax 2010 Plugin), and imported to our code (here) using the "DotSceneLoader".
/// DOTSCENELOADER library: (Included)
#include "dotsceneloader.h"   /// To load ".scene" files (Depends on TinyXML)
#include "dotsceneloader.cpp" /// To load ".scene" files (Depends on TinyXML)

/// TINYXML library: (Included)
#include "tinystr.h"          /// To load ".scene" files
#include "tinyxml.h"          /// To load ".scene" files
#include "tinystr.cpp"        /// To load ".scene" files
#include "tinyxml.cpp"        /// To load ".scene" files
#include "tinyxmlerror.cpp"   /// To load ".scene" files
#include "tinyxmlparser.cpp"  /// To load ".scene" files

/// RTAUDIO library: (Included - Compiled with: 1) Direct Sound + 2) ASIO + 3) WASAPI for MS Windows)
#include "RtAudio.h"          /// To play audio files

/// The ".CPP" Files:
#include "_engine.cpp"            /// Global needed code
Engine* engine = new Engine();    /// Create instance...
#include "_audio.cpp"             /// Specific code for audios (at the moment, this only works in MS Windows OS (tested under Windows 7) only !!!)
Audio* audio = new Audio();       /// Create instance...
#include "_scene001.cpp"          /// Code for Scene N.1
Scene001* scene = new Scene001(); /// Create instance...




int main(int argc, char *argv[]){

    /// engine:
    engine->loadCustomSettings();
    engine->createVideoAndInputs();
    engine->createEnvironment();

    /// scene:
    scene->loadScene();


    /// Main Loop:
    while(exitAll == false){
    /// Display Scene:
    scene->doLoop();
    /// Listen audio:
    audio->doLoop();
    };

    /// Exiting, in the right order:
    scene->destroy();
    audio->destroy();
    engine->destroy();

};
