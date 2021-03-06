TEMPLATE = app
TARGET = unittest_cg_open_modify
CONFIG   += console
CONFIG   += c++11

CONFIG(debug, debug|release) {
	# cgnslib
	LIBS += -L"E:/iricdev_2013/lib/install/cgnslib-3.2.1/debug/lib"

	# iriclib
	LIBS += -L"../debug"
}
else {
	# cgnslib
	LIBS += -L"E:/iricdev_2013/lib/install/cgnslib-3.2.1/release/lib"

	# iriclib
	LIBS += -L"../release"
}
LIBS += -lcgnsdll
LIBS += -liriclib

# cgnslib
INCLUDEPATH += "E:/iricdev_2013/lib/install/cgnslib-3.2.1/debug/include"

# iriclib
INCLUDEPATH += ".."

# Input
SOURCES += main.cpp
