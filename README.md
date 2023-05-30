Simple calculator app to test developing GUI apps on vintage Windows versions (Windows 98). 

Don't use this as a real calculator... it overflows and only supports addition. Implementing a correct
calculator isn't the purpose of this project.

## Developing
This uses modern development tools and methods to produce vintage software. I use my standard workflow (CLion, Docker, Linux, cli). 

This does have some drawbacks: 
- The`windows.h` and various other header files provided by MinGW to provide the Win32 API is more modern than what is 
  available in Windows 98. It is possible to use a function that does not exist, and this is not determined until 
  the `.exe` is run on the target Windows 98 when dynamically linking.
- Parts of the MinGW C library are statically linked into the `.exe`, making it larger that it would otherwise be. This
  is still a WIP on using more native, dynamically linked objects on the target Windows 98 if possible.

The code must also be able to compile on the target OS too (see below for building on Windows 98). Todo: use more native
Windows compilers (Visual C++).

## Building

This uses Docker/Linux to build the Windows .EXE. These methods all use MinGW [[1]](https://sourceforge.net/projects/mingw/),
[[2]](https://www.mingw-w64.org/). 

```
./dev/build
```

### (Manual) Linux
Check the `Dockerfile` for what is needed to build and run `make`.

### (Manual) Windows 98 
The project was originally created using [Dev-C++ 5](https://www.bloodshed.net/). The actual installer is v4.9.9.2, but
it's called V5 on the homepage.

Either:
1. Open the `rcalc.dev` project file and compile using the GUI
2. Use the bundled mingw environment under `c:\DEV-CPP` and run `make -f Makefile.win` in a `command` shell.
