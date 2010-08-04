TARGET = wmit
TEMPLATE = app
HEADERS += MainWindow.hpp \
    ImportDialog.hpp \
    UVEditor.hpp \
    ConfigDialog.hpp \
    TransformDock.hpp \
    Vector.hpp \
    VectorTypes.hpp \
    Polygon.hpp \
    Pie.hpp \
    WZM.hpp \
    TeamColoursDock.hpp \
    Generic.hpp
SOURCES += main.cpp \
    MainWindow.cpp \
    ImportDialog.cpp \
    UVEditor.cpp \
    ConfigDialog.cpp \
    TransformDock.cpp \
    Polygon_t.cpp \
    WZM.cpp \
    TeamColoursDock.cpp \
    Generic.cpp \ 
	Pie_t.cpp \
	Pie.cpp
FORMS += MainWindow.ui \
    ImportDialog.ui \
    UVEditor.ui \
    ConfigDialog.ui \
    TransformDock.ui \
    TeamColoursDock.ui
QT += opengl \
    xml

# If your system uses different paths for QGLViewer, create a file named
# "config" and override the necessary variables below (with "=").
QGLVIEWER_INCL = /usr/include/QGLViewer
QGLVIEWER_LIBS = -lQGLViewer
include("config")
INCLUDEPATH += $$QGLVIEWER_INCL
LIBS += -l3ds \
    -lm \
    $$QGLVIEWER_LIBS

UI_DIR = ui
MOC_DIR = moc
OBJECTS_DIR = bin

#QMAKE_CXX = g++-4.5
#QMAKE_CC = gcc-4.5
QMAKE_CXXFLAGS +=  -pipe \#-std=c++0x \
	-fno-nonansi-builtins -ffor-scope \
	-fno-exceptions -fno-rtti \
	-fno-signed-zeros -fno-math-errno #\
#	-flto -fuse-linker-plugin -fwhole-program
CONFIG -= rtti exceptions
CONFIG += warn_on
QMAKE_CXXFLAGS_WARN_ON += -pedantic	\
	-Wextra -Wc++0x-compat \
	-Wnon-virtual-dtor \
	-Wno-long-long -Wold-style-cast \
	-Wredundant-decls #-DWARNMORE # if you want more warnings to quash
QMAKE_CFLAGS_DEBUG+= -fstack-protector-all
QMAKE_CXXFLAGS_DEBUG+= -fstack-protector-all

OTHER_FILES += TODO.txt COPYING