- this binary only run on linux 64-bit, and is worked fine on ubuntu 14.04 and 13.10
- for 13.10, copy extra shared libs in folder local-lib/only-need-for-ubuntu13-10 to /usr/lib

- copy all contents in folder local-libs to /usr/ to have other dependency
- if the app doesn’t not run, check the library dependency using tools like “ldd” or “objdump”, and find the missing shared lib on internet such as from rpmfind.net, then copy  those missing libs to /usr/lib as well