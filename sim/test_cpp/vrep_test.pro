TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += NON_MATLAB_PARSING
DEFINES += MAX_EXT_API_CONNECTIONS=255
DEFINES += QT_COMPIL

LIBS += -lWs2_32
LIBS += -lWinmm

VREP_PATH = "C:/Program Files/V-REP3/V-REP_PRO_EDU"

VREP_REMOTEAPI_PATH = $$VREP_PATH/programming/remoteApi

INCLUDEPATH += $$VREP_REMOTEAPI_PATH
INCLUDEPATH += $$VREP_PATH/programming/include

SOURCES += main.cpp

SOURCES += $$VREP_REMOTEAPI_PATH/extApi.c
SOURCES += $$VREP_REMOTEAPI_PATH/extApiPlatform.c
SOURCES += $$VREP_PATH/programming/common/shared_memory.c

HEADERS += $$VREP_REMOTEAPI_PATH/extApi.h
HEADERS += $$VREP_REMOTEAPI_PATH/extApiPlatform.h
HEADERS += $$VREP_REMOTEAPI_PATH/extApiIntrenal.h
