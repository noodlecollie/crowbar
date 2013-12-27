Crowbar
=======

An open-source level editor for Source games, built using Qt5.

Crowbar is built to improve on what Valve's Hammer, largely unchanged since the days of Quake, lacks in terms of functionality and ease of use. Built using the Qt5 framework, Crowbar is compatible with Windows, Mac and Linux and is created from the ground up to be modular and easily extensible. It supports scripting for basic tasks as well as the creation of custom C++ plugins for more advanced or performance-critical functions.

Prerequisites
=============

The Crowbar project now requires the Qt3D framework, which can be found at git://gitorious.org/qt/qt3d.git. Crowbar.pro assumes that the relevant Qt3D.pro file is stored at ../qt3d, but modify this to suit your needs.