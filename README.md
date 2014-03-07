Crowbar
=======

An open-source level editor for Source games, built using Qt5.

Current Instructions
====================

If you're reading this, you're in the wrong branch! The master will hold stable builds once these exist. As of writing,
the most current branch is qt3d-experimental - this holds attempts to utilise the Qt3D library for 3D rendering. You
should be able to obtain this from http://doc-snapshot.qt-project.org/qt3d-1.0/ - the current setup is to have the root
qt3d directory alongside the crowbar root directory (ie. place it one up from crowbar.pro). Qt3D should be compiled and
integrated with your current Qt installation in order for it to be functional within Crowbar.

Crowbar is set up to compile modules in the correct order when building from the root node in the subdirs project layout.
This does not mean that it'll do so successfully, however! As I muddle through, certain things will work and certain
things won't, so there's no guarantee that a given commit will be in a runnable state. This is very much a learning
experience for myself, so things may not work or be laid out as more experienced programmers might expect them to be.
I'm always open to advice and improvements from people who have been doing this kind of thing longer than I have.
If you want an example of a commit where everything was relatively functional and we had rudimentary drawing capabilities,
check out the tests in https://github.com/x6herbius/crowbar/commit/c94508bc51b6946a79b40c041cfd31b67e8b2333

Some Doxygen documentation does exist, although it's not currently complete as new things are being added/removed/changed.
Run Doxygen on the doxyfile to generate it.

- x6herbius