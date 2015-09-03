# drawproc
An interactive graphics programming environment familiar to Processing, using C/C++

drawproc.exe is the shell which is capable of running graphics 'sketches', which are encapsulated in .dll files.  Typical execution looks like this:

```c:\>drawproc module.dll```

In order to create the module in the first place, the module writer would include "drawproc.h" in their project, and compile their code as a .dll.  The module will need to refer to the drawproc.lib file, so that it picks up the references to the various drawing routines that are within the drawproc library.

Then you just package up drawproc.exe, and the module.dll file, and away you go.

At some point, these modules will need their own extension so that they can be double clicked from the file explorer.  Perhaps they will be '.dproc' files.

The examples directory contains a couple of examples.
