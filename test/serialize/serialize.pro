TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
	test.cpp

HEADERS += \
	archive.hpp \
	lool.hpp \
	shell.hpp \
	simple_dynamic_random_access_stream.hpp
