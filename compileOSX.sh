g++ src/*.cpp -std=c++17 -Wall -I./include/ -I/usr/X11R6/include -L/usr/X11R6/lib -I/opt/X11/include/libpng15/ -lX11 -D cimg_use_png -lpng -O3 -o snowcrash
