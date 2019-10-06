TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += NON_MATLAB_PARSING
DEFINES += MAX_EXT_API_CONNECTIONS=255
DEFINES += QT_COMPIL

LIBS += -lWs2_32
LIBS += -lWinmm

INCLUDEPATH += ../vrep

SOURCES += main.cpp \
    ../vrep/extApi.c \
    ../vrep/extApiCustom.c \
    ../vrep/extApiPlatform.c

HEADERS += \
    ../vrep/extApi.h \
    ../vrep/extApiCustom.h \
    ../vrep/extApiCustomConst.h \
    ../vrep/extApiInternal.h \
    ../vrep/extApiPlatform.h \
    ../vrep/socketInConnection.h \
    ../vrep/socketOutConnection.h \
    ../vrep/v_repConst.h \
    ../vrep/v_repLib.h \
    ../vrep/v_repTypes.h
