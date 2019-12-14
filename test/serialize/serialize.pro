TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../nucleo/lib/serialize

SOURCES += main.cpp \
	test.cpp

HEADERS += \
	../nucleo/lib/serialize/archive.hpp \
	../nucleo/lib/serialize/dynamic_archive_file_system.hpp \
	../nucleo/lib/serialize/dynamic_archive_random_access_stream.hpp \
	lool.hpp \
	../nucleo/lib/serialize/shell.hpp
