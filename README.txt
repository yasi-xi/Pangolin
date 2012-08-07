== What is Pangolin ==

Pangolin is a Lightweight collection of utilities for rapid vision prototyping.
At its heart is a simple OpenGl display library which can help manage viewports
and offers an advanced but intuitive 3D navigation handler. Pangolin also
provides a mechanism for manipulating program variables through config files and
ui integration, and offers a simple mechanism for lightweight video input.

== About this fork ==

This fork attempts to make Pangolin compiled in Win32 using MSVC.

For the original Pangolin, please refer to: https://github.com/stevenlovegrove/Pangolin

=====================

Update:

Merged with the recent Steven's version.

Known bugs:

1. Typeid(class).name() comparison errors (fixed).

2. glutFullScreen() doesn't behave correct with glutMainLoopEvet() but work fine with the non-return event loop glutMainLoop().
