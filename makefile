CC=g++
CFLAGS=-c -Wall -DTIXML_USE_STL=YES -std=c++11 -I/usr/include/libnotifymm-1.0 -I/usr/lib64/libnotifymm-1.0/include -I/usr/include/gtkmm-3.0 -I/usr/lib64/gtkmm-3.0/include -I/usr/include/atkmm-1.6 -I/usr/include/gtk-3.0/unix-print -I/usr/include/gdkmm-3.0 -I/usr/lib64/gdkmm-3.0/include -I/usr/include/giomm-2.4 -I/usr/lib64/giomm-2.4/include -I/usr/include/pangomm-1.4 -I/usr/lib64/pangomm-1.4/include -I/usr/include/glibmm-2.4 -I/usr/lib64/glibmm-2.4/include -I/usr/include/gtk-3.0 -I/usr/include/at-spi2-atk/2.0 -I/usr/include/at-spi-2.0 -I/usr/include/dbus-1.0 -I/usr/lib64/dbus-1.0/include -I/usr/include/gtk-3.0 -I/usr/include/gio-unix-2.0/ -I/usr/include/cairo -I/usr/include/pango-1.0 -I/usr/include/harfbuzz -I/usr/include/pango-1.0 -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/cairomm-1.0 -I/usr/lib64/cairomm-1.0/include -I/usr/include/cairo -I/usr/include/pixman-1 -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/freetype2 -I/usr/include/libdrm -I/usr/include/libpng16 -I/usr/include/sigc++-2.0 -I/usr/lib64/sigc++-2.0/include -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/libpng16 -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include
LDFLAGS=-ltinyxml -lnotifymm-1.0 -lgtkmm-3.0 -latkmm-1.6 -lgdkmm-3.0 -lgiomm-2.4 -lpangomm-1.4 -lglibmm-2.4 -lgtk-3 -lgdk-3 -lpangocairo-1.0 -lpango-1.0 -latk-1.0 -lcairo-gobject -lcairomm-1.0 -lcairo -lsigc-2.0 -lnotify -lgdk_pixbuf-2.0 -lgio-2.0 -lgobject-2.0 -lglib-2.0 -lcurlpp -lstdc++ -lcurl
SOURCES=notifier.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=notifier

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

