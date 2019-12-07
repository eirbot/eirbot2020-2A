TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
	test.cpp

HEADERS += \
	archive.hpp \
	dynamic_archive_file_system.hpp \
	dynamic_archive_random_access_stream.hpp \
	lool.hpp \
	shell.hpp
