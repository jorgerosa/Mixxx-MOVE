# Mixxx-MOVE
"Gives" to the music some 3D real-time visual effects...


MOVE Development:

v.0.0.7 - Includes now all the needed libraries and accepts foreign (latin/portuguese) characters
v.0.0.6 - Lots of optimizations (including code and graphics)
v.0.0.5 - All 3D scene is animated (zoomed) acording audio input levels
v.0.0.4 - Much better loop trought all your audio devices, and with on screen infos. (Keys "O" and "P")
v.0.0.3 - Yep!... it CAN listen your microphone (audio input) now!
v.0.0.2 - Added support for texts and external fonts. Just type your text in the ".\bin\Data\settings.xml" file!

--------------------------------------------------------------------------------------------------

Developer: Jorge Rosa (jorge.bigarte@gmail.com) - With love to: ASCR
Website: https://sites.google.com/site/jorgerosaportfolio
Mixxx DarkMetal Skin: http://www.mixxx.org/forums/viewtopic.php?f=8&t=6052

--------------------------------------------------------------------------------------------------

> MOVE - Mixxx Ogre3D Visual Effects:

To RUN it, just go to the "bin" folder, and double click on the executable "MOVE_RELEASE.exe"
Should work great with a computer with 2 monitors connected.
(One with your music software, and the other with these visuals)

--------------------------------------------------------------------------------------------------

> What is this MOVE good for?...

1) When playing music, people are always watching TV in the pubs, bars, etc... so... From now on, those TV sets will be broadcasting the Mixxx (or the DJ) logo. Visuals are simple by now (but very time consuming to develop them, since they are fully in 3D), but at least people can be focused only in the music now! :P
2) Tries to create the right environment for people to listen music...
3) Makes publicity to the DJ name or to any brand. Maybe publicity to the DJ official drinks sponsor?...

--------------------------------------------------------------------------------------------------

> Main KEYS are:

ESC        <-- Exits.
F1         <-- Particles.
F          <-- Change between Fullscreen and Windowed modes.
O          <-- Connects to the previous Audio Device (Press if MOVE cant "listen" any input audio).
P          <-- Connects to the next Audio Device (Press if MOVE cant "listen" any input audio).

> Beat KEYS are: (All are in the numeric keypad, except the SPACE key)

SPACE      <-- Beat now!
ENTER      <-- Time between 1st and 2nd ENTER key press sets the beat interval.
               (Visuals should now loop with that set interval)
0 to 9     <-- Pre defined beats intervals. (0 == Disabled, 9 == Faster beats)
-          <-- Sets intervals between beats.
+          <-- Sets intervals between beats.

--------------------------------------------------------------------------------------------------

> For you to start:
   
I recommend first to start to play with all the included files located in the ".\Data" folder:

1) ".xml", ".cfg", ".fontdef", ".particle", ".scene", ".material", etc...
   All these files could be opened and edited with any kind of text editor (eg: Notepad).

2) The ".mesh" files contains the 3D objects, you may use Blender or 3DStudioMax to open and edit them.

--------------------------------------------------------------------------------------------------

> For you TODO:

1) Design your own 3D logo (eg: in Blender) and export it (as ".scene" file format),
   add it to the ".\Data\3D-Logos" folder.
   (Add all your needed textures, meshes and materials there. See where the included examples are)
   
2) Edit the "settings.xml" file.
   (Its located inside the ".\Data" folder)

3) Run "MOVE_RELEASE.exe" or "MOVE_DEBUG.exe"
   (The debug version is better in case something went wrong, you may read the log in that console)

--------------------------------------------------------------------------------------------------

> For DEVELOPERS:

1) The included "MOVE.cbp" file is a project for CODE::BLOCKS
2) The "main.cpp" file contains information about the needed libraries, etc...
3) The code is clean and fully commented (I think that could help a lot to start)
   
--------------------------------------------------------------------------------------------------

> Help would be great TODO all this:

1) Integrate it with Mixxx (as a plugin - or - as an external program launched from there)
2) Make it decent, its very very poor in features, but its better than nothing...
3) Works only on MS Windows (needs to be compiled for other platforms too)

--------------------------------------------------------------------------------------------------
