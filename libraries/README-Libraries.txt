
About all the needed libraries for MOVE:

- OR - They just dont needed to be compiled...

1) DotSceneLoader
2) PugiXML
3) TinyXML

- OR - Had been compiled using MinGW 4.7.2 (for MS Windows)...

1) Ogre3D SDK (I´ve found MS Windows binaries (already compiled) in their downloads page)
2) Boost (Was included with Ogre3D)
3) OIS (Was included with Ogre3D)
4) RtAudio (Must be compiled. cmake-gui --> configure --> make --> install)
   Compiled with: Direct Sound + ASIO + WASAPI for MS Windows